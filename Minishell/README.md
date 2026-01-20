# Minishell

*This project has been created as part of the 42 curriculum by oaletham, malhendi.*

## Description

**Minishell** is a simplified implementation of a Unix shell, developed as part of the 42 curriculum. Its goal is to deepen understanding of process creation, file descriptors, parsing, and terminal behavior by recreating core functionalities of Bash in C.

This project allows users to interact with the system via an interface that interprets and executes shell commands. Through this, students explore the internal mechanics of how shells parse, handle redirections and pipes, manage child processes, and integrate built-in commands.

Key learning outcomes include:
- Mastering system calls like `fork`, `execve`, `pipe`, and `dup2`
- Handling signals and process exit statuses
- Managing memory allocations and avoiding leaks
- Implementing a lexical analyzer and parser for shell syntax

## Instructions

### Compilation

To compile the project, run:

```bash
make
```

The project includes a standard `Makefile` with the following rules: `all`, `clean`, `fclean`, `re`.

### Execution

Once compiled, launch the shell with:

```bash
./minishell
```

This will display a custom shell prompt and allow for command-line input.

### Requirements

- Use of **GNU readline** for input handling and command history
- Compilation must adhere to 42's Norm
- No memory leaks (excluding internal leaks from `readline`)
- Only one global variable is allowed (for signal handling)

## Features

- Display prompt and accept user input
- Parse commands and arguments
- Support for single and double quotes
- Input/output redirections: `<`, `>`, `<<`, `>>`
- Environment variable expansion, including `$?`
- Piping (`|`) between commands
- Signal handling (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`)
- Execution of both built-in and external commands

### Built-in Commands

- `echo [-n]`
- `cd [dir]`
- `pwd`
- `export [VAR=value]`
- `unset [VAR]`
- `env`
- `exit`

## Resources

### References

- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Advanced Linux Programming](https://www.advancedlinuxprogramming.com/)
- `man` pages: `bash`, `execve`, `fork`, `pipe`, `dup2`, `signal`, `readline`
- [UNIX Programming FAQ](https://www.faqs.org/faqs/unix-faq/)

### Use of AI

AI tools were used responsibly to:
- Draft parts of this README
- Review and debug parsing logic in the lexer
- Generate test scenarios for edge cases
- Refactor repetitive code segments (only after manual understanding)

All AI-generated content was critically reviewed and verified with peers to ensure full understanding and compliance with 42’s academic integrity guidelines.

## Technical Notes

- Only one global variable is used for signal communication, per spec
- Built-in commands are handled internally; others are executed via `execve`
- Parsing supports tokenization, quoting rules, and environment expansion
- Redirections and pipes are managed using file descriptors and process forking
- Interactive shell behavior mimics Bash in terms of signal response and prompt handling

## Authors

- [oaletham](https://github.com/oaletham)
- [malhendi](https://github.com/malhendi)
