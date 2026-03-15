# 🎮 GameBoyWeb

**GameBoyWeb** is a high-performance Game Boy emulator built with **C++** and the **Qt Framework**, compiled into **WebAssembly (WASM)** using **Emscripten**. 

This project was developed as my Bachelor's Degree Thesis, titled: 
> *"The Web-based Implementation of the Game Boy Series Console Emulator in WASM"*

It demonstrates the power of cross-compiling native C++ logic for near-native performance in a web browser.

[**🕹️ Play the Demo Now**](https://kiwigrenade.github.io/)

---

## 🚀 Features

* **Native Performance:** Core logic written in C++ for efficient and accurate emulation.
* **Web-Ready:** Fully accessible via any modern browser thanks to WebAssembly.
* **Qt Interface:** Leverages the Qt framework for robust state management and rendering.
* **Lightweight:** No installation required — just load the ROM and play.

---

## 🕹️ Quick Start

1.  **Download a ROM:** Get a homebrew ROM from the [/roms/games/](roms/games/) directory (e.g., [Bone Finder](https://github.com/KiwiGrenade/GameBoyWeb/raw/refs/heads/main/roms/games/bone_finder-djarky.gb)).
2.  **Launch the App:** Go to [KiwiGrenade.io](https://kiwigrenade.github.io/).
3.  **Load Game:** Navigate to `File` -> `Open` and select your downloaded `.gb` file.
4.  **Play:** Use your keyboard to control the action!

---

## ⌨️ Controls

The mapping is designed to be intuitive for desktop users:

| Game Boy Button | Keyboard Key |
| :--- | :--- |
| **D-Pad Up** | `Arrow Up` |
| **D-Pad Down** | `Arrow Down` |
| **D-Pad Left** | `Arrow Left` |
| **D-Pad Right** | `Arrow Right` |
| **Button A** | `D` |
| **Button B** | `S` |
| **Start** | `Enter` |
| **Select** | `Shift` |

---

## 🛠️ Technical Stack

* **Language:** C++17
* **Framework:** Qt 6
* **Compiler:** Emscripten
* **Platform:** WebAssembly (WASM)

---

## 📜 License

This project is licensed under the **GPL v3**. See the [LICENSE](LICENSE) file for details.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Author: Jakub Jaśków
