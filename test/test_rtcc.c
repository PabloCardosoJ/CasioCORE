#include "unity.h"
#include "rtcc.h"
#include <time.h>


#define TRUE 1
#define FALSE 0


Rtcc_Clock Rtcc;


long milliseconds( void );


void setUp(void)
{
    Rtcc_clockInit( &Rtcc );
}

void tearDown(void)
{
}


/**
 * @brief   Test Rtcc_clockInit function
 *
 * The test verify that the clock is initialized
 */
void test__initClock()
{
    Rtcc_clockInit( &Rtcc );


    TEST_ASSERT_EQUAL( 1900, Rtcc.tm_year );
    TEST_ASSERT_EQUAL( 1, Rtcc.ctrl.bits.clk_en );
    TEST_ASSERT_EQUAL( 0, Rtcc.ctrl.bits.al_active );

    printf( "initClock test succeed" );
}



/**
 * @brief Test setTime Function
 * 
 * The test verify if setTime function works as intended
*/
void test__setTime()
{
    uint8_t success = Rtcc_setTime( &Rtcc, 22, 29, 6 );
    uint8_t hError = Rtcc_setTime( &Rtcc, 24, 29, 6 );
    uint8_t mError = Rtcc_setTime( &Rtcc, 10, 70, 6 );
    uint8_t sError = Rtcc_setTime( &Rtcc, 17, 5, 80 );

    TEST_ASSERT_EQUAL( TRUE, success );
    TEST_ASSERT_EQUAL( FALSE, hError );
    TEST_ASSERT_EQUAL( FALSE, mError );
    TEST_ASSERT_EQUAL( FALSE, sError );


    printf( "setTime test succeed" );
}


/**
 * @brief Test setDate Function
 * 
 * The test verify if setDate works as intended, it tries to put some invalid
 * values and see if the answer is the expected
*/
void test__setDate()
{
    uint8_t success = Rtcc_setDate( &Rtcc, 20, 7, 2002, 0 );
    uint8_t dError = Rtcc_setDate( &Rtcc, 35, 2, 1900, 1 );
    uint8_t dError2 = Rtcc_setDate( &Rtcc, 0, 2, 1800, 1 );
    uint8_t mError = Rtcc_setDate( &Rtcc, 10, 70, 1950, 2 );
    uint8_t yError = Rtcc_setDate( &Rtcc, 31, 6, 2200, 3 );
    uint8_t wError = Rtcc_setDate( &Rtcc, 3, 5, 1956, 7 );
    uint8_t aError = Rtcc_setDate( &Rtcc, 40, 0, 2200, 7 );

    TEST_ASSERT_EQUAL( TRUE, success );
    TEST_ASSERT_EQUAL( FALSE, dError );
    TEST_ASSERT_EQUAL( FALSE, dError2 );
    TEST_ASSERT_EQUAL( FALSE, mError );
    TEST_ASSERT_EQUAL( FALSE, yError );
    TEST_ASSERT_EQUAL(FALSE, wError);
    TEST_ASSERT_EQUAL( FALSE, aError);


    printf( "setDate test succeed" );
}



/**
 * @brief Test getTime Function
 * 
 * The test verify sets a time and uses getTime to see
 * if it gets the correct value
*/
void test__getTime()
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    
    Rtcc_setTime( &Rtcc, 22, 29, 6 );
    Rtcc_getTime(&Rtcc, &hour, &min, &sec);

    TEST_ASSERT_EQUAL( 22, hour );
    TEST_ASSERT_EQUAL( 29, min );
    TEST_ASSERT_EQUAL( 6, sec );

    printf( "getTime test succeed" );
}


/**
 * @brief Test getDate Function
 * 
 * The test verify sets a date and uses getDate to see
 * if it gets the correct value
*/
void test__getDate()
{
    uint8_t day;
    uint8_t month;
    uint16_t year;
    uint8_t weekDay;
    
    Rtcc_setDate( &Rtcc, 22, 8, 2002, 4 );

    Rtcc_getDate(&Rtcc, &day, &month, &year, &weekDay);

    TEST_ASSERT_EQUAL( 22, day );
    TEST_ASSERT_EQUAL( 8, month );
    TEST_ASSERT_EQUAL( 4, weekDay );
    TEST_ASSERT_EQUAL( 2002, year );

    printf( "getDate test succeed" );
}


/**
 * @brief Test getAlarm Function
 * 
 * The test verify sets an alarm and uses getAlarm to see
 * if it gets the correct value
*/
void test__getAlarm()
{
    uint8_t hour;
    uint8_t min;
    
    Rtcc_setAlarm( &Rtcc, 22, 8 );

    Rtcc_getAlarm( &Rtcc, &hour, &min );

    TEST_ASSERT_EQUAL( 22, hour );
    TEST_ASSERT_EQUAL( 8, min );

    printf( "getAlarm test succeed" );
}


