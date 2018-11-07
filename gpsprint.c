/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gpsprint.c - This file contains functions that prints the GPS information scanned.
--                             
-- PROGRAM: dcgps
--
-- FUNCTIONS: 
--          void print_gps_information(struct gps_data_t *gps_data)
-- 		    void print_satellite_information(struct gps_data_t gps_data)
--
-- DATE: November 2, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- NOTES:
-- The functions in this file deals with the printing of the GPS information that is obtained through the GPS dongle
-- for the user to see.
-- 
-- The following details are displayed to the users:
-- PRN, Elevation, Azimuth, SNR, usage of the satellite, Latitude and Longitude.
--
-- Green text printed on the console indicates that the information is successfully obtained, while red text indicates
--  the information was not successfully obtained.
----------------------------------------------------------------------------------------------------------------------*/
#include "gpsprint.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_gps_information
--
-- DATE: November 2, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: void print_gps_information(struct gps_data_t *gps_data)
--              struct gps_data_t *gps_data : pointer to the gps_data_t structure
--
-- RETURNS: void
--
-- NOTES:
-- This function prints the time, longitude and latitude of the satellite scanned by the GPS dongle.
-- 
-- The time, latitude and longitude are displayed if a satellite was scanned successfully and will display the information
-- about all the satellites that were scanned.
----------------------------------------------------------------------------------------------------------------------*/
void print_gps_information(struct gps_data_t *gps_data)
{
    time_t t = (time_t)gps_data->fix.time;
    struct tm *time = localtime(&t);

    if (isnan(gps_data->fix.time) == 0)
    {
        printf("\n\033[0;37m%d-%d-%dT%d:%d:%d\t",
               time->tm_year + 1900,
               time->tm_mon + 1,
               time->tm_mday,
               time->tm_hour,
               time->tm_min,
               time->tm_sec);
    }

    if (gps_data->fix.mode >= MODE_2D && isnan(gps_data->fix.latitude) == 0)
        printf(" \033[0;32mLatitude: %f %c ", fabs(gps_data->fix.latitude), (gps_data->fix.latitude < 0) ? 'S' : 'N');

    if (gps_data->fix.mode >= MODE_2D && isnan(gps_data->fix.longitude) == 0)
        printf("\t\033[0;32mLongitude: %f %c\n\n", fabs(gps_data->fix.longitude), (gps_data->fix.longitude < 0) ? 'W' : 'E');
    else if (isnan(gps_data->fix.time) == 0)
        printf("%s", " \033[0;31mLatitude: n/a \tLongitude: n/a\n\n");

    if (gps_data->satellites_visible)
    {
        print_satellite_information(*gps_data);
    }

    gps_clear_fix(&gps_data->fix);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_satellite_information
--
-- DATE: November 2, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: void print_satellite_information(struct gps_data_t gps_data)
--              struct gps_data_t *gps_data : pointer to the gps_data_t structure
--
-- RETURNS: void
--
-- NOTES:
-- This function prints the detailed information about the satellites that were scanned.
--
-- Each satellite's PRN, Elevation, Azimuth, SNR and the usage of the satellite are printed
----------------------------------------------------------------------------------------------------------------------*/
void print_satellite_information(struct gps_data_t gps_data)
{
    for (int i = 0; i < MAX_POSSIBLE_SATS && gps_data.skyview[i].used; ++i)
    {
        printf("\033[0;32mPRN: %3d \tElevation: %02d \tAzimuth: %03d \tSNR: %02d \tUsed: %c\n",
               gps_data.skyview[i].PRN,
               gps_data.skyview[i].elevation,
               gps_data.skyview[i].azimuth,
               (int)gps_data.skyview[i].ss,
               ((gps_data.skyview[i].used) ? 'Y' : 'N'));
    }
}
