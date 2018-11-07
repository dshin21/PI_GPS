/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gps-utils.c - This file contains the core functionalities of the program.
--                            
--                             
-- PROGRAM: dcgps
--
-- FUNCTIONS: 
-- 		    int search(struct gps_data_t *gps_data)
--          void flush(struct gps_data_t *gps_data)
--          void *checkInput()
--          void errorHandler(int errorNumber)
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
-- The functions in this file are wrapper functions of the gpsd library	needed to scan the satellites
-- using the gpsd daemon along with the GPS dongle. 
-- 
-- These functions are responsible for searching for the satellites, which is then validated to make 
-- sure they are valid information and notifying the user if an error has occurred. Once the program
-- is terminated, all resources that were acquired are deallocated.
----------------------------------------------------------------------------------------------------------------------*/
#include "gps-utils.h"
int inputTracker = 0;

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
--              struct gps_data_t *gps_data : pointer to the gps_data_t structure
--
-- RETURNS: int; if the GPS is timed out for more than 5 seconds, GPS cannot be read or user terminates
--               the program, returns -1 
--
-- NOTES:
-- Constantly monitors the connection of the GPS dongle to make sure the GPS connection is sound and creates
-- another thread to listen for user input. If it detects that either: gps timed out, gps failed to read, or the user
-- entered "s", the function exits returning -1. Otherwise, it will call a function to print out the gps information
-- that was scanned.
----------------------------------------------------------------------------------------------------------------------*/
int search(struct gps_data_t *gps_data)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, checkInput, NULL);

    while (true)
    {
        if (!gps_waiting(gps_data, 5000000))
        {
            errorHandler(TIMEOUT);
            return -1;
        }
        else if (gps_read(gps_data) == -1)
        {
            errorHandler(READ_FAIL);
            return -1;
        }
        else if (inputTracker)
        {
            printf("\033[0;31m\nUser terminating program.\n");
            return -1;
        }
        else
        {
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
--              struct gps_data_t *gps_data : pointer to the gps_data_t structure
--
-- RETURNS: void
--
-- NOTES:
-- This function is called to release all resources aquired by the lifetime of the program. It stops the 
-- GPS dongle from scanning anymore satellites and disconnects. Furthremore, the function deallocates the acquired
-- memory of the gps_data_t structure.
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
-- This function constantly listens to the user input of 's' on another thread. If the user enters 's', the program 
-- is terminated.
----------------------------------------------------------------------------------------------------------------------*/
void *checkInput()
{
    char input;
    while ((input = getchar()))
    {
        if (input == 's')
        {
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
-- INTERFACE: void errorHandler(int errorNumber, struct gps_data_t *gps_data)
--              int errorNumber: represents the type of the error that had occurred
--              struct gps_data_t *gps_data : pointer to the gps_data_t structure
--
-- RETURNS: void
--
-- NOTES:
-- This function is responsible for informing the user about the error that had occured by printing a message 
-- corresponding to the error and releases all resources acquired. 
----------------------------------------------------------------------------------------------------------------------*/
void errorHandler(int errorNumber, struct gps_data_t *gps_data)
{
    switch (errorNumber)
    {
    case -2:
        printf("\033[0;31m\nGPS device timedout\n");
        break;
    case -3:
        printf("\033[0;31m\nReading GPS device failed\n");
        break;
    case -4:
        printf("\033[0;31m\nUnable to connect to GPS device\n");
        printf("%s\n", gps_errstr(errno));
        break;
    case -5:
        printf("\033[0;31m\nerror streaming to GPS device\n");
        break;
    default:
        flush(gps_data);
        break;
    }
}
