#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
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

char *is_in_path(char *cmd) {
    char *path = strdup(getenv("PATH"));
    char *path_copy = path;
    char *dir;

    while ((dir = strsep(&path, ":")) != NULL) {
        char path_to_check[MAX_LENGTH];
        snprintf(path_to_check, MAX_LENGTH, "%s/%s", dir, cmd);
        if (access(path_to_check, X_OK) == 0) {
            free(path_copy);
            char *result = strdup(path_to_check);
            return result;
        };
    }

    free(path_copy);
    return NULL;
}

void user_input(char *input) {
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
}

void execute_type(char **args) {
    if (args == NULL || args[1] == NULL) {
        printf("type needs an argument\n");
        return;
    }
    char *first_arg = args[1];

    if (is_builtin(first_arg) >= 0) {
        printf("%s is a shell builtin\n", first_arg);
        return;
    }

    char *dir = is_in_path(first_arg);
    if (dir == NULL) {
        printf("%s: not found\n", first_arg);
        return;
    };

    printf("%s is %s\n", first_arg, dir);
    free(dir);
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

void execute_echo(char **arg) {
    if (arg == NULL) {
        printf("\n");
    } else {
        for (int i = 0; arg[i] != NULL; i++) {
            if (i == 0) {
                printf("%s", arg[i]);
            } else {
                printf(" %s", arg[i]);
            }
        }
        printf("\n");
    }
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

char **get_tokens(char *cmd) {
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

int main(int argc, char *argv[]) {
    char input[100];

    while (1) {
        setbuf(stdout, NULL);
        printf("$ ");

        user_input(input);

        char **tokens = get_tokens(input);
        if (tokens == NULL) {
            fprintf(stderr, "Error: Failed to tokenize input\n");
            break;
        }

        if (tokens[0] == NULL) {
            fprintf(stderr, "Error: Empty command\n");
            free_tokens(tokens);
            continue;
        }

        char *cmd = tokens[0];
        char **args = &tokens[1];

        if (is_equal(cmd, "exit")) {
            free_tokens(tokens);
            break;
        } else if (is_equal(cmd, "echo")) {
            execute_echo(args);
        } else if (is_equal(cmd, "type")) {
            execute_type(tokens);
        } else {
            char *exec = is_in_path(cmd);
            if (exec != NULL) {
                int status = execute_external(exec, tokens);
                if (status == -1) {
                    printf("Error while executing: %s\n", exec);
                }
            } else {
                not_found(cmd);
            }
        }
    }
    return 0;
}
