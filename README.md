# Seamless

A complete, minimal Android Studio/Gradle starter repository for a local AMOLED music player.

## Build an APK
1. Create a new public or private GitHub repository named `Seamless`.
2. Upload the **contents** of this ZIP (the `.github` and `app` folders plus Gradle files) to the repository root.
3. Open **Actions** → **Build Seamless APK**. It also runs automatically after a push to `main` or `master`.
4. When the run finishes, download the `Seamless-APK` artifact from the run's Artifacts section.

## Included in this foundation
- Android project structure and valid Gradle configuration (no obsolete `sdkmanager tools` package).
- AMOLED-black interface and local audio library browsing.
- Basic play, pause, next/previous, and seek controls.
- Resizable Android home-screen widget with play/pause and open-player actions.
- Small accent palette selector.

## Honest feature status
This is a working foundation, not a Spotify-equivalent finished product. Word-level lyric alignment, Whisper transcription, true gapless/crossfade playback, a beat-reactive visualizer, persistent full theme studio, and background media-session controls are **not implemented in this starter**. The widget's Next action opens the app; it does not yet advance playback while the app process is closed. These require further implementation and device testing. The build workflow compiles the Android app; it does not certify every device feature.

The original uploaded repository contained only 17 top-level files and no Gradle project or Android application source. This archive supplies the missing buildable project foundation instead of pretending the old files were a complete app.
