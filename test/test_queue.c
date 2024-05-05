#include "unity.h"
#include "queue.h"

#define TRUE    1
#define FALSE   0

unsigned char arreglo[200];
Que_Queue queue;

void setUp(void)
{
    queue.Buffer = arreglo;
    queue.Elements = 200u;
    Queue_initQueue( &queue );
}

void tearDown(void)
{
}

/**
 * @brief   Test Queue_Init function
 *
 * The test verify that the queue is initialized correctly using a queue of uint8_t types
 */
void test__Queue_initQueue__char_queue( void )
{
    Que_Queue hqueue;
    uint8_t buffer[ 8 ];

    hqueue.Buffer   = buffer;
    hqueue.Elements = 8;
    hqueue.Size     = 1;
    Queue_initQueue( &hqueue );

    TEST_ASSERT_EQUAL_PTR( buffer, hqueue.Buffer );
    TEST_ASSERT_EQUAL( 8, hqueue.Elements );
    TEST_ASSERT_EQUAL( 1, hqueue.Size );
    TEST_ASSERT_EQUAL( 0, hqueue.Head );
    TEST_ASSERT_EQUAL( 0, hqueue.Tail );
    TEST_ASSERT_EQUAL( TRUE, hqueue.Empty );
    TEST_ASSERT_EQUAL( FALSE, hqueue.Full );
}


/**
 * @brief Test Queue_writeData Function
 * 
 * The test verify if all data is witten correctly and if the Head and he Tail of the buffers moves correctly
*/
void test__Queue__writeData()
{
    uint8_t dato = 0xFF;
    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = 1;

    uint8_t  res = Queue_writeData( &queue, &dato );

    TEST_ASSERT_EQUAL( 1, res );
    TEST_ASSERT_EQUAL( 1, queue.Head );
    TEST_ASSERT_EQUAL( dato, *((uint8_t *)queue.Buffer) );


    printf("Write data test succeed");
}


/**
 * @brief Test writeData function with a struct
 * 
 * This test checks if a structure can be written into the buffer
*/
void test__Queue__writeData_struct()
{
    typedef struct
    {
    uint32_t    bits32;
    uint8_t    bits8;
    } Ejem;

    Ejem structure1, structure2, structure3;

    structure1.bits8 = 8;
    structure1.bits32 = 32;
    structure2.bits8 = 16;
    structure2.bits32 = 64;
    structure3.bits8 = 32;
    structure3.bits32 = 128;
    

    queue.Elements = 2;

    Queue_initQueue( &queue );
    Ejem array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( structure1 );


    Queue_writeData( &queue, &structure1 );
    Queue_writeData( &queue, &structure2 );
    Queue_writeData( &queue, &structure3 );


    TEST_ASSERT_EQUAL( 1, queue.Head );
    TEST_ASSERT_EQUAL( structure2.bits8, array[1].bits8 );
    TEST_ASSERT_EQUAL( structure3.bits32, array[0].bits32 );



    printf("Write struct test succeed");
}


/**
 * @brief Test writeData function with a pointer
 * 
 * This test checks if a pointer can be written into the buffer
*/
void test__Queue_writeData_pointer()
{
    uint8_t *pointer = NULL;
    uint8_t data = 0xFF;
    pointer = &data;

    uint8_t *pointer2 = NULL;
    uint8_t data2 = 0X55;
    pointer2 = &data2;
    
    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( pointer );


    uint8_t  res = Queue_writeData( &queue, pointer );
    uint8_t  res2 = Queue_writeData( &queue, pointer2 );


    
    TEST_ASSERT_EQUAL( 2, queue.Head );
    TEST_ASSERT_EQUAL( *((uint8_t *)queue.Buffer), array[0] );
    TEST_ASSERT_EQUAL( *((uint8_t *)( queue.Buffer + 1)), array[1] );
    
    printf("Write pointer overflow succeed");
}



/**
 * @brief Test buffer's overflow
 * 
 * This test verifies whether the circularity of the buffer functions as intended
*/
void test__writeData_pointer_overflow()
{
    uint8_t *pointer = NULL;
    uint8_t data = 0xFF;
    pointer = &data;

    uint8_t *pointer2 = NULL;
    uint8_t data2 = 0X55;
    pointer2 = &data2;

    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( pointer );
    queue.Elements = 3u;

    for ( char i = 0; i < queue.Elements; i++ )
    {
        Queue_writeData( &queue, pointer );
        Queue_writeData( &queue, pointer2 );
    }

    TEST_ASSERT_EQUAL( 0, queue.Head );

}


