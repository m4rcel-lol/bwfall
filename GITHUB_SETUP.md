# GitHub Repository Setup Guide

## Creating the Repository

1. **Create a new repository on GitHub:**
   - Name: `bwfall`
   - Description: "Binary Waterfall CLI - Raw Data Media Player for Linux"
   - Public repository
   - Do NOT initialize with README (we have one)

2. **Push the code:**
   ```bash
   cd /home/m5rcel/binary-waterfall
   git init
   git add .
   git commit -m "Initial commit: bwfall v1.0.0"
   git branch -M main
   git remote add origin git@github.com:yourname/bwfall.git
   git push -u origin main
   ```

3. **Create the first release:**
   ```bash
   git tag -a v1.0.0 -m "Release v1.0.0"
   git push origin v1.0.0
   ```

4. **Create GitHub Release:**
   - Go to: https://github.com/yourname/bwfall/releases/new
   - Tag: `v1.0.0`
   - Title: `bwfall 1.0.0 - Initial Release`
   - Description: Copy from CHANGELOG.md
   - Attach tarball: GitHub will auto-generate from tag

## Verify Release Tarball URL

The AUR PKGBUILD expects this URL format:
```
https://github.com/yourname/bwfall/archive/refs/tags/v1.0.0.tar.gz
```

Test it:
```bash
curl -L https://github.com/yourname/bwfall/archive/refs/tags/v1.0.0.tar.gz -o test.tar.gz
tar -tzf test.tar.gz | head
```

Should extract to: `bwfall-1.0.0/`

## Update PKGBUILD with Real Checksums

```bash
cd pkg/aur

# Download the actual release tarball
curl -L https://github.com/yourname/bwfall/archive/refs/tags/v1.0.0.tar.gz -o bwfall-1.0.0.tar.gz

# Generate b2sum
b2sum bwfall-1.0.0.tar.gz

# Update PKGBUILD: replace SKIP with the actual b2sum
vim PKGBUILD

# Regenerate .SRCINFO
makepkg --printsrcinfo > .SRCINFO
```

## Publish to AUR

```bash
# Clone AUR repo (first time)
git clone ssh://aur@aur.archlinux.org/bwfall.git aur-bwfall
cd aur-bwfall

# Copy files
cp ../pkg/aur/PKGBUILD .
cp ../pkg/aur/.SRCINFO .

# Test build
makepkg -f
namcap PKGBUILD
namcap bwfall-*.pkg.tar.zst

# Commit and push
git add PKGBUILD .SRCINFO
git commit -m "Initial release: bwfall 1.0.0"
git push origin master
```

## Repository Settings

### Topics (GitHub)
Add these topics to your repository:
- `cli`
- `audio`
- `visualization`
- `binary`
- `alsa`
- `ncurses`
- `linux`
- `arch-linux`
- `alpine-linux`
- `media-player`

### About Section
```
Binary Waterfall CLI - Play any binary file as audio and/or visual output. Raw data media player for Linux.
```

### README Badges (Optional)

Add to top of README.md:
```markdown
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![AUR version](https://img.shields.io/aur/version/bwfall)](https://aur.archlinux.org/packages/bwfall)
[![GitHub release](https://img.shields.io/github/v/release/yourname/bwfall)](https://github.com/yourname/bwfall/releases)
```

## Issue Templates

Create `.github/ISSUE_TEMPLATE/bug_report.md`:
```markdown
---
name: Bug report
about: Create a report to help us improve
title: '[BUG] '
labels: bug
assignees: ''
---

**Describe the bug**
A clear description of what the bug is.

**To Reproduce**
Steps to reproduce:
1. Run command: `bwfall ...`
2. See error

**Expected behavior**
What you expected to happen.

**Environment:**
- OS: [e.g. Arch Linux]
- bwfall version: [e.g. 1.0.0]
- Terminal: [e.g. alacritty, xterm]

**Additional context**
Any other context about the problem.
```

## CI/CD (Optional)

Create `.github/workflows/build.yml`:
```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    - name: Install dependencies
      run: sudo apt-get update && sudo apt-get install -y libasound2-dev libncurses-dev
    - name: Build
      run: make
    - name: Test
      run: make test
```

## Documentation

Ensure these files are present and up-to-date:
- ✅ README.md
- ✅ CHANGELOG.md
- ✅ CONTRIBUTING.md
- ✅ LICENSE
- ✅ QUICKSTART.md

## Post-Release Checklist

- [ ] GitHub repository created
- [ ] Code pushed to main branch
- [ ] v1.0.0 tag created
- [ ] GitHub Release published
- [ ] Release tarball URL verified
- [ ] PKGBUILD updated with real b2sums
- [ ] AUR package published
- [ ] AUR package tested with yay/paru
- [ ] Repository topics added
- [ ] README badges added (optional)
- [ ] Issue templates created (optional)
- [ ] CI/CD workflow added (optional)

## Maintenance

### For new releases:

1. Update version in `src/cli.c` (VERSION macro)
2. Update CHANGELOG.md
3. Commit changes
4. Tag release: `git tag v1.x.x`
5. Push: `git push && git push --tags`
6. Create GitHub Release
7. Update AUR package (see CONTRIBUTING.md)

### Monitoring:

- Watch GitHub issues
- Monitor AUR comments
- Respond to bug reports
- Review pull requests

---

**Your project is ready for the world!** 🚀
