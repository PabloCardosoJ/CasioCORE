#include <assert.h>
#include "unity.h"
#include "scheduler.h"


#define TRUE 1
#define FALSE 0

#define TASKS_N     12
#define TIMERS_N    12
#define TICK_VAL    100

static Sched_Task tasks[ TASKS_N ];
static Sched_Scheduler Sche;
static Sched_Timer timers[ TIMERS_N ];
static uint8_t count = 0;


void fun1(void);
void fun2(void);

void setUp(void)
{
    Sche.tick = TICK_VAL;
    Sche.tasks = TASKS_N;
    Sche.timeout = 500;
    Sche.taskPtr = tasks;
    Sche.timers = TIMERS_N;
    Sche.timerPtr = timers;
}

void tearDown(void)
{
}


/**
 * @brief Test initScheduler function
 * 
 * This test verifies if all the parameters of the scheduler are correctly initialized
*/
void test__initScheduler()
{
    Sched_initScheduler( &Sche );

    TEST_ASSERT_EQUAL( 0, Sche.tasksCount );

    printf("initScheduler test succeed");
}


/**
 * @brief Test stopTask function
 * 
 * This test if the task is correctly stoped
*/
void test__stopTask(void)
{
    Sched_initScheduler( &Sche );

    Sched_registerTask( &Sche, fun1, fun2, 500 );

    Sched_startScheduler( &Sche );

    uint32_t elapedBeforeStop = Sche.taskPtr[0].elapsed;

    Sched_stopTask( &Sche, 1 );

    Sched_startScheduler( &Sche );

    uint32_t elapedAftertop = Sche.taskPtr[0].elapsed;

    TEST_ASSERT_EQUAL( elapedBeforeStop, elapedAftertop );

    printf( "Test stopTask function succeed" );
}


/**
 * @brief Test stop non exisiting Task function
 * 
 * This test tries to stop an unnexisting
*/
void test__stopNonExistingTask(void)
{
    Sched_initScheduler( &Sche );

    uint8_t res = Sched_stopTask( &Sche, 0 );
    uint8_t res2 = Sched_stopTask( &Sche, 34 );

    TEST_ASSERT_EQUAL( FALSE, res );
    TEST_ASSERT_EQUAL( FALSE, res2 );

    printf( "Test stop non exisisting task succeed" );
}


/**
 * @brief Test stop multiple tasks
 * 
 * This test initialize multiple tasks and stop some of them, after that it verifies if the tasks were correctly stop and if the
 * others works as intended
*/
void test__stopMultipleTasks(void)
{
    uint32_t elapsedBeforeStop[2];
    uint32_t elapsedAfterStop[5];

    Sched_initScheduler( &Sche );

    Sched_registerTask( &Sche, fun1, fun2, 200 );
    Sched_registerTask( &Sche, fun1, fun2, 200 );
    Sched_registerTask( &Sche, fun1, fun2, 200 );

    /* Getting elapsed time of taks 1 and 3 */
    elapsedBeforeStop[ 0 ] = Sche.taskPtr[0].elapsed;
    elapsedBeforeStop[ 1 ] = Sche.taskPtr[2].elapsed;


    Sched_startScheduler( &Sche );

    /* Stopping task 1 and 3 */
    Sched_stopTask( &Sche, 1 );
    Sched_stopTask( &Sche, 3 );

        
    Sched_startScheduler( &Sche );

    /* Getting elapsed time of task 1 and 3 after stop them and re-run */
    elapsedAfterStop[ 0 ] = Sche.taskPtr[0].elapsed;
    elapsedAfterStop[ 1 ] = Sche.taskPtr[2].elapsed;

    /* Both times should be equal */
    TEST_ASSERT_EQUAL( elapsedBeforeStop[0], elapsedAfterStop[0] );
    TEST_ASSERT_EQUAL( elapsedBeforeStop[1], elapsedAfterStop[1] );


    printf( "Test stopMultipleTasks function succeed" );
    
}


