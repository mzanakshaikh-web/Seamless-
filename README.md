# Seamless

Seamless is an Android music player with gapless/crossfade playback, synchronized lyrics, a reactive beat circle, and an AMOLED theme.

## Updated in this project
- Seven beat-circle visual styles: Bars, Rings, Flutter, Wave, Orbit, Particles, and Spiral.
- Original generated fallback cover artwork is used when a track has no embedded artwork.
- The lyrics panel refreshes partial Whisper segments more frequently while local transcription is running.
- Minimal infinity launcher icon resources are retained.
- GitHub Actions fetches whisper.cpp and the quantized speech model, then builds an ARM64 APK. If the native engine build fails, it produces a fallback APK without offline transcription; check the release notes and build log.

## Lyrics and permissions
Grant Seamless access to your audio library when Android asks. For an existing local music library, open Settings → Lyrics → **Listen to all songs** to run on-device transcription across tracks that do not already have lyrics. The **Listen automatically** option transcribes the currently played track when lyrics are unavailable.

Android does not provide an ordinary permission that lets a third-party app silently capture and transcribe every sound from every other app. Capturing other apps' playback requires Android's user-approved MediaProjection playback-capture flow and only works for source apps that allow capture. Seamless therefore transcribes audio files in its own permitted media library, not arbitrary system-wide audio.

Whisper transcription is local once the model is installed. The first GitHub build downloads the model and may take longer; the model increases APK size substantially. If GitHub Actions falls back to the non-Whisper build, offline audio transcription will not be available in that APK.

## Build
Push the repository to GitHub and run **Build Seamless APK** under Actions, or let it run after a push to `main`/`master`. The APK is published as the `latest` GitHub Release.
