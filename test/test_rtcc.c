#include "unity.h"
#include "rtcc.h"
#include <time.h>


Rtcc_Clock Rtcc;


long milliseconds( void );


void setUp(void)
{
    Rtcc_clockInit( &Rtcc );
}

void tearDown(void)
{
}


void test__initClock()
{
    Rtcc_clockInit( &Rtcc );


    TEST_ASSERT_EQUAL( 1900, Rtcc.tm_year );
    TEST_ASSERT_EQUAL( 1, Rtcc.ctrl.bits.clk_en );
    TEST_ASSERT_EQUAL( 0, Rtcc.ctrl.bits.al_active );

    printf( "initClock test succeed" );
}


void test__setTime()
{
    uint8_t success = Rtcc_setTime( &Rtcc, 22, 29, 6 );
    uint8_t hError = Rtcc_setTime( &Rtcc, 24, 29, 6 );
    uint8_t mError = Rtcc_setTime( &Rtcc, 10, 70, 6 );
    uint8_t sError = Rtcc_setTime( &Rtcc, 17, 5, 80 );

    TEST_ASSERT_EQUAL( 1, success );
    TEST_ASSERT_EQUAL( 0, hError );
    TEST_ASSERT_EQUAL( 0, mError );
    TEST_ASSERT_EQUAL( 0, sError );


    printf( "setTime test succeed" );
}


void test__setDate()
{
    uint8_t success = Rtcc_setDate( &Rtcc, 20, 7, 2002, 0 );
    uint8_t dError = Rtcc_setDate( &Rtcc, 35, 2, 1900, 1 );
    uint8_t mError = Rtcc_setDate( &Rtcc, 10, 70, 1950, 2 );
    uint8_t yError = Rtcc_setDate( &Rtcc, 31, 6, 2200, 3 );

    TEST_ASSERT_EQUAL( 1, success );
    TEST_ASSERT_EQUAL( 0, dError );
    TEST_ASSERT_EQUAL( 0, mError );
    TEST_ASSERT_EQUAL( 0, yError );


    printf( "setDate test succeed" );
}


void test__getTime()
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    
    Rtcc_setTime( &Rtcc, 22, 29, 6 );Rtcc_getTime(&Rtcc, &hour, &min, &sec);

    TEST_ASSERT_EQUAL( 22, hour );
    TEST_ASSERT_EQUAL( 29, min );
    TEST_ASSERT_EQUAL( 6, sec );

    printf( "getTime test succeed" );
}


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


void test__getAlarm()
{
    uint8_t hour;
    uint8_t min;
    
    Rtcc_setAlarm( &Rtcc, 22, 8 );

    Rtcc_getAlarm( &Rtcc, &hour, &min );

    TEST_ASSERT_EQUAL( 22, hour );
    TEST_ASSERT_EQUAL( 8, min );

    printf( "getDate test succeed" );
}


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
void test__leapYear(void)
{
    Rtcc_setDate( &Rtcc, 31, 12, 2024, 5 );

    TEST_ASSERT_EQUAL(29, Rtcc.mt_days[2]);
}



long milliseconds( void )
{
    /*
        Returns time in miliseconds
    */
    return clock() / ( CLOCKS_PER_SEC / 1000 );
}




