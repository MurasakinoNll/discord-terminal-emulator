#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

char **splitstr(char *line) {
  char **args = malloc(sizeof(char *) * 64);
  if (!args)
    return NULL;

  char *tokens = strtok(line, " \t\n");
  int i = 0;

  while (tokens && i < 63) {
    args[i++] = tokens;
    tokens = strtok(NULL, " \t\n");
  }
  args[i] = NULL;
  return args;
}

int exec_builtin(char **args) {
  if (strcmp(args[0], "cd") == 0) {
    if (chdir(args[1]) != 0) {
      perror("chdir failed");
      return 1;
    }
    return 0;
  } else if (strcmp(args[0], "exit") == 0) {
    return -2;
  }
  return -1;
}

void dumbshell(void) {
  char *line = NULL;
  char **args;
  int status = -1;
  do {
    fflush(stdout);
    line = readline("jarak$ >");
    if (!line) {
      putchar('\n');
      break;
    }
    if (*line) {
      add_history(line);
    }

    args = splitstr(line);
    if (!args || !args[0]) {
      free(args);
      continue;
    }

    int retb = exec_builtin(args);
    if (retb == -2) {
      free(args);
      break;
    } else if (retb >= 0) {
      status = retb;
      free(args);
      continue;
    }

    pid_t pid = fork();
    if (pid == 0) {
      execvp(args[0], args);
      perror("execvp failed");
      exit(127);
    } else if (pid < 0) {
      perror("fork failed");
      status = -1;
    } else {
      int wstatus;
      waitpid(pid, &wstatus, 0);
      if (WIFEXITED(wstatus))
        continue;
      else
        status = -1;
    }
    free(args);
  } while (status == -1);
}
