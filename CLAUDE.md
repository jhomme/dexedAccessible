# Dexed Accessible

A fork of asb2m10/dexed, the DX7 FM synth plugin. The long-term goal is VoiceOver and screen
reader accessibility for the plugin UI. The user, jhomme, relies on VoiceOver and cannot use a
mouse, so describe any UI testing as VoiceOver testing, not generic manual testing.

## Communication

When explaining what you want to do, are doing, or have done, or in writing any documentation
including plans, use plain language without dumbing the content down. For example, say what
"pr" is in plain words, but don't replace what it connotes. Also follow these style rules:

- No bold.
- No m dashes.
- No n dashes.
- Prefer bulleted lists to comma-separated ones.
- Prefer lists to tables.
- Use commas rather than parentheses.
- Avoid choppy phrasing.
- No AI filler, such as actually.
- Write conversationally.
- Write in pyramid style. Put the most important content first. Save the detail for afterward.
  Keep the detail in the same order as the up-front summary.
- Explain the consequences of your planned actions.
- Remember that I may not know the programming language you are using.

## What this is

Dexed is a multi-platform, multi-format FM synth plugin modeled on the Yamaha DX7, built on
JUCE. It supports VST3, AU, CLAP, and Standalone. The synthesis engine is a C++ port of
Google's music-synthesizer-for-android, called msfa, which stays under the Apache 2.0 license
instead of the project's GPLv3, specifically so it can be shared across projects. Do not
relicense or restructure `Source/msfa`.

The project's goal is to stay compatible with the original DX7. Anything that adds capability
beyond the DX7, like new engine features, float parameters, or extra waveforms, belongs in a
fork, not here.

## Remotes

- `origin` is `https://github.com/jhomme/dexedAccessible.git`, this fork.
- `upstream` is `https://github.com/asb2m10/dexed.git`, the real project.

## Branches

- `master` tracks upstream, with no local changes.
- `fix/parameter-registration-crash`, a bug fix branch, opened as a PR upstream. See History
  below.
- `accessibility`, where the accessibility work happens, now the only actively developed
  branch. It has the crash fix work merged in directly, so it never depended on upstream
  accepting that PR.

## History so far

1. Vendored JUCE was upgraded to 8.0.15, which broke some API usage. See `cfab624`.
2. That upgrade introduced a crash on any slider or control interaction, fixed in `6da8345`.
3. Added a headless regression test suite, `DexedTests`, to catch this kind of break in CI.
   See `d969d33` and `ea897f9`.
4. Opened upstream PR asb2m10/dexed#534 from `fix/parameter-registration-crash`, carrying the
   JUCE upgrade, the crash fix, and the regression test. It is open and has not been reviewed.
5. Merged `fix/parameter-registration-crash` into `accessibility` directly, instead of waiting
   for PR 534 to be reviewed. Upstream asb2m10/dexed moves slowly: several pull requests have
   sat open for over a year, and others were closed without being merged after months. Treat
   upstream review as a nice to have, never a dependency for this fork's own progress.
6. Removed `ComboBoxAccessibilityHandlerFix` from `Source/DXComponents.h`. It was a temporary
   workaround for a ComboBox accessibility bug, and JUCE 8.0.15 now fixes that same bug
   internally, so the workaround was redundant.

## Build

Submodules must be initialized before the first build:
```
git submodule update --init --recursive
```

Configure and build with CMake, out of source, in a build directory:
```
mkdir build && cd build
cmake .. -DJUCE_COPY_PLUGIN_AFTER_BUILD=TRUE
cmake --build .
```
Binaries land in `build/Source/Dexed_artefacts`.

Useful CMake options:
- `-DBUILD_ID=<id>` sets `DEXED_ID`, used for nightly or release version stamping. CI sets
  this to a date and hash, or to the tag name.
- `-DDEXED_SKIP_VST3=ON` skips building the VST3 target.
- `-DDEXED_NO_ALSA=ON` builds without ALSA, for Linux.
- `--target installer` builds the packaged installer target, the same one CI uses, instead of
  just the plugin.

On Linux, missing headers usually mean missing dev packages. See the Linux build dependencies
wiki page linked from README.md. CI installs `libasound2-dev`, `libx11-dev`,
`libxinerama-dev`, `libxext-dev`, `libfreetype6-dev`, `libwebkit2gtk-4.1-dev`,
`libglu1-mesa-dev`, `xvfb`, `libjack-dev`, and `libsndfile-dev`.

There is a small headless regression test suite in `Source/Tests`, built as the `DexedTests`
CMake target. Run it with:
```
cmake --build . --target DexedTests
ctest
```
It covers plumbing level regressions like parameter registration. It does not cover DSP
correctness, UI behavior, or host automation, and it does not touch the editor at all, so it
gives no coverage for accessibility work. Run it after any change touching parameter
registration, sliders, or JUCE API usage, since that is the area that broke last time. For
everything else, including all accessibility changes, validate by building and loading the
plugin or standalone in a DAW or host, using VoiceOver to check the actual behavior.

