#define _GNU_SOURCE /* for memfd */
#include <sys/mman.h>

#include <unistd.h> /* write */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memexecve.h"

void exec_elf(char* raw_elf, size_t elf_size, char** argv, char** envp)
{
    int  fd;
    char procfs_filename[0x100] = {};
    /*
     * Create an anonymous file which we can refer
     * to via `fd` */

    fd = memfd_create("ELF_HANDLE", MFD_CLOEXEC);

    /*
     * Write our elf data to it - subsequent reads by execve 
     * will return our written data. */

    write(fd, raw_elf, elf_size);
    
    /* loop back, execve will fail otherwise */
    lseek(fd, SEEK_SET, 0);

    sprintf(procfs_filename, "/proc/self/fd/%u", fd);
    
    execve(procfs_filename, argv, envp);
    exit(0);
    

    /* Make sure we're never ever returning */
    __builtin_unreachable();
}


