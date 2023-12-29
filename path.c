#include "main.h"

/**
 * _getenv - Retrieves the value of an environment variable.
 * @name: Name of the environment variable.
 * Return: Pointer to the value of the environment variable.
 *
 * Description: Scans the environment variables list for the specified name.
 * Returns the value associated with that name, or NULL if not found.
 */

char *_getenv(char *name)
{
	int i = -1;
	size_t name_len;

	if (name == NULL || *name == '\0')
		return (NULL);
	if (environ == NULL)
		return (NULL);

	name_len = _strlen(name);

	while (environ[++i])
	{
		if (!_strncmp(environ[i], name, name_len) && environ[i][name_len] == '=')
		{
			return (environ[i] + name_len + 1);
		}
	}
	return (NULL);
}

/**
 * _which - Locates the executable file associated with a given command.
 * @d: Data structure containing command information.
 * Return: 0 if found, -1 otherwise.
 *
 * Description: Searches the PATH environment variable for an executable.
 * file that matches the given command.
 * Updates the command in the data structure
 * if found.
 */

int _which(data *d)
{
	char *token, *path,
		*paths = malloc(_strlen(_getenv("PATH") ? _getenv("PATH") : "") + 1);
	size_t token_len;
	int find = -1;

	if (!_getenv("PATH"))
		goto step_out;
	_strcpy(paths, _getenv("PATH"));
	if (paths == NULL)
		goto step_out;
	token = strtok(paths, ":");
	while (token)
	{
		token_len = _strlen(token) + _strlen(d->av[0]) + 2;
		path = malloc(token_len);
		if (path == NULL)
			return (find);
		_strcpy(path, token);
		_strcat(path, "/");
		_strcat(path, d->av[0]);
		if (access(path, F_OK) == 0)
		{
			free(d->av[0]);
			d->av[0] = _strdup(path);
			free(path);
			find = 0;
			break;
		}
		free(path);
		token = strtok(NULL, ":");
	}
step_out:
	free(paths);
	return (find);
}

/**
 * create_new_entry - Initializes or modifies an environment variable.
 * @name: Name of the environment variable.
 * @value: Value to be assigned to the environment variable.
 * Return: Pointer to the new environment variable entry.
 *
 * Description: Creates a new environment variable entry in the format
 * "name=value".
 */

char *create_new_entry(char *name, char *value)
{
	size_t new_len = strlen(name) + strlen(value) + 2;
	char *new_entry = malloc(new_len);

	if (new_entry == NULL)
		return (NULL);

	strcpy(new_entry, name);
	strcat(new_entry, "=");
	strcat(new_entry, value);

	return (new_entry);
}

/**
 * _new_environ - Initializes or modifies an environment variable array.
 * @name: Name of the environment variable.
 * @value: Value to be assigned to the environment variable.
 * Return: Pointer to the new environment variable array.
 *
 * Description: Creates a new array of environment variables, either
 * adding a new variable or modifying an existing one.
 */

char **_new_environ(char *name, char *value)
{
	int env_len = 0, i = 0;
	char *new_entry;
	char **new_environ;

	while (environ[env_len])
		env_len++;
	new_entry = create_new_entry(name, value);
	if (new_entry == NULL)
		return (NULL);
	new_environ = _getenv(name) ? malloc((env_len + 1) * sizeof(char *))
								: malloc((env_len + 2) * sizeof(char *));

	if (!new_environ)
	{
		free(new_entry);
		return (NULL);
	}
	for (i = 0; environ[i]; i++)
	{
		new_environ[i] = malloc(strlen(environ[i]) + 1);
		if (!new_environ[i])
		{
			free_array(new_environ);
			free(new_entry);
			return (NULL);
		}
		if (strncmp(environ[i], name, strlen(name)) == 0
		&& environ[i][strlen(name)] == '=')
			strcpy(new_environ[i], new_entry);
		else
			strcpy(new_environ[i], environ[i]);
	}
	if (!_getenv(name))
	{
		new_environ[env_len] = new_entry;
		new_environ[env_len + 1] = NULL;
	}
	else
		new_environ[env_len] = NULL;
	return (new_environ);
}

/**
 * _setenv - Initializes or modifies an environment variable.
 * @d: Data structure to utilize the flag.
 * @name: Name of the environment variable.
 * @value: Value of the environment variable.
 * Return: 0 on success, -1 on failure.
 *
 * Description: Updates or adds an environment variable to the current
 * environment. Utilizes a flag within the data structure to indicate
 * successful setting of the environment variable.
 */
int _setenv(data *d, char *name, char *value)
{
	char **new_environ;

	if (!name || !value)
		return (-1);

	new_environ = _new_environ(name, value);
	if (!new_environ)
		return (-1);
	environ = new_environ;
	d->flag_setenv = 1;

	return (0);
}
