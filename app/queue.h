#include <stdint.h>

#ifndef QUEUE_H_
#define QUEUE_H_


typedef struct
{
    void        *Buffer;  //pointer to array that store buffer data
    uint32_t    Elements; //number of elements to store (the queue lenght) 
    uint8_t    Size;     //size of the elements to store
    uint8_t    Head;     //variable to signal the next queue space to write 
    uint8_t    Tail;     //variable to signal the next queue space to read
    uint8_t    Empty;    //flag to indicate if the queue is empty
    uint8_t    Full;     //flag to indicate if the queue is full
    //agregar más elementos si se requieren
} Que_Queue;


void Queue_initQueue( Que_Queue *queue );
uint8_t Queue_writeData( Que_Queue *queue, void *data );
uint8_t Queue_readData( Que_Queue *queue, void *data );
uint8_t Queue_isQueueEmpty( Que_Queue *queue );
void Queue_flushQueue( Que_Queue *queue );


#endif