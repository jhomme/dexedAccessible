# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Dexed is a multi-platform, multi-format (VST3/AU/CLAP/Standalone) FM synth plugin modeled on the Yamaha DX7, built on JUCE. The synthesis engine is a C++ port of Google's `music-synthesizer-for-android` ("msfa"), kept under the Apache 2.0 license (vs. GPLv3 for the rest of the project) specifically so it can be shared across projects — do not relicense or restructure `Source/msfa`.

Project goal/constraint: stay compatible with the original DX7. Anything that adds capability beyond the DX7 (new engine features, float params, extra waveforms) belongs in a fork, not here.

## Build

Submodules must be initialized before the first build:
```
git submodule update --init --recursive
```

Configure and build with CMake (out-of-source, `build/` dir):
```
mkdir build && cd build
cmake .. -DJUCE_COPY_PLUGIN_AFTER_BUILD=TRUE
cmake --build .
```
Binaries land in `build/Source/Dexed_artefacts/*`.

Useful CMake options:
- `-DBUILD_ID=<id>` — sets `DEXED_ID`, used for nightly/release version stamping (CI sets this to a date+hash or the tag name).
- `-DDEXED_SKIP_VST3=ON` — skip building the VST3 target.
- `-DDEXED_NO_ALSA=ON` — build without ALSA (Linux).
- `--target installer` — builds the packaged installer target (what CI uses) instead of just the plugin.

On Linux, missing headers usually mean missing dev packages — see the "Linux build dependencies" wiki page linked from README.md. CI installs: `libasound2-dev libx11-dev libxinerama-dev libxext-dev libfreetype6-dev libwebkit2gtk-4.1-dev libglu1-mesa-dev xvfb libjack-dev libsndfile-dev`.

There is no test suite in this repository — changes are validated by building and loading the plugin/standalone in a DAW or host.

CI (`.github/workflows/project-pipeline.yml`) builds Linux/Windows/macOS in parallel and publishes to a rolling "Nightly" GitHub release on branch pushes, or to a tagged release when a tag is pushed.

## Architecture

- `Source/msfa/` — the DX7 synthesis engine (oscillators, envelopes, LFO, FM core, portamento, tuning). This is the DSP core; it is format-agnostic and has no JUCE dependency. Apache 2.0 licensed, kept separate from the GPLv3 plugin code on purpose.
- `Source/EngineMkI.*`, `Source/EngineOpl.*` — alternate synthesis engine backends layered on top of msfa (Mark I emulation and an OPL-series engine). `PluginProcessor` selects between engines.
- `Source/PluginProcessor.*` — the JUCE `AudioProcessor`: audio/MIDI I/O, parameter handling, sysex handling glue, owns a `Dexed`/engine instance per voice architecture.
- `Source/PluginEditor.*`, `GlobalEditor.*`, `OperatorEditor.*`, `AlgoDisplay.*`, `DXComponents.*`, `DXLookNFeel.*`, `VUMeter.*` — the UI layer (JUCE components), split roughly into the global/voice editor, per-operator editor, FM algorithm diagram, and shared custom widgets/look-and-feel.
- `Source/PluginParam.*` — plugin parameter definitions and the mapping between DX7 sysex/voice data and JUCE `AudioProcessorParameter`s.
- `Source/SysexComm.*`, `Source/CartManager.*`, `Source/ProgramListBox.*` — DX7 sysex import/export and cartridge (bank of 32 voices) management; see `Documentation/sysex-format.txt` for the wire format.
- `Source/PluginData.*` — built-in factory programs/cartridges.
- `Source/TuningShow.*` — microtuning (SCL/KBM, MTS-ESP) UI, backed by `libs/tuning-library` and `libs/MTS-ESP`.
- `Source/ParamDialog.*` — in-window parameter dialogs (these used to be separate windows; now embedded in the main plugin window).

### Libraries (`libs/`, mostly git submodules)
- `JUCE` — app/plugin framework and wrapper (audio formats, GUI).
- `vst3sdk` — VST3 format support.
- `clap-juce-extensions` — CLAP format support via JUCE.
- `tuning-library` / `surgesynthteam_tuningui` — microtuning (SCL/KBM) support and its UI, from Surge Synth Team.
- `MTS-ESP` — MTS-ESP microtuning client, linked directly into the `Dexed` target in `Source/CMakeLists.txt` (not yet its own CMake subproject).

### Build graph
Top-level `CMakeLists.txt` pulls in JUCE and `clap-juce-extensions`, then `add_subdirectory(libs)`, `add_subdirectory(assets)`, `add_subdirectory(Source)`. `assets/` builds `DexedResources` (binary data: UI images, fonts, the built-in program zip) via `juce_add_binary_data`. `Source/CMakeLists.txt` defines the `Dexed` plugin target via `juce_add_plugin`, lists all sources (including `msfa/*`), and links JUCE modules, `tuning-library`, `surgesynthteam_tuningui`, and `DexedResources`.