/**
 * @brief Test periodicTask Function
 * 
 * The test verify time and date values are correctly updated
 * when periodicTask function is used
*/
void test__periodicTask()
{
    long t1 = milliseconds();

    Rtcc_clockInit( &Rtcc );

    Rtcc_setTime( &Rtcc, 23, 59, 59 );
    Rtcc_setDate( &Rtcc, 31, 12, 2002, 5 );


    Rtcc_periodicTask(&Rtcc);

    TEST_ASSERT_EQUAL( 31, Rtcc.mt_days[Rtcc.tm_mon] );
    TEST_ASSERT_EQUAL( 0, Rtcc.tm_hour );
    TEST_ASSERT_EQUAL( 0, Rtcc.tm_min );
    TEST_ASSERT_EQUAL( 0, Rtcc.tm_sec );
    TEST_ASSERT_EQUAL( 1, Rtcc.tm_mday );
    TEST_ASSERT_EQUAL( 6, Rtcc.tm_wday );
    TEST_ASSERT_EQUAL( 1, Rtcc.tm_mon );
    TEST_ASSERT_EQUAL( 2003, Rtcc.tm_year );

    printf( "periodicTask test succeed" );
    
}


/**
 * @brief test leapYear 
 * 
 * This test checks if February gets the correct number of days depending on whether
 * the year is a leap year or not
*/
void test__leapYear_2024(void)
{
    Rtcc_setDate( &Rtcc, 31, 12, 2024, 5 );

    TEST_ASSERT_EQUAL(29, Rtcc.mt_days[2]);
    printf( "leapYear2024 test succeed" );
}

/**
 * @brief test leapYear2001 
 * 
 * This test checks if February gets the correct number of days depending on whether
 * the year is a leap year or not
*/
void test__leapYear_2001(void)
{
    Rtcc_setDate( &Rtcc, 31, 12, 2001, 5 );

    TEST_ASSERT_EQUAL(28, Rtcc.mt_days[2]);
    printf( "leapYear2001 test succeed" );
}


/**
 * @brief test ClearAlarm function
 * 
 * This test checks if ClearAlarm function works as intended
*/
void test__ClearAlarm(void)
{
    Rtcc_clockInit( &Rtcc );
    Rtcc_setAlarm( &Rtcc, 5, 5 );
    Rtcc_setTime( &Rtcc, 5, 5, 0);

    Rtcc_clearAlarm( &Rtcc );

    TEST_ASSERT_EQUAL( FALSE, Rtcc.ctrl.bits.al_active );
    TEST_ASSERT_EQUAL( FALSE, Rtcc.ctrl.bits.al_set );
    printf( "ClearAlarm test succeed" );
}


/**
 * @brief test getAlarmFlag function
 * 
 * This test checks if Rtcc_getAlarmFlag function works as intended
*/
void test__Rtcc_getAlarmFlag(void)
{
    Rtcc_clockInit( &Rtcc );
    Rtcc_setTime( &Rtcc, 5, 5, 0);
    Rtcc_setAlarm( &Rtcc, 5, 5 );

    Rtcc_periodicTask( &Rtcc);

    uint8_t alarmFlagT = Rtcc_getAlarmFlag( &Rtcc );

    Rtcc_setTime( &Rtcc, 5, 7, 0);
    Rtcc_periodicTask( &Rtcc);
    Rtcc_clearAlarm( &Rtcc );

    uint8_t alarmFlagF = Rtcc_getAlarmFlag( &Rtcc );

    TEST_ASSERT_EQUAL( TRUE, alarmFlagT );
    TEST_ASSERT_EQUAL( FALSE, alarmFlagF );
    printf( "GetAlarmFlag test succeed" );
}


/**
 * @brief test setAlarm function
 * 
 * This function test all the posible scenarios of setAlarm and verifies if
 * the function works as intended
*/
void test__Rtcc_setAlarm(void)
{
    uint8_t res1 = Rtcc_setAlarm( &Rtcc, -1, -1);
    uint8_t res2 = Rtcc_setAlarm( &Rtcc, 24, 60);
    uint8_t res3 = Rtcc_setAlarm( &Rtcc, 24, 40);
    uint8_t res4 = Rtcc_setAlarm( &Rtcc, 20, 60);

    TEST_ASSERT_EQUAL( FALSE, res1 );
    TEST_ASSERT_EQUAL( FALSE, res2 );
    TEST_ASSERT_EQUAL( FALSE, res3 );
    TEST_ASSERT_EQUAL( FALSE, res4 );
    printf( "SetAlarm test succeed" );
}


/**
 * @brief test periodicTask when year is 2100
 * 
 * This test verify if periodicTask set year as 1900 when year gets bigger
 * than 2100
*/
void test__Rtcc_periodictTask_Year_Bigger_Than_2100(void)
{
    uint8_t day, month, weekday;
    uint16_t year;

    Rtcc_setDate( &Rtcc, 31, 12, 2100, 3 );
    Rtcc_setTime( &Rtcc, 23, 59, 59 );

    Rtcc_periodicTask( &Rtcc );

    Rtcc_getDate( &Rtcc, &day, &month, &year, &weekday );

    TEST_ASSERT_EQUAL( 1900, year );
    printf( "Year Bigger than 2100 test succeed" );
}



long milliseconds( void )
{
    /*
        Returns time in miliseconds
    */
    return clock() / ( CLOCKS_PER_SEC / 1000 );
}




