#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int is_equal(char *str1, char *str2) { return strcmp(str1, str2) == 0; }

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
