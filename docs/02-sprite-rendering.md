# Lecture 02 — Sprite Rendering: From Coloured Rectangles to Real Graphics

---

## 1. Why Rectangles Are Not Enough

Your current `RenderComponent` stores three things: a width, a height, and a `Color`. Your `RenderSystem` takes those and calls `DrawRectangle`. The result is a solid-coloured box on screen.

This is not a limitation of your engine. It is a deliberate starting point. Coloured rectangles are ideal for learning physics and collision because they have no visual noise — you can see exactly what the hitbox is, where the entity is, and whether the position looks right. If you had started with a character sprite, you would be debugging artwork instead of code.

But rectangles teach you nothing about how real-time computer graphics actually work. Sprites, textures, spritesheets, GPU memory — these are the vocabulary of every game you have ever played. This lecture closes that gap.

By the end of this lecture you will understand:
- What a texture actually is, at the hardware level
- Why you must never load a texture inside the game loop
- What a sprite is and how it differs from a texture
- Why texture atlases exist
- How a `SpriteComponent` would fit into your ECS
- The Resource Manager pattern that prevents a common architectural mistake

---

## 2. What Is a Texture?

This is the central question, and the answer has three layers. Most beginner tutorials skip the first two and jump straight to "call this function." Understanding all three layers will save you from subtle bugs and performance problems later.

---

### Layer 1 — The File on Disk

A PNG, JPEG, or BMP file is pixel data in a **compressed** form. JPEG compresses by discarding colour information your eye cannot easily detect. PNG compresses by finding patterns in rows of pixels. The details differ, but both formats store data much more compactly than raw pixels.
Your operating system can open and read these files. But your GPU — the chip that draws every pixel on your screen — cannot use compressed data directly. It needs raw, uncompressed pixel values.

---

### Layer 2 — The Image in CPU RAM

When you decompress an image (for example with `LoadImage("player.png")` in raylib), the compressed bytes are expanded into a flat array of pixels sitting in ordinary system RAM (the same RAM your variables live in). Each pixel is typically four bytes: Red, Green, Blue, Alpha.

At this point the data is accessible to your C++ code. You could, in principle, loop through the array and read every pixel value manually. The CPU can work with this data freely.

But the CPU is not what draws your game. The GPU is.

---

### Layer 3 — The Texture in GPU Memory (VRAM)

Modern GPUs have their own dedicated memory, called VRAM (Video RAM). It sits physically on the graphics card, connected to the GPU cores by a very fast internal bus. The GPU can sample texture data from VRAM at billions of operations per second.

When you call `LoadTexture(...)` in raylib, the raw pixel array from CPU RAM is **uploaded** across the PCIe bus to VRAM. The GPU now has its own copy. The CPU's copy can be discarded — raylib frees it automatically. What you receive back from `LoadTexture` is a **handle**: a small data structure containing an integer ID. That ID is how you tell the GPU "use the texture you stored under this number."

The key insight: **the GPU cannot see your CPU RAM, and your CPU cannot easily read VRAM.** After the upload, the texture data lives on the GPU side of the fence. You refer to it only by its handle.

---

### The Library Analogy

This three-layer process is exactly like borrowing a book:

- **The file on disk** is a book stored in a warehouse far away — compressed, archived, not immediately useful.
- **Loading to CPU RAM** is the book being delivered to your local library branch and placed on the shelf.
- **Uploading to VRAM** is the library stamping it and putting it in the "available to read in this room only" section.
- **The texture handle** is your library card number — it does not contain the book itself, but it is how you check out "book ID 7" whenever you need it.

You do not carry the entire book in your pocket. You carry the card number.

---

## 3. Why You Must Never Load Textures Every Frame

This is the single most expensive mistake beginners make, and it is an easy one to stumble into.

The PCIe bus — the electrical connection between your CPU and GPU — has a limited bandwidth. Sending a few megabytes of texture data from CPU RAM to VRAM takes measurable time: typically several milliseconds for a moderately sized image.

At 60 frames per second, each frame has a budget of approximately 16 milliseconds. If you call `LoadTexture` inside your game loop, you are spending several of those milliseconds on a data upload that could have happened once, at startup. With enough entities and textures, you exceed your frame budget. Your frame rate drops. The game stutters or freezes.

The rule is absolute and has no exceptions:

> **Load textures before the game loop begins. Store the handle. Use the handle inside the loop. Never call LoadTexture inside the loop.**

This is why your `RenderSystem` does not load images — it only draws. The loading belongs in the scene setup (Load phase), or in a Resource Manager (discussed in section 7), but never in the per-frame update path.

---

## 4. What Is a Sprite?

People use the word "sprite" loosely to mean "a 2D game image." Precisely, it means something more specific:

