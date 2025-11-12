# voxel-engine

A cross‑platform voxel engine. This repo is configured to build on Windows using Visual Studio Insider so it can use Microsoft’s latest C/C++ compiler and toolset (MSVC vNext).

## Prerequisites (Windows)
- Visual Studio 2026 (Insider) — latest available build
- Workload: "Desktop development with C++"
- Components (checked in the VS Installer):
  - MSVC toolset (latest vNext/Preview)
  - Windows 10/11 SDK (latest)
  - CMake tools for Windows (optional)

## Getting started
1. Install Visual Studio Insider/Preview via Visual Studio Installer (Preview channel).
2. Ensure the latest MSVC toolset is installed (VS Installer → Modify → Individual components → "MSVC vNext" / latest MSVC).
3. Open the solution: `voxel-engine.slnx`.
4. In "Configuration Manager", select:
   - Platform: `x64`
   - Configuration: `Debug` or `Release`
5. Build: Ctrl+Shift+B

## Verify the latest compiler is used
- Project → Properties → General → Platform Toolset → ensure it points to the newest toolset (e.g., "Visual Studio v150").
- Help → About Microsoft Visual Studio should show an Insider/Preview build.

## Notes
- Third‑party headers and libraries live under `extern\include` and `extern\lib` and are referenced by the Windows projects.
- Windows‑specific sources are under `Windows\source` and headers in `Windows\include`.

## Troubleshooting
- If the toolset field is missing the vNext/Preview option, reopen the VS Installer and add the latest MSVC components.
- If linking fails, confirm you selected the `x64` platform to match the provided libraries in `extern\lib\x64`.

