/* ========================================================================== */
/* bug 8587 */
/* Allan CORNET - 2010 - DIGITEO */
/* ========================================================================== */
#include <api_scilab.h>
#include <Scierror.h>
/* ========================================================================== */
int sci_bug_11106(char *fname, void* pvApiCtx)
{
    int* piAddr = NULL;
    char pstRet[64];

    getVarAddressFromPosition(pvApiCtx, 1, &piAddr);

    if (isStringType(pvApiCtx, piAddr))
    {
        //named check
        char* pstVar = NULL;
        getAllocatedSingleString(pvApiCtx, piAddr, &pstVar);

        if (isNamedListType(pvApiCtx, pstVar))
        {
            sprintf(pstRet, "%s", "isNamedList");
        }
        else if (isNamedTListType(pvApiCtx, pstVar))
        {
            sprintf(pstRet, "%s", "isNamedTList");
        }
        else if (isNamedMListType(pvApiCtx, pstVar))
        {
            sprintf(pstRet, "%s", "isNamedMList");
        }
        else
        {
            sprintf(pstRet, "%s", "unmanaged named type");
        }
        FREE(pstVar);
    }
    else
    {
        if (isListType(pvApiCtx, piAddr))
        {
            sprintf(pstRet, "%s", "isList");
        }
        else if (isTListType(pvApiCtx, piAddr))
        {
            sprintf(pstRet, "%s", "isTList");
        }
        else if (isMListType(pvApiCtx, piAddr))
        {
            sprintf(pstRet, "%s", "isMList");
        }
        else
        {
            sprintf(pstRet, "%s", "unmanaged type");
        }
    }
    createSingleString(pvApiCtx, Rhs + 1, pstRet);
    LhsVar(1) = Rhs + 1;
    return 0;
}
/* ========================================================================== */
