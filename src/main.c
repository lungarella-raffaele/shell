#include "builtins.h"
#include "parser.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_TOKENS 100

void user_input(char *input) {
  setbuf(stdout, NULL);
  printf("$ ");
  fgets(input, 100, stdin);
  input[strlen(input) - 1] = '\0';
}

int run_external(char *bin, char **argv) {
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

int main() {
  char input[100];
  char *tokens[MAX_TOKENS];

  int token_count;
  while (1) {
    token_count = 0;

    user_input(input);

    parse_tokens(input, tokens, &token_count);

    if (strcmp(tokens[0], "exit") == 0) {
      break;
    } else if (is_builtin(tokens[0]) == 0) {
      run_builtin(tokens[0], &tokens[1]);
    } else {
      char *exec = is_in_path(tokens[0]);
      if (exec != NULL) {
        int status = run_external(exec, tokens);
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
