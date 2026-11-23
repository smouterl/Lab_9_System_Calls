/*
 * Author: Logan Smouter
 * Date: 11/23/2026
 * Assignment: CPE-2600 Lab 9: System Calls - Part 3
 * Section: CPE-2600 131
 * Description: Display information about a file using a system call.
 * Compilation: $ make
 */

#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct stat file_info;
    struct tm *modified_time;
    char time_text[80];
    char permissions[10];
    const char *file_type;
    mode_t mode;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file name>\n", argv[0]);
        return 1;
    }

    // lstat describes the named file itself, including a symlink.
    if (lstat(argv[1], &file_info) == -1) {
        perror("lstat");
        return 1;
    }
    mode = file_info.st_mode;

    // The file type and permission bits are both stored in st_mode.
    if (S_ISREG(mode)) {
        file_type = "Regular file";
    } else if (S_ISDIR(mode)) {
        file_type = "Directory";
    } else if (S_ISLNK(mode)) {
        file_type = "Symbolic link";
    } else if (S_ISCHR(mode)) {
        file_type = "Character device";
    } else if (S_ISBLK(mode)) {
        file_type = "Block device";
    } else if (S_ISFIFO(mode)) {
        file_type = "FIFO (named pipe)";
    } else if (S_ISSOCK(mode)) {
        file_type = "Socket";
    } else {
        file_type = "Unknown";
    }

    // Show read, write, and execute permissions for owner, group, and others.
    permissions[0] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[9] = '\0';

    // Match ls command format for the set-user-ID, set-group-ID, and sticky bits.
    if (mode & S_ISUID) permissions[2] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID) permissions[5] = (mode & S_IXGRP) ? 's' : 'S';
    if (mode & S_ISVTX) permissions[8] = (mode & S_IXOTH) ? 't' : 'T';

    // Convert the modification time to readable format.
    modified_time = localtime(&file_info.st_mtime);
    if (modified_time == NULL ||
        strftime(time_text, sizeof(time_text), "%Y-%m-%d %H:%M:%S", modified_time) == 0) {
        fprintf(stderr, "Unable to format the modification time.\n");
        return 1;
    }

    printf("File: %s\n", argv[1]);
    printf("File type: %s\n", file_type);
    printf("Permissions: %s\n", permissions);
    printf("Owner user ID: %ju\n", (uintmax_t) file_info.st_uid);
    printf("Size (bytes): %jd\n", (intmax_t) file_info.st_size);
    printf("Last modified: %s\n", time_text);
    return 0;
}
