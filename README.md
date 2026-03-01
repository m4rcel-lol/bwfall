# Binary Waterfall CLI (bwfall)

**Raw Data Media Player** — Play any binary file as audio and/or visual output.

Binary Waterfall reads any file byte-by-byte and interprets the raw data as audio (PCM samples) and/or visual output (scrolling colored terminal display). No format parsing, no headers — just pure binary data transformed into a surreal audio-visual experience.

## Features

- **Audio playback**: Raw bytes as 8-bit PCM audio via ALSA
- **Visual display**: Scrolling waterfall of colored characters in terminal
- **Multiple modes**: Waterfall, hex dump, binary, matrix, spectrum, waveform
- **Playback control**: Speed adjustment, pause/resume, looping
- **Pipe support**: Read from stdin or files
- **Minimal dependencies**: ALSA + ncurses only

## Installation

### Arch Linux (AUR)

```bash
# Using yay
yay -S bwfall

# Using paru
paru -S bwfall

# Manual
git clone https://aur.archlinux.org/bwfall.git
cd bwfall
makepkg -si
```

### Alpine Linux

```bash
# Build from source
apk add alsa-lib-dev ncurses-dev gcc make musl-dev
git clone https://github.com/yourname/bwfall.git
cd bwfall
make
sudo make install
```

### From Source

```bash
# Dependencies: alsa-lib, ncurses, gcc, make
git clone https://github.com/yourname/bwfall.git
cd bwfall
make
sudo make install PREFIX=/usr/local
```

## Usage

```bash
# Play a binary file
bwfall /bin/ls

# Pipe random data
cat /dev/urandom | bwfall --speed 0.5

# Visual only (no audio)
bwfall --no-audio --visual-mode matrix /usr/bin/python3

# Audio only (no visual)
bwfall --no-visual /boot/vmlinuz

# Play first 4KB of a file
bwfall --length 4096 image.jpg

# Loop playback
bwfall --loop music.mp3

# Different visual modes
bwfall --visual-mode hex file.bin
bwfall --visual-mode spectrum --colormap heat data.db
```

## Interactive Controls

During playback:

- `SPACE` — Pause/Resume
- `q` or `ESC` — Quit
- `→` / `l` — Seek forward
- `←` / `h` — Seek backward
- `↑` / `k` — Increase speed
- `↓` / `j` — Decrease speed
- `+` — Increase volume
- `-` — Decrease volume
- `r` — Restart from beginning
- `m` — Toggle mute
- `v` — Cycle visual mode
- `c` — Cycle colormap

## Options

```
  -i, --input <file>         Input file (or use stdin)
  --no-audio                 Disable audio output
  --no-visual                Disable visual output
  -s, --speed <float>        Playback speed (default: 1.0)
  -l, --loop                 Loop playback
  --audio-mode <mode>        pcm|freq|fm|noise (default: pcm)
  --sample-rate <hz>         Sample rate (default: 44100)
  --volume <0-100>           Volume (default: 80)
  --visual-mode <mode>       waterfall|hex|binary|matrix|spectrum|waveform
  --colormap <map>           rainbow|heat|grayscale|binary
  --fps <n>                  Target FPS (default: 30)
  -v, --verbose              Verbose output
  -h, --help                 Show help
  --version                  Show version
```

## Examples

```bash
# Visualize your shell binary
bwfall /bin/bash

# Play kernel image as audio
bwfall --no-visual /boot/vmlinuz-linux

# Matrix-style falling data
bwfall --visual-mode matrix --colormap binary /dev/urandom

# Hex dump visualization
bwfall --visual-mode hex --no-audio firmware.bin

# Slow motion binary visualization
bwfall --speed 0.2 --visual-mode binary executable

# Play only middle section of file
bwfall --start 1024 --length 2048 data.bin
```

## Building

```bash
make                    # Build binary
make clean              # Clean build artifacts
make install            # Install to PREFIX (default: /usr/local)
make uninstall          # Uninstall
make test               # Run tests
```

## License

GPL-3.0-or-later

## Author

Your Name <you@example.com>

## Contributing

See CONTRIBUTING.md for AUR maintenance and contribution guidelines.
