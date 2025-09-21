# Copilot / AI contributor notes — BloxEngine

This file contains concise, actionable guidance for AI coding agents working in this repository. Focus on patterns and commands that are discoverable in the tree so you can be immediately productive.

1) Big picture
- BloxEngine is a small C/C++ game engine split into: core (src/core), rendering (src/rendering), internal platform/opengl (internal/), physics (internal/physicsWorld.c, src/physics/) and small sandbox & unit-test apps under `sandbox/` and `unit-test/`.
- The engine is built as a Windows DLL (`bin/BloxEngine.dll`) and the sandbox/unit-tests link against it. Look at the top-level `Makefile` to see DLL build flags and exported `BLXAPI` conventions.

2) Where to look first (examples)
- Entry & app lifecycle: `src/core/blx_application.h`, `src/blx_entry.h`, `src/core/blx_gameType.h`
- Rendering API surface: `src/rendering/blx_rendering.h`, `src/rendering/blx_shader.c`, `src/rendering/blx_material.c`, `internal/opengl.c`
- Resource & memory helpers: `src/core/blx_memory.h`, `src/core/blx_freeList.h`, `src/core/blx_string.h`
- Build and packaging: top-level `Makefile`, `sandbox/Makefile`, `unit-test/Makefile`, and `buildCopy.bat` (copies built DLL and headers to dependent folders).

3) Build / test / debug workflows (explicit)
- Build engine (root DLL): from PowerShell run `make` in repo root. The root `Makefile` target `all` runs `configure`, `scaffold`, `compile`, `link` and produces `bin/BloxEngine.dll`.
- Build & run sandbox (fast path): run the VS Code task `Build Engine` or manually:
```powershell
.\buildCopy.bat sandbox; cd sandbox; make -f Makefile all
```
  This sequence runs the engine make, then copies `bin/include`/`bin/builtin` into `sandbox/Dependencies/BloxEngine` and builds `sandbox/bin/sandbox.exe`.
- Build unit tests: similar to sandbox; run the task `Build Tests` or:
```powershell
.\buildCopy.bat unit-test; cd unit-test; make -f Makefile all
```
- Run the sandbox executable: `sandbox\bin\sandbox.exe` (ensure `BloxEngine.dll` and `builtin/` are in `sandbox/bin`).
- Debug: builds include `-g` and produce PDBs in `sandbox/bin` when building the sandbox target. Use your Windows debugger (Visual Studio / WinDbg) pointed at `sandbox/bin/sandbox.exe` with `BloxEngine.dll` in the same directory.

4) Project-specific conventions and patterns
- Mixed C and C++: the root `Makefile` compiles `.c` and `.cpp` sources. Many public APIs use the `BLXAPI` export macro and `blx` prefix. Prefer `blx`-prefixed functions for engine-facing changes.
- Header publishing: build scaffolding copies headers to `bin/include/BLX` and third-party headers to `bin/include/*`. When adding public APIs, update headers under `src/*.h` (the Makefile copies them into `bin/include` during `scaffold`).
- Object layout: object files are placed under `obj-bin/...` mirroring `src/` paths. When adding source files, follow existing directory structure so compiled objects land in the correct place.
- Custom containers: code uses custom list types (e.g., `vList_blxVertex`, `vList_indices`) and small memory arenas. Search for `vList_` and `blxInitListWithSize` to find usage patterns.

5) Integration points & external deps
- Third-party libs are vendored under `Dependencies/` (CGLM, GLEW, GLFW). The Makefiles reference these with relative include and lib paths (see `INCLUDE_FLAGS` and `LINKER_FLAGS` in `Makefile`).
- Graphics backend: renderer interfaces (`blxInitRenderer`, `blxDrawModel`, `blxUpdateMesh`) are implemented in `src/rendering` and `internal/opengl.c`. Modifying rendering behavior usually requires touching both the public header and the `internal/opengl` implementation.
- Sandbox linking: `sandbox/Makefile` links against `-lBloxEngine` and uses `Dependencies/BloxEngine` as a local dependency; the `buildCopy.bat` step is required to set up that dependency directory.

6) Code patterns and examples (copy-paste friendly)
- Exported function pattern: `BLXAPI void blxInitRenderer(GraphicsAPI graphicsToUse);` — used throughout rendering code.
- App lifecycle: `blxCreateApplication(...)` and `blxRunApplication()` are called by the entry in `src/entry.c` / `sandbox/src/entry.c`.

7) Quick checks to avoid common mistakes
- Always ensure `bin/include` is populated (run root `make` or `scaffold`) before building `sandbox/` or `unit-test/`.
- The build assumes Windows/MSVC-style system libs are available (`-llibcmt`, `-luser32`, etc.) and uses absolute relative paths to `Dependencies` libs; do not change linker flags without testing locally on Windows.
- When modifying public headers, know the Makefile will copy `src/*.h` into `bin/include/BLX` — update header names/guards consistently.

8) Where automated agents can safely act (low-risk)
- Small bug fixes inside a single module (e.g., `src/core/blx_logger.c`) — follow existing style and use `blxLog()` patterns.
- Add new renderer helpers by updating `src/rendering/*` and corresponding declarations in `src/rendering/*.h` and then run the scaffold+build.

9) When to ask for human review
- Any changes to linker flags, build scripts (`Makefile`, `sandbox/Makefile`, `buildCopy.bat`) or dependency versions.
- Large API changes that alter `BLXAPI` signatures or public header structure.

If anything above is unclear or you'd like short examples for a specific subsystem (rendering, memory, app lifecycle), tell me which area and I'll expand the instructions and add inline code examples.
