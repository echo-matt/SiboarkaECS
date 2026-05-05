# Lecture 01 — Scene Management: Giving Your Game a Memory

---

## 1. The Problem You Already Have

Open your `game/src/main.cpp`. Look at the block right after `World world;` — the part where you create the player, set its position to `{400, 400}`, attach its `RenderComponent`, add the floor entity, and so on.

That block *is* a scene. You just have not named it yet.

Right now it is anonymous, invisible, and permanently locked inside `main()`. That works fine for a single prototype. But ask yourself: what happens when you want a main menu before the game starts? Or a second level with different platforms? Or a "Game Over" screen that shows your score?

You would have to keep stuffing more and more code into `main()`, surrounding it with `if/else` chains and boolean flags like `bool gameStarted = false` and `bool levelTwoLoaded = false`. Within a few hundred lines, `main.cpp` becomes unreadable. You cannot find the player setup code. You cannot tell where the menu ends and the gameplay begins.

This is not a theoretical future problem. Every game that gets past the "one room prototype" stage hits this wall.

**The analogy that will stick with you.**

Think of a theatre. The stage — the physical floor, the curtains, the lighting rigs — is always the same structure. It does not change between plays. But the *set* changes completely: for one play the stage is a Victorian drawing room, for the next it is a spaceship, for the next it is an empty field.

In your engine:
- The **stage** is `World` — the ECS registry that holds entities and components.
- A **scene** is the set of instructions for arranging one particular play on that stage.

The stage does not know or care what play is being performed. It just provides the space.

---

## 2. What Is a Scene, Precisely?

Before we talk about code structure, it is important to define the concept clearly. A scene is:

> A named, self-contained unit of game content. It knows how to populate the World with the entities it needs, how to update itself each frame, and how to clean up after itself when it ends.

Every scene has the same three moments in its life:

**Load** — the scene receives control of the World and populates it. This is where you create entities, attach components, and set initial positions. Think of this as the stagehands moving the set into place before the curtain rises.

**Update** — the normal per-frame game loop. The scene is "running." Systems process entities, physics happens, the player moves. This is the play itself, in performance.

**Unload** — the scene is told it is ending. It destroys every entity it created, frees any resources it loaded (textures, sounds), and leaves the World empty and clean for the next scene. This is striking the set after the final curtain.

Look again at your `main.cpp`:
- Everything from `Entity player = world.createEntity()` to `world.addComponent(floor, StaticComponent{})` is the **Load** phase.
- Everything inside `while (!WindowShouldClose())` is the **Update** phase.
- `CloseWindow()` is the closest thing you have to an **Unload** phase — but notice that you never actually destroy your entities. It works only because the whole program is ending anyway.

The moment you want to transition to a different scene mid-game, that missing Unload step becomes a problem.

---

## 3. Why Not Just Have Multiple Copies of main.cpp?

This is the first thing a beginner tries. "I'll have `main_menu.cpp` and `gameplay.cpp` and just... switch between them somehow."

You cannot. The `main()` function is the **entry point** — the one place the operating system hands control to your program. There is exactly one `main()`, and it runs from start to finish in a single, uninterrupted execution. You cannot pause it and swap in a different one.

The game loop runs continuously. The window stays open. The entities in the World remain in memory. Everything is live, all the time. To transition between scenes, you need a mechanism that works *inside the running loop* — not something that replaces the loop entirely.

---

## 4. The Scene Interface Pattern

The solution is to define a **contract** that every scene must fulfil.

In software architecture, a contract is a set of functions you promise to implement. The game loop does not care which scene it is talking to — it just calls `load()`, calls `update()` every frame, and eventually calls `unload()`. The loop is completely decoupled from the content.

In C++ this contract is called a **pure abstract base class** (sometimes called an interface). Every concrete scene — `MainMenuScene`, `GameplayScene`, `GameOverScene` — inherits from this base and fills in its own implementation.

Here is the relationship drawn out plainly:

```
GameLoop
  │
  └── holds a pointer to: current scene (any scene that fulfils the contract)
        │
        ├── calls  scene->load(world)          ← once, when the scene starts
        ├── calls  scene->update(world, dt)    ← every frame
        └── calls  scene->unload(world)        ← once, when the scene ends
```

The loop does not have an `if/else` for each scene. It calls the same three functions, and polymorphism — the C++ mechanism for "call the right version of this function based on the actual type" — ensures the correct scene's code runs.

If you have used Unity, you have already seen this pattern. `Start()` is Load, `Update()` is Update, `OnDestroy()` is Unload. Unity hides the contract behind MonoBehaviour, but it is the same idea.

---

## 5. Scene Transitions

When one scene ends and another begins, three strategies are commonly used. Each trades off simplicity against visual quality.

**Immediate swap.** Unload the current scene, load the next scene, all in the same frame. The screen will show one frame of black (or whatever ClearBackground renders) during the swap. This is jarring but trivially simple to implement. For a learning project, this is the right starting point.

