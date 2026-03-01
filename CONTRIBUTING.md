# Contributing to bwfall

## AUR Package Maintenance

This document explains how to maintain the AUR package for `bwfall`.

### Prerequisites

1. AUR account at https://aur.archlinux.org
2. SSH key added to your AUR account
3. SSH config for AUR:
   ```
   Host aur.archlinux.org
       IdentityFile ~/.ssh/aur
       User aur
   ```

### Initial AUR Setup

```bash
# Clone the AUR repository
git clone ssh://aur@aur.archlinux.org/bwfall.git aur-bwfall
cd aur-bwfall

# Copy packaging files
cp ../pkg/aur/PKGBUILD .
cp ../pkg/aur/.SRCINFO .

# Commit and push
git add PKGBUILD .SRCINFO
git commit -m "Initial release: bwfall 1.0.0"
git push origin master
```

### Updating the AUR Package

When releasing a new version:

1. **Tag and release on GitHub:**
   ```bash
   git tag v1.1.0
   git push origin v1.1.0
   # Create GitHub Release with tarball
   ```

2. **Update PKGBUILD:**
   ```bash
   cd aur-bwfall
   
   # Edit PKGBUILD: bump pkgver, reset pkgrel to 1
   vim PKGBUILD
   
   # Regenerate checksums
   updpkgsums
   
   # Regenerate .SRCINFO
   makepkg --printsrcinfo > .SRCINFO
   ```

3. **Test locally:**
   ```bash
   makepkg -f
   namcap PKGBUILD
   namcap bwfall-*.pkg.tar.zst
   ```

4. **Commit and push:**
   ```bash
   git add PKGBUILD .SRCINFO
   git commit -m "Upgrade to bwfall 1.1.0"
   git push origin master
   ```

### Versioning Rules

- **New upstream release**: Bump `pkgver`, reset `pkgrel=1`
- **PKGBUILD fix only**: Bump `pkgrel` only
- **Dependency fix**: Bump `pkgrel` only

### Testing Checklist

Before pushing to AUR:

- [ ] `makepkg -f` builds successfully
- [ ] `namcap PKGBUILD` shows no errors
- [ ] `namcap *.pkg.tar.zst` shows no errors
- [ ] Package installs: `makepkg -si`
- [ ] Binary runs: `bwfall --version`
- [ ] Test with AUR helpers: `yay -S bwfall` or `paru -S bwfall`

### Common Issues

**Missing dependencies:**
- Check with `ldd /usr/bin/bwfall`
- Add missing libraries to `depends=()` in PKGBUILD

**Wrong checksums:**
- Run `updpkgsums` to regenerate
- Never use `SKIP` in published packages

**Out of sync .SRCINFO:**
- Always regenerate: `makepkg --printsrcinfo > .SRCINFO`
- Commit both PKGBUILD and .SRCINFO together

### Responding to User Comments

Monitor the AUR package page for:
- Build failures
- Missing dependencies
- Feature requests
- Bug reports

Respond promptly and update the package as needed.

## Code Contributions

### Building from Source

```bash
# Install dependencies
sudo pacman -S alsa-lib ncurses gcc make

# Build
make

# Test
make test

# Install locally
sudo make install PREFIX=/usr/local
```

### Code Style

- C11 standard
- Compile with `-Wall -Wextra -Wpedantic` with zero warnings
- Use `clang-format` for formatting
- No memory leaks (test with `valgrind`)

### Submitting Changes

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

By contributing, you agree that your contributions will be licensed under GPL-3.0-or-later.
