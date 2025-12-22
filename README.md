ابو الهندي

هذه الفايلات هي ليست نهاية الجزء الخاص بي بل يجب ان اضيف امور اخرى ويجب علي ان اقصر بعض الفانكشينز وهذا سيؤدي الى فايلات كثيرة جدا اقول لك هذا ليكون لك علم ماذا يحصل هنا ^_^

هكذا سيصلك الinput 
```bash
cmd1:
    command = "echo"
    args = ["echo", "hello"]
    infile = NULL
    outfile = NULL
    append = FALSE
    heredoc = NULL
```
-----------------------------------------------------------لما ارجع بكمل طالع مشوار================================================
❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗❗
Tests to run now (Checklist)
1) Pipes
ls | wc -l
cat file | grep hello | wc -l

2) Redirections
echo a > out && cat < out
echo b >> out && cat < out
cat < missingfile (should show error)

3) Quotes (must behave like bash)
echo "hello world"
echo 'hello $USER' (no expansion in single quotes later)
echo "$USER" (expansion later)
echo "a | b" (pipe inside quotes must NOT split)
echo "a > b" (redir inside quotes must NOT apply)

4) Mixed redirections + pipes
cat < out | wc -l
echo hello | wc -l > count.txt
cat < out | wc -l >> log.txt

5) Heredoc (to implement/verify)
cat << EOF
type lines
end with EOF

6) Exit status (to implement/verify)
false then check $? later (expansion later)
ls notfound should return non-zero

What is left to reach a real Minishell
Parsing improvements (Omar)
Correct handling of quotes (remove quotes, keep content, don’t split operators inside quotes)
Better syntax errors and edge cases:
| |, | at start/end
missing file after < > >> <<
Proper heredoc parsing & delimiter rules
Environment expansion:
$VAR
$?

Execution improvements (Hindi)
Heredoc execution details (feeding input safely)
Builtins (later):
cd, export, unset, exit, env, pwd, echo -n
Signal behavior like bash:
Ctrl-C new prompt
Ctrl-D exit
Ctrl-\ ignore
Exit status rules (last command in pipeline)

شو هذا مهم
```
## Project status (current)

### What is working now
- minishell prompt using readline
- command execution with fork / execve
- pipes `|`
- redirections:
  - input `<`
  - output `>`
  - append `>>`

Tested examples:
- `echo hello | wc -l`
- `echo hello > out`
- `cat < out`

Everything above is working.

---

### Work split
**Execution (Hindi):**
- execute_pipeline()
- handling pipes, fork, dup2, execve
- input/output redirections
- waiting for children and exit status

**Parsing (Omar):**
- tokenizing input
- syntax validation
- building `t_cmd` structure from user input

---

### Shared structure (contract)
Parsing sends execution this structure:

```c
typedef struct s_cmd {
    char **args;
    char *infile;
    char *outfile;
    int   append;
    char *heredoc;
} t_cmd;

```
