/*
** exec.c for  in /home/jeanj/Rendu/PSU_2015/PSU_2015_minishell2
**
** Made by Jean Jonathan
** Login   <jeanj@epitech.net>
**
** Started on  Tue Apr 12 15:15:13 2016 Jean Jonathan
** Last update Mon May 30 14:24:28 2016 tonell_m
*/

#include "sh.h"
#include "my.h"

int     check_ops2(char *str, t_tree *tree)
{
  if (my_strcmp(str, ">") == 0)
    tree->fd[1] = open(tree->right->str, O_RDWR | O_CREAT | O_TRUNC, 0666);
  else if (my_strcmp(str, ">>") == 0)
    tree->fd[1] = open(tree->right->str, O_RDWR | O_CREAT | O_APPEND, 0666);
  else if (my_strcmp(str, "<") == 0)
    tree->fd[0] = open(tree->right->str, O_RDWR, 0666);
  else
    return (1);
  if (tree->fd[1] < 0)
    {
      my_printf("Can't open %s\n", tree->right->str);
      tree->fd[1] = -1;
    }
  return (0);
}

int     check_ops(char *str, t_tree *tree)
{
  if (tree->parent != NULL)
    {
      if ((my_strcmp(tree->parent->str, ">") == 0
           || my_strcmp(tree->parent->str, ">>") == 0)
          && tree == tree->parent->right)
        return (0);
      if ((my_strcmp(tree->parent->str, "<") == 0)
          && tree == tree->parent->right)
        return (0);
    }
  if (my_strcmp(str, ";") == 0)
    return (0);
  else if (my_strcmp(str, "|") == 0)
    {
      pipe(tree->fd);
      tree->pipe = 1;
      return (0);
    }
  else
    return (check_ops2(str, tree));
}

void    dad(t_sh *sh, pid_t pid)
{
  int   status;

  if (sh->actual->parent != NULL)
    {
      if (sh->actual->parent->pipe == 1)
	{
	  if (sh->actual == sh->actual->parent->left)
	    close(sh->actual->parent->fd[1]);
	  else if (sh->actual == sh->actual->parent->right)
	    close(sh->actual->parent->fd[0]);
	}
      else
	{
	  if (sh->actual->parent->fd[0] != 0)
	    close(sh->actual->parent->fd[0]);
	  if (sh->actual->parent->fd[1] != 1)
	    close(sh->actual->parent->fd[1]);
	}
    }
  waitpid(pid, &status, 0);
  if (WTERMSIG(status) == SIGSEGV)
    write(2, "Segmentation Fault\n", 19);
  freetab(sh->av);
}

int    son(t_sh *sh, char *path, char **e)
{
  if (sh->actual->parent != NULL) {
    if (sh->actual->parent->pipe == 1)
      {
        if (sh->actual == sh->actual->parent->left)
          {
            close(sh->actual->parent->fd[0]);
            dup2(sh->actual->parent->fd[1], 1);
          }
        else if (sh->actual == sh->actual->parent->right) {
          close(sh->actual->parent->fd[1]);
          dup2(sh->actual->parent->fd[0], 0);
        }
      }
    else {
      dup2(sh->actual->parent->fd[0], 0);
      dup2(sh->actual->parent->fd[1], 1);
    }
  }
  e = envtotab(sh->env);
  if ((execve(path, sh->av, e)) == 0)
    my_delete_list(sh->env);
  else
    return (my_printf("%s: Can't execute\n", sh->av[0]));
  return (0);
}

int             my_exec(t_sh *sh, char *path)
{
  pid_t         pid;

  if (sh->actual->parent != NULL
      && (sh->actual->parent->fd[0] < 0 || sh->actual->parent->fd[1] < 0))
    return (0);
  pid = fork();
  if (pid == 0)
    son(sh, path, NULL);
  else if (pid > 0)
    dad(sh, pid);
  else
    my_printf("%s: Fork failed\n", sh->av[0]);
  return (0);
}
