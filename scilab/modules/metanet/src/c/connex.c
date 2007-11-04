#include "connex.h"
#include "localization.h"

void C2F(concom)(int *icomp, int *n, int *ncomp, int *nsize, int **ns)
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