/**
 * @brief Test stopTimer function
 * 
 * This test verify if the timer is correctly stoped
*/
void test__stopTimer(void)
{
    Sched_initScheduler( &Sche );

    Sched_registerTimer( &Sche, 200, fun1 );

    Sched_startTimer( &Sche, 1 );

    Sched_startScheduler( &Sche );

    uint32_t countBeforeStop = Sche.timerPtr[0].count;     // Takes de final value of count

    Sched_stopTimer( &Sche, 1 );        // Stop timer
    Sched_reloadTimer( &Sche, 1, 300);       // Reloads it's value

    Sched_startScheduler( &Sche );

    uint32_t countAftertop = Sche.timerPtr[0].count;

    uint8_t res = Sched_stopTimer( &Sche, 0 );
    uint8_t res2 = Sched_stopTimer( &Sche, 99 );

    TEST_ASSERT_NOT_EQUAL( countAftertop, countBeforeStop );
    TEST_ASSERT_EQUAL( Sche.timerPtr->timeout, countAftertop );
    TEST_ASSERT_EQUAL( FALSE, res );
    TEST_ASSERT_EQUAL( FALSE, res2 );
}


/**
 * @brief Test stop multiple timers
 * 
 * This test initialize multiple timers and stop some of them, after that it verifies if the tasks were correctly stop and if the
 * others works as intended
*/
void test__stopMultipleTimers(void)
{
    uint32_t countBeforeStop[5];
    uint32_t countAfterStop[5];

    Sched_initScheduler( &Sche );

    Sched_registerTimer( &Sche, 200, fun1 );
    Sched_registerTimer( &Sche, 200, fun1 );
    Sched_registerTimer( &Sche, 200, fun1 );

    Sched_startTimer( &Sche, 1 );
    Sched_startTimer( &Sche, 2 );
    Sched_startTimer( &Sche, 3 );


    Sched_startScheduler( &Sche );

    countBeforeStop[ 0 ] = Sche.timerPtr[0].count;
    countBeforeStop[ 1 ] = Sche.timerPtr[2].count;

    Sched_stopTimer( &Sche, 1 );
    Sched_reloadTimer( &Sche, 1, 300);
    Sched_stopTimer( &Sche, 3 );
    Sched_reloadTimer( &Sche, 3, 300);

        
    Sched_startScheduler( &Sche );


    countAfterStop[ 0 ] = Sche.timerPtr[0].count;
    countAfterStop[ 1 ] = Sche.timerPtr[2].count;

    TEST_ASSERT_NOT_EQUAL( countBeforeStop[0], countAfterStop[0] );
    TEST_ASSERT_NOT_EQUAL( countBeforeStop[1], countAfterStop[1] );

    printf( "Test stopMultipleTimers function succeed" );
    
}


/**
 * @brief Test reloadTimer task
 * 
 * This test verify if reloadTimer changes the timer's timout
*/
void test__reloadTimer(void)
{
    uint16_t reloadTime = 900;

    Sched_initScheduler( &Sche );

    Sched_initScheduler( &Sche );

    Sched_registerTimer( &Sche, 200, fun1 );

    Sched_startTimer( &Sche, 1);

    Sched_startScheduler( &Sche );

    uint8_t res = Sched_reloadTimer( &Sche, 1, reloadTime );
    uint8_t res2 = Sched_reloadTimer( &Sche , 1, 1550 );
    uint8_t res3 = Sched_reloadTimer( &Sche , 0, 1000 );
    uint8_t res4 = Sched_reloadTimer( &Sche , 1, 0 );
    uint8_t res5 = Sched_reloadTimer( &Sche , 99, 0 );

    TEST_ASSERT_EQUAL( reloadTime, Sche.timerPtr->timeout );
    TEST_ASSERT_EQUAL(TRUE, res);
    TEST_ASSERT_EQUAL(FALSE, res2);
    TEST_ASSERT_EQUAL(FALSE, res3);
    TEST_ASSERT_EQUAL(FALSE, res4);
    TEST_ASSERT_EQUAL(FALSE, res5);
}


