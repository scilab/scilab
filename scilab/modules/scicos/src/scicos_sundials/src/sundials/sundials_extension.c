/*
* -----------------------------------------------------------------
* Allan CORNET - 2009
* For details, see sundials/shared/LICENSE.
* -----------------------------------------------------------------
*/

#include "sundials/sundials_extension.h"

BOOL bsundialsExtended = FALSE;

BOOL is_sundials_with_extension(void)
{
    return bsundialsExtended;
}

BOOL set_sundials_with_extension(BOOL _mode)
{
    bsundialsExtended = _mode;
    return bsundialsExtended;
}
