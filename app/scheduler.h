#include <stdint.h>

#ifndef SCHEDULER_H_
#define SCHEDULER_H_


/* STRUCTURES */
typedef struct _AppSched_Timer
{
    uint32_t timeout;       /*!< timer timeout to decrement and reload when the timer is re-started */
    uint32_t count;         /*!< actual timer decrement count */
    uint8_t startFlag;     /*!< flag to start timer count */
    void(*callbackPtr)(void);  /*!< pointer to callback function function */
} Sched_Timer;


typedef struct _task
{
    uint32_t period;          /*How often the task shopud run in ms*/
    uint32_t elapsed;         /*the cuurent elapsed time*/
    uint8_t startFlag;        /*flag to run task*/
    void (*initFunc)(void);   /*pointer to init task function*/
    void (*taskFunc)(void);   /*pointer to task function*/
    uint32_t absLastTime;     /* Milliseconds that there was the last time the function were executed */
    //Add more elements if required
} Sched_Task;


typedef struct _AppSched_Scheduler
{
    uint8_t tasks;         /*number of task to handle*/
    uint32_t tick;          /*the time base in ms*/
    uint8_t tasksCount;    /*internal task counter*/ 
    uint32_t timeout;      /*the number of milliseconds the scheduler should run*/
    Sched_Task *taskPtr;            /*Pointer to buffer for the TCB tasks*/
    uint8_t timers;        /*number of software timer to use*/
    Sched_Timer *timerPtr;       /*Pointer to buffer timer array*/
    uint32_t ticksCount;         /* Ticks count */ 
    //Add more private elements if required
} Sched_Scheduler;


long miliseconds( void );


/* Scheduler */
void Sched_initScheduler( Sched_Scheduler *scheduler );
uint8_t Sched_registerTask( Sched_Scheduler *scheduler, void (*initPtr)(void), void (*taskPtr)(void), uint32_t period );
uint8_t Sched_stopTask( Sched_Scheduler *scheduler, uint8_t task );
uint8_t Sched_startTask( Sched_Scheduler *scheduler, uint8_t task );
uint8_t Sched_periodTask( Sched_Scheduler *scheduler, uint8_t task, uint32_t period );
void Sched_startScheduler( Sched_Scheduler *scheduler );

/* Timer */
uint8_t Sched_registerTimer( Sched_Scheduler *scheduler, uint32_t timeout, void (*callbackPtr)(void) );
uint32_t Sched_getTimer( Sched_Scheduler *scheduler, uint8_t timer );
uint8_t Sched_reloadTimer( Sched_Scheduler *scheduler, uint8_t timer, uint32_t timeout );
uint8_t Sched_startTimer( Sched_Scheduler *scheduler, uint8_t timer );
uint8_t Sched_stopTimer( Sched_Scheduler *scheduler, uint8_t timer );


#endif