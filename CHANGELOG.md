# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-03-01

### Added
- Initial release
- Audio playback via ALSA (8-bit PCM)
- Visual waterfall display via ncurses
- Multiple visual modes: waterfall, hex, binary, matrix, spectrum, waveform
- Multiple colormaps: rainbow, heat, grayscale, binary
- Playback controls: speed, pause/resume, loop
- Stdin pipe support
- Byte range selection (--start, --length)
- Command-line interface with comprehensive options
- Arch Linux AUR package
- Alpine Linux APKBUILD
- Man page documentation

### Features
- Lock-free ring buffer for thread synchronization
- Separate threads for file reading, audio, and visual output
- Graceful signal handling (SIGINT, SIGTERM)
- Terminal resize support (SIGWINCH)
- Volume control
- FPS limiting for visual output

[1.0.0]: https://github.com/m4rcel-lol/bwfall/releases/tag/v1.0.0
