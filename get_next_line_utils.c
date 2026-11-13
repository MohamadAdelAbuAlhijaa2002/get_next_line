/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabualha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 13:00:43 by mabualha          #+#    #+#             */
/*   Updated: 2026/11/13 14:09:11 by mabualha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line_bonus.h"

static char	*gnl_grow(char *stash, size_t len, size_t cap)
{
	char	*new_stash;
	size_t	i;

	new_stash = malloc(cap);
	if (!new_stash)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_stash[i] = stash[i];
		i++;
	}
	free(stash);
	return (new_stash);
}

static int	gnl_append(char **stash, size_t *len, size_t *cap,
		char *buffer)
{
	char	*temp;
	size_t	i;
	size_t	add;

	add = gnl_strlen(buffer);
	if (*len + add + 1 > *cap)
	{
		if (*cap == 0)
			*cap = 64;
		while (*cap < *len + add + 1)
			*cap *= 2;
		temp = gnl_grow(*stash, *len, *cap);
		if (!temp)
			return (0);
		*stash = temp;
	}
	i = 0;
	while (i < add)
	{
		(*stash)[*len] = buffer[i];
		(*len)++;
		i++;
	}
	(*stash)[*len] = '\0';
	return (1);
}

static char	*gnl_cleanup(char *stash, char *buffer)
{
	free(buffer);
	free(stash);
	return (NULL);
}

static char	*gnl_read_loop(int fd, char *stash,
		size_t *len, size_t *cap)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (gnl_cleanup(stash, NULL));
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!gnl_append(&stash, len, cap, buffer))
			return (gnl_cleanup(stash, buffer));
		if (gnl_has_newline(buffer))
			break ;
	}
	free(buffer);
	if (bytes_read < 0)
		return (gnl_cleanup(stash, NULL));
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[2024];
	char		*line;
	size_t		len;
	size_t		cap;

	if (fd < 0 || fd >= 2024 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!gnl_has_newline(stash[fd]))
	{
		len = gnl_strlen(stash[fd]);
		cap = len + (stash[fd] != NULL);
		stash[fd] = gnl_read_loop(fd, stash[fd], &len, &cap);
	}
	if (!stash[fd])
		return (NULL);
	line = gnl_extract_line(stash[fd]);
	if (!line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	stash[fd] = gnl_update_stash(stash[fd]);
	return (line);
}
