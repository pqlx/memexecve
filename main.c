#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/stat.h>   /* for fstat */
#include <fcntl.h>      /* for open */
#include <unistd.h>     /* for read */

#include "memexecve.h"

#define printf_exit(format, ...) do { fprintf(stderr, format, ##__VA_ARGS__); exit(1);} while(0);

int main(int argc, char** argv, char** envp)
{
    int         fd;
    struct stat st;
    char*       filename;
    char*       loaded_target;
    
    if(argc < 2)
    {
        printf_exit("USAGE: ./memexecve /path/to/file.elf\n");
    }

    filename = argv[1];
    
    if( (fd = open(filename, O_RDONLY)) < 0)
        printf_exit("open \"%s\": %s\n", filename, strerror(errno) );

    if( fstat(fd, &st) < 0)
        printf_exit("fstat %d: %s\n", fd, strerror(errno));

    loaded_target = malloc(st.st_size);

    read(fd, loaded_target, st.st_size);
     
    /* Employ dirty hacks to ensure our target
     * does not dislike the first argv argument -
     * make it a full path */

    argv[1] = realpath(argv[1], NULL);


    /*
     * Execute from memory and make sure the first argv argument
     * is not included */
    exec_elf(loaded_target, st.st_size, &argv[1], envp); 
}


