# std2

A **100% native**, **header-only** C/C++ standard library designed for extreme performance. `std2` bypasses the standard `libc` entirely, communicating directly with the Linux kernel via pure inline assembly system calls.

## 🚀 Key Features

* **100% Native (No libc):** Zero external dependencies. Speaks directly to the kernel using software interrupts (`svc #0`).
* **Header-Only:** Simply drop the `std2.h` file into your project to get started instantly.
* **Extreme Performance:** No wrapping overhead, utilizing direct CPU register manipulation.
* **Thread-Safe:** Reentrant functions with atomic memory barriers to ensure data consistency across multi-threaded environments.
* **Target Architecture:** Optimized for **ARM64 (AArch64) Linux**.

## 💻 Usage Example

This example demonstrates how to read user input and print it back to the screen completely natively:

```cpp
#include "std2.h"

int main() {
    write("Enter a message: ");
    
    char buffer[256];
    read(buffer);
    
    write("You wrote: ");
    write(buffer);
    write("\n");
    
    return 0;
}
```

## ⚙️ Bare-Metal Compilation

To compile your project and guarantee that no standard system libraries are linked, use the `-nostdlib` flag with `g++` or `clang++`:

```bash
g++ -nostdlib -O3 main.cpp -o main
```

## 📈 Implemented System Calls

| Function | Linux Syscall (ARM64) | Register Code (`x8`) | Description |
| :--- | :--- | :--- | :--- |
| `write(const char*)` | `sys_write` | `64` | Writes a null-terminated string to `stdout`. |
| `read(buffer)` | `sys_read` | `63` | Reads input from `stdin` and safely appends the `\0` terminator. |

## 🛡️ License

This project is licensed under the **GNU GPL v3**. See the `LICENSE` file for more details.