> A **texture** is the entire image stored on the GPU.
> A **sprite** is a *rectangular region* of that texture — a crop.

To draw a sprite, you tell the GPU: "From texture number 7, take the rectangle starting at pixel column 64, row 0, with width 32 and height 32. Stretch or shrink it to fit this destination rectangle on screen."

This crop region is called the **source rectangle**. Where it appears on screen is the **destination rectangle**.

---

### Why Not Just Use One Texture Per Sprite?

You could have `player_walk_1.png`, `player_walk_2.png`, `player_walk_3.png`, and so on — one file per animation frame. This works. But switching between textures mid-frame is expensive for the GPU.

Here is why: the GPU processes draw calls in sequence. Every time you switch which texture is active, the GPU must pause, swap in the new texture from VRAM, and resume. This pause is called a **state change**, and it stalls the GPU pipeline. A few hundred state changes per frame are fine. Thousands are not.

The solution game artists and engine programmers settled on decades ago is the **texture atlas**, sometimes called a spritesheet:

```
┌──────────────────────────────────────────────────────────┐
│  [walk_1][walk_2][walk_3][walk_4]  [idle_1][idle_2]      │
│                                                          │
│  [jump_1][jump_2][jump_3]          [fall_1][fall_2]      │
│                                                          │
│  [attack_1][attack_2][attack_3]    [hurt_1][hurt_2]      │
└──────────────────────────────────────────────────────────┘
        ▲ one single texture — one upload — one handle
```

Every animation frame for a character is packed into one large image. To play an animation, you change only the **source rectangle** — which crop of this single texture to draw. The texture itself never changes. No GPU state switch occurs. The draw call is fast.

This is why sprite artists deliver "spritesheets" rather than individual files. It is not aesthetic preference — it is a performance requirement baked into how GPU hardware works.

---

## 5. How raylib Handles This

raylib provides a function called `DrawTexturePro` that draws a sprite from a texture atlas. Conceptually it takes:

- **The texture** — the GPU handle (a `Texture2D` in raylib)
- **Source rectangle** — which region of the texture to crop (in pixels)
- **Destination rectangle** — where on screen to draw it, and at what size (in pixels)
- **Origin point** — the pivot for rotation, relative to the destination rectangle (usually top-left or centre)
- **Rotation angle** — in degrees
- **Tint colour** — multiplied with each pixel's colour; use `WHITE` to draw with original colours

Compare this to your current `DrawRectangle`, which takes only a position, size, and colour. `DrawRectangle` never consults the GPU's texture memory at all — it draws a solid colour directly using the GPU's rasteriser. That is why your current renderer is so simple: it sidesteps the entire texture pipeline.

The moment you call `DrawTexturePro`, you are engaging the full texture-sampling pipeline. The GPU looks up the source rectangle in VRAM, filters the pixels (smoothing or keeping them sharp depending on your settings), and paints them onto the screen.

---

## 6. SpriteComponent: Where It Sits in Your ECS

Your current architecture has a clean separation:
- `engine/` knows nothing about raylib
- `game/` owns everything raylib-dependent

Your `RenderComponent` already lives in `game/src/components/` because it uses raylib's `Color` type. A `SpriteComponent` would live in the same place, for the same reason: it would need to reference raylib's `Texture2D` and `Rectangle` types.

Conceptually, a `SpriteComponent` would need to hold:
- A reference to the texture (the GPU handle — but *not* a copy of the whole texture, more on this in section 7)
- A source rectangle (which crop of the texture to draw)
- Optionally, a tint colour

Notice the distinction from `RenderComponent`:

| Component | Meaning |
|---|---|
| `RenderComponent` | "Draw me as a coloured shape" |
| `SpriteComponent` | "Draw me as a region of a texture" |

Both can coexist. An entity can have a `RenderComponent` (for a solid-colour debug overlay) and a `SpriteComponent` (for its actual visual). Or just one. Or neither. This is composition: capabilities are assembled from components, not inherited from a class hierarchy.

Your `RenderSystem` would need to be extended — or a new `SpriteRenderSystem` would be added alongside it — to handle entities that have a `SpriteComponent`. The separation of "which system handles which component combination" is one of the ECS pattern's strengths: you do not break the existing rectangle-drawing path, you simply add a parallel path for sprite-drawing.

---

## 7. The Resource Manager Problem

Here is an architectural problem you will hit the moment you have more than one entity using the same texture.

Suppose you have 50 enemy entities. Each enemy has a `SpriteComponent`. You write the naive version: the `SpriteComponent` stores a `Texture2D` **by value** — meaning each component holds its own copy of the full texture handle struct.

On the surface this seems fine: each component is independent, no shared state, simple to reason about.

