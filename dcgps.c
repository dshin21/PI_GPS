/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: dcgps.c - 
--                             
-- PROGRAM: dcgps
--
-- FUNCTIONS: 
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
--		
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
-- RETURNS: int
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
int main(void)
{
    static struct fixsource_t source;

    struct gps_data_t *gps_data = (struct gps_data_t *)malloc(sizeof(struct gps_data_t));

    printf("Connecting to GPS Device\n\n");
    if (gps_open(source.server, source.port, gps_data) != 0)
    {
        errorHandler(CONNECTION_FAIL);
        flush(gps_data);
        return -1;
    }

    unsigned int flags = WATCH_ENABLE;

    if (source.device != NULL)
        flags |= WATCH_DEVICE;

    if (gps_stream(gps_data, flags, NULL) == -1)
    {
        errorHandler(STREAM_FAIL);
        flush(gps_data);
        return -1;
    }

    if (search(gps_data) == -1)
        flush(gps_data);

    return 0;
}
