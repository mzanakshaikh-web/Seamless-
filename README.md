# Seamless

Seamless is intended to be a premium, offline-first Android music player with synchronized lyrics, a reactive audio visualizer, deep visual customization, and a shape-adjustable home-screen playback widget.

## Visual identity
- AMOLED-first near-black launcher background.
- Restrained continuous sound-ribbon icon; no infinity mark and no lettering.
- Muted sage/stone accent colors.

## Product requirements
See [`docs/IMPLEMENTATION-ROADMAP.md`](docs/IMPLEMENTATION-ROADMAP.md) for the detailed requirements for the resizable playback widget, theme studio, word-synchronized lyrics, audio-reactive visualizer, and performance quality gates.

## Important repository status
This ZIP is a resource/native-code snapshot, not a complete Android Studio project. It currently contains no Gradle wrapper or Gradle project files and no Java/Kotlin app source. The GitHub Actions workflow now checks for those files first and reports the missing project structure instead of failing later during SDK setup/build. An APK cannot be built from this snapshot alone.

## Android widget design decision
The requested adjustable mini-player is specified as a resizable Android home-screen App Widget with compact, standard, and expanded layouts. This is preferable to forcing an always-on-top overlay. A separate optional overlay can be considered later, but requires explicit special permission and should not be needed for playback controls.
