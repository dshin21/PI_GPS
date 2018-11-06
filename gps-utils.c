/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gps-utils.c - 
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
-- DESIGNER: Simon Chen, Daniel Shin
--
-- PROGRAMMER: Simon Chen
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
#include "gps-utils.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: search
--
-- DATE: November 4, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Simon Chen, Daniel Shin 
--
-- PROGRAMMER: Simon Chen
--
-- INTERFACE: search(struct gps_data_t *gps_data)
--
-- RETURNS: int
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
int search(struct gps_data_t *gps_data)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, checkInput, NULL);

    while (true) {
        if (!gps_waiting(gps_data, 5000000)) {
            errorHandler(TIMEOUT);
            return -1;
        }
        else if (gps_read(gps_data) == -1) {
            errorHandler(READ_FAIL);
            return -1;
        }
        else if (inputTracker) {
            printf("User terminating program.");
            return -1;
        }
        else {
            print_gps_information(gps_data);
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: flush
--
-- DATE: November 4, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: void flush(struct gps_data_t *gps_data)
--
-- RETURNS: void
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
void flush(struct gps_data_t *gps_data)
{
    gps_stream(gps_data, WATCH_DISABLE, NULL);
    gps_close(gps_data);
    free(gps_data);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: checkInput
--
-- DATE: November 4, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Simon Chen
--
-- PROGRAMMER: Simon Chen
--
-- INTERFACE: void *checkInput()
--
-- RETURNS: void*
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
void *checkInput()
{
    char input;
    while((input = getchar())) {
        if(input == 's') {
            inputTracker = USER_TERMINATED;
            break;
        }
    }

    pthread_exit(0);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: errorHandler
--
-- DATE: November 4, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Simon Chen
--
-- PROGRAMMER: Simon Chen
--
-- INTERFACE: void errorHandler(int errorNumber)
--
-- RETURNS: void
--
-- NOTES:
--		
----------------------------------------------------------------------------------------------------------------------*/
void errorHandler(int errorNumber)
{
    switch(errorNumber){
    case -2:
        printf("GPS device timedout\n");
        break;
    case -3:
        printf("Reading GPS device failed\n");
        break;
    case -4:
        printf("Unable to connect to GPS device\n");
        printf("%s\n", gps_errstr(errno));
        break;
    case -5:
        printf("error streaming to GPS device\n");
        break;
    default :
        break;
    }
}

