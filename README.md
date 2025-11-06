# Minishell

A Unix-like shell implemented in **C** supporting pipelines, redirection, variable expansion and built-in commands.

Built to deepen understanding of **Linux process models**, **system calls** and **shell execution flows**.

---
## Requirements

macOS or Linux

POSIX shell / cc

GNU Readline (required)
## Features

- `|` pipelines
- `<`, `>`, `>>` redirection
- `$VAR` environment variable expansion
- built-ins: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`
- execution via `fork` + `execve`
- correct file descriptor management
- Note: Does not handle multiple commands in one line, '&&', '||' 
---

## Build & Run

```bash
make
./minishell
