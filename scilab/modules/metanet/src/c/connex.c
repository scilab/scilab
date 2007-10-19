#include <string.h>
#include <stdlib.h>

#include "MALLOC.h"

extern void cerro();

#include "machine.h"

void C2F(concom)(icomp,n,ncomp,ns,nsize)
int *icomp, *n, *ncomp, *nsize;
int **ns;
{
  int i;
  *nsize = 0;
  if ((*ns = (int *)MALLOC(*n * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
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
    cerro(_("Running out of memory"));
    return;
  }
  for (i = 0; i < *n; i++){
    if (nfcomp[i] == *icomp) {
      (*ns)[*nsize] = i + 1;
      (*nsize)++;
    }
  }
}
