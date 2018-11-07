#pragma once

#define TIMEOUT -2
#define READ_FAIL -3
#define CONNECTION_FAIL -4
#define STREAM_FAIL -5
#define USER_TERMINATED 1;

#include <gps.h>
#include <stdlib.h>
#include "gpsprint.h"
#include <pthread.h>
#include <errno.h>

int search(struct gps_data_t *gps_data);
void flush(struct gps_data_t *gps_data);
void *checkInput();
void errorHandler(int errorNumber);