**Fade to black.** Before unloading, render a progressively more opaque black rectangle over the screen across several frames until it is fully black. While the screen is black, do the scene swap. Then gradually fade the black rectangle back out. The player sees a smooth visual transition; the potentially slow scene-load operation is hidden behind the blackout. This is what most indie games do.

**Loading screen.** A minimal dedicated scene whose only purpose is to display "Loading..." while the real next scene prepares in the background (typically on a second thread). This is only necessary when scene loads take more than a fraction of a second — usually because large textures or audio files are being read from disk. For your current engine, this is overkill.

---

## 6. The Scene Manager: The Doorman

Something needs to track which scene is currently running and orchestrate the swap when it is time. This is the **SceneManager**.

Think of the SceneManager as the theatre's stage manager — the person standing in the wings who signals when to bring in the new set and who coordinates the crew during scene changes. The audience (the player) does not see the stage manager. The performers (your systems) do not talk to the stage manager directly. But nothing would change without them.

The SceneManager holds two things:
- The **current scene** — the one being updated every frame right now.
- An optional **pending next scene** — the one requested to load, but not yet activated.

The critical rule: **never swap scenes mid-frame.**

If a system calls "switch to GameOver scene" while `CollisionSystem.update()` is halfway through iterating over entities, you could unload the scene (and destroy all entities) while the loop is still reading from the entity list. That is undefined behaviour — a crash, a corrupted World, or silent wrong results.

The safe pattern: when a system wants to change scenes, it *requests* a change by telling the SceneManager "queue up GameOverScene." The SceneManager stores this request but does not act on it immediately. At the very end of the frame — after all systems have updated, after `world.events.clear()` — the SceneManager checks whether a swap was requested and performs it then, in the empty space between frames.

---

## 7. How This Fits Your ECS

Your `World` is **not** a scene. The World is the stage — a neutral container. It has no opinion about what is loaded into it.

A scene is the **script** that tells the World what to contain. The scene owns the entity IDs it created (it keeps a list of them, like a stage manager's prop inventory). When the scene unloads, it calls `world.destroyEntity()` on each of those IDs, leaving the World clean.

One important design choice you will encounter: should each scene have its own `World`, or do all scenes share one?

**Shared World** — simpler. One World is created at program start and never destroyed. Each scene clears out the entities from the previous scene during its Unload phase. This is what your engine naturally supports right now.

**Per-scene World** — more isolated. Each scene creates and owns a fresh World. When the scene ends, the World is destroyed entirely. No risk of leftover entities from a previous scene accidentally persisting. This is what larger engines often do, but it requires more infrastructure.

For your engine, start with a shared World. The discipline of "always destroy your own entities in unload()" teaches the same hygiene habits that make the per-scene approach valuable later.

---

## 8. The Game State Machine

Scene management is the practical implementation of a concept you will study in computer science: the **finite state machine** (FSM).

A finite state machine has:
- A fixed set of possible states
- A current state (only one is active at any moment)
- Transition rules (what triggers a move from one state to another)

Your game's scenes are its states:

```
┌────────────┐   press Start   ┌──────────┐   die   ┌──────────┐
│  MainMenu  │ ──────────────► │ Gameplay │ ──────► │ GameOver │
└────────────┘                 └──────────┘         └──────────┘
      ▲                              │                    │
      │                        press Esc            press Retry
      │                              ▼                    │
      │                        ┌────────┐                 │
      │                        │ Paused │                 │
      │                        └────────┘                 │
      └────────────────────────────────────────────────────┘
```

The arrows are **transitions**. They happen in response to events — a key press, an entity dying, a timer reaching zero. The SceneManager is the machine that reads these transitions and performs the swaps.

Students who go further in game development or computer science will recognise that a more powerful variant — the **pushdown automaton** — allows you to "pause" the current scene on a stack and run a new one on top of it (the classic use case: pausing a game without destroying the game state). A stack-based scene manager is a natural next step after a simple one.

---

## 9. Summary

- Your `main.cpp` setup block is already a scene — it just has no structure.
- A scene has three phases: Load, Update, Unload.
- Scenes must be swappable at runtime while the loop is running.
- The Scene Interface Pattern lets the game loop treat all scenes identically via a common contract.
- The SceneManager coordinates swaps, but always between frames, never mid-frame.
- `World` is the stage; scenes are scripts for what goes on the stage.
- Scene management is a finite state machine over your game's content states.

**Your immediate next step.** Extract the entity-setup block from `main.cpp` into a class called `GameScene`. Give it a `load(World&)` method that contains everything that is currently in your setup block, and an `unload(World&)` method that destroys those entities. Call `gamescene.load(world)` before the loop and `gamescene.unload(world)` after. Nothing else changes yet. That one refactoring teaches you the Load/Unload discipline without requiring a SceneManager at all — add that later when you have a second scene to transition to.
