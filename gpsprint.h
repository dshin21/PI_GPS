#ifndef GPSPRINT_H
#define GPSPRINT_h

#include <gps.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)

void print_gps_information(struct gps_data_t *gps_data);

void print_satellite_information(struct gps_data_t gps_data);
#endif
