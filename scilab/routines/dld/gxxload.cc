#include <stdio.h>
#include <string.h>

#include <dld.h>

extern char* dyn_libraries[];

typedef void (*func_ptr)(void);

static print_undefined()
{
  char **symbols;
  int i;

  symbols = dld_list_undefined_sym();
  if (dld_undefined_sym_count != 0) printf("Undefined symbols:\n");
  for (i=0; i<dld_undefined_sym_count; i++) {
    printf("%s\n",symbols[i]);
    free(symbols[i]);
  }
  free(symbols);
}
 
void dyn_load(const char * name)
{
  char nm_command[80];
  char dummy[80];
  char symbol[80];
  func_ptr addr;
  FILE* pipe;

/*
 *  bad cards with C++ on linux
 *  so we must collect by ourselves
 */

  strcpy(nm_command,"nm -p ");
  strcat(nm_command,name);
  pipe = popen(nm_command,"r");
  while (!feof(pipe)) {
    fgets(dummy,80,pipe);
    strcpy(symbol,&dummy[12]); /* skip the leading underscore */
    if (strncmp(symbol,"_GLOBAL_$I",10) ) continue;
    symbol[strlen(symbol) - 1] = '\0';
    if (dld_create_reference(symbol)) /* load at least all constructors */
      dld_perror("reference");
    if (dld_link(name))  /* link it in, if needed */
    {
      dld_perror("link");
    }
    for (int i=0; dyn_libraries[i] != 0; i++)
      if (dld_link(dyn_libraries[i])) // link in needed libraries
	dld_perror(dyn_libraries[i]);
    print_undefined();
    addr = (func_ptr) dld_get_func(symbol);
    if ((addr != 0) && dld_function_executable_p(symbol)) (*addr)();
  }
}
 
void dyn_unload(const char * name)
{
  char nm_command[80];
  char dummy[80];
  char symbol[80];
  func_ptr addr;
  FILE* pipe;

/*
 *  bad cards with C++ on linux
 *  so we must collect by ourselves
 */

  dld_unlink_by_file(name,0); 
  strcpy(nm_command,"nm -p ");
  strcat(nm_command,name);
  pipe = popen(nm_command,"r");
  while (!feof(pipe)) {
    fgets(dummy,80,pipe);
    strcpy(symbol,&dummy[12]); /* skip the leading underscore */
    if (strncmp(symbol,"_GLOBAL_$D",10) ) continue;
    symbol[strlen(symbol) - 1] = '\0';
    addr = (func_ptr) dld_get_func(symbol);
    if (addr != 0) (*addr)();
  }
}
