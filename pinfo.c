/*
 * Author: Logan Smouter
 * Date: 11/23/2026
 * Assignment: CPE-2600 Lab 9: System Calls - Part 2
 * Section: CPE-2600 131
 * Description: Display the priority and scheduling method of a process.
 * Compilation: $ make
 */

#define _GNU_SOURCE
#include <errno.h>
#include <limits.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int priority;
    int scheduler;
    char *end;
    long input_pid;
    const char *method;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [process ID]\n", argv[0]);
        return 1;
    }

    // If no process is specified, examine this program's process.
    pid = getpid();
    if (argc == 2) {
        errno = 0;
        input_pid = strtol(argv[1], &end, 10);
        if (errno != 0 || end == argv[1] || *end != '\0' ||
            input_pid <= 0 || input_pid > INT_MAX || (pid_t) input_pid != input_pid) {
            fprintf(stderr, "Invalid process ID: %s\n", argv[1]);
            return 1;
        }
        pid = (pid_t) input_pid;
    }

    // A priority of -1 can be valid, so errno distinguishes it from an error.
    errno = 0;
    priority = getpriority(PRIO_PROCESS, pid);
    if (errno != 0) {
        perror("getpriority");
        return 1;
    }

    // sched_getscheduler returns a numeric policy; translate it to text.
    scheduler = sched_getscheduler(pid);
    if (scheduler == -1) {
        perror("sched_getscheduler");
        return 1;
    }

    switch (scheduler) {
        case SCHED_OTHER: method = "SCHED_OTHER"; break;
        case SCHED_FIFO:  method = "SCHED_FIFO"; break;
        case SCHED_RR:    method = "SCHED_RR"; break;
        case SCHED_BATCH: method = "SCHED_BATCH"; break;
        case SCHED_IDLE:  method = "SCHED_IDLE"; break;
        default:          method = "Unknown scheduling method"; break;
    }

    printf("Process ID: %ld\n", (long) pid);
    printf("Process priority (nice value): %d\n", priority);
    printf("Scheduling method: %s\n", method);
    return 0;
}
