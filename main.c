
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;
	int		i;

	printf("Length of NULL = %zu\n", gnl_strlen(NULL));
	printf("Length of Hell = %zu\n", gnl_strlen("Hell"));
	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	i = 1;
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("Line %d: %s", i, line);
		free(line);
		i++;
		line = get_next_line(fd);
	}
	printf("\n[EOF reached]\n");
	close(fd);
	return (0);
}

