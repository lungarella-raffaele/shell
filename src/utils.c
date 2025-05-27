#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *is_in_path(char *cmd) {
  const char *env_path = getenv("PATH");
  if (env_path == NULL) {
    return NULL;
  }

  char *path = strdup(env_path);
  if (path == NULL) {
    return NULL;
  }

  char *path_copy = path;
  char *dir;

  while ((dir = strsep(&path, ":")) != NULL) {
    char path_to_check[MAX_LENGTH];
    snprintf(path_to_check, MAX_LENGTH, "%s/%s", dir, cmd);
    if (access(path_to_check, X_OK) == 0) {
      char *result = strdup(path_to_check);
      free(path_copy);
      return result;
    }
  }

  free(path_copy);
  return NULL;
}

void not_found(char *cmd) { printf("%s: command not found\n", cmd); }

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

void free_tokens(char **tokens) {
  if (tokens == NULL) {
    return;
  }

  for (int i = 0; tokens[i] != NULL; i++) {
    free(tokens[i]);
  }
  free(tokens);
}
