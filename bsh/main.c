#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
  Function Declarations for builtin shell commands:
 */
int blackout_cd(char **args);
int blackout_help(char **args);
int blackout_exit(char **args);
int blackout_echo(char **args);
int blackout_lsdir(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "echo",
  "lsdir"
};

int (*builtin_func[]) (char **) = {
  &blackout_cd,
  &blackout_help,
  &blackout_exit,
  &blackout_echo,
  &blackout_lsdir
};

int blackout_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
    builtin command launch
**/

int blackout_echo(char **args)
{
	int length = 1;
	while (args[length] != NULL)
	{
		if (strcmp(args[length], "\n")== 0)
		{
			printf("\n");
		}
		else
		{
			printf("%s ", args[length]);
		}
		length++;
	}
	printf("\n");
	return 1;
}


/**
   builtin command echo
**/

int blackout_lsdir(char **args)
{
	if (args[1] != NULL)
	{
		printf("bsh: expected zero arguments\n");
	}
	else
	{
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			DIR *d;
			struct dirent *dir;
			d = opendir("./");
			if (d != NULL)
			{
				while (dir = readdir(d))
				{
					printf("%s\n", dir->d_name);
				}
			}
			else
			{
				printf("Couldn' t open the directory (are you root?)\n");
				return 1;
			}
		}
		else
		{
			printf("bsh:error while entering directory\n");
		}
	}
	return 1;
}


/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int blackout_cd(char **args)
{
  if (args[1] == NULL) {
    printf("bsh: expected argument to 'cd'\n");
  } else {
    if (chdir(args[1]) != 0) {
      printf("bsh : error while changing directory\n");
    }
  }
  return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int blackout_help(char **args)
{
  int i;
  printf("bsh - blackout shell\n");
  printf("by leonardo meitz\n");
  printf("development beta 1\n");
  printf("builtin commands\n");

  for (i = 0; i < blackout_num_builtins(); i++) {
    printf("%s\n", builtin_str[i]);
  }

  printf("Use the help command for information on other programs.\n");
  return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int blackout_exit(char **args)
{
  return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < blackout_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  printf("bsh: command not found '%s'\n", args[0]);
}

#define RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *read_line(void)
{
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    printf("bsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        printf("bsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **split_line(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    printf("bsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        printf("lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void shell_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
  	char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
		}
		else
		{
			perror("bsh:error while entering directory");
		}
    printf("%s $ ", cwd);
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  shell_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}