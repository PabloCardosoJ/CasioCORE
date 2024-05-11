/**
 * @file    queue.c
 * @brief   Queue's source code
 *
 * This is the queue source code
 */


#include <stdio.h>
#include <string.h>
#include "queue.h"


/** 
  * @defgroup BOOL MACROS brief boolean variables declaration
  @{ */
#define TRUE 1
#define FALSE 0
/**
  @} */



/**
 * @brief Init Queue function
 * 
 * This function initializes the queue
 * 
 * @param queue[in] Pointer to a Que_Queue struct type. This is the queue's control struct
 * 
 * @retval None
*/
void Queue_initQueue( Que_Queue *queue )
{
   queue->Head = 0;
   queue->Tail = 0;
   queue->Empty = TRUE;
   queue->Full = FALSE;
}


/**
 * @brief Write data function
 * 
 * This function writes data into the queue
 * 
 * @param queue[in] Pointer to a Que_Queue struct type. This is the queue's control struct
 * @param data[in] Pointer to the variable that has the info to write into the queue
 * 
 * @retval False in case the data couldn't be written, otherwise True
*/
uint8_t Queue_writeData( Que_Queue *queue, void *data )
{
   uint8_t exit;

   
    memcpy( queue->Buffer + (queue->Head * queue->Size), data, queue->Size); // Put the data into the buffer


    // Info has been copied
    exit = TRUE;
    queue->Empty = FALSE;
    queue->Head++;


    if ( queue->Head == queue->Elements )
    {
        queue->Head = 0;
    }


    if ( queue->Head == queue->Tail )
    {
        queue->Full = TRUE;
    }


    return exit;
}


/**
 * @brief Read data function
 * 
 * This function reads data from the queue
 * 
 * @param queue[in] Pointer to a Que_Queue struct type. This is the queue's control struct
 * @param data[in] Pointer to the variable that has the info to read into the queue
 * 
 * @retval False in case the data couldn't be read, otherwise True
*/
uint8_t Queue_readData( Que_Queue *queue, void *data )
{
    uint8_t exit = FALSE;

    if (queue->Empty == FALSE)
    {

        memcpy( data, ( queue->Buffer + ( queue->Tail * queue->Size ) ), queue->Size );

        exit = TRUE;
        queue->Full = FALSE;                            // Queue is not full anymore
        queue->Tail++;
        

        if ( queue->Tail == queue->Elements)
        {
            queue->Tail = 0;                            // Go to the beginning
        }


        if ( queue->Tail == queue->Head )
        {
            queue->Empty = TRUE;                        // Queue is empty
        }

    }

    return exit;
}


/**
 * @brief QueueEmpty function
 * 
 * This function says if the queue is empty
 * 
 * @param queue[in] Pointer to a Que_Queue struct type. This is the queue's control struct
 * 
 * @retval True in case the queue is empty, otherwise False
*/
uint8_t Queue_isQueueEmpty( Que_Queue *queue )
{
    return queue->Empty;
}


/**
 * @brief FlushQueue function
 * 
 * This function restarts the queue
 * 
 * @param queue[in] Pointer to a Que_Queue struct type. This is the queue's control struct
 * 
 * @retval None
*/
void Queue_flushQueue( Que_Queue *queue )
{
    Queue_initQueue(queue);
}


