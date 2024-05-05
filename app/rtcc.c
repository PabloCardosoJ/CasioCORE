/**
 * @file    rtcc.c
 * @brief   Rtcc's source code
 *
 * This is the rtcc source code
 */


#include <stdint.h>
#include "rtcc.h"


/** 
  * @defgroup BOOL MACROS brief boolean variables declaration
  @{ */
#define TRUE 1
#define FALSE 0
/**
  @} */


/**
 * @brief Clock init function
 * 
 * This function initializes the clock
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * 
 * @retval None
*/
void Rtcc_clockInit( Rtcc_Clock *rtcc )
{
    /*
        Initialize the RTCC
    */

    /* Initializing hour */
    rtcc->tm_sec = 0;
    rtcc->tm_min = 0;
    rtcc->tm_hour = 0;

    /* Initializing date */
    rtcc->tm_mday = 1;
    rtcc->tm_mon = 1;
    rtcc->tm_year = 1900;
    rtcc->tm_wday = 0;


    /* Initializing clock bit */
    rtcc->ctrl.bits.clk_en = 1;


    /* Initializing alarm */
    rtcc->al_min = 0;
    rtcc->al_hour = 0;

    rtcc->ctrl.bits.al_active = 0;      // Disabling the alarm
    rtcc->ctrl.bits.al_set = 0;

    /* Max days of each month */
    rtcc->mt_days[0] = 0;
    rtcc->mt_days[1] = 31;
    rtcc->mt_days[2] = 28;
    rtcc->mt_days[3] = 31;
    rtcc->mt_days[4] = 30;
    rtcc->mt_days[5] = 31;
    rtcc->mt_days[6] = 30;
    rtcc->mt_days[7] = 31;
    rtcc->mt_days[8] = 31;
    rtcc->mt_days[9] = 30;
    rtcc->mt_days[10] = 31;
    rtcc->mt_days[11] = 30;
    rtcc->mt_days[12] = 31;
}


/**
 * @brief SetTime function
 * 
 * This function sets the clock's time
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * @param hour[in] Hour value
 * @param minutes[in] Minutes value
 * @param seconds[in] Seconds value
 * 
 * @retval True if the time were settled, otherwise FALSE
*/
uint8_t Rtcc_setTime( Rtcc_Clock *rtcc, uint8_t hour, uint8_t minutes, uint8_t seconds )
{
    uint8_t success = FALSE;


    if ( ( hour >= 0 && hour <= 23 ) && ( minutes >= 0 && minutes <= 59 ) && ( seconds >= 0 && seconds <= 59 ) )
    {
        rtcc->tm_hour = hour;
        rtcc->tm_min = minutes;
        rtcc->tm_sec = seconds;
        success = TRUE;
    }

    return success;
}

/**
 * @brief SetDate function
 * 
 * This function sets the clock's date
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * @param day[in] Day of the month
 * @param month[in] Actual month
 * @param year[in] Year value
 * @param weekday[in] Day of the week
 * 
 * @retval True if the time were settled, otherwise FALSE
*/
uint8_t Rtcc_setDate( Rtcc_Clock *rtcc, uint8_t day, uint8_t month, uint16_t year, uint8_t weekday )
{
    uint8_t success = FALSE;

    FebruarysDays( rtcc, year );    // Sets the number of days of February


    if ( ( day >= 1 && day <= rtcc->mt_days[month] ) && ( month >= 1 && month <= 12 ) && ( year >= 1900 && year <= 2100 ) && ( weekday >= 0 && weekday <= 6 ) )
    {
        rtcc->tm_mday = day;
        rtcc->tm_mon = month;
        rtcc->tm_year = year;
        rtcc->tm_wday = weekday;
        success = TRUE;
    }

    return success;
}


/**
 * @brief SetAlarm function
 * 
 * Sets the activation hour for the alarm
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * @param hour[in] Alarm's hour
 * @param minutes[in] Alarm's minutes
 * 
 * @retval True if the time were settled, otherwise FALSE
*/
uint8_t Rtcc_setAlarm( Rtcc_Clock *rtcc, uint8_t hour, uint8_t minutes )
{
    uint8_t success = FALSE;

    if ( ( hour >= 0 && hour <= 23 ) && ( minutes >= 0 && minutes <= 59 ) )
    {
        rtcc->al_hour = hour;
        rtcc->al_min = minutes;
        rtcc->ctrl.bits.al_set = 1;
        success = TRUE;
    }

    return success;
}


/**
 * @brief GetTime funtion
 * 
 * Returns the actual time of the clock
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * @param hour[in] Pointer to a variable where the actual hour will be save
 * @param minutes[in] Pointer to a variable where the actual minutes will be save
 * @param seconds[in] Pointer to a variable where the actual seconds will be save
 * 
 * @retval None
*/
void Rtcc_getTime( Rtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes, uint8_t *seconds )
{
   *hour = rtcc->tm_hour;
   *minutes = rtcc->tm_min;
   *seconds = rtcc->tm_sec;
}


/**
 * @brief GetDate funtion
 * 
 * Returns the actual date of the clock
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * @param day[in] Pointer to a variable where the actual day will be save
 * @param month[in] Pointer to a variable where the actual month will be save
 * @param year[in] Pointer to a variable where the actual year will be save
 * @param weekday[in] Pointer to a variable where the actual weekday will be save
 * 
 * @retval None
*/
void Rtcc_getDate( Rtcc_Clock *rtcc, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *weekDay )
{
    *day = rtcc->tm_mday;
    *month = rtcc->tm_mon;
    *year = rtcc->tm_year;
    *weekDay = rtcc->tm_wday;
}


/**
 * @brief GetAlarm function
 * 
 * Returns the actual time of the alarm
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * @param hour[in] Pointer to a variable where the alarm's hour will be save
 * @param minutes[in] Pointer to a variable where the alarm's minutes will be save
 * 
 * @retval Value of al_set
*/
uint8_t Rtcc_getAlarm( Rtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes )
{
   *hour = rtcc->al_hour;
   *minutes = rtcc->al_min;
   
   return rtcc->ctrl.bits.al_set;
}


/**
 * @brief ClearAlarm function
 * 
 * Erease the configured time of the alarm
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * 
 * @retval None
*/
void Rtcc_clearAlarm( Rtcc_Clock *rtcc )
{
   if ( ( rtcc->tm_hour == rtcc->al_hour ) && ( rtcc->tm_min == rtcc->al_min ) )
   {
        rtcc->ctrl.bits.al_active = 0;
        rtcc->ctrl.bits.al_set = 0;
   }
}


/**
 * @brief GetAlarmFlag function
 * 
 * Returns if the alarm is activated
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * 
 * @retval 1 if the alarm is activater, otherwise 0
*/
uint8_t Rtcc_getAlarmFlag( Rtcc_Clock *rtcc )
{
   return rtcc->ctrl.bits.al_active;
}


/**
 * @brief PeriodicTask function
 * 
 * Control of the clock, change the value of seconds, minutes, hours, etc.
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * 
 * @retval None
*/
void Rtcc_periodicTask( Rtcc_Clock *rtcc )
{
    /* Minutes increment */
    rtcc->tm_sec++;

    if ( rtcc->tm_sec == 60 )
    {
        rtcc->tm_min++;         // Add a minute
        rtcc->tm_sec = 0;       // Restart seconds

        /* Hours increment */
        if ( rtcc->tm_min == 60 )
        {
            rtcc->tm_hour++;         // Add an hour
            rtcc->tm_min = 0;       // Restart seconds

            /* Days increment */
            if ( rtcc->tm_hour == 24 )
            {
                rtcc->tm_mday++;         // Add a day
                rtcc->tm_wday = ( rtcc->tm_wday + 1) % 7;         // Change weekday
                rtcc->tm_hour = 0;       // Restart seconds

                /* Months increment */
                if ( rtcc->tm_mday > rtcc->mt_days[rtcc->tm_mon] )
                {
                    rtcc->tm_mday = 1;
                    rtcc->tm_mon++;

                    /* Years increment */
                    if ( rtcc->tm_mon > 12 )
                    {
                        rtcc->tm_mon = 1;
                        rtcc->tm_year++;

                        /* Year conditions */
                        if ( rtcc->tm_year > 2100)
                        {
                            rtcc->tm_year = 1900;
                        }

                        /* Getting the number of days of February */
                        FebruarysDays( rtcc, rtcc->tm_year );
                    }
                }
            }
        }
    }

    /* Alarm check */
    if ( ( rtcc->al_hour == rtcc->tm_hour ) && ( rtcc->al_min == rtcc->tm_min ) )
    {
        rtcc->ctrl.bits.al_active = TRUE;
    }


}


/**
 * @brief isLeapYear function
 * 
 * This function checks if the passed year is a leap year
 * 
 * @param year[in] This is the yoear you want to now if is a leapy year or not
 * 
 * @retval True if is a leap year, otherwise a False is returned
*/
uint8_t IsLeapYear( uint16_t year )
{
    uint8_t isLeapYear = FALSE;

    if ( ( year % 4 == 0 ) || ( ( year % 100 != 0 ) && ( year % 400 == 0 ) ) )
    {
        isLeapYear = TRUE;
    }

    return isLeapYear;
}


/**
 * @brief FebrarysDays function
 * 
 * This function sets the number of days in February depending on whether it is a leap year or not
 * 
 * @param rtcc[in] Pointer to a Rtcc_Clock variable. This is the clock control struct
 * @param year[in] Actual year
 * 
 * @retval None
*/
void FebruarysDays(Rtcc_Clock *rtcc, uint8_t year )
{
    uint8_t leapYear = IsLeapYear( year );

    if ( leapYear == TRUE )
    {
        rtcc->mt_days[2] = 29;
    }
    else
    {
        rtcc->mt_days[2] = 28;
    }
}
