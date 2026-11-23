# Seamless — Premium product and integration roadmap

This file records the intended product behavior. It is a specification, not a claim that the features are already implemented. The supplied repository snapshot does not include the Android Gradle project, UI source, playback implementation, or app-widget host code needed to integrate and test these features.

## Product direction

Seamless should feel like a carefully engineered premium offline-first music player. Keep the default appearance restrained and AMOLED-friendly, while allowing the listener to personalize nearly every visual surface.

## 1. Shape-adjustable floating player (Android home-screen widget)

Deliver this as a resizable Android App Widget, not an always-on-top overlay by default. Home-screen widgets are safer, battery-friendlier, and controlled by the launcher. An optional floating overlay would require a separate explicit user-granted "display over other apps" permission and should not be required for core controls.

Requirements:
- Launcher-resizable width and height; provide compact, standard, and expanded layouts that adapt to the available widget size.
- Compact: artwork or abstract visual mark, track title, play/pause, next.
- Standard: artwork, title/artist, play/pause, previous, next, progress preview.
- Expanded: larger artwork, seek/progress, title/artist, previous, play/pause, next, optional lyrics snippet.
- User settings for widget background (transparent/solid/blur-like solid), corner radius, padding, artwork visibility, text visibility, accent, button arrangement, and compactness. Do not promise arbitrary pixel sizing beyond Android launcher resize limits.
- Use Media3 MediaSession/MediaController (or the project's existing session) as the single source of playback state. Widget actions must issue media session commands; never create a second player instance.
- Update metadata and playback state from session callbacks. Use RemoteViews-supported layouts and PendingIntents with immutable/update-current flags.
- Progress preview should be read-only unless a seek bar implementation is added; avoid frequent per-second widget refreshes that drain battery.
- Correctly handle no active track, paused/stopped playback, long titles, missing artwork, process death, and Android 12+ widget sizing APIs.
- Add widget preview/provider metadata and test compact/expanded sizes on multiple launchers.

## 2. Premium app icon identity

The revised launcher foreground replaces the infinity motif with a restrained continuous sound-ribbon mark. Palette: near-black background, muted sage/stone highlights. No lettering, no infinity symbol, no loud gradients. The icon is an initial visual direction and should be reviewed at actual launcher sizes and adaptive-icon masks.

## 3. Theme studio

- True-black AMOLED preset plus optional charcoal surfaces.
- Independent controls for background/surface colors, accent, lyric highlight, text hierarchy, icon treatment, player controls, visualizer, artwork treatment, corner radii, spacing, and typography scale.
- Live preview, reset per section, export/import theme JSON, and named saved presets.
- Keep contrast and touch-target accessibility checks; custom colors must not make controls unreadable.

## 4. Word-synchronized lyrics

- Prefer embedded/provider lyrics with word-level timestamps (LRC enhanced format or equivalent).
- When only line-level lyrics exist, clearly label word sync as unavailable rather than pretending line timing is word timing.
- Alignment/transcription may generate word timings locally, but must be asynchronous, cached per track, cancellable, and visibly identified as generated. Never block playback while alignment runs.
- Drive highlighting from the playback clock / media session position, not UI frame timers. Use interpolation only between known word timestamps; handle seek, pause, speed changes, and track changes.

## 5. Reactive audio visualizer

- Analyze actual playback audio where permitted by the app's playback pipeline; derive low/mid/high energy and transient/beat response.
- Use smoothed envelopes and frame-rate-aware rendering. Pause or reduce rendering when not visible.
- Provide sensitivity, smoothing, and style controls; avoid claiming beat detection from a decorative animation.

## 6. Performance and quality gates

- Keep scrolling and animations on the UI thread only for lightweight drawing; move decoding, lyric alignment, and audio analysis off the main thread.
- Cache artwork and lyric alignment; avoid unbounded allocations in animation loops.
- Add tests for media controls, widget resize states, theme persistence, lyric seeking, and missing metadata.
- Profile jank and memory on a representative low/mid-range Android device before calling the experience release-ready.

## Current snapshot limitation

The provided ZIP includes a manifest, resource XML, CMake file, README, workflow, artwork, and a Whisper JNI source file. It does **not** include `settings.gradle`, `build.gradle`, `gradlew`, `app/build.gradle`, or app Java/Kotlin sources. Therefore this archive cannot produce an APK as supplied, and the requested widget/player/theme/lyrics features cannot be truthfully integrated or runtime-tested from these files alone.
