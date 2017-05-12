#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>
#include <unistd.h>
#include <sys/wait.h>

#define FreeRTOS
#define MAX_STACK_SIZE 0x2000

caddr_t _sbrk(int incr)
{
        extern char end asm("end");
        static char *heap_end;
        char *prev_heap_end,*min_stack_ptr;

        if (heap_end == 0)
                heap_end = &end;

        prev_heap_end = heap_end;

#ifdef FreeRTOS
        /* Use the NVIC offset register to locate the main stack pointer. */
        min_stack_ptr = (char*)(*(unsigned int *)*(unsigned int *)0xE000ED08);
        /* Locate the STACK bottom address */
        min_stack_ptr -= MAX_STACK_SIZE;

        if (heap_end + incr > min_stack_ptr)
#else
        if (heap_end + incr > stack_ptr)
#endif
        {
//              write(1, "Heap and stack collision\n", 25);
//              abort();
                errno = ENOMEM;
                return (caddr_t) -1;
        }

        heap_end += incr;

        return (caddr_t) prev_heap_end;
}
