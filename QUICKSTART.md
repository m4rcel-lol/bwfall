# Quick Start Guide - Binary Waterfall CLI

## Installation

### Arch Linux
```bash
yay -S bwfall
```

### From Source
```bash
# Install dependencies (Arch/Manjaro)
sudo pacman -S alsa-lib ncurses gcc make

# Install dependencies (Debian/Ubuntu)
sudo apt install libasound2-dev libncurses-dev gcc make

# Install dependencies (Alpine)
sudo apk add alsa-lib-dev ncurses-dev gcc make musl-dev

# Build and install
git clone https://github.com/yourname/bwfall.git
cd bwfall
make
sudo make install
```

## Basic Usage

### Play a binary file
```bash
bwfall /bin/ls
```

This will:
- Play the raw bytes as 8-bit PCM audio through your speakers
- Display a scrolling colored "waterfall" of the data in your terminal

### Pipe data
```bash
cat /dev/urandom | bwfall --speed 0.5
```

### Visual only (no audio)
```bash
bwfall --no-audio /usr/bin/python3
```

### Audio only (no visual)
```bash
bwfall --no-visual /boot/vmlinuz
```

## Common Options

```bash
# Slow down playback
bwfall --speed 0.5 file.bin

# Loop forever
bwfall --loop file.bin

# Play only first 4KB
bwfall --length 4096 file.bin

# Different visual style
bwfall --visual-mode matrix /dev/urandom
bwfall --visual-mode hex firmware.bin

# Different colors
bwfall --colormap heat data.db
bwfall --colormap binary /bin/bash

# Adjust volume
bwfall --volume 50 file.bin

# Start paused
bwfall --pause file.bin
```

## Visual Modes

- `waterfall` - Scrolling colored blocks (default)
- `hex` - Hexadecimal dump display
- `binary` - Binary 0/1 representation
- `matrix` - Matrix-style falling data
- `spectrum` - Byte distribution histogram
- `waveform` - ASCII waveform

## Colormaps

- `rainbow` - Full color spectrum (default)
- `heat` - Heat map (blue → yellow → red)
- `grayscale` - Monochrome
- `binary` - Black and white only

## Fun Examples

```bash
# Visualize your shell
bwfall /bin/bash

# Play a JPEG as audio
bwfall --no-visual photo.jpg

# Matrix-style random data
bwfall --visual-mode matrix --colormap binary /dev/urandom

# Slow-motion binary visualization
bwfall --speed 0.2 --visual-mode binary /usr/bin/ls

# Hex dump with heat colors
bwfall --visual-mode hex --colormap heat firmware.bin

# Loop a small file
bwfall --loop --length 1024 /dev/urandom
```

## Tips

1. **Start with visual-only** if you're not sure what a file will sound like:
   ```bash
   bwfall --no-audio file.bin
   ```

2. **Use --length** to preview just the beginning:
   ```bash
   bwfall --length 4096 largefile.bin
   ```

3. **Adjust speed** for better visualization:
   ```bash
   bwfall --speed 0.3 file.bin
   ```

4. **Pipe interesting data sources**:
   ```bash
   cat /proc/meminfo | bwfall
   dmesg | bwfall
   ps aux | bwfall
   ```

## Troubleshooting

### No audio output
- Check ALSA is working: `speaker-test -t wav -c 2`
- Try adjusting volume: `bwfall --volume 100 file.bin`
- Use visual-only mode: `bwfall --no-audio file.bin`

### Terminal display issues
- Ensure your terminal supports colors
- Try a different terminal emulator
- Use audio-only mode: `bwfall --no-visual file.bin`

### File not found
- Use absolute paths: `bwfall /full/path/to/file`
- Check file permissions: `ls -l file.bin`

## Getting Help

```bash
bwfall --help
man bwfall
```

## Exit

Press `Ctrl+C` to stop playback and exit.

---

**Have fun exploring the raw data universe!** 🌊
