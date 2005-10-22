#include <string.h>
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

extern void cerro();

#include "../machine.h"

void C2F(concom)(icomp,n,ncomp,ns,nsize)
int *icomp, *n, *ncomp, *nsize;
int **ns;
{
  int i;
  *nsize = 0;
  if ((*ns = (int *)MALLOC(*n * sizeof(int))) == NULL) {
    cerro("Running out of memory");
    return;
  }
  for (i = 0; i < *n; i++){
    if (ncomp[i] == *icomp) {
      (*ns)[*nsize] = i + 1;
      (*nsize)++;
    }
  }
}

void C2F(sconcom)(icomp,n,nfcomp,ns,nsize)
int *icomp, *n, *nfcomp, *nsize;
int **ns;
{
  int i;
  *nsize = 0;
  if ((*ns = (int *)MALLOC(*n * sizeof(int))) == NULL) {
    cerro("Running out of memory");
    return;
  }
  for (i = 0; i < *n; i++){
    if (nfcomp[i] == *icomp) {
      (*ns)[*nsize] = i + 1;
      (*nsize)++;
    }
  }
}
