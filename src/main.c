#include "builtins.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void user_input(char *input) {
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
}

int execute_external(char *bin, char **argv) {
    pid_t ext_pid;
    int status, timeout;

    if (0 == (ext_pid = fork())) {
        if (-1 == (execve(bin, argv, NULL))) {
            perror("External command failed");
            exit(EXIT_FAILURE);
        }
    } else if (ext_pid > 0) {
        waitpid(ext_pid, &status, 0);
    } else {
        perror("Fork failed");
        return -1;
    }

    return 0;
}

void not_found(char *cmd) { printf("%s: command not found\n", cmd); }
void free_tokens(char **tokens) {
    if (tokens == NULL) {
        return;
    }

    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

// git commit -m 'daje roma'
char **parse_tokens(char *cmd) {
    if (cmd == NULL) {
        return NULL;
    }

    char *tmp_cmd_cpy = strdup(cmd);
    if (tmp_cmd_cpy == NULL) {
        return NULL;
    }

    int token_count = 0;
    char *temp_ptr = tmp_cmd_cpy;
    char *token;

    while ((token = strsep(&temp_ptr, " ")) != NULL) {
        if (*token != '\0') {
            token_count++;
        }
    }
    free(tmp_cmd_cpy);

    char **tokens = malloc((token_count + 1) * sizeof(char *));

    if (tokens == NULL) {
        return NULL;
    }

    char *cmd_copy = strdup(cmd);
    if (cmd_copy == NULL) {
        free(tokens);
        return NULL;
    }

    char *curr_word;
    int i = 0;
    char *str_ptr = cmd_copy;

    while ((curr_word = strsep(&str_ptr, " "))) {
        if (*curr_word != '\0') {
            tokens[i] = strdup(curr_word);
            if (tokens[i] == NULL) {
                for (int j = 0; j < i; j++) {
                    free(tokens[j]);
                }
                free(tokens);
                free(cmd_copy);
                return NULL;
            }
            i++;
        }
    }

    tokens[i] = NULL;
    free(cmd_copy);
    return tokens;
}

int main() {
    char input[100];

    while (1) {
        setbuf(stdout, NULL);
        printf("$ ");

        user_input(input);

        char **tokens = parse_tokens(input);
        if (tokens == NULL) {
            fprintf(stderr, "Error: Failed to tokenize input\n");
            break;
        }

        if (tokens[0] == NULL) {
            fprintf(stderr, "Error: Empty command\n");
            free_tokens(tokens);
            continue;
        }

        if (is_equal(tokens[0], "exit")) {
            free_tokens(tokens);
            break;
        } else if (is_builtin(tokens[0]) == 0) {
            run_builtin(tokens[0], &tokens[1]);
        } else {
            char *exec = is_in_path(tokens[0]);
            if (exec != NULL) {
                int status = execute_external(exec, tokens);
                if (status == -1) {
                    printf("Error while executing: %s\n", exec);
                }
            } else {
                not_found(tokens[0]);
            }
        }
    }
    return 0;
}
