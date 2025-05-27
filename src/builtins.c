#include "builtins.h"
#include "constants.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Command BUILTIN_COMMANDS[] = {
    {"cd", execute_cd},   {"echo", execute_echo},
    {"pwd", execute_pwd}, {"type", execute_type},
    {"exit", NULL},       {NULL, NULL} // sentinel to mark end of table
};

int run_builtin(char *cmd, char **argv) {
  for (int i = 0; BUILTIN_COMMANDS[i].name != NULL; i++) {
    if (strcmp(BUILTIN_COMMANDS[i].name, cmd) == 0) {
      BUILTIN_COMMANDS[i].func(argv);
      return 1;
    }
  }
  return 0;
}

int is_builtin(char *cmd) {
  for (int i = 0; i < sizeof(BUILTIN_COMMANDS); i++) {
    const char *curr = BUILTIN_COMMANDS[i].name;
    if (curr == NULL) {
      return -1;
    }

    if (strcmp(cmd, curr) == 0) {
      return 0;
    }
  }

  return -1;
}

void execute_type(char **argv) {
  if (argv == NULL || argv[0] == NULL) {
    printf("\n");
    return;
  }

  for (int i = 0; argv[i] != NULL; i++) {
    if (is_builtin(argv[i]) >= 0) {
      printf("%s is a shell builtin\n", argv[i]);
    } else {

      char *dir = is_in_path(argv[i]);
      if (dir == NULL) {
        printf("%s: not found\n", argv[i]);
      } else {
        printf("%s is %s\n", argv[i], dir);
      };
      free(dir);
    }
  }
}

void execute_pwd(char **argv) {
  char dir[MAX_LENGTH_DIR];

  if (getcwd(dir, MAX_LENGTH_DIR)) {
    printf("%s\n", dir);
  } else {
    perror("getcwd() error\n");
  }
}

void execute_cd(char **argv) {
  if (argv[0] == NULL) {
    // TODO If empty move current directory to ~
    return;
  }
  int error = chdir(argv[0]);

  if (error == -1) {
    fprintf(stderr, "cd: %s: %s\n", argv[0], strerror(errno));
  }
}

void execute_echo(char **argv) {
  if (argv == NULL) {
    printf("\n");
  } else {
    for (int i = 0; argv[i] != NULL; i++) {
      if (i == 0) {
        printf("%s", argv[i]);
      } else {
        printf(" %s", argv[i]);
      }
    }
    printf("\n");
  }
}
