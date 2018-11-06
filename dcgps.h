//dcgps.h
#pragma once

#include <gps.h>
#include "gps-utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOCAL_HOST "127.0.0.1"

struct fixsource_t
{
    char *spec;
    char *server;
    char *port;
    char *device;
};
