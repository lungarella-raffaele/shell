#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 256
#define MAX_TOKENS 100

char *remove_leading(char *cmd);
void reset_tokens(char *tokens[MAX_TOKEN_LENGTH]);
void parse_tokens(char *cmd, char *tokens[MAX_TOKEN_LENGTH], int *token_count);
