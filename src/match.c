/*
** match.c for 42sh in /home/tonell-m/rendu/PSU/42sh
**
** Made by tonell_m
** Login   <tonell-m@epitech.net>
**
** Started on  Mon May 30 13:58:25 2016 tonell_m
** Last update Mon May 30 14:02:50 2016 tonell_m
*/

#include <stdlib.h>

char		*check_letter(char *str, char c)
{
  inti;
  intpos;

  i = 0;
  pos = 0;
  while (str[i] != '\0')
    {
      if (str[i] == c)
	{
	  pos = i;
	}
      i += 1;
    }
  return (&str[pos]);
}

char		match(char *s1, char *s2)
{
  if (s1 == NULL || s2 == NULL)
    return (0);
  if (s2[0] == '*')
    {
      while (s2[0] == '*')
	s2 = &s2[1];
      if (s2[0] == '\0') return (1);
      while (s1[0] != '\0' && s1[0] != s2[0])
	s1 = &s1[1];
      s1 = check_letter(s1, s2[0]);
      return (match(s1, s2) && s1[0] != '\0');
    }
  else if (s2[0] == '\0' || s1[0] == '\0')
    return (s2[0] == s1[0]);
  else if (s2[0] == s1[0] || s2[0] == '?')
    return (match(&s1[1], &s2[1]));
  return (0);
}