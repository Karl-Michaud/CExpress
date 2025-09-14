# Installation

CExpress can be installed as a shared C library on your system, making it easy to include and link in your own C projects.

## Prerequisites

- GCC or Clang compiler
- Make utility
- macOS or Linux (for Windows, manual steps may differ)

## Installation

### Option 1: Install from GitHub (macOS & Linux)

1. **Clone the repository:**

   ```bash
   git clone https://github.com/Karl-Michaud/CExpress.git
   cd CExpress
   ```

2. **Build and Install the library and headers:**

   ```bash
   make install
   ```

   This will:
   - Copy the shared library (`libCExpress.dylib` on macOS, or `libCExpress.so` on Linux) to `/usr/local/lib/`
   - Copy the header files to `/usr/local/include/CExpress/`

#### Uninstall

To remove CExpress, run:

```bash
make uninstall
```

Or manually:

```bash
sudo rm /usr/local/lib/libCExpress.dylib    # macOS
sudo rm /usr/local/lib/libCExpress.so       # Linux
sudo rm -r /usr/local/include/CExpress/
```

#### Notes

- To install to a different location, set the `PREFIX` variable:
  ```bash
  sudo make install PREFIX=/custom/path
  ```
- For Linux, the shared library extension is `.so`; for macOS, it is `.dylib`.


### Option 2: Install via Homebrew (macOS)

#### Install


```bash
brew tap Karl-Michaud/cexpress
brew install cexpress
```

(Optional) Verify installation:

```bash
gcc -o test test.c -lCExpress
./test
```

#### Uninstall

```bash
brew uninstall cexpress
```

(Optional) Remove the tap and clear cached downloads:

```bash
brew untap Karl-Michaud/cexpress
brew cleanup
```

This will remove the CExpress library and headers installed by Homebrew.


#### Notes

- To install to a different location, set the `PREFIX` variable:
  ```bash
  sudo make install PREFIX=/custom/path
  ```
- For Linux, the shared library extension is `.so`; for macOS, it is `.dylib`.

---

## Usage in Your Project

After installation, you can include CExpress in your C code:

```c
#include <CExpress/server.h>
```

And compile your project with:

```bash
gcc yourfile.c -lCExpress
```