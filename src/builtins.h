#ifndef BUILTINS_H
#define BUILTINS_H

int run_builtin(char *cmd, char **argv);
int is_builtin(char *cmd);
void execute_type(char **argv);
void execute_pwd(char **argv);
void execute_cd(char **argv);
void execute_echo(char **argv);

typedef struct {
    const char *name;
    void (*func)(char **argv);
} Command;

#endif
