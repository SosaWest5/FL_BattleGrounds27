# FL_BattleGrounds27

An original nightclub battle-rap rhythm game that loads as a native VST3 effect
in FL Studio's Mixer. Four selectable fictional MCs, a DJ rhythm-game-inspired
four-lane highway, W/A/S/D tap streaks, turn-taking, flow points and crowd control.
The nightclub has an on-stage audience surrounding both MCs and a floor crowd.

One round lasts 30 seconds total: six alternating five-second turns. Each MC
performs six original roast bars. The beat is synthesized in the plugin; all
24 synthetic vocal WAV clips are embedded at build time. No other plugin,
voice engine, browser, sample library or network connection is needed to play.
The bundled vocals sound like stylized computer speech, not live human rapping.

| MC | Overall | Flow | Humor | Attack |
|---|---:|---:|---:|---:|
| Peace-maker | 88 | 87 | 84 | 92 |
| Sunlyt | 94 | 97 | 82 | 94 |
| Plenty | 90 | 89 | 98 | 84 |
| BASHH | 92 | 93 | 76 | 98 |

Ratings are original game design values. These characters are fictional.
Funny/attack ratings add a punchline bonus to successful notes. AI flow rating
influences its timing success. Streaks and player timing remain decisive.

## Windows plugin via GitHub website

Follow START-HERE.txt. The source ZIP is not an installable plugin.
The Actions artifact contains the compiled VST3, installer, instructions and an
optional standalone EXE. Install the VST3 under Program Files/Common Files/VST3
then scan with verification and use an empty Mixer effect slot. See INSTALL.txt.

## Build locally

Requires Git, CMake 3.22+, Visual Studio 2022 with Desktop development with C++.
JUCE 8.0.6 is downloaded when building. The provided WAV assets are already
included: you do not need eSpeak or FFmpeg to build or run the game.

```sh
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
```

Output: `build/FL_BattleGrounds27_artefacts/Release/VST3/FL_BattleGrounds27.vst3`.

Gameplay-only tests need no JUCE or audio dependencies:

```sh
cmake -S . -B build-tests -DBATTLE_ENGINE_ONLY=ON
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

Optional native smoke test: `-DBATTLE_SMOKE_TEST=ON`, then run BattleSmoke with
an absolute writable output directory to save actual UI previews.
Linux GUI tests require a display server (a virtual display works).

## Code and assets

- Source/Battle.h: 24 original roast bars, MC ratings, charts and scoring engine.
- Source/PluginEditor.cpp: selection UI, nightclub, crowd meter, note highway/input.
- Source/PluginProcessor.cpp: embedded vocals, synthesized beat, clock, passthrough.
- Assets/*.wav: 24 generated synthetic vocal clips, embedded into BinaryData.
- Assets/generate_vocals.py: optional build-time regeneration with eSpeak NG/FFmpeg.
- Tests: deterministic matchup/scoring tests and native audio/render smoke checks.

The user-facing game uses its own code, artwork, music and lyrics; no DJ Hero
code, music, logos or assets are included. JUCE is a separately licensed build
dependency. Review its licensing before distributing a commercial product.
The eSpeak engine is not distributed or required at runtime.

Current scope: solo versus AI, keyboard controls, one 30-second round. No online
play or gamepad support. Saved project preferences exclude active battle progress.
