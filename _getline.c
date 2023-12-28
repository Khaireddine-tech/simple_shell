#include "main.h"

/**
 * _getline_helper - Initializes or resizes a line buffer
 * @lineptr: Pointer to the buffer
 * @n: Pointer to the buffer size
 *
 * This function checks if the buffer is
 * uninitialized (NULL) or empty (size 0).
 * If so, it initializes or resizes the buffer to a default size of 128 bytes.
 * Return: 0 on success, -1 on memory allocation failure.
 */

int _getline_helper(char **lineptr, size_t *n)
{
	/* Check if buffer is uninitialized or size is 0 */
	if (*lineptr == NULL || *n == 0)
	{
		*n = 128;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}
	return (0);
}

/**
 * _getline - Reads a line from a file
 * @lineptr: Pointer to the buffer
 * @n: Pointer to the buffer size
 * @stream: File stream to read from
 *
 * Reads characters from a file stream until a newline is encountered.
 * The buffer is dynamically resized if necessary. The function handles
 * both the initialization of the buffer and its dynamic resizing.
 * Return: The number of characters read, including the newline character,
 * or -1 on failure or EOF without any bytes read.
 */

ssize_t _getline(char **lineptr, size_t *n, FILE *stream)
{
	ssize_t bytesRead = 0;
	size_t position = 0, newSize;
	static char buf[READ_BUF_SIZE];
	static size_t bufSize, bufPos;
	char *newBuffer;

	if (lineptr == NULL || n == NULL || stream == NULL
		|| _getline_helper(lineptr, n) == -1)
		return (-1);
	/* Main loop to read characters */
	while (1)
	{
		/* Validate input parameters and initialize buffer if needed */
		if (bufPos >= bufSize)
		{
			bytesRead = read(stream->_fileno, buf, READ_BUF_SIZE);
			if (bytesRead <= 0 && position == 0)
				return (-1);
			else if (bytesRead <= 0)
				break;
			bufSize = bytesRead;
			bufPos = 0;
		}
		if (position >= *n - 1)
		{
			newSize = *n * 2;
			newBuffer = realloc(*lineptr, newSize);
			if (newBuffer == NULL)
				return (-1);
			*lineptr = newBuffer;
			*n = newSize;
		}

		/* Copy character from static buffer to line buffer */
		(*lineptr)[position++] = buf[bufPos++];
		if ((*lineptr)[position - 1] == '\n')
			break;
	}
	(*lineptr)[position] = '\0';
	return (position);
}
