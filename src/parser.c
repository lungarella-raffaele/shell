#include "parser.h"
#include <stdio.h>

char *remove_leading(char *str) {
  while (*str == ' ' || *str == '\t') {
    str++;
  }
  return str;
}

void reset_tokens(char *tokens[MAX_TOKEN_LENGTH]) {
  int i = 0;
  while (tokens[i] != NULL) {
    tokens[i] = NULL;
    i++;
  }
}

void sanitize_tokens(char *tokens[MAX_TOKEN_LENGTH], int token_count) {
  for (int i = 0; i < token_count; i++) {
    if (tokens[i] == NULL)
      continue; // Skip NULL pointers

    char *src = tokens[i];
    char *dst = tokens[i];

    // Copy characters, skipping single quotes
    while (*src) {
      if (*src != '\'') {
        *dst = *src;
        dst++;
      }
      src++;
    }

    // Null-terminate the modified string
    *dst = '\0';
  }
}

char *remove_quotes(char *str, int str_size) {
  str[str_size - 1] = '\0';
  return &str[1];
}

void parse_tokens(char *cmd, char *tokens[MAX_TOKEN_LENGTH],
                  int *tokens_count) {
  cmd = remove_leading(cmd);
  int word_count = 0;
  int i = 0;
  bool is_in_quote = false;

  while (true) {
    if (!is_in_quote && (cmd[i] == ' ' || cmd[i] == '\0' || cmd[i] == '\n')) {
      bool is_end = (cmd[i] == '\0');
      cmd[i] = '\0';
      tokens[word_count] = &cmd[0];
      word_count++;

      if (is_end)
        break;

      cmd = remove_leading(&cmd[i + 1]);
      i = 0;
    } else {
      if (cmd[i] == '\'') {
        is_in_quote = !is_in_quote;
      }

      i++;
    }
  }

  tokens[word_count] = NULL;
  *tokens_count = word_count;

  sanitize_tokens(tokens, *tokens_count);
}
