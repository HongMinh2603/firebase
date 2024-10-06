#ifndef FIREBASE_H
#define FIREBASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_http_client.h"

typedef struct {
    const char* host;
    const char* auth;
} firebase_t;

esp_err_t firebase_set_int(firebase_t* firebase, const char* path, int value);

#endif // FIREBASE_H