/**
 * @brief Test register task function
 * 
 * This test verifies if registerTask function works as intended
*/
void test__registerTask(void)
{
    Sched_initScheduler( &Sche );

    uint8_t res = Sched_registerTask( &Sche, fun1, fun2, 50 );
    uint8_t res2 = Sched_registerTask( &Sche, fun1, fun2, 200 );
    uint8_t res3 = Sched_registerTask( &Sche, fun1, NULL, 200 );
    uint8_t res4 = Sched_registerTask( &Sche, fun1, NULL, 120 );

    TEST_ASSERT_EQUAL( FALSE, res );
    TEST_ASSERT_EQUAL( TRUE, res2 );
    TEST_ASSERT_NOT_EQUAL( FALSE, res3 );
    TEST_ASSERT_EQUAL( FALSE, res4 );
    TEST_ASSERT_EQUAL( NULL, Sche.taskPtr[res3-1].taskFunc );
}


/**
 * @brief Test stop non exisiting Task function
 * 
 * This test tries to stop an unnexisting
*/
void test__startNonExistingTask(void)
{
    Sched_initScheduler( &Sche );

    uint8_t res = Sched_startTask( &Sche, 0 );

    TEST_ASSERT_EQUAL( FALSE, res );

    printf( "Test start non exisisting task succeed" );
}


/**
 * @brief Test period task function
 * 
 * This test verifies if pediodTask function works as intended
*/
void test__periodTask(void)
{
    Sched_initScheduler( &Sche );

    uint8_t res = Sched_periodTask( &Sche , 0, 100 );
    uint8_t res2 = Sched_periodTask( &Sche , 1, 50 );
    uint8_t res3 = Sched_periodTask( &Sche , 1, 100 );
    uint8_t res4 = Sched_periodTask( &Sche , 99, 100 );

    TEST_ASSERT_EQUAL( FALSE, res );
    TEST_ASSERT_EQUAL( FALSE, res2 );
    TEST_ASSERT_EQUAL( TRUE, res3 );
    TEST_ASSERT_EQUAL( FALSE, res4 );
}


/**
 * @brief Test register timer function
 * 
 * This test verifies if registerTimer function works as intended
*/
void test__registerTimer(void)
{
    Sched_initScheduler( &Sche );

    uint8_t res = Sched_registerTimer( &Sche , 50, fun1 );
    uint8_t res2 = Sched_registerTimer( &Sche, 0, fun1 );

    TEST_ASSERT_EQUAL(0, res);
    TEST_ASSERT_EQUAL(0, res2);
}


/**
 * @brief Test get timer function
 * 
 * This test verifies if getTimer function works as intended
*/
void test__getTimer(void)
{
    Sched_initScheduler( &Sche );

    uint32_t res = Sched_getTimer( &Sche , 0 );
    uint32_t res2 = Sched_getTimer( &Sche, 1 );
    uint32_t res3 = Sched_getTimer( &Sche, 13 );

    TEST_ASSERT_EQUAL(FALSE, res);
    TEST_ASSERT_EQUAL( Sche.timerPtr[0].count, res2 );
    TEST_ASSERT_EQUAL( FALSE, res3 );
}


/**
 * @brief Test start timer function
 * 
 * This test verifies if startTimer function works as intended
*/
void test__startTimer(void)
{
    Sched_initScheduler( &Sche );

    uint32_t res = Sched_startTimer( &Sche , 0 );
    uint32_t res2 = Sched_startTimer( &Sche, 1 );
    uint32_t res3 = Sched_startTimer( &Sche, 99 );

    TEST_ASSERT_EQUAL( FALSE, res );
    TEST_ASSERT_EQUAL( TRUE, res2 );
    TEST_ASSERT_EQUAL( FALSE, res3 );
}


/**
 * @brief Test startTask function
 * 
 * This test if the task is correctly sarted
*/
void test__startTask(void)
{
    Sched_initScheduler( &Sche );

    Sched_stopTask( &Sche, 1 );

    Sched_startScheduler( &Sche );

    uint8_t res = Sched_startTask( &Sche, 0 );
    uint8_t res2 = Sched_startTask( &Sche, 1 );
    uint8_t res3 = Sched_startTask( &Sche, 99 );

    TEST_ASSERT_EQUAL( FALSE, res );
    TEST_ASSERT_EQUAL( TRUE, res2 );
    TEST_ASSERT_EQUAL( FALSE, res3 );

    printf( "Test startTask function succeed" );
}





void fun1(void) {}
void fun2(void) {}