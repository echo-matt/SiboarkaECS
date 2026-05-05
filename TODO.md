# ECS Engine — TODO

---

## Step 1: Input System

### 1.1 — Create `ControllableComponent`
File: `engine/include/ecs/components/ControllableComponent.h`

- Copy the structure of an existing component header (e.g. `TagComponent.h`) as a starting point
- This component has **no fields at all** — it is an empty struct
- Its only purpose is to act as a tag/flag so the InputSystem knows which entity the player controls
- Think about it: if you later wanted a second player, you'd just add this component to another entity — no system changes needed

### 1.2 — Create `InputSystem`
File: `engine/include/ecs/systems/InputSystem.h`

- Copy the structure of `MovementSystem.h` as a starting point — the class shape is identical
- Inside `update()`, your query should ask for entities that have **both** `TransformComponent` and `ControllableComponent`
  - Look at how `RenderSystem` queries two components at once — same idea
- For each entity returned by the query, get a **reference** to its `TransformComponent` (look at how `MovementSystem` does this)
- Use raylib's `IsKeyDown()` function to check if W/A/S/D are held. Look up `IsKeyDown` in the raylib cheatsheet — it takes a `KeyboardKey` enum value like `KEY_W`
- When a key is held, **set** `velX` or `velY` to a speed value (e.g. 200.f). When it is not held, set it back to 0
  - Ask yourself: should you set velocity, or add to it? What happens if you press two opposite keys at once?
- Don't forget to include `<raylib.h>` and all the component headers you use

### 1.3 — Register the new files in `engine/CMakeLists.txt`
- Open `engine/CMakeLists.txt` and find where the other headers are listed (e.g. `include/ecs/components/TagComponent.h`)
- Add a line for `ControllableComponent.h` and one for `InputSystem.h` in the same format
- This doesn't affect compilation — it just makes the files appear in Visual Studio's Solution Explorer

### 1.4 — Wire everything into `main.cpp`
- Add `#include <ecs/components/ControllableComponent.h>` and `#include <ecs/systems/InputSystem.h>` at the top, next to the other includes
- Find where the player entity is created and attach a `ControllableComponent{}` to it — same pattern as the other `addComponent` calls
- Instantiate an `InputSystem` the same way `MovementSystem` and `RenderSystem` are instantiated
- Call `inputSys.update(world, dt)` in the game loop — it must come **before** `moveSys.update()`, because input should set the velocity that movement will then apply in the same frame

### What to test
- Run the game. The blue square (player) should stop bouncing and respond to W/A/S/D
- The other entities should keep bouncing as before — they have no `ControllableComponent`, so `InputSystem` ignores them
- Try pressing two opposite keys (W+S) — what happens? Is that the behaviour you want?

---

## Step 2: Collision System

### 2.1 — Create `ColliderComponent`
File: `engine/include/ecs/components/ColliderComponent.h`

- This component needs two float fields: `width` and `height` (the size of the collision box)
- For now these can match the render size, but keeping them separate means you can have invisible colliders later
- Give them sensible default values

### 2.2 — Understand AABB collision before writing anything
Two rectangles overlap if **all four** of these are true at the same time:
```
A's left edge   < B's right edge
A's right edge  > B's left edge
A's top edge    < B's bottom edge
A's bottom edge > B's top edge
```
Where:
- left edge   = `transform.x`
- right edge  = `transform.x + collider.width`
- top edge    = `transform.y`
- bottom edge = `transform.y + collider.height`

Draw this on paper with two boxes before writing code. It will save you a lot of debugging.

### 2.3 — Create `CollisionSystem`
File: `engine/include/ecs/systems/CollisionSystem.h`

- Query all entities with `TransformComponent + ColliderComponent` — store the result in a local variable (you need to loop over it twice)
- Use a **nested loop**: for every entity A, loop over every entity B — but skip the pair if A and B are the same entity
- Inside the inner loop, get both transforms and both colliders, then apply the AABB check from 2.2
- On overlap, the simplest response is to reverse both entities' `velX` and `velY` — like they bounced off each other
- Register the new files in `engine/CMakeLists.txt`, same as Step 1.3

### 2.4 — Wire into `main.cpp`
- Add a `ColliderComponent` to the player, enemy, and box — use the same width/height values as their `RenderComponent`
- Instantiate `CollisionSystem` and call it in the update phase (before rendering)

### What to test
- Entities should now react when they overlap — they won't pass through each other
- The ghost entity (no `ColliderComponent`) should still be ignored

---

## Step 3: Health + Death

### 3.1 — Create `HealthComponent`
File: `engine/include/ecs/components/HealthComponent.h`

- Two int fields: `hp` (current) and `maxHp`
- Give `hp` a default value of 3

### 3.2 — Modify `CollisionSystem` to deal damage
- When the player overlaps an enemy, reduce the enemy's `hp` by 1
- To know if an entity is the player, use `hasComponent<ControllableComponent>(e)` — the player is the only entity with that component
- Only reduce HP once per collision, not every frame — think about how you'd prevent repeated damage (hint: a cooldown timer, or removing a component temporarily)

### 3.3 — Destroy entities with 0 HP — the safe way
**The problem:** if you call `world.destroyEntity()` inside the loop that `getEntitiesWith()` returned, you are modifying a list while iterating over it — this causes crashes or skipped entities.

**The solution — deferred delete:**
1. Before the loop, create a local `std::vector<Entity>` called something like `toDestroy`
2. Inside the loop, instead of destroying immediately, push the entity into `toDestroy`
3. After **both** loops have finished, loop over `toDestroy` and call `world.destroyEntity()` for each one

### What to test
- Enemies should disappear after being hit 3 times
- The player and box should be unaffected (no `HealthComponent`)
- Nothing should crash when an entity is destroyed mid-frame

---

## Step 4: Spawn System

### 4.1 — Understand why this system is different
All previous systems were **stateless** — they had no member variables. `SpawnSystem` needs to remember how much time has passed since the last spawn. This means it needs a **member variable** (`float m_timer = 0.f`), making it the first stateful system you write.

### 4.2 — Create `SpawnSystem`
File: `engine/include/ecs/systems/SpawnSystem.h`

- Add a private `float m_timer` initialised to `0.f`
- In `update()`, add `deltaTime` to `m_timer` every frame
- When `m_timer` exceeds a threshold (e.g. 3.0f seconds), reset it to 0 and spawn a new enemy:
  1. Call `world.createEntity()` to get a new entity ID
  2. Attach a `TransformComponent` with a random starting position and velocity — look at raylib's `GetRandomValue()` for random numbers
  3. Attach a `RenderComponent` and a `ColliderComponent`
  4. Attach a `HealthComponent`
  5. Do **not** attach `ControllableComponent` — it's an enemy, not the player

### 4.3 — Wire into `main.cpp`
- Instantiate `SpawnSystem` and call it in the update phase
- You can now also remove the hardcoded enemy from the entity setup section if you want — the spawn system will create them

### What to test
- Wait a few seconds — a new enemy should appear
- The new enemy should move, collide, and die just like the original one
- Spawning should keep happening on a regular interval
