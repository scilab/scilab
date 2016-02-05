#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"

int sci_bug_11046(char *fname, void* pvApiCtx)
{
    CheckRhs(0, 0);
    sciprint("Overload call macro\n"); // %_myscifun
    OverLoad(0);
    return 0;
}