The problem: `Texture2D` in raylib is not just a small integer. It is a struct that contains the GPU handle *and* several other fields. More importantly, when you copy it between components, you do not copy the GPU data (that stays in VRAM), but you do create a situation where multiple C++ objects each believe they "own" the same GPU resource. When one of them is destroyed and tries to free the texture, the GPU resource is unloaded even though the other 49 still need it.

Beyond the lifetime problem, there is a conceptual one: if 50 enemies share the same `enemy.png`, why would you call `LoadTexture("enemy.png")` 50 times? Each call would upload the same image to VRAM 50 times, wasting memory and bandwidth.

The solution is a **Resource Manager** (also called an Asset Manager or Asset Cache):

> A Resource Manager is an object whose job is to map file paths to loaded texture handles. The first time you request `"enemy.png"`, it loads the texture and stores it. Every subsequent request for `"enemy.png"` returns the same handle immediately, without re-loading.

The structure looks like this:

```
ResourceManager
  "player.png"  →  Texture2D { id=1, width=128, height=128, ... }   ← loaded once
  "enemy.png"   →  Texture2D { id=2, width=64,  height=64,  ... }   ← loaded once
  "tileset.png" →  Texture2D { id=3, width=512, height=512, ... }   ← loaded once

Enemy SpriteComponent → points to entry for "enemy.png" (id=2)
Enemy SpriteComponent → points to entry for "enemy.png" (id=2)   ← 50 enemies,
Enemy SpriteComponent → points to entry for "enemy.png" (id=2)   ← same handle
```

Your `SpriteComponent` stores a *pointer or reference* to the cached texture in the Resource Manager, not the texture itself. The Resource Manager owns the texture data. The components just borrow it.

This pattern appears in virtually every game engine in existence, under different names: Asset Manager, Content Manager, Resource Cache, Texture Registry. The specific name does not matter. The concept is always the same: **centralised loading, shared access, single ownership.**

Where does the Resource Manager live in your engine? In `game/`, not `engine/`. It uses `LoadTexture` and `Texture2D`, which are raylib types. The engine stays pure.

---

## 8. Where This Leads: Animation

Now that you understand sprites (crops of a texture atlas), animation is a short logical step.

An animated character is a sequence of sprites shown one after another fast enough to create the illusion of motion — typically 8 to 12 frames per second for a pixel art game.

In ECS terms, this maps cleanly to two components working together:

A `SpriteAnimationComponent` would store:
- An ordered list of source rectangles — one per animation frame
- A frame duration — how many seconds to show each frame
- A timer — how many seconds the current frame has been showing
- A current frame index — which rectangle in the list is active right now

An `AnimationSystem` would run each frame. It adds `deltaTime` to the timer. When the timer exceeds the frame duration, it increments the current frame index (wrapping around to 0 at the end) and resets the timer. It then writes the new source rectangle into the entity's `SpriteComponent`.

The `SpriteRenderSystem` draws whatever source rectangle the `SpriteComponent` currently holds. It does not know or care about animation — it just draws what is there.

The animation system changes only a crop region. The render system draws that crop. Neither system talks to the other directly. The data (`SpriteComponent`) is the communication channel between them. This is ECS composition working exactly as intended.

```
AnimationSystem:
  every frame → advance timer → update SpriteComponent.sourceRect

SpriteRenderSystem:
  every frame → read SpriteComponent.sourceRect → call DrawTexturePro
```

The systems are decoupled. You could swap the animation system for a different one (scripted animations, inverse kinematics, anything) without touching the render system at all.

---

## 9. Summary

- A texture lives in three places: compressed on disk, raw in CPU RAM, sampled in GPU VRAM. The handle bridges CPU and GPU.
- You pay the upload cost once (at load time), then read from the handle every frame for free.
- A sprite is a crop of a texture, defined by a source rectangle. This is what enables texture atlases.
- Texture atlases pack many frames into one image to avoid GPU state switches, which are expensive.
- A `SpriteComponent` stores a source rectangle and a reference to a texture handle. It lives in `game/` because it uses raylib types.
- A Resource Manager centralises texture loading, prevents duplicate uploads, and manages GPU resource lifetime.
- Animation is just a timer that changes the source rectangle stored in `SpriteComponent` each frame.

**Your immediate next step.** Find a free spritesheet on [OpenGameArt](https://opengameart.org) (search for "platformer character spritesheet"). Add a single `LoadTexture` call in your `main.cpp` before the game loop. In your `RenderSystem`, for one specific entity, replace `DrawRectangle` with a `DrawTexturePro` call using a hardcoded source rectangle pointing at one frame. You do not need a `SpriteComponent` yet. You do not need a Resource Manager yet. The only goal is to see a real sprite on screen. Then you will know exactly what the full system needs to support, and building it will make sense rather than feel abstract.
