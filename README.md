# MiniShell - Linux Command Line Shell in C

## Description

MiniShell is a custom Linux command-line shell developed in C that provides basic shell functionalities similar to Bash. It supports built-in commands, execution of external commands, process management, job control, signal handling, command piping, and customizable prompts.

The project demonstrates core Linux system programming concepts such as process creation, signal handling, inter-process communication, and command execution.

---

## Features

### Built-in Commands
- pwd
- cd
- echo
- jobs
- fg
- bg
- exit

### External Command Execution
- Executes Linux commands using `fork()` and `execvp()`.

### Pipe Support
- Supports multiple commands connected using pipes (`|`).

Example:
```bash
ls | grep txt
cat file.txt | wc
```

### Job Control
- Stop running processes using `Ctrl + Z`
- View stopped jobs using `jobs`
- Resume jobs in foreground using `fg`
- Resume jobs in background using `bg`

### Signal Handling
- Handles `SIGINT` (Ctrl + C)
- Handles `SIGTSTP` (Ctrl + Z)

### Prompt Customization
Change shell prompt dynamically using:

```bash
PS1=MyShell
```

### Shell Variables
Supports:

```bash
echo $$
```
Displays shell PID.

```bash
echo $?
```
Displays exit status of previous command.

```bash
echo $SHELL
```
Displays current shell path.

---

## Project Structure

```text
MiniShell/
│
├── main.c
├── scan_input.c
├── commands.c
├── mini.h
├── Makefile
├── extr_cmt.txt
└── README.md
```

---

## Compilation

Compile using Makefile:

```bash
make
```

Or manually:

```bash
gcc main.c scan_input.c commands.c -o minishell
```

---

## Running the Shell

```bash
./minishell
```

Example:

```bash
minishell:~pwd
/home/user

minishell:~echo $$
1234

minishell:~ls

minishell:~jobs
```

---

## Concepts Used

- Linux System Programming
- Process Creation (`fork`)
- Program Execution (`execvp`)
- Signal Handling
- Job Control
- Pipes and File Descriptors
- Linked Lists
- Dynamic Memory Allocation

---

## Challenges Faced

One of the major challenges in this project was implementing job control and signal handling. Managing stopped processes using `Ctrl + Z`, maintaining a job list, and correctly resuming jobs using `fg` and `bg` required careful coordination between process management and signal handlers.

Another challenge was implementing command piping, where the output of one command becomes the input of another using pipes and file descriptor redirection.

---

## Future Enhancements

- Command history support
- Auto-completion
- Arrow key navigation
- Input/Output redirection (`>`, `<`, `>>`)
- Background execution using `&`
- Environment variable management
- Improved job scheduling

---


B.E. Electronics and Telecommunication Engineering

MiniShell Project - Linux System Programming in C
