*This project has been created as part of the 42 curriculum by mabualha.*

# Get Next Line

## Description

**Get Next Line** is a C project that implements the following function:

```c
char *get_next_line(int fd);
```

Each call returns the next line from a file descriptor. A returned line includes the terminating newline character (`\n`) when one exists; the final line is still returned if the file ends without a newline. The function returns `NULL` when no more data is available or an error occurs.

The project develops an understanding of file descriptors, `read()`, static variables, dynamic memory allocation, string manipulation, and error handling. It works with regular text files and standard input.

The **bonus** extends the implementation to handle multiple file descriptors in an interleaved sequence while using only one static variable.

## Project Files

### Mandatory

| File | Purpose |
| --- | --- |
| `get_next_line.c` | Main function and incremental reading logic |
| `get_next_line_utils.c` | Helpers for checking newlines, joining strings, extracting lines, and keeping unread data |
| `get_next_line.h` | Function declarations, required includes, and the default `BUFFER_SIZE` |

### Bonus

| File | Purpose |
| --- | --- |
| `get_next_line_bonus.c` | Multi-file-descriptor implementation and dynamically growing read buffer |
| `get_next_line_utils_bonus.c` | Helper functions used by the bonus implementation |
| `get_next_line_bonus.h` | Bonus declarations and configuration |

`README.md` is included at the root of the repository. Temporary test programs such as `main.c` and `main_bonus.c` are for local development, not mandatory submission files.

## Instructions

### Compilation

The project is a function, not a standalone executable. To run it, compile it together with your own test `main.c`.

**Mandatory:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
  get_next_line.c get_next_line_utils.c main.c -o gnl_test
./gnl_test
```

**Bonus:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
  get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o bonus_test
./bonus_test
```

The code can also be compiled without `-D BUFFER_SIZE=...`, in which case the header supplies the default value. Evaluators may use different positive buffer sizes.

### Example Usage

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd = open("example.txt", O_RDONLY);
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
```

The caller must `free()` each non-`NULL` returned line. The example is illustrative test code and is not intended to demonstrate 42 Norm formatting.

## Algorithm and Design Choices

### Mandatory: retain unread data between calls

1. A **static `stash` pointer** stores bytes that were read but have not yet been returned to the caller.
2. `gnl_read_to_stash()` reads up to `BUFFER_SIZE` bytes at a time with `read()`. Each successful read is null-terminated and appended to `stash`.
3. Reading stops when a newline is present or when `read()` reaches end-of-file. The implementation does not intentionally read the entire file before returning its first line.
4. `gnl_extract_line()` allocates and copies the first line, including `\n` if present.
5. `gnl_update_stash()` allocates a copy of any bytes after that line and frees the previous `stash` allocation.
6. `get_next_line()` returns the extracted line. The caller owns this allocation and frees it after use.

This design separates **reading**, **line extraction**, and **retention of remaining bytes**, making each step easier to reason about and debug. A static variable is necessary because the unread bytes must survive after the function returns.

### Bonus: one static array, independent reading states

The bonus uses one static variable:

```c
static char *stash[2024];
```

Each `stash[fd]` holds the unread data for one descriptor, allowing calls such as `get_next_line(fd1)`, `get_next_line(fd2)`, and then `get_next_line(fd1)` without mixing their contents. The array size is an **implementation choice**, not a requirement of the subject; this version explicitly rejects `fd` values outside `0..2023`.

The bonus also improves how accumulated text is stored:

- `len` tracks the number of bytes currently used in a stash.
- `cap` tracks its available allocation size.
- `gnl_append()` appends each newly read chunk. If the existing capacity is insufficient, it requests additional space.
- `gnl_grow()` allocates a larger block, copies the existing bytes, and releases the old block.
- Capacity typically starts at 64 bytes and doubles as needed, reducing repeated copying for long lines compared with allocating and joining the entire accumulated string after every read.

After a line is returned, the remaining unread text is retained for the next call on that particular file descriptor. The implementation maintains only one static variable while still tracking multiple files.

## Testing

During development, the mandatory implementation passed **73/73 local functional test cases**, and the same set passed with AddressSanitizer enabled. These are local tests, **not an official 42 evaluation result**.

Cases exercised included empty files, a single newline, multiple lines, blank lines, final lines without `\n`, long lines, line boundaries, standard input, invalid/closed descriptors, and various `BUFFER_SIZE` values (`1`, `4`, `42`, `9999`, and `10000000`, plus the default).

Recommended additional checks before submission:

```bash
norminette get_next_line.c get_next_line_utils.c get_next_line.h
norminette get_next_line_bonus.c get_next_line_utils_bonus.c get_next_line_bonus.h
```

Use AddressSanitizer or Valgrind to look for memory errors and leaks, and test interleaved file-descriptor reads for the bonus.

## Constraints and Limitations

- Allowed external functions for the project implementation: `read`, `malloc`, and `free`.
- No use of `libft`, `lseek()`, or global variables.
- Binary-file behavior and changes to a file between calls are not guaranteed by the subject.
- The bonus array has a fixed index range (`fd` values from `0` through `2023`).

## Resources

- 42 **Get Next Line** subject (project requirements and evaluation constraints).
- `man 2 read` — reading bytes from file descriptors and understanding EOF/error return values.
- `man 3 malloc` and `man 3 free` — dynamic allocation and memory management.
- `man 2 open` and `man 2 close` — opening and closing descriptors in local test programs.
- GCC documentation for `-Wall`, `-Wextra`, `-Werror`, and AddressSanitizer.

### AI Usage Disclosure

ChatGPT was used during learning and development to explain static variables, pointers, double pointers, file descriptors, `BUFFER_SIZE`, and memory ownership; to discuss and suggest helper-function implementations and bonus buffer-growth logic; to interpret AddressSanitizer diagnostics; to prepare local test cases; and to draft this README. The project code was compiled, tested, debugged, and reviewed during development. AI assistance is a learning aid and does not replace understanding or peer evaluation.
