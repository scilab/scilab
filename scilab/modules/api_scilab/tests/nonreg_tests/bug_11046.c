#include "api_scilab.h"
#if API_SCILAB_VERSION < 3
#include "stack-c.h"
#endif
#include "localization.h"
#include "Scierror.h"

int sci_bug_11046(char *fname)
{
    CheckRhs(0, 0);
    sciprint("Overload call macro\n"); // %_myscifun
    OverLoad(0);
    return 0;
}
