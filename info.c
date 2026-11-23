/*
 * Author: Logan Smouter
 * Date: 11/23/2026
 * Assignment: CPE-2600 Lab 9: System Calls - Part 1
 * Section: CPE-2600 131
 * Description: Display information about the system using system calls.
 * Compilation: $ make
 */

#include <inttypes.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    struct timespec current_time;
    struct utsname system_info;
    long page_size;
    long total_pages;
    long free_pages;

    // CLOCK_REALTIME gives the current wall-clock time since the Unix epoch. (00:00, January 1, 1970)
    if (clock_gettime(CLOCK_REALTIME, &current_time) == -1) {
        perror("clock_gettime");
        return 1;
    }

    // uname fills in the network name, operating system, and hardware type.
    if (uname(&system_info) == -1) {
        perror("uname");
        return 1;
    }

    // sysconf reports memory in pages, multiply by the page size for bytes.
    page_size = sysconf(_SC_PAGESIZE);
    total_pages = sysconf(_SC_PHYS_PAGES);
    free_pages = sysconf(_SC_AVPHYS_PAGES);
    if (page_size <= 0 || total_pages < 0 || free_pages < 0) {
        fprintf(stderr, "Unable to determine physical memory information.\n");
        return 1;
    }

    // Print seconds followed by nine nanosecond digits as one nanosecond value.
    printf("Current time (nanoseconds since Unix epoch): %jd%09ld\n",
           (intmax_t) current_time.tv_sec, current_time.tv_nsec);

    // Skip any strings that are empty.
    if (system_info.nodename[0] != '\0') {
        printf("Network name: %s\n", system_info.nodename);
    }
    if (system_info.sysname[0] != '\0') {
        printf("Operating system: %s\n", system_info.sysname);
    }
    if (system_info.release[0] != '\0') {
        printf("OS release: %s\n", system_info.release);
    }
    if (system_info.version[0] != '\0') {
        printf("OS version: %s\n", system_info.version);
    }
    if (system_info.machine[0] != '\0') {
        printf("Hardware type: %s\n", system_info.machine);
    }

    // get_nprocs reports the number of processors currently online. 
    printf("Number of CPUs: %d\n", get_nprocs());
    printf("Total physical memory (bytes): %ju\n",
           (uintmax_t) total_pages * (uintmax_t) page_size);
    printf("Free physical memory (bytes): %ju\n",
           (uintmax_t) free_pages * (uintmax_t) page_size);

    return 0;
}
