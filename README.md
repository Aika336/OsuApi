
# OsuApi

OsuApi is a library that reads live game state directly from osu!lazer process memory.

Currently exposes: **current combo**, **active mods**, and **whether a map is being played**.

> **Platform:** Windows  
> **Game version:** osu! lazer

---

## How it works

osu! lazer is a .NET application, so its internal objects live on the managed heap at addresses that change every time the game restarts. To find them reliably, the library uses a **signature scan**:

1. A known byte pattern is searched across the game's committed memory pages.
2. The match gives a stable anchor — the address of the `OsuGame` base object — with a fixed offset applied after the pattern.
3. From there, everything else is reachable by following pointer chains using the offsets in `OsuOffset.h`.

For reading .NET strings (like the mods JSON), there's a dedicated `DotNetString` reader that handles the CLR string layout (`length` at `+0x8`, UTF-16 data at `+0xC`).

```
Signature scan → OsuGame base
    └─ ScreenStack → current screen (Player)
            ├─ ScoreProcessor → combo value
            ├─ Score → ScoreInfo → mods JSON → parse acronyms
            └─ API + ScoreManager pointers → compare with game-level → is_playing
```

### Why ntdll?

All process interaction in this version goes through `ntdll.dll` (the Windows NT native API) rather than the standard WinApi layer in `kernel32.dll`.  
Security software (antivirus, EDR) typically intercepts calls at the Win32 level using `ReadProcessMemory` and similar functions. By resolving core NT functions at runtime using `GetProcAddress` in ntdll.dll, these interceptions are bypassed. Functions are lazily loaded, so there is no overhead for functions that are not called.

---

## Project structure

```
OsuApi/
├── NtDll/
│   ├── include/
│   │   ├── ntdll.h      # NtDll wrapper class + GetNtDll() singleton
│   │   └── domain.h     # NT function pointer typedefs, LoadModule helper
│   └── src/
│       ├── ntdll.cpp
│       └── domain.cpp
├── include/
│   ├── Logger/              # Macros for event logging
│   ├── Memory/              # NtReadVirtualMemory + .NET string reader
│   ├── MemoryScanning/      # Signature matcher
│   ├── ProcessManager/      # Process discovery, handle management (RAII)
│   └── ProvideOsuInfo/      # osu!-specific logic: offsets, state extraction
│       └── OsuOffsets/      # Offsets for finding values
├── src/
├── main.cpp                 # Demo: prints combo and mods
└── CMakeLists.txt
```

### Modules

| Module | What it does |
|---|---|
| `NtDll` | Loads `ntdll.dll` via `GetModuleHandle` and resolves NT functions on demand. Provides a singleton `ntdll::GetNtDll()` used by every other module that touches process memory. |
| `ProcessManager` | Enumerates running processes via `NtQuerySystemInformation(SystemProcessInformation)`, finds osu! by name, opens a handle via `NtOpenProcess`. `HandleRaii` ensures the handle is always closed. |
| `MemoryScanning` | `PatternMatcher` holds a signature + mask and scans a byte buffer for a match. `ProcessMemoryScanner` walks all committed, accessible memory regions via `NtQueryVirtualMemory` and runs the matcher against each. |
| `Memory` | Two thin wrappers around `NtReadVirtualMemory`: `ReadAs<T>` for fixed-size values, `ReadMemoryRegion` for raw byte chunks. Both return `std::optional` — no exceptions on read failure. |
| `ProvideOsuInfo` | Knows the osu! object layout. `OsuInfoProvider` walks pointer chains to extract combo, mods, and playing state. `OsuHandler` is the public-facing class that owns the provider and exposes `UpdateGameState()` / `GetGameState()`. |
| `Logger` | Minimal — `LOG(msg)` goes to stdout, `LOG_ERROR(msg)` to stderr. Both prepend `__FUNCSIG__` so you always know where the message came from. |

---

## Building

Requirements:
- Windows (MSVC)
- CMake 3.11+
- A C++20 compiler

```bash
git clone https://github.com/yourname/OsuApi
cd OsuApi
mkdir build
cd build
cmake ..
cd ..
cmake --build build --config Release
```

The demo executable ends up at `build/Release/OsuApi.exe`

---

## Usage

```cpp
#include <OsuHandler.h>

int main() {
    // Throws if osu! isn't running or the signature isn't found
    OsuHandler handler;

    while (true) {
        handler.UpdateGameState();
        const GameState state = handler.GetGameState();

        if (state.is_playing) {
            std::cout << "Combo: " << state.current_combo << "\n";
            for (const auto& mod : state.current_mods)
                std::cout << "[" << mod << "]";
            std::cout << "\n";
        } else {
            std::cout << "Not in a map\n";
        }

        Sleep(300);
    }
}
```

`GameState` fields:

| Field | Type | Description |
|---|---|---|
| `is_playing` | `bool` | `true` when the Player screen is active and its API/ScoreManager pointers match the game's |
| `current_combo` | `int` | Current combo; `-1` if unreadable or not playing |
| `current_mods` | `vector<string>` | Mod acronyms from the score's JSON field (e.g. `"HD"`, `"DT"`); empty list if no mods |
