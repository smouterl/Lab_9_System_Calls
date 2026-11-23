/*
 * Author: Logan Smouter
 * Date: 11/23/2026
 * Assignment: CPE-2600 Lab 9: System Calls - Part 2
 * Section: CPE-2600 131
 * Description: Lower the process priority, sleep, and print a message.
 * Compilation: $ make
 */

#include <errno.h>
#include <stdio.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    struct timespec delay = {1, 837272638};
    struct timespec remaining;
    int priority;

    printf("Process ID: %ld\n", (long) getpid());

    // getpriority can also return -1 as a valid nice value.
    errno = 0;
    priority = getpriority(PRIO_PROCESS, 0);
    if (errno != 0) {
        perror("getpriority");
        return 1;
    }
    printf("Starting nice value: %d\n", priority);

    // A higher nice value means a lower scheduling priority.
    // nice can return -1 on success, so check errno to detect an error.
    errno = 0;
    priority = nice(10);
    if (priority == -1 && errno != 0) {
        perror("nice");
        return 1;
    }
    printf("Requested a nice value increase of 10 to lower process priority.\n");
    printf("Current nice value: %d\n", priority);

    // 1,837,272,638 ns is 1 second plus 837,272,638 ns.
    // If a signal interrupts the sleep, continue for the remaining time.
    printf("Sleeping for 1,837,272,638 nanoseconds (about 1.84 seconds).\n");
    fflush(stdout);
    while (nanosleep(&delay, &remaining) == -1) {
        if (errno != EINTR) {
            perror("nanosleep");
            return 1;
        }
        delay = remaining;
    }

    printf("Goodbye!\n");
    return 0;
}
