#include "main.h"

/**
 * _printf - print a string to standard output
 * @str: string input
 * Return: void
 */
void _printf(const char *str)
{
	if (!str)
		return;

	size_t len = strlen(str);

	write(STDOUT_FILENO, str, len);
}

/**
 * free_array - free an array of pointers
 * @array: array of pointers
 * Return: void
 */
void free_array(char **array)
{
	if (!array)
		return;

	for (int i = 0; array[i]; i++)
	{
		free(array[i]);
		array[i] = NULL;
	}

	free(array);
}

/**
 * split - split a given string by a delimiter
 * @d: data struct input
 * @delim: string input
 * Return: void
 */
void split(data *d, const char *delim)
{
	char *token;
	int ntoken = 0;

	d->av = malloc(sizeof(char *));
	if (d->av == NULL)
	{
		perror(d->shell_name);
		exit(EXIT_FAILURE);
	}
	d->av[0] = NULL;

	token = strtok(d->cmd, delim);
	while (token)
	{
		d->av = realloc(d->av, (ntoken + 2) * sizeof(char *));
		if (d->av == NULL)
		{
			free_array(d->av);
			free(d->cmd);
			perror(d->shell_name);
			exit(EXIT_FAILURE);
		}
		d->av[ntoken] = _strdup(token);
		if (d->av[ntoken] == NULL)
		{
			free_array(d->av);
			free(d->cmd);
			perror(d->shell_name);
			exit(EXIT_FAILURE);
		}
		ntoken++;
		token = strtok(NULL, delim);
	}
	d->av[ntoken] = NULL;
}

/**
 * init_data - initialize data
 * @d: data struct input
 * @shell_name: string input
 * Return: void
 */

void init_data(data *d, const char *shell_name)
{
	d->cmd = NULL;
	d->av = NULL;
	d->shell_name = shell_name;
	d->last_exit_status = EXIT_SUCCESS;
	d->flag_setenv = 0;
}

/**
 * read_cmd - get the commend from the prompt and structure it into data struct
 * @d: data struct input
 * Return: void
 */
void read_cmd(data *d)
{
	size_t n = 0;
	ssize_t nread;

	nread = _getline(&d->cmd, &n, stdin);

	if (nread == -1)
	{
		free(d->cmd);
		exit(EXIT_SUCCESS);
	}

	d->cmd[nread - 1] = '\0';
	_trim(d->cmd);

	/* Replace hashtag with end of line */
	for (int i = 0; d->cmd[i] != '\0'; i++)
	{
		if (d->cmd[0] == '#' || (d->cmd[i] == '#' && d->cmd[i - 1] == ' '))
		{
			d->cmd[i] = '\0';
			break;
		}
	}
	_trim(d->cmd);
}
