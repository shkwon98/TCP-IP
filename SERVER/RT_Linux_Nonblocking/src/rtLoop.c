#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "sched_deadline.h"
#include "rtLoop.h"

double t_loopStart, t_lastLoopStart, t_taskEnd;
struct timespec time_line;

void myTask(void *buffer)
{
    if (!strcmp(buffer, "exit"))
        pthread_exit(NULL);
    printf("[Received] %s\n", buffer);
}

void *rt_loop(void *buffer)
{
    /* scheduler attributes */
    struct sched_attr attr;
    float period_ms = 1;

    printf("deadline rt_thread started [%ld]\n", gettid());

    /* set up sched_attr structure */
    attr.size = sizeof(attr);
    attr.sched_policy = SCHED_DEADLINE;
    attr.sched_flags = 0;
    attr.sched_nice = 0;
    attr.sched_priority = 0;
    attr.sched_runtime = 0.95 * period_ms * 1000 * 1000;
    attr.sched_period = attr.sched_deadline = period_ms * 1000 * 1000;

    /* set sched deadline */
    if (sched_setattr(gettid(), &attr, 0))
    {
        perror("sched_setattr");
        exit(1);
    }

    /* 1kHz REAL-TIME LOOP STARTS */
    while (1)
    {
        /********************** LOOP TIME MEASUREMENT **********************/
        clock_gettime(CLOCK_MONOTONIC, &time_line);
        t_loopStart = time_line.tv_nsec;
        printf("Loop time: %lf ms\n", (t_loopStart - t_lastLoopStart) / 1000000.0);
        t_lastLoopStart = t_loopStart;
        /*******************************************************************/

        /******* LOOP TASKS *******/
        myTask(buffer);
        /**************************/

        /********************** TASK TIME MEASUREMENT **********************/
        clock_gettime(CLOCK_MONOTONIC, &time_line);
        t_taskEnd = time_line.tv_nsec;
        printf("Task time: %lf ms\n", (t_taskEnd - t_loopStart) / 1000000.0);
        /*******************************************************************/

        sched_yield();
    }

    printf("deadline rt_thread dies [%ld]\n", gettid());
    return NULL;
}
