#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

static char *open_json(const char *json) {
    FILE *fp = fopen(json, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char *buffer = malloc(sz);
    if (buffer && fread(buffer, 1, sz, fp) == sz) {
        return buffer;
    }

     fprintf(stderr, "Failed to read file\n");
    return NULL;
}

int main(void) {
    char *buf = open_json("../display.json");
    if (buf == NULL) {
        fprintf(stderr, "Failed to open json file\n");
        return -EINVAL;
    }

    cJSON *root = cJSON_Parse(buf);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        goto end;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("Monitor \"%s\"\n", name->valuestring);
    }

    cJSON *resolution = NULL;
    cJSON *resolutions = cJSON_GetObjectItemCaseSensitive(root, "resolutions");
    cJSON_ArrayForEach(resolution, resolutions) {
        cJSON *width = cJSON_GetObjectItemCaseSensitive(resolution, "width");
        cJSON *height = cJSON_GetObjectItemCaseSensitive(resolution, "height");
        if (cJSON_IsNumber(width) && cJSON_IsNumber(height)) {
            printf("Resolution: %d x %d\n", (int) width->valuedouble, (int)height->valuedouble);
        }
    }
end:
    free(buf);
    cJSON_Delete(root);
    return 0;
}