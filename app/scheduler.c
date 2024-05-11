/**
 * @file    scheduler.c
 * @brief   Scheduler's source code
 *
 * This is a scheduler that manages different amount of timers and task
 */


#include <time.h>
#include <stdint.h>
#include "scheduler.h"


/**
 * @brief Counting of number of timers
*/
uint8_t timerID = 0;


/**
 * @brief   milliseconds count function
 *
 * Function to get the milliseconds the program has been running
 *
 * @retval  long The number of millisecond the program has been running
 *
 */
long milliseconds( void )
{
    return clock() / ( CLOCKS_PER_SEC / 1000 );
}

/**
 * @brief Init Scheduler function
 * 
 * This funcitons initializes the scheduler
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable, this is the scheduler
 * 
 * @retval None
*/
void Sched_initScheduler( Sched_Scheduler *scheduler )
{
    scheduler->tasksCount = 0;
    scheduler->ticksCount = 0;
}



/**
 * @brief Register task function
 * 
 * This function register a task into the scheduler
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param initPtr[in] Pointer to a function with no parameters and without return, this is the init function
 * @param taskPtr[in] Pointer to a function without parameters and without return value. This function will be executed periodically
 * @param period[in] How often you want the task run
 * 
 * @retval The number of the task you just registered. 0 in case of error
*/
uint8_t Sched_registerTask( Sched_Scheduler *scheduler, void (*initPtr)(void), void (*taskPtr)(void), uint32_t period )
{
    Sched_Task *task = scheduler->taskPtr;
    uint32_t tick = scheduler->tick;
    uint32_t exit;

    if ( ( period < tick ) || ( period % tick != 0 ) )
    {
        exit = 0;
    }
    else
    {
        
        ( task + scheduler->tasksCount )->period = period;             // Seting the period
        ( task + scheduler->tasksCount )->initFunc = initPtr;
        ( task + scheduler->tasksCount )->taskFunc = taskPtr;
        ( task + scheduler->tasksCount )->elapsed = 0;                 // Initializing time elapsed as 0
        ( task + scheduler->tasksCount )->startFlag = 1;

        ( task + scheduler->tasksCount )->initFunc();                  // Run init function
        
        exit = ++scheduler->tasksCount;
    }

    return exit;
    


}


/**
 * @brief Stop task function
 * 
 * This function stops a scheduler's task
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param task[in] The number of the task you want to stop
 * 
 * @retval 1 if the task has been stopped correctly, or 0 otherwise
*/
uint8_t Sched_stopTask( Sched_Scheduler *scheduler, uint8_t task )
{
    uint8_t exit;

    if( task < 1 || task >= scheduler->tasks )
    {
        exit = 0;
    }
    else
    {
        Sched_Task *actual_task =  (scheduler->taskPtr + task - 1);

        actual_task->startFlag = 0;
        exit = 1;
    }
    

    return exit;

}


/**
 * @brief Start task function
 * 
 * This function starts a secheduler's task
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param task[in] The number of the task you want to start
 * 
 * @retval 1 if the task has been started correctly, or 0 otherwise
*/
uint8_t Sched_startTask( Sched_Scheduler *scheduler, uint8_t task )
{
    uint8_t exit = 0;

    if ( ( task >= 1 ) && ( task <= scheduler->tasks ) )
    {
        Sched_Task *actual_task =  (scheduler->taskPtr + task - 1);

        actual_task->startFlag = 1;

        exit = 1;
    }

    return exit;
}

/**
 * @brief Period task function
 * 
 * This function changes a task's period
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param task[in] The number of the task you want to stop
 * @param period[in] Task periodicity
 * 
 * @retval 1 if the task has been stopped correctly, or 0 otherwise
*/
uint8_t Sched_periodTask( Sched_Scheduler *scheduler, uint8_t task, uint32_t period )
{
   uint8_t exit = 0;

    if( (period % scheduler->tick == 0 ) && ( task >= 1 ) && ( task < scheduler->tasks ) )                         // Period has to be a multiple of tick
    {
        ( scheduler->taskPtr + task - 1 )->period = period;
        exit = 1;
    }

    return exit;
}


