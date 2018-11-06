/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gpsprint.c - 
--                             
-- PROGRAM: 
--
-- FUNCTIONS: 
--		   
--
-- DATE: November 12, 2018
--
-- REVISIONS: None
--
-- DESIGNER: 
--
-- PROGRAMMER: 
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
#include "gpsprint.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_gps_information
--
-- DATE: November 12, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin, Simon Chen
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: void print_gps_information()
--
-- RETURNS: void
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
void print_gps_information(struct gps_data_t *gps_data)
{
    time_t t = (time_t)gps_data->fix.time;
    struct tm *time = localtime(&t);

    printf("\n%d-%d-%dT%d:%d:%d\t",
           time->tm_year + 1900,
           time->tm_mon + 1,
           time->tm_mday,
           time->tm_hour,
           time->tm_min,
           time->tm_sec);


    if (gps_data->fix.mode >= MODE_2D && isnan(gps_data->fix.latitude) == 0)
        printf(" Latitude: %f %c ", fabs(gps_data->fix.latitude), (gps_data->fix.latitude < 0) ? 'S' : 'N');
     else
        printf("%s", " Latitude: n/a");


    if (gps_data->fix.mode >= MODE_2D && isnan(gps_data->fix.longitude) == 0)
        printf("\tLongitude: %f %c\n\n", fabs(gps_data->fix.longitude), (gps_data->fix.longitude < 0) ? 'W' : 'E');
    else
        printf("%s", " Longitude: n/a\n\n");


    if (gps_data->satellites_visible)
    {
        print_satellite_information(*gps_data);
    }

    gps_clear_fix(&gps_data->fix);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_satellite_information
--
-- DATE: November 12, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin, Simon Chen
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: void print_satellite_information(struct gps_data_t gps_data)
--
-- RETURNS: void
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
void print_satellite_information(struct gps_data_t gps_data)
{
    for (int i = 0; i < MAX_POSSIBLE_SATS && gps_data.skyview[i].used; ++i)
    {
        printf("PRN: %3d \tElevation: %02d \tAzimuth: %03d \tSNR: %02d \tUsed: %c\n",
               gps_data.skyview[i].PRN,
               gps_data.skyview[i].elevation,
               gps_data.skyview[i].azimuth,
               (int)gps_data.skyview[i].ss,
               ((gps_data.skyview[i].used) ? 'Y' : 'N'));
    }
}
