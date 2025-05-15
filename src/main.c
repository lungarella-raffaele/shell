#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_LIST_SIZE 3

char *cmd_list[] = {"echo", "exit", "type"};

int is_equal(char *str1, char *str2) { return strcmp(str1, str2) == 0; }
int is_builtin(char *command) {
    for (int i = 0; i < CMD_LIST_SIZE; i++) {

        if (is_equal(command, cmd_list[i])) {
            return i;
        }
    }
    return -1;
}

void user_input(char *input) {
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
}

void execute_echo(char *arg) {
    if (arg == NULL) {
        printf("\n");
    } else {
        printf("%s\n", arg);
    }
}

void execute_type(char *arg) {
    char *first_arg = strsep(&arg, " ");
    if (first_arg != NULL && is_builtin(first_arg) >= 0) {
        printf("%s is a shell builtin\n", first_arg);
    } else if (first_arg == NULL) {
        printf("type needs an argument\n");
    } else {
        printf("%s: not found \n", first_arg);
    }
}

void not_found(char *cmd) { printf("%s: command not found\n", cmd); }

int main(int argc, char *argv[]) {
    char input[100];

    while (1) {
        setbuf(stdout, NULL);
        printf("$ ");

        user_input(input);

        char *args = input;
        char *cmd = strsep(&args, " ");

        if (is_equal(cmd, "exit")) {
            break;
        } else if (is_equal(cmd, "echo")) {
            execute_echo(args);
        } else if (is_equal(cmd, "type")) {
            execute_type(args);
        } else {
            not_found(cmd);
        }
    }
    return 0;
}