/**
 * @brief Test Queue_readeData Function
 * 
 * The test verify if all data is read correctly and if the Head and he Tail of the buffers moves correctly
*/
void test__Queue__readData()
{
    uint8_t dato = 0xFF;
    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( dato );

    Queue_writeData( &queue, &dato );

    uint8_t  res = Queue_readData( &queue, &array );


    TEST_ASSERT_EQUAL( *array, *((uint8_t *)queue.Buffer) );
    TEST_ASSERT_EQUAL( array[0], *(uint8_t *)(queue.Buffer) );
    TEST_ASSERT_EQUAL(1, queue.Tail );

    printf("Read data test succeed");

}



/**
 * @brief Test readData function with a struct
 * 
 * This test checks if a structure can be read from the buffer
*/
void test__Queue__readData_struct()
{
    typedef struct
    {
        uint32_t    bits32;
        uint8_t    bits8;
    } Ejem;

    Ejem structure1, structure2, structure3;

    structure1.bits8 = 8;
    structure1.bits32 = 32;
    structure2.bits8 = 16;
    structure2.bits32 = 64;
    structure3.bits8 = 32;
    structure3.bits32 = 128;

    Queue_initQueue( &queue );

    Ejem array[queue.Elements];
    Ejem arrayR[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( structure1 );

    Queue_writeData( &queue, &structure1 );

    uint8_t  res = Queue_readData( &queue, &arrayR );


    TEST_ASSERT_EQUAL( structure1.bits8, arrayR[0].bits8 );
    TEST_ASSERT_EQUAL(1, queue.Tail );

    printf("Read struct test succeed");

}


/**
 * @brief Test readData function with a pointer
 * 
 * This test checks if a pointer can be read from the buffer
*/
void test__Queue_readData_pointer()
{
    char *pointer = NULL;
    char data = 0xFF;
    pointer = &data;
    
    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( pointer );


    Queue_writeData( &queue, pointer );

    uint8_t  res = Queue_readData( &queue, &array );

    TEST_ASSERT_EQUAL( 1, queue.Tail );
    TEST_ASSERT_EQUAL( array[0], *( ( uint8_t * ) queue.Buffer ) );
    
    printf("Read pointer test succeed");
}


/**
 * @brief Test isQueueEmpty function
 * 
 * This test checks if the Queue_isQueueEmpty gives a correct value when the buffer is empty
*/
void test__Queue__isQueueEmpty()
{
    uint8_t dato = 0xFF;
    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( dato );

    Queue_writeData( &queue, &dato );

    Queue_readData( &queue, &array );
    
    
    uint8_t res = Queue_isQueueEmpty( &queue );

    TEST_ASSERT_EQUAL( 1, res );

    printf("Empty queue test succeed");
}


/**
 * @brief Test flushQueue function
 * 
 * This test checks if flushQueue resets the buffer
*/
void test__Queue__flushQueue()
{
    uint8_t dato = 0xFF;
    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;

    Queue_writeData( &queue, &dato );

    Queue_readData( &queue, &array );
    
    
    Queue_flushQueue( &queue );

    TEST_ASSERT_EQUAL( queue.Tail, queue.Head );

    printf("Flush queue test succeed");
}


/**
 * @brief Read no data test
 * 
 * This test verify if buffer works as intended when is empty
*/
void test__Queue_readNoData()
{
    uint8_t dato = 0xFF;
    Queue_initQueue( &queue );
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( dato );


    uint8_t  res = Queue_readData( &queue, &array );


    TEST_ASSERT_EQUAL( FALSE, res );
    TEST_ASSERT_EQUAL( TRUE, queue.Empty );
    printf("Read No data test succeed");
}


/**
 * @brief Buffer is full test
 * 
 * This test verify if buffer works as intended when is full
*/
void test__Queue_WriteInFullBuffer()
{
    uint8_t dato = 0xFF;
    Queue_initQueue( &queue );
    queue.Elements = 1u;
    uint8_t array[queue.Elements];
    queue.Buffer = &array;
    queue.Size = sizeof( dato );
    
    Queue_writeData( &queue, &dato );


    TEST_ASSERT_EQUAL( TRUE, queue.Full );
    printf("Read No data test succeed");
}