/**
 * @brief Start scheduler function
 * 
 * This is the scheduler, this function manages the scheduler's timers and tasks
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * 
 * @retval None
*/
void Sched_startScheduler( Sched_Scheduler *scheduler )
{
    long time = milliseconds();

    while ( 1 )
    {
        
        // Tasks
        for ( uint8_t i = 0; i < scheduler->tasksCount; i++ )
        {

            Sched_Task *actual_task = scheduler->taskPtr + i;

            if ( actual_task->startFlag )         // Run task only if starFlag is True 
            {
                if ( actual_task->elapsed >= actual_task->period )     
                {

                    actual_task->taskFunc();                                // Run function 
                    actual_task->elapsed = 0;

                }
                else
                {
                    actual_task->elapsed += scheduler->tick;   // Counting time
                }

            }
            
        }


        // Timers
        for ( uint8_t i = 0; i < scheduler->timers; i++ )
        {
            Sched_Timer *actual_timer = scheduler->timerPtr + i;

            if ( actual_timer->startFlag )         // Run task only if starFlag is True
            {
                if ( actual_timer->count == 0 )
                {
                    actual_timer->callbackPtr();
                }
                else
                {
                    actual_timer->count -= scheduler->tick; // Subtracting elapsed time from timeout
                }

            }

        }


        if (scheduler->ticksCount >= ( scheduler->timeout / scheduler->tick))
        {
            break;          // Finish the scheduler
        }

        while ( milliseconds() - time < scheduler->tick )        // Wait for the tick
        {
            // Wait tick seconds
        }

        time = milliseconds();
        scheduler->ticksCount++;
        
    }
}


/**
 * @brief Register timer function
 * 
 * This function register a timer in the scheduler
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param timeout[in] The time the timer should run
 * @param callbackPtr[in] The function that will be executed when the time is over
 * 
 * @retval The number of the timer you just registered. 0 in case of error
*/
uint8_t Sched_registerTimer( Sched_Scheduler *scheduler, uint32_t timeout, void (*callbackPtr)(void) )
{
   uint8_t exit = 0;
   Sched_Timer *actual_timer = scheduler->timerPtr + timerID;


    if ( timeout % scheduler->tick == 0 && timeout >= scheduler->tick )             // Timeout has to be multiple of tick
    {

        actual_timer->timeout = timeout;
        actual_timer->count = timeout;
        actual_timer->callbackPtr = callbackPtr;
        actual_timer->startFlag = 0;
        

        
        exit = ++timerID;
    }

    return exit;
}


/**
 * @brief Get timer function
 * 
 * This function gets the time remeaning of the timer
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param timer[in] The number of the timer you want to start
 * 
 * @retval The time remeaning of the timer
*/
uint32_t Sched_getTimer( Sched_Scheduler *scheduler, uint8_t timer )
{ 
    uint32_t exit = 0;
    
    if ( ( timer <= scheduler->timers ) && ( timer > 0 ) )
    {
        exit = (scheduler->timerPtr + timer - 1)->count;
    }
    
    return exit;
}


/**
 * @brief Reload timer function
 * 
 * This function realoads the timer's count and it can be used to change it's period
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param timer[in] The number of the timer you want to start
 * @param timeout[in] The time the timer count before executing the callback function
 * 
 * @retval 1 if the task has been reloaded correctly, or 0 otherwise
*/
uint8_t Sched_reloadTimer( Sched_Scheduler *scheduler, uint8_t timer, uint32_t timeout )
{
    uint8_t reload = 0;

    if ( ( timer > 0 ) && (timer <= scheduler->timers) && (timeout % scheduler->tick == 0) && (timeout > scheduler->tick) )   // Cheking if the timer is registered and if
    {                                                                                                       // timeout is multiple of tick

        Sched_Timer *actual_timer = (scheduler->timerPtr + timer - 1);       // Pointing to actual timer

        actual_timer->timeout = timeout;
        actual_timer->count = timeout;      // Reload count


        reload = 1;
    }

    return reload;
}


/**
 * @brief Start timer function
 * 
 * This function enables the selected timer
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param timer[in] The number of the timer you want to start
 * 
 * @retval 1 if the timer has been started correctly, or 0 otherwise
*/
uint8_t Sched_startTimer( Sched_Scheduler *scheduler, uint8_t timer )
{ 
    unsigned char exit = 0;

    if ( ( timer > 0 ) && ( timer <= scheduler->timers ) )
    {
        Sched_Timer *actual_timer = scheduler->timerPtr + timer - 1;

        
        actual_timer->count = actual_timer->timeout;             // Restart timer
        actual_timer->startFlag = 1;
        exit = 1;
    }
    
    return exit;
    
}


/**
 * @brief Stop timer function
 * 
 * This function disables the selected timer
 * 
 * @param scheduler[in] Pointer to a Sched_Scheduler variable
 * @param timer[in] The number of the timer you want to start
 * 
 * @retval 1 if the timer has been stipped correctly, or 0 otherwise
*/
uint8_t Sched_stopTimer( Sched_Scheduler *scheduler, uint8_t timer )
{
    unsigned char exit = 0;

    if ( ( timer > 0 ) && ( timer <= scheduler->timers ) )
    {
        Sched_Timer *actual_timer = scheduler->timerPtr + timer - 1;

        actual_timer->startFlag = 0;
    }
    
    return exit;

}
