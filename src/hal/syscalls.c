/**
 * @file syscalls.c
 * @brief System call implementations for periodRTOS
 */

#include <errno.h>
#include <sys/stat.h>

/* External symbols from linker script */
extern char _end;           /* End of BSS */
extern char _estack;        /* End of stack */

/* Current heap pointer */
static char *heap_end = &_end;

/**
 * @brief System call for memory allocation
 */
void *_sbrk(ptrdiff_t incr)
{
    char *prev_heap_end = heap_end;
    
    /* Check if we have enough memory */
    if (heap_end + incr > &_estack) {
        errno = ENOMEM;
        return (void *)-1;
    }
    
    /* Update heap pointer */
    heap_end += incr;
    
    return (void *)prev_heap_end;
}

/**
 * @brief Write system call (stub)
 */
int _write(int file, char *ptr, int len)
{
    /* For now, just return success */
    /* In a real system, this would send data via UART */
    return len;
}

/**
 * @brief Read system call (stub)
 */
int _read(int file, char *ptr, int len)
{
    /* For now, just return 0 (EOF) */
    return 0;
}

/**
 * @brief Close system call (stub)
 */
int _close(int file)
{
    return -1;
}

/**
 * @brief Fstat system call (stub)
 */
int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/**
 * @brief Isatty system call (stub)
 */
int _isatty(int file)
{
    return 1;
}

/**
 * @brief Lseek system call (stub)
 */
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/**
 * @brief Kill system call (stub)
 */
int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

/**
 * @brief Getpid system call (stub)
 */
int _getpid(void)
{
    return 1;
}
