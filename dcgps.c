/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: dcgps.c - A console based program that scans satellites to print out information about scanned satellites.
--                        
--                             
-- PROGRAM: dcgps
--
-- FUNCTIONS: 
--          int main(void)
--		   
--
-- DATE: November 4, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin, Simon Chen
--
-- PROGRAMMER: Daniel Shin
--
-- NOTES:
-- The program will continuously scan for available satellites using using the gpsd daemon with a GPS dongle 
-- until the user enters "s" to terminate.
----------------------------------------------------------------------------------------------------------------------*/
#include "dcgps.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: November 4, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin, Simon Chen
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: int main(void)
--
-- RETURNS: int; if the program completes successfully, returns 0.
--               otherwise, the function returns another integer.
--
-- NOTES:
-- This function serves as an entry point and controls the flow of the program.
-- All the necessary structures are allocated and deallocated in this function.
----------------------------------------------------------------------------------------------------------------------*/
int main(void)
{
    static struct fixsource_t source;
    struct gps_data_t *gps_data = (struct gps_data_t *)malloc(sizeof(struct gps_data_t));

    printf("Connecting to GPS Device\n\n");
    if (gps_open(source.server, source.port, gps_data) != 0)
    {
        errorHandler(CONNECTION_FAIL, gps_data);
        return -1;
    }

    unsigned int flags = WATCH_ENABLE;

    if (source.device != NULL)
        flags |= WATCH_DEVICE;

    if (gps_stream(gps_data, flags, NULL) == -1)
    {
        errorHandler(STREAM_FAIL, gps_data);
        return -1;
    }

    if (search(gps_data) == -1)
        errorHandler(-1, gps_data);

    return 0;
}
