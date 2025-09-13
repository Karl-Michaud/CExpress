# Installation

CExpress can be installed as a shared C library on your system, making it easy to include and link in your own C projects.

## Prerequisites

- GCC or Clang compiler
- Make utility
- macOS or Linux (for Windows, manual steps may differ)

## Build and Install

### Option 1: Install through MakeFile

1. **Clone the repository:**

   ```bash
   git clone https://github.com/Karl-Michaud/CExpress.git
   cd CExpress
   ```

2. **Build and Install the library and headers:**

   ```bash
   sudo make install
   ```

   This will:
   - Copy the shared library (`libCExpress.dylib` on macOS, or `libCExpress.so` on Linux) to `/usr/local/lib/`
   - Copy the header files to `/usr/local/include/CExpress/`

## Usage in Your Project

After installation, you can include CExpress in your C code:

```c
#include <CExpress/server.h>
```

And compile your project with:

```bash
gcc yourfile.c -lCExpress
```

## Uninstall

To remove CExpress, delete the installed files:

```bash
sudo rm /usr/local/lib/libCExpress.dylib
sudo rm -r /usr/local/include/CExpress/
```

## Notes

- If you want to install to a different location, set the `PREFIX` variable:
  ```bash
  sudo make install PREFIX=/custom/path
  ```
- For Linux, you may want to change the library extension in the Makefile from `.dylib` to `.so`.

---
Now you are ready to use CExpress in your C projects!