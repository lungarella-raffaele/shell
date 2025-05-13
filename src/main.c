#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  while (1) {
    // Flush after every printf
    setbuf(stdout, NULL);
    printf("$ ");
    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';

    if (strcmp(input, "exit 0") == 0) {
      break;
    }

    char *ptr = input;
    char *cmd = strsep(&ptr, " ");

    if (strcmp(cmd, "echo") == 0) {
      if (ptr == NULL) {
        printf("\n");
      }
      printf("%s\n", ptr);
    } else {
      printf("%s: command not found\n", cmd);
    }
  }
  return 0;
}