CI, in `.github/workflows/project-pipeline.yml`, builds Linux, Windows, and macOS in parallel,
and publishes to a rolling Nightly GitHub release on branch pushes, or to a tagged release
when a tag is pushed.

## Architecture

- `Source/msfa` holds the DX7 synthesis engine: oscillators, envelopes, LFO, FM core,
  portamento, tuning. This is the DSP core. It is format agnostic and has no JUCE dependency.
  It is Apache 2.0 licensed, kept separate from the GPLv3 plugin code on purpose.
- `Source/EngineMkI` and `Source/EngineOpl` are alternate synthesis engine backends layered on
  top of msfa, a Mark I emulation and an OPL series engine. `PluginProcessor` selects between
  engines.
- `Source/PluginProcessor` is the JUCE `AudioProcessor`: audio and MIDI I/O, parameter
  handling, sysex handling glue, and it owns a `Dexed` or engine instance per voice
  architecture.
- `Source/PluginEditor`, `GlobalEditor`, `OperatorEditor`, `AlgoDisplay`, `DXComponents`,
  `DXLookNFeel`, and `VUMeter` make up the UI layer, the JUCE components. Roughly: the global
  or voice editor, the per-operator editor, the FM algorithm diagram, and shared custom
  widgets and look and feel.
- `Source/PluginParam` defines plugin parameters and maps DX7 sysex or voice data to JUCE
  `AudioProcessorParameter`s.
- `Source/SysexComm`, `Source/CartManager`, and `Source/ProgramListBox` handle DX7 sysex
  import and export, and cartridge management, a cartridge being a bank of 32 voices. See
  `Documentation/sysex-format.txt` for the wire format.
- `Source/PluginData` holds the built-in factory programs and cartridges.
- `Source/TuningShow` is the microtuning UI, for SCL, KBM, and MTS-ESP, backed by
  `libs/tuning-library` and `libs/MTS-ESP`.
- `Source/ParamDialog` holds in-window parameter dialogs. These used to be separate windows,
  and are now embedded in the main plugin window.

### Libraries, in `libs`, mostly git submodules
- `JUCE` is the app and plugin framework and wrapper, for audio formats and GUI.
- `vst3sdk` provides VST3 format support.
- `clap-juce-extensions` provides CLAP format support via JUCE.
- `tuning-library` and `surgesynthteam_tuningui` provide microtuning, SCL and KBM, support and
  its UI, from Surge Synth Team.
- `MTS-ESP` is the MTS-ESP microtuning client, linked directly into the `Dexed` target in
  `Source/CMakeLists.txt`. It is not yet its own CMake subproject.

### Build graph
The top-level `CMakeLists.txt` pulls in JUCE and `clap-juce-extensions`, then adds the libs,
assets, and Source subdirectories. `assets` builds `DexedResources`, binary data such as UI
images, fonts, and the built-in program zip, via `juce_add_binary_data`.
`Source/CMakeLists.txt` defines the `Dexed` plugin target via `juce_add_plugin`, lists all
sources including msfa, and links JUCE modules, `tuning-library`, `surgesynthteam_tuningui`,
and `DexedResources`.

## Build targets

- This Mac is Apple silicon. Every macOS build must include arm64. Never produce an
  x86_64-only binary.
- Before the first build, read the CMake files. Say which architectures the project builds
  for, and where it installs the app and plugins.
- After every build, run the file command on each built binary and show the output. Stop if
  any binary lacks arm64.
- Before installing a build, list every existing Dexed copy in `/Applications`,
  `/Library/Audio/Plug-Ins`, and `~/Library/Audio/Plug-Ins`, with its architecture. Ask before
  overwriting or deleting any of them.

## Plan

- Accessibility work proceeds on `accessibility` now, regardless of PR 534's status upstream.
  Upstream review is not a dependency for anything here.
- Next: `EnvDisplay` and `PitchEnvDisplay`, in `Source/DXComponents.h`, need real accessibility
  handlers. They are custom-painted envelope diagrams with no title or role right now, so
  VoiceOver reports them as unnamed. Follow the pattern already in the repo, `ProgramLabelAH`
  in `Source/ProgramListBox.h`: a nested `AccessibilityHandler` subclass with a `getTitle`
  method describing the current envelope values, returned from a `createAccessibilityHandler`
  override. Also fire a `valueChanged` accessibility event wherever these displays currently
  repaint in response to a parameter change, so VoiceOver announces live edits, not just the
  first time a display gets focus. `LcdDisplay`, in the same file, is a simpler version of the
  same pattern: announce `paramMsg`, and fire `valueChanged` from `setSystemMsg`.
- After that: `AlgoDisplay`, in `Source/AlgoDisplay.h` and `.cpp`, the algorithm routing
  diagram. This is harder, since describing 6 operators, feedback loops, and carrier or
  modulator routing in words is its own small design problem. It deserves its own pass once
  the simpler displays prove out the pattern.
- Future fixes and features belong on branches off `accessibility`. Upstream-bound bug fixes
  unrelated to accessibility can still go out as their own PRs off `master`, the way 534 did,
  but they are never a gate for anything on `accessibility`.
