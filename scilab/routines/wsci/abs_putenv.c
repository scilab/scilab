
#include <windows.h>

void 
abs_putenv (char *buf)
{
  char *str1, *str2;

  str1 = strtok (buf, "=");
  str2 = strtok ((char *) 0, "\0");

  SetEnvironmentVariable (str1, str2);
}
