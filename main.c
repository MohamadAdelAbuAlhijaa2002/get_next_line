#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd = open("test1.txt", O_RDONLY);
    char *line;
    int i = 0;

    if (fd < 0)
        return (1);
    while (++i < 4)
    {
        line = get_next_line(fd);
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}