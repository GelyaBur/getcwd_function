#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/dir.h>
#include <ctype.h>
#include <limits.h>

#define _BSD_SOURCE
#define MAX_ARR 20

char *getcwd_impl(char *cwdbuf, size_t size)
{
    int fd;
    struct stat sb;
    char *arr[MAX_ARR];
    static int i = 0;
    fd = open(".", O_RDONLY);
    for (;;) {
        if (stat(".", &sb) == -1)
            errExit("stat");
        ino_t x;
        dev_t dv1, dv2;
        DIR *dirp;
        struct dirent *dp;
        x = sb.st_ino;
        dv1 = major(sb.st_dev);
        dv2 = minor(sb.st_dev);
        if (chdir("..") == -1)
            errExit("chdir");
        dirp = opendir(".");
        if (dirp == NULL) {
            errMsg("opendir failed");
        }
        for (;;) {
            errno = 0;
            struct stat sb2;
            char name[PATH_MAX];
            dp = readdir(dirp);
            sprintf(name, "./%s", dp->d_name);
            if ((dp == NULL) && (errno != 0))
                errExit("readdir");
            if (stat(name, &sb2) == -1)
                errExit("stat");
            if (strcmp(dp->d_name, ".") == 0 && sb2.st_ino == x && major(sb2.st_dev) == dv1 && minor(sb2.st_dev) == dv2) {
                for (int j = i-1; j >= 0; j--) {
                    strcat(strcat(cwdbuf, arr[j]), "/");
                }
                int j = 0;
                for (; cwdbuf[j] != '\0'; j++);
                cwdbuf[j-1] = '\0';
                return cwdbuf;
            }
            if (sb2.st_ino == x && major(sb2.st_dev) == dv1 && minor(sb2.st_dev) == dv2) {
                arr[i++] = dp->d_name;//strcat("/", dp->d_name);
                break;
            }
        }
    }
    fchdir(fd);
    close(fd);
}

int main()
{
    char cwdbuf[PATH_MAX] = "/";
    printf("%s\n", getcwd_impl(cwdbuf, PATH_MAX));
}
