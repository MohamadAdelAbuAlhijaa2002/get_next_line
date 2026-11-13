
#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd1;
	int		fd2;
	int		i;
	char	*line;

	fd1 = open("file1.txt", O_RDONLY);
	fd2 = open("file2.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
	{
		perror("open");
		if (fd1 >= 0)
			close(fd1);
		if (fd2 >= 0)
			close(fd2);
		return (1);
	}
	i = 0;
	while (i < 3)
	{
		line = get_next_line(fd1);
		if (line)
		{
			printf("File 1: %s", line);
			free(line);
		}
		else
			printf("File 1: NULL\n");
		line = get_next_line(fd2);
		if (line)
		{
			printf("File 2: %s", line);
			free(line);
		}
		else
			printf("File 2: NULL\n");
		i++;
	}
	close(fd1);
	close(fd2);
	return (0);
}
