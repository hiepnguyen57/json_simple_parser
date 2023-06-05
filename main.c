#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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
  if (buffer && fread(buffer, sz, 1, fp) == sz) {
    return buffer;
  }

  return NULL;
}

int main(void) {
    char *buf = open_json("../test.json");
    if (buf == NULL) {
        fprintf(stderr, "Failed to open json file\n");
        return 1;
    }

    cJSON *root = cJSON_Parse(buf);

    return 0;
}