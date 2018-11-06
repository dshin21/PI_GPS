#include "gps-utils.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: November 12, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin, Simon Chen
--
-- PROGRAMMER: Simon Chen
--
-- INTERFACE: int main(void)
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
            printf("User terminating program.");
            return -1;
        }
        else
        {
            print_gps_information(gps_data);
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: November 12, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
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
void flush(struct gps_data_t *gps_data)
{
    gps_stream(gps_data, WATCH_DISABLE, NULL);
    gps_close(gps_data);
    free(gps_data);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: November 12, 2018
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
-- FUNCTION: main
--
-- DATE: November 12, 2018
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
void errorHandler(int errorNumber){
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

