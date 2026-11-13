#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

// int main(void)
// {
//     int fd1 = open("test1.txt", O_RDONLY);
//     int fd2 = open("test2.txt", O_RDONLY);

//     char *line;
//     int i = 0;

//     if (fd1 < 0 || fd2 < 0)
//     {
//         close(fd1);
//         close(fd2);
//         return (1);
//     }
//     while (++i < 40)
//     {
//         line = get_next_line(fd1);
//         if (line != NULL)
//             printf("f1 : %s", line);
//         free(line);
//         line = get_next_line(fd2);
//         if (line != NULL)
//             printf("f2 : %s", line);
//         free(line);
//     }
//     close(fd1);
//     close(fd2);
//     return (0);
// }


int main(void)
{
    int fd = open("test1.txt", O_RDONLY);
    char *line;

    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}