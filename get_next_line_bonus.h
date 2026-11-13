#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*gnl_read_to_stash(int fd, char *stash);

size_t	gnl_strlen(const char *str);
int		gnl_has_newline(char *stash);
char	*gnl_strjoin(char *stash, char *buffer);
char	*gnl_extract_line(char *stash);
char	*gnl_update_stash(char *stash);

#endif
