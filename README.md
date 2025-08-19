# 📂 File Organizer Assistant  

[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)  
A modern **file management assistant** built with **C++17 + Qt6**.  
It can **watch directories in real time** and **automatically organize files** into categorized folders.  
Think of it as your personal **hands-free file butler** — set it once, and it keeps everything tidy.  

---

## ✨ Key Features

- ⚡ **One-Click Auto Organization**  
  Instantly sorts images, documents, music, videos, and archives into the right folders.  
  Just pick a directory, hit **Start**, and watch the magic happen.  

- 👀 **Real-Time Monitoring**  
  Whenever a file is added, changed, or removed, the app reacts immediately.  
  Your folders stay clean and organized at all times — completely hands-free.  

- 🖥️ **System Tray Integration**  
  Runs quietly in the background after minimizing.  
  Quick **Start / Stop / Quit** options are always available from the tray.  

- ⚙️ **Fully Customizable Rules**  
  - Ships with a default `config.yaml`  
  - Define your own extension-to-folder rules  
  - Ignore specific files  
  - Set your own fallback category  

- 🍎 **Native macOS Experience**  
  - Beautiful **Dock icon** and **DMG installer**  
  - Verified on **Apple Silicon (M1/M2/M3)**  
  - Drag-and-drop installation — no terminal required  

---

## 📦 Download & Installation (macOS)

1. Download the latest [`organizer_gui.dmg`](#) from the **Releases** section.  
2. Double-click to mount the DMG, then drag **`organizer_gui`** into your **Applications** folder.  

⚠️ **macOS Gatekeeper Notice**  
Since this app is not notarized by Apple, you may see:  
> “`organizer_gui` can't be opened because Apple cannot check it for malicious software.”

To run the app:  
- **Option 1**: Right-click the app in Finder → **Open** → Confirm once.  
- **Option 2**: Remove the quarantine attribute via Terminal:  
  ```bash
  sudo xattr -dr com.apple.quarantine /Applications/organizer_gui.app
