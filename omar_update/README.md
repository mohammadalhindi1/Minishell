# Minishell

A minimal Unix shell implemented in C as part of the 42 curriculum.

This project focuses on understanding how a shell works internally:
process creation, pipes, redirections, signals, and command parsing.

---
```bash
make
./minishell
valgrind ./minishell
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes ./minishell
```

Project Status (Current)
What is working now good
Interactive shell prompt using readline
Command execution using fork / execve
Pipes |
Redirections:
input <
output >
append >>
Waiting for children processes
Proper error messages for:
command not found
missing files
permission denied

Tested examples:
```
echo hello | wc -l
echo hello > out
cat < out
echo b >> out
cat < out | wc -l
``` 
Everything above is working correctly.

Shared Structure (Execution Contract)
Parsing sends execution an array of commands using the following structure:
```
typedef struct s_cmd
{
    char **args;      // argv-style array, args[0] is the command
    char *infile;     // file for '<' redirection (NULL if none)
    char *outfile;    // file for '>' or '>>' (NULL if none)
    int   append;     // 0 => '>', 1 => '>>'
    char *heredoc;    // delimiter for '<<' (NULL if none)
} t_cmd;
```
The parser is responsible for building this structure correctly
The executor only relies on this structure and does NOT re-parse input

Work Split :

```
Execution (Hindi)
Responsible for everything after parsing:
execute_pipeline()
Pipes handling (pipe, dup2)
Process creation (fork, execve)
Redirections <, >, >>
Heredoc execution logic
Waiting for children & exit status
Error handling and messages


Parsing (Omar)
Responsible for transforming raw input into t_cmd[]:
Tokenizing input
Syntax validation
Handling quotes
Building the t_cmd array correctly
Important for parsing:
Execution assumes parsing is correct.
If parsing outputs invalid t_cmd, execution behavior is undefined.
```

Tests to Run Now (Execution + Parsing)
Pipes
```
ls | wc -l
cat file | grep hello | wc -l
```

Redirections
```
echo a > out && cat < out
echo b >> out && cat < out
cat < missingfile
```


Mixed pipes + redirections
```
cat < out | wc -l
echo hello | wc -l > count.txt
cat < out | wc -l >> log.txt
```

Parsing Rules That MUST Work (Omar)
Quotes must behave like bash:
```
echo "hello world"
echo 'hello $USER'     # no expansion inside single quotes
echo "$USER"           # expansion later
echo "a | b"           # pipe inside quotes must NOT split
echo "a > b"           # redirection inside quotes must NOT apply
```

Syntax errors must be detected:

| |
pipe at start or end
missing file after < > >> <<
Heredoc parsing:
delimiter must be exact
stop reading only on exact match
no history entry for heredoc input
Environment expansion (later):
$VAR
$?

Heredoc (To Implement / Verify)
```
cat << EOF
type lines
end with EOF
```

Also:
```
cat << EOF | wc -l
a
b
EOF
```
Ctrl-C during heredoc must:
cancel heredoc
return to a clean prompt
not execute the pipeline

```
Exit Status (To Implement / Verify) :
Last command in pipeline defines exit status
command not found => 127
signal termination => 128 + signal number
$? expansion (later)

What Is Still Missing (Planned):

Parsing (Omar):
Correct quote handling
Full syntax validation
Environment expansion $VAR, $?
Robust heredoc parsing rules
```

Execution (Hindi):
Final heredoc polish
Builtins:
```
cd
export
unset
exit
env
pwd
echo -n
Signal behavior like bash:
Ctrl-C → new prompt
Ctrl-D → exit shell
Ctrl-\ → ignored
Valgrind / memory cleanup (planned)
```
Notes:
Execution and parsing are strictly separated
The t_cmd structure is the only contract between them
Keeping this separation is critical for project stability
