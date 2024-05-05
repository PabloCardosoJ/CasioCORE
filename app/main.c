#include <stdio.h>
#include <stdint.h>
#include "scheduler.h"
#include "queue.h"
#include "rtcc.h"


/**
 * @brief   message to be used for intercomunicate both tasks
*/
typedef struct _Message
{
    uint8_t msg;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t day;
    uint8_t wday;
    uint8_t month;
    uint16_t year;
} Message;


#define TASKS_N     2
#define TIMERS_N    4
#define TICK_VAL    100
#define TIME_MSG    0
#define DATE_MSG    1


static Sched_Task tasks[ TASKS_N ];
static Sched_Scheduler Sche;
static Rtcc_Clock rtccClock;
static Que_Queue rtccQueue;
static Sched_Timer timers[ TIMERS_N ];
static uint8_t SetDateTimeID;
static uint8_t SetDateTimeID1;
static uint8_t SetDateTimeID2;
static uint8_t SetDateTimeID3;
static uint8_t Rtcc_callbackID;
static uint32_t setTimes[] = {5000, 3000, 8000};
static uint8_t setTimesIndx = 0;


void Init_500ms(void);
void Init_1000ms(void);
void Task_500ms(void);
void Task_1000ms(void);
void Rtcc_callback(void);
void SetDateTime(void);


int main( void )
{
    static Message Messages[6u];

    /*create the queue to store as max 6 items*/
    rtccQueue.Buffer = Messages;
    rtccQueue.Elements = 6;
    rtccQueue.Size = sizeof( Message );
    Queue_initQueue( &rtccQueue );

    /*init the scheduler with two tasks and a tick time of 100ms and run for 10 seconds only*/
    Sche.tick = TICK_VAL;
    Sche.tasks = TASKS_N;
    Sche.taskPtr = tasks;
    Sche.timeout = 25000;
    Sche.timers = TIMERS_N;
    Sche.timerPtr = timers;

    Sched_initScheduler( &Sche );
    
    /*register two task with thier corresponding init fucntions and their periodicyt, 100ms and 500ms*/
    Sched_registerTask( &Sche, Init_500ms, Task_500ms, 500 );
    Sched_registerTask( &Sche, Init_1000ms, Task_1000ms, 1000 );
    
    SetDateTimeID = Sched_registerTimer( &Sche, setTimes[0], SetDateTime);
    Rtcc_callbackID = Sched_registerTimer( &Sche, 1000, Rtcc_callback );

    /*run the scheduler for the mount of time stablished in Sche.timeout*/
    Sched_startTimer( &Sche, SetDateTimeID );
    Sched_startTimer( &Sche, Rtcc_callbackID );

    Sched_startScheduler( &Sche );
    
    return 0;
}



/**
 * @brief   Init task for 500ms task
 * 
 * Print a message to the console to indicate that the task is running
*/
void Init_500ms(void)
{
    printf("Init task 500 millisecond\n");
}

/**
 * @brief   Init task for 1000ms task
 * 
 * Print a message to the console to indicate that the task is running
 * and init the clock to 12:30:00 24/6/1984
*/
void Init_1000ms(void)
{
    Rtcc_clockInit( &rtccClock );
    Rtcc_setTime( &rtccClock, 12, 30, 0 );
    Rtcc_setDate( &rtccClock, 24, 6, 1984, 5 );
    printf("Init task 1000 millisecond\n");
}

/**
 * @brief   500ms task
 * 
 * Read the queue and print the time and date if a new message arrives
*/
void Task_500ms(void)
{
    Message msgToRead;

    /*Query if a new message arrive from the queue*/
    while ( Queue_isQueueEmpty( &rtccQueue ) == 0 )
    {
        /*Read the message in the queue*/
        Queue_readData( &rtccQueue, &msgToRead );

        if( msgToRead.msg == TIME_MSG )
        {
            printf("Time - %d:%d:%d\n", msgToRead.hour, msgToRead.minutes, msgToRead.seconds );
        }
        else if ( msgToRead.msg == DATE_MSG)
        {
            printf("Date - %d/%d/%d\n", msgToRead.day, msgToRead.month, msgToRead.year );
        }
    }
}


/**
 * @brief   1000ms task
 * 
 * Poll the Rtcc periodic task and get the time and date from the clock and send it to 
 * the 500ms task using a queue
*/
void Task_1000ms(void)
{
    Message msgToWriteTime;
    Message msgToWriteDate;

    /*get time and date*/
    Rtcc_getTime( &rtccClock, &msgToWriteTime.hour, &msgToWriteTime.minutes, &msgToWriteTime.seconds );
    Rtcc_getDate( &rtccClock, &msgToWriteDate.day, &msgToWriteDate.month, &msgToWriteDate.year, &msgToWriteDate.wday );

    msgToWriteTime.msg = TIME_MSG;
    msgToWriteDate.msg = DATE_MSG;
    
    /*send time and date to 500ms task using a queue*/
    Queue_writeData( &rtccQueue, &msgToWriteTime );
    Queue_writeData( &rtccQueue, &msgToWriteDate );

}


/**
 * @brief   Rtcc_calback
 * 
 * Used to call Rtcc periodic task every second
*/
void Rtcc_callback(void)
{
    Rtcc_periodicTask( &rtccClock );
    Sched_reloadTimer( &Sche, Rtcc_callbackID, 1000 );
}


/**
 * @brief SetDateTime
 * 
 * Set a new value for date and time at differerent elapsed times
 * 
*/
void SetDateTime(void)
{
    if (setTimesIndx == 0)
    {
        Rtcc_setDate( &rtccClock, 10, 12, 2002, 5 );
        Rtcc_setTime( &rtccClock, 8, 54, 2 );

    }
    else if (setTimesIndx == 1)
    {
        Rtcc_setDate( &rtccClock, 5, 9, 1998, 4 );
        Rtcc_setTime( &rtccClock, 4, 39, 11 );

    }
    else if (setTimesIndx == 2)
    {
        Rtcc_setDate( &rtccClock, 9, 1, 2024, 3 );
        Rtcc_setTime( &rtccClock, 16, 5, 6 );

    }

    setTimesIndx = ( setTimesIndx + 1 ) % ( sizeof( setTimes ) / sizeof( setTimes[0] ) );

    Sched_reloadTimer( &Sche, SetDateTimeID, setTimes[ setTimesIndx ] );


}
