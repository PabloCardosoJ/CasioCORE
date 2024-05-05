#include <stdint.h>

#ifndef CALENDAR_H_
#define CALENDAR_H_


typedef union _Rtcc_Ctrl
{
    uint8_t Register;   /*8 bit vaiable*/
    struct _bits
    {
        uint8_t clk_en : 1;     /*!< clock enable flag      */
        uint8_t al_set : 1;     /*!< alarm set flag         */
        uint8_t al_active : 1;  /*!< alarm active flag      */
        uint8_t none : 4;
    } bits;
} Rtcc_Ctrl;

typedef struct _Rtcc_Clock 
{
    uint8_t tm_sec;         /*!< seconds,  range 0 to 59          */
    uint8_t tm_min;         /*!< minutes, range 0 to 59           */
    uint8_t tm_hour;        /*!< hours, range 0 to 23             */
    uint8_t tm_mday;        /*!< day of the month, range 1 to 31  */
    uint8_t tm_mon;         /*!< month, range 1 to 12             */
    uint16_t tm_year;       /*!< years in rage 1900 2100          */
    uint8_t tm_wday;        /*!< day of the week, range 0 to 6    */
    uint8_t al_min;         /*!< alarm minutes, range 0 to 59     */
    uint8_t al_hour;        /*!< alarm hours, range 0 to 23       */
    uint8_t mt_days[13u];   /*!< max days on each month */
    Rtcc_Ctrl ctrl;      /*!< clock control bits */
} Rtcc_Clock;

uint8_t isLeapYear( uint16_t year );
void FebruarysDays(Rtcc_Clock *rtcc, uint8_t year );

void Rtcc_clockInit( Rtcc_Clock *rtcc );
uint8_t Rtcc_setTime( Rtcc_Clock *rtcc, uint8_t hour, uint8_t minutes, uint8_t seconds );
uint8_t Rtcc_setDate( Rtcc_Clock *rtcc, uint8_t day, uint8_t month, uint16_t year, uint8_t weekday );
uint8_t Rtcc_setAlarm( Rtcc_Clock *rtcc, uint8_t hour, uint8_t minutes );
void Rtcc_getTime( Rtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes, uint8_t *seconds );
void Rtcc_getDate( Rtcc_Clock *rtcc, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *weekDay );
uint8_t Rtcc_getAlarm( Rtcc_Clock *rtcc, uint8_t *hour, uint8_t *minutes );
void Rtcc_clearAlarm( Rtcc_Clock *rtcc );
uint8_t Rtcc_getAlarmFlag( Rtcc_Clock *rtcc );
void Rtcc_periodicTask( Rtcc_Clock *rtcc );

#endif