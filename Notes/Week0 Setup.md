# Week 0 — Laptop Setup Notes
 
Date: 8 Nov 2026
Result: WSL2 + Ubuntu, g++ 15, CMake, Git, gdb, VS Code (WSL) and GitHub all working.
Repo: github.com/rajan314/mini-oms
 
---
 
## 1. WSL basics
 
- WSL = Windows Subsystem for Linux. Real Ubuntu running inside Windows.
- Windows stays untouched. Work setup (Docker, Postgres, Node, VS Code) is separate.
- `docker-desktop` distro belongs to Docker — never use it for my own work.
- Ubuntu is now the default distro.
Useful PowerShell commands:
 
| Command | What it does |
| --- | --- |
| `wsl` | Enter Ubuntu from PowerShell |
| `wsl -l -v` | List distros (`*` = default) |
| `wsl --set-default Ubuntu` | Make Ubuntu the default (name must match exactly) |
| `wsl --shutdown` | Stop all of WSL, frees memory (do this before switching to job work) |
 
Switching:
- Learning → open Ubuntu, `cd ~/projects/mini-oms`, `code .`
- Job → close Ubuntu VS Code window, `wsl --shutdown`, start Docker Desktop
## 2. Know which terminal I'm in
 
- `PS C:\...>` = PowerShell (Windows)
- `rajan@RAJAN-SINGH:~$` = Ubuntu (Linux) — build C++ only here
- `~` in PowerShell = `C:\Users\ASUS`; `~` in Ubuntu = `/home/rajan`
- Never work in `/mnt/c/...` (that's the Windows drive): slow and risky. Keep projects in `~/projects/`.
## 3. Installed tools
 
```
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential cmake git gdb clang
```
 
- g++ 15.2, cmake 4.2.3, git 2.53.0
- Linux password is invisible while typing — normal.
- `sudo` = run as admin; asks for my Linux password.
## 4. VS Code + WSL
 
- Open a project from Ubuntu with `code .`
- Bottom-left must show **WSL: Ubuntu**.
- Extensions are installed separately for Windows and WSL → click "Install in WSL: Ubuntu".
- Needed in WSL: **C/C++** (`ms-vscode.cpptools`), **CMake Tools** (`ms-vscode.cmake-tools`), or just the **C/C++ Extension Pack**.
- AI autocomplete (Copilot) disabled for this workspace while learning. The normal suggestion dropdown (IntelliSense) is fine.
- Format code: Shift+Alt+F.
## 5. Compiling
 
```
g++ -std=c++20 -Wall -Wextra -fsanitize=address -g file.cpp -o file
./file
```
 
| Flag | Meaning |
| --- | --- |
| `-std=c++20` | Use the C++20 standard |
| `-Wall -Wextra` | Turn on warnings |
| `-fsanitize=address` | Catch memory bugs at runtime (AddressSanitizer) |
| `-g` | Add debug info so the debugger shows variables and lines |
| `-o file` | Name of the output program |
 
- No output from g++ = no errors, no warnings = good.
## 6. Debugger (VS Code + gdb)
 
- F9 = toggle breakpoint (red dot) on current line
- F5 = start debugging / continue to next breakpoint
- F10 = step over (next line)
- F11 = step into a function
- Shift+F5 = stop
- First time: choose "C/C++: g++ build and debug active file" (plain g++, not g++-15)
- Watch values in the **Variables** panel or by hovering.
- "Loaded ...so.6" lines in the Debug Console are normal.
## 7. Git + GitHub
 
One-time setup (done):
```
git config --global user.name "Rajan Singh"
git config --global user.email "singhrajan21112001@gmail.com"
ssh-keygen -t ed25519 -C "singhrajan21112001@gmail.com"
cat ~/.ssh/id_ed25519.pub        # copy → GitHub Settings → SSH keys
ssh -T git@github.com            # "Hi rajan314! You've successfully authenticated"
```
 
- `.pub` = public key, safe to share. `id_ed25519` (no .pub) = private key, NEVER share.
- One GitHub account for personal + work is fine. Create personal repos with Owner = `rajan314`, not the org.
- Ubuntu git config is separate from Windows git config.
Daily flow:
```
git status
git add .
git status                       # check nothing unwanted is staged
git commit -m "Add order book cancel"
git push
```
 
Fixing mistakes:
- Wrong remote URL: `git remote set-url origin git@github.com:rajan314/mini-oms.git`
- Check remote: `git remote -v`
- First push of a branch: `git push -u origin main` (after that just `git push`)
## 8. Linux commands
 
| Windows | Linux | Does |
| --- | --- | --- |
| `cls` | `clear` / Ctrl+L | Clear screen |
| `dir` | `ls`, `ls -la` | List files (`-la` shows hidden + details) |
| `cd` | `pwd` | Show current folder |
| `type` | `cat` | Show file contents |
| `copy` / `move` | `cp` / `mv` | Copy / move or rename |
| `del` | `rm`, `rm -r` | Delete file / folder — NO recycle bin |
| `mkdir` | `mkdir -p a/b` | Make folders (`-p` makes parents too) |
| `findstr` | `grep -n "text" file` | Search in files, with line numbers |
 
## 9. Coding habits (keep following these)
 
1. No `using namespace std;` — write `std::`. At most `using std::vector;` inside a .cpp, never in a header.
2. No `#include <bits/stdc++.h>` — include only what you use (`<vector>`, `<map>`).
3. `'\n'` instead of `std::endl` (endl flushes = slow in loops).
4. `const` on values that never change.
5. `std::size_t` for loop counters compared with `.size()` (avoid signed/unsigned warning).
6. Spaces around operators: `total += x;` not `total+=x;`.
7. Every compiler warning is a bug — fix it.
8. Prices as integers (ticks), never `double`.
9. Never commit compiled files — list them in `.gitignore`.
10. Commit messages = short command: "Add order book cancel", not "update".
11. Check with `ls` before `rm`.
## 10. Mistakes I made today (and fixes)
 
- Two commands pasted on one line → `error: no action specified`. Run one command per line.
- Typo `Ubantu` → distro names must match exactly: `Ubuntu`.
- Ran `cd ~/projects/...` in PowerShell → type `wsl` first.
- VS Code connected to `docker-desktop` → set Ubuntu as default.
- SSH key "invalid" on GitHub → copy lost the space before the email. Paste carefully.
- Remote URL typo `rajan/314` → fixed with `git remote set-url`.
 