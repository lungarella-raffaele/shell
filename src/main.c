#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CMD_LIST_SIZE 3
#define MAX_LENGTH 1024

char *cmd_list[] = {"echo", "exit", "type"};

int is_equal(char *str1, char *str2) { return strcmp(str1, str2) == 0; }

int is_builtin(char *cmd) {
    for (int i = 0; i < CMD_LIST_SIZE; i++) {
        if (is_equal(cmd, cmd_list[i])) {
            return i;
        }
    }

    return -1;
}

int is_in_path(char *cmd) {
    char *path = strdup(getenv("PATH"));
    char *path_copy = path;
    char *dir;

    while ((dir = strsep(&path, ":")) != NULL) {
        char path_to_check[MAX_LENGTH];
        snprintf(path_to_check, MAX_LENGTH, "%s/%s", dir, cmd);
        if (access(path_to_check, X_OK) == 0) {
            free(path_copy);
            printf("%s is %s\n", cmd, path_to_check);
            return 0;
        };
    }

    free(path_copy);
    return -1;
}

void user_input(char *input) {
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
}

void execute_type(char *arg) {
    char *first_arg = strsep(&arg, " ");

    if (first_arg == NULL) {
        printf("type needs an argument\n");
    }

    if (first_arg != NULL && is_builtin(first_arg) >= 0) {
        printf("%s is a shell builtin\n", first_arg);
        return;
    }

    if (is_in_path(first_arg) == -1) {
        printf("%s: not found\n", first_arg);
    };
}

void execute_echo(char *arg) {
    if (arg == NULL) {
        printf("\n");
    } else {
        printf("%s\n", arg);
    }

    if (1) {
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
