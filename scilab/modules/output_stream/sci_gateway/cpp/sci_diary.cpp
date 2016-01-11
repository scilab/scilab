/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "diary_manager.hxx"
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "gw_output_stream.h"
#include "api_scilab.h"
#include "localization.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
#define DIARY_SECOND_ARG_LIST "list"
#define DIARY_SECOND_ARG_CLOSE "close"
#define DIARY_SECOND_ARG_PAUSE "pause"
#define DIARY_SECOND_ARG_OFF "off"
#define DIARY_SECOND_ARG_RESUME "resume"
#define DIARY_SECOND_ARG_ON "on"
#define DIARY_SECOND_ARG_NEW "new"
#define DIARY_SECOND_ARG_APPEND "append"
#define DIARY_SECOND_ARG_EXISTS "exists"
#define DIARY_THIRD_ARG_FILTER_COMMAND "filter=command"
#define DIARY_THIRD_ARG_FILTER_OUTPUT "filter=output"
#define DIARY_THIRD_ARG_PREFIX_UNIX_EPOCH "prefix=U"
#define DIARY_THIRD_ARG_PREFIX_DEFAULT "prefix=YYYY-MM-DD hh:mm:ss"
#define DIARY_THIRD_ARG_PREFIX_ONLY_COMMANDS "prefix-only-commands"
/*--------------------------------------------------------------------------*/
static int sci_diary_no_rhs(char *fname, void* pvApiCtx);
static int sci_diary_one_rhs(char *fname, void* pvApiCtx);
static int sci_diary_two_rhs(char *fname, void* pvApiCtx);
static int sci_diary_three_rhs(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
static double *getInputArgumentOneIDs(char *fname, void* pvApiCtx, int *sizeReturnedArray, int *ierror);
static char** getInputArgumentOneFilenames(char *fname, void* pvApiCtx, int *sizeReturnedArray, int *ierror);
static char* getInputArgumentTwo(char *fname, void* pvApiCtx, int *ierror);
static char** getInputArgumentThree(char *fname, void* pvApiCtx, int *sizeReturnedArray, int *ierror);
static int checkExistByIDs(char *fname, void* pvApiCtx, double *IDs, int size_IDs);
static int checkExistByFilenames(char *fname, void* pvApiCtx, char** filenames, int size_IDs);
static int CloseByFilenames(char *fname, void* pvApiCtx);
static int CloseByIds(char *fname, void* pvApiCtx);
static int PauseByFilenames(char *fname, void* pvApiCtx);
static int PauseByIds(char *fname, void* pvApiCtx);
static int ResumeByFilenames(char *fname, void* pvApiCtx);
static int ResumeByIds(char *fname, void* pvApiCtx);
static int ExistByFilenames(char *fname, void* pvApiCtx);
static int ExistByIds(char *fname, void* pvApiCtx);
static int AppendByFilenames(char *fname, void* pvApiCtx,
                             diary_filter filterMode,
                             diary_prefix_time_format prefixMode,
                             diary_prefix_time_filter prefixModeFilter,
                             bool suspended);
static int NewByFilenames(char *fname, void* pvApiCtx,
                          diary_filter filterMode,
                          diary_prefix_time_format prefixMode,
                          diary_prefix_time_filter prefixModeFilter,
                          bool suspended);
/*--------------------------------------------------------------------------*/
int sci_diary(char *fname, void* pvApiCtx)
{
    CheckRhs(0, 3);
    CheckLhs(0, 2);

    switch (Rhs)
    {
        case 0:
            return sci_diary_no_rhs(fname, pvApiCtx);
        case 1:
            return sci_diary_one_rhs(fname, pvApiCtx);
        case 2:
            return sci_diary_two_rhs(fname, pvApiCtx);
        case 3:
            return sci_diary_three_rhs(fname, pvApiCtx);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_no_rhs(char *fname, void* pvApiCtx)
{
    // [ids, filenames] = diary()
    // [ids, filenames] = diary([],"list")
    SciErr sciErr;
    int nb_diary_ids = 0;
    double *diary_ids = getDiaryIDsAsDouble(&nb_diary_ids);

    if ((diary_ids) && (nb_diary_ids > 0))
    {
        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, nb_diary_ids, 1, diary_ids);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs + 1;

        delete[] diary_ids;
        diary_ids = NULL;
        nb_diary_ids = 0;
    }
    else
    {
        if (nb_diary_ids == 0)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            LhsVar(1) = Rhs + 1;

            if (diary_ids)
            {
                delete[] diary_ids;
                diary_ids = NULL;
                nb_diary_ids = 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }

    if (Lhs == 2)
    {
        int nb_diary_filenames = 0;
        char **diary_filenames = getDiaryFilenames(&nb_diary_filenames);

        if ((diary_filenames) && (nb_diary_filenames > 0))
        {
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, nb_diary_filenames, 1, diary_filenames);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(2) = Rhs + 2;

            freeArrayOfString(diary_filenames, nb_diary_filenames);
            nb_diary_filenames = 0;
        }
        else
        {
            if (nb_diary_filenames == 0)
            {
                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, 0, 0, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }

                LhsVar(2) = Rhs + 2;
                if (diary_filenames)
                {
                    freeArrayOfString(diary_filenames, nb_diary_filenames);
                    nb_diary_filenames = 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
    }

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_one_rhs(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iType = 0;
    int *piAddressVarOne = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType == sci_matrix)
    {
        int IDs_size = 0;
        int ierr = 0;
        double * IDs = getInputArgumentOneIDs(fname, pvApiCtx, &IDs_size, &ierr);
        if (ierr)
        {
            return 0;
        }

        // diary([])
        // diary(0)
        // close all
        if ((IDs_size == 0) ||
            ((IDs_size == 1) && ((int)IDs[0] == 0)))
        {
            diaryCloseAll();
            PutLhsVar();
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 0 expected.\n"), fname, 1);
        }
    }
    else if (iType == sci_strings)
    {
        int ierr = 0;
        int sizeFilenames = 0;
        char ** filenames = getInputArgumentOneFilenames(fname, pvApiCtx, &sizeFilenames, &ierr);
        if (ierr)
        {
            return 0;
        }

        if (sizeFilenames == 1)
        {
            if (diaryExists(filenames[0]))
            {
                double dID = (double)diaryNew(filenames[0], false);
                if (dID == -1)
                {
                    Scierror(999, _("%s: error can not create diary: %s.\n"), fname, filenames[0]);
                    return 0;
                }

                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &dID);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }

                LhsVar(1) = Rhs + 1;

                if (Lhs == 2)
                {
                    char* filenameUsed = getDiaryFilename((int)dID);
                    sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, 1, 1, &filenameUsed);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        return 0;
                    }

                    LhsVar(2) = Rhs + 2;
                    FREE(filenameUsed);
                }
            }
            else // diary(filename) exists (close diary)
            {
                if (diaryClose(filenames[0]))
                {
                    if (filenames)
                    {
                        freeArrayOfString(filenames, 1);
                        Scierror(999, _("%s: error can not close diary.\n"), fname);
                        return 0;
                    }
                }
                LhsVar(1) = 0;
            }

            freeArrayOfString(filenames, sizeFilenames);
            PutLhsVar();
        }
        else
        {
            freeArrayOfString(filenames, sizeFilenames);
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_two_rhs(char *fname, void* pvApiCtx)
{
    int ierr = 0;
    char *argumentTwo = getInputArgumentTwo(fname, pvApiCtx, &ierr);
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (ierr)
    {
        return 0;
    }

    if (argumentTwo)
    {
        if (strcmp(argumentTwo, DIARY_SECOND_ARG_LIST) == 0)
        {
            FREE(argumentTwo);
            int m1 = 0, n1 = 0;
            sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if ((m1 == n1) && (n1 == 0))
            {
                // diary() == diary([], "list")
                return sci_diary_no_rhs(fname, pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
            }
        }
        else if (strcmp(argumentTwo, DIARY_SECOND_ARG_CLOSE) == 0)
        {
            FREE(argumentTwo);
            int iType = 0;
            sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (iType == sci_matrix)
            {
                return CloseByIds(fname, pvApiCtx);
            }
            else if (iType == sci_strings)
            {
                return CloseByFilenames(fname, pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else if ((strcmp(argumentTwo, DIARY_SECOND_ARG_PAUSE) == 0) ||
            (strcmp(argumentTwo, DIARY_SECOND_ARG_OFF) == 0))
        {
            FREE(argumentTwo);
            int iType = 0;
            sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (iType == sci_matrix)
            {
                return PauseByIds(fname, pvApiCtx);
            }
            else if (iType == sci_strings)
            {
                return PauseByFilenames(fname, pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else if ((strcmp(argumentTwo, DIARY_SECOND_ARG_RESUME) == 0) ||
            (strcmp(argumentTwo, DIARY_SECOND_ARG_ON) == 0))
        {
            FREE(argumentTwo);
            int iType = 0;
            sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (iType == sci_matrix)
            {
                return ResumeByIds(fname, pvApiCtx);
            }
            else if (iType == sci_strings)
            {
                return ResumeByFilenames(fname, pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else if (strcmp(argumentTwo, DIARY_SECOND_ARG_NEW) == 0)
        {
            FREE(argumentTwo);
            int iType = 0;
            sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (iType == sci_strings)
            {
                return NewByFilenames(fname, pvApiCtx, DIARY_FILTER_INPUT_AND_OUTPUT,
                    PREFIX_TIME_FORMAT_UNIX_EPOCH,
                    PREFIX_FILTER_NONE, false);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else if (strcmp(argumentTwo, DIARY_SECOND_ARG_APPEND) == 0)
        {
            FREE(argumentTwo);
            int iType = 0;
            sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (iType == sci_strings)
            {
                return AppendByFilenames(fname, pvApiCtx, DIARY_FILTER_INPUT_AND_OUTPUT,
                    PREFIX_TIME_FORMAT_UNIX_EPOCH,
                    PREFIX_FILTER_NONE, false);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else if (strcmp(argumentTwo, DIARY_SECOND_ARG_EXISTS) == 0)
        {
            FREE(argumentTwo);
            int iType = 0;
            sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (iType == sci_matrix)
            {
                return ExistByIds(fname, pvApiCtx);
            }
            else if (iType == sci_strings)
            {
                return ExistByFilenames(fname, pvApiCtx);
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else
        {
            FREE(argumentTwo);
            Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_three_rhs(char *fname, void* pvApiCtx)
{
    int ierr = 0;
    int size_ArgThree = 0;
    char** argumentThree = getInputArgumentThree(fname, pvApiCtx, &size_ArgThree, &ierr);
    if (ierr)
    {
        return 0;
    }

    if (argumentThree)
    {
        diary_filter filterMode = DIARY_FILTER_INPUT_AND_OUTPUT;
        diary_prefix_time_format iPrefixMode = PREFIX_TIME_FORMAT_UNIX_EPOCH;
        diary_prefix_time_filter iPrefixIoModeFilter = PREFIX_FILTER_NONE;
        bool suspendedDiary = false;

        for (int i = 0; i < size_ArgThree; i++)
        {
            if (strcmp(argumentThree[i], DIARY_THIRD_ARG_FILTER_COMMAND) == 0)
            {
                filterMode = DIARY_FILTER_ONLY_INPUT; // input only
            }
            else if (strcmp(argumentThree[i], DIARY_THIRD_ARG_FILTER_OUTPUT) == 0)
            {
                filterMode = DIARY_FILTER_ONLY_OUTPUT; // output only
            }
            else if (strcmp(argumentThree[i], DIARY_THIRD_ARG_PREFIX_UNIX_EPOCH) == 0)
            {
                iPrefixMode = PREFIX_TIME_FORMAT_UNIX_EPOCH;
                if (iPrefixIoModeFilter == PREFIX_FILTER_NONE)
                {
                    iPrefixIoModeFilter = PREFIX_FILTER_INPUT_AND_OUTPUT;
                }
            }
            else if (strcmp(argumentThree[i], DIARY_THIRD_ARG_PREFIX_DEFAULT) == 0)
            {
                iPrefixMode = PREFIX_TIME_FORMAT_ISO_8601;
                if (iPrefixIoModeFilter == PREFIX_FILTER_NONE)
                {
                    iPrefixIoModeFilter = PREFIX_FILTER_INPUT_AND_OUTPUT;
                }
            }
            else if (strcmp(argumentThree[i], DIARY_THIRD_ARG_PREFIX_ONLY_COMMANDS) == 0)
            {
                iPrefixIoModeFilter = PREFIX_FILTER_ONLY_INPUT;
            }
            else if ((strcmp(argumentThree[i], DIARY_SECOND_ARG_ON) == 0) || (strcmp(argumentThree[i], DIARY_SECOND_ARG_RESUME) == 0))
            {
                suspendedDiary = false;
            }
            else if ((strcmp(argumentThree[i], DIARY_SECOND_ARG_OFF) == 0) || (strcmp(argumentThree[i], DIARY_SECOND_ARG_PAUSE) == 0))
            {
                suspendedDiary = true;
            }
            else
            {
                freeArrayOfString(argumentThree, size_ArgThree);
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 3);
                return 0;
            }
        }
        freeArrayOfString(argumentThree, size_ArgThree);

        char* argumentTwo = getInputArgumentTwo(fname, pvApiCtx, &ierr);
        if (ierr)
        {
            return 0;
        }
        if (argumentTwo)
        {
            int *piAddressVarOne = NULL;
            SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (strcmp(argumentTwo, DIARY_SECOND_ARG_NEW) == 0)
            {
                int iType = 0;
                sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                FREE(argumentTwo);
                argumentTwo = NULL;
                if (iType == sci_strings)
                {
                    return NewByFilenames(fname, pvApiCtx, filterMode, iPrefixMode, iPrefixIoModeFilter, suspendedDiary);
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                    return 0;
                }
            }
            else if (strcmp(argumentTwo, DIARY_SECOND_ARG_APPEND) == 0)
            {
                int iType = 0;
                sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                FREE(argumentTwo);
                argumentTwo = NULL;
                if (iType == sci_strings)
                {
                    return AppendByFilenames(fname, pvApiCtx, filterMode, iPrefixMode, iPrefixIoModeFilter, suspendedDiary);
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
                    return 0;
                }
            }
            else
            {
                FREE(argumentTwo);
                argumentTwo = NULL;
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
        }
    }
    else
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static double *getInputArgumentOneIDs(char *fname, void* pvApiCtx, int *sizeReturnedArray, int *ierror)
{
    double *IDs = NULL;
    *sizeReturnedArray = 0;
    *ierror = 0;

    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;

    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    int iType = 0;
    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType == sci_matrix)
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &IDs);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if ((m1 == 1) || (n1 == 1))
        {
            *sizeReturnedArray = m1 * n1;
        }
        else if ((m1 == 0) || (n1 == 0))
        {
            *sizeReturnedArray = 0;
            *ierror = 2;
            return NULL;
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), fname, 1);
            *ierror = 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), fname, 1);
        *ierror = 1;
    }

    return IDs;
}
/*--------------------------------------------------------------------------*/
static char** getInputArgumentOneFilenames(char *fname, void* pvApiCtx, int *sizeReturnedArray, int *ierror)
{
    char** filenames = NULL;
    *sizeReturnedArray = 0;
    *ierror = 0;

    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;

    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    int iType = 0;
    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType == sci_strings)
    {
        int *lenStVarOne = NULL;

        sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (m1 * n1 < 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
            *ierror = 1;
        }
        else if ((m1 == 1) || (n1 == 1))
        {
            lenStVarOne = (int *)MALLOC(sizeof(int) * (m1 * n1));
            if (lenStVarOne == NULL)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                *ierror = 1;
            }
            else
            {
                sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, filenames);
                if (sciErr.iErr)
                {
                    FREE(lenStVarOne);
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    *ierror = 1;
                    return NULL;
                }

                filenames = (char**)MALLOC(sizeof(char*) * (m1 * n1));
                if (filenames == NULL)
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    *ierror = 1;
                }
                else
                {
                    for (int i = 0; i < m1 * n1; i++)
                    {
                        filenames[i] = (char*)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
                        if (filenames[i] == NULL)
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            *ierror = 1;
                        }
                    }

                    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, filenames);
                    FREE(lenStVarOne);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                        *ierror = 1;
                        return NULL;
                    }

                    *sizeReturnedArray = m1 * n1;
                }
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), fname, 1);
            *ierror = 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        *ierror = 1;
    }
    return filenames;
}
/*--------------------------------------------------------------------------*/
static char* getInputArgumentTwo(char *fname, void* pvApiCtx, int *ierror)
{
    char* inputArgumentTwo = NULL;
    *ierror = 0;

    int m2 = 0, n2 = 0;
    int *piAddressVarTwo = NULL;

    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    int iType = 0;
    sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType == sci_strings)
    {
        int lenStVarTwo = 0;

        sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &m2, &n2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if ((m2 != n2) && (n2 != 1))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 2);
            *ierror = 1;
        }
        else
        {
            // get length lenStVarTwo
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &inputArgumentTwo);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

            inputArgumentTwo = (char*)MALLOC(sizeof(char) * (lenStVarTwo + 1));

            if (inputArgumentTwo == NULL)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                *ierror = 1;
            }

            sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &inputArgumentTwo);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
        *ierror = 1;
    }
    return inputArgumentTwo;
}
/*--------------------------------------------------------------------------*/
static char** getInputArgumentThree(char *fname, void* pvApiCtx, int *sizeReturnedArray, int *ierror)
{
    char** inputArgumentThree = NULL;
    *sizeReturnedArray = 0;
    *ierror = 0;

    int m3 = 0, n3 = 0;
    int *piAddressVarThree = NULL;

    SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    int iType = 0;
    sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (iType == sci_strings)
    {
        int *lenStVarThree = NULL;

        sciErr = getVarDimension(pvApiCtx, piAddressVarThree, &m3, &n3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (m3 * n3 < 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 3);
            *ierror = 1;
        }
        else if ((m3 == 1) || (n3 == 1))
        {
            lenStVarThree = (int *)MALLOC(sizeof(int) * (m3 * n3));
            if (lenStVarThree == NULL)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                *ierror = 1;
            }
            else
            {
                sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, lenStVarThree, inputArgumentThree);
                if (sciErr.iErr)
                {
                    FREE(lenStVarThree);
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    *ierror = 1;
                    return NULL;
                }

                inputArgumentThree = (char**)MALLOC(sizeof(char*) * (m3 * n3));
                if (inputArgumentThree == NULL)
                {
                    FREE(lenStVarThree);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    *ierror = 1;
                }
                else
                {
                    for (int i = 0; i < m3 * n3; i++)
                    {
                        inputArgumentThree[i] = (char*)MALLOC(sizeof(char) * (lenStVarThree[i] + 1));
                    }

                    sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, lenStVarThree, inputArgumentThree);
                    FREE(lenStVarThree);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                        return 0;
                    }

                    *sizeReturnedArray = m3 * n3;
                }
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), fname, 3);
            *ierror = 1;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
        *ierror = 1;
    }
    return inputArgumentThree;
}
/*--------------------------------------------------------------------------*/
static int checkExistByIDs(char *fname, void* pvApiCtx, double *IDs, int size_IDs)
{
    if (IDs)
    {
        for (int i = 0; i < size_IDs; i++)
        {
            if (diaryExists((int)IDs[i]))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: diary ID %d not exists.\n"), fname, 1, (int)IDs[i]);
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
static int checkExistByFilenames(char *fname, void* pvApiCtx, char** filenames, int size_IDs)
{
    if (filenames)
    {
        for (int i = 0; i < size_IDs; i++)
        {
            if (diaryExists(filenames[i]))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: diary filename not exists.\n"), fname, 1);
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
static int CloseByFilenames(char *fname, void* pvApiCtx)
{
    char **filenames = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    filenames = getInputArgumentOneFilenames(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr)
    {
        return 0;
    }

    ierr = checkExistByFilenames(fname, pvApiCtx, filenames, dIDs_size);
    if (ierr)
    {
        freeArrayOfString(filenames, dIDs_size);
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        if (diaryClose(filenames[i]))
        {
            freeArrayOfString(filenames, dIDs_size);
            Scierror(999, _("%s: Wrong value for input argument #%d: error can not close diary.\n"), fname);
            return 0;
        }
    }

    freeArrayOfString(filenames, dIDs_size);
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int CloseByIds(char *fname, void* pvApiCtx)
{
    double *dIDs = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    dIDs = getInputArgumentOneIDs(fname, pvApiCtx, &dIDs_size, &ierr);

    if (ierr == 2)
    {
        // diary([], 'close')
        diaryCloseAll();
        PutLhsVar();
        return 0;
    }
    else if (ierr)
    {
        return 0;
    }

    ierr = checkExistByIDs(fname, pvApiCtx, dIDs, dIDs_size);
    if (ierr)
    {
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        if (diaryClose((int)dIDs[i]))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: error can not close diary %d.\n"), fname, (int)dIDs[i]);
            return 0;
        }
    }
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int PauseByFilenames(char *fname, void* pvApiCtx)
{
    char** filenames = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    filenames = getInputArgumentOneFilenames(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr)
    {
        return 0;
    }

    ierr = checkExistByFilenames(fname, pvApiCtx, filenames, dIDs_size);
    if (ierr)
    {
        freeArrayOfString(filenames, dIDs_size);
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        if (diaryPause(filenames[i]))
        {
            freeArrayOfString(filenames, dIDs_size);
            Scierror(999, _("%s: Wrong value for input argument #%d: error can not pause diary.\n"), fname);
            return 0;
        }
    }

    freeArrayOfString(filenames, dIDs_size);

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int PauseByIds(char *fname, void* pvApiCtx)
{
    double *dIDs = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    dIDs = getInputArgumentOneIDs(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr == 2)
    {
        // diary([],"pause")
        diaryPauseAll();
        PutLhsVar();
        return 0;
    }
    else if (ierr)
    {
        return 0;
    }

    ierr = checkExistByIDs(fname, pvApiCtx, dIDs, dIDs_size);
    if (ierr)
    {
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        if (diaryPause((int)dIDs[i]))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: error can not pause diary %d.\n"), fname, (int)dIDs[i]);
            return 0;
        }
    }

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int ResumeByFilenames(char *fname, void* pvApiCtx)
{
    char** filenames = NULL;
    int dIDs_size = 0;
    int ierr = 0;
    filenames = getInputArgumentOneFilenames(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr)
    {
        return 0;
    }

    ierr = checkExistByFilenames(fname, pvApiCtx, filenames, dIDs_size);
    if (ierr)
    {
        freeArrayOfString(filenames, dIDs_size);
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        if (diaryResume(filenames[i]))
        {
            freeArrayOfString(filenames, dIDs_size);
            Scierror(999, _("%s: Wrong value for input argument #%d: error can not resume diary.\n"), fname);
            return 0;
        }
    }

    freeArrayOfString(filenames, dIDs_size);

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int ResumeByIds(char *fname, void* pvApiCtx)
{
    double *dIDs = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    dIDs = getInputArgumentOneIDs(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr == 2)
    {
        //diary([],"resume")
        diaryResumeAll();
        PutLhsVar();
        return 0;
    }
    else if (ierr)
    {
        return 0;
    }

    ierr = checkExistByIDs(fname, pvApiCtx, dIDs, dIDs_size);
    if (ierr)
    {
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        if (diaryResume((int)dIDs[i]))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: error can not resume diary %d.\n"), fname, (int)dIDs[i]);
            return 0;
        }
    }

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
static int ExistByFilenames(char *fname, void* pvApiCtx)
{
    char** filenames = NULL;
    int dIDs_size = 0;
    int ierr = 0;
    filenames = getInputArgumentOneFilenames(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr)
    {
        return 0;
    }

    int *resultExist = (int*)MALLOC(sizeof(int) * dIDs_size);
    if (resultExist == NULL)
    {
        freeArrayOfString(filenames, dIDs_size);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        int bExists = diaryExists(filenames[i]);
        if (bExists == 0)
        {
            resultExist[i] = TRUE;
        }
        else
        {
            resultExist[i] = FALSE;
        }
    }
    freeArrayOfString(filenames, dIDs_size);

    SciErr sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, dIDs_size, resultExist);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        FREE(resultExist);
        return 0;
    }

    FREE(resultExist);
    resultExist = NULL;
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int ExistByIds(char *fname, void* pvApiCtx)
{
    double *dIDs = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    dIDs = getInputArgumentOneIDs(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr)
    {
        return 0;
    }

    int *resultExist = (int*)MALLOC(sizeof(int) * dIDs_size);
    if (resultExist == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (int i = 0; i < dIDs_size; i++)
    {
        int bExists = diaryExists((int)dIDs[i]);
        if (bExists == 0)
        {
            resultExist[i] = TRUE;
        }
        else
        {
            resultExist[i] = FALSE;
        }
    }

    SciErr sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, dIDs_size, resultExist);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        FREE(resultExist);
        return 0;
    }

    FREE(resultExist);
    resultExist = NULL;
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
static int AppendByFilenames(char *fname, void* pvApiCtx,
    diary_filter filterMode,
    diary_prefix_time_format prefixMode,
    diary_prefix_time_filter prefixModeFilter,
    bool suspended)
{
    SciErr sciErr;
    char** filenames = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    filenames = getInputArgumentOneFilenames(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr)
    {
        return 0;
    }

    if (dIDs_size == 1)
    {
        double dID = (double)diaryAppend(filenames[0]);
        if (dID == -1)
        {
            Scierror(999, _("%s: error can not create diary: %s.\n"), fname, filenames[0]);
            freeArrayOfString(filenames, dIDs_size);
            return 0;
        }

        freeArrayOfString(filenames, dIDs_size);

        diarySetFilterMode((int)dID, filterMode);
        diarySetPrefixMode((int)dID, prefixMode);
        diarySetPrefixIoModeFilter((int)dID, prefixModeFilter);
        if (suspended)
        {
            diaryPause((int)dID);
        }

        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &dID);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs + 1;

        if (Lhs == 2)
        {
            char* filenameUsed = getDiaryFilename((int)dID);
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, 1, 1, &filenameUsed);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(2) = Rhs + 2;
            freeArrayOfString(&filenameUsed, 1);
        }
        PutLhsVar();
    }
    else
    {
        freeArrayOfString(filenames, dIDs_size);
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static int NewByFilenames(char *fname, void* pvApiCtx,
    diary_filter filterMode,
    diary_prefix_time_format prefixMode,
    diary_prefix_time_filter prefixModeFilter,
    bool suspended)
{
    char** filenames = NULL;
    int dIDs_size = 0;
    int ierr = 0;

    filenames = getInputArgumentOneFilenames(fname, pvApiCtx, &dIDs_size, &ierr);
    if (ierr)
    {
        return 0;
    }

    if (dIDs_size == 1)
    {
        double dID = (double)diaryNew(filenames[0], true);
        if (dID == -1)
        {
            Scierror(999, _("%s: error can not create diary: %s.\n"), fname, filenames[0]);
            freeArrayOfString(filenames, dIDs_size);
            return 0;
        }

        freeArrayOfString(filenames, dIDs_size);

        diarySetFilterMode((int)dID, filterMode);
        diarySetPrefixMode((int)dID, prefixMode);
        diarySetPrefixIoModeFilter((int)dID, prefixModeFilter);
        if (suspended)
        {
            diaryPause((int)dID);
        }

        SciErr sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &dID);
        LhsVar(1) = Rhs + 1;

        if (Lhs == 2)
        {
            char* filenameUsed = getDiaryFilename((int)dID);
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, 1, 1, &filenameUsed);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(2) = Rhs + 2;
            FREE(filenameUsed);
        }
        PutLhsVar();
    }
    else
    {
        freeArrayOfString(filenames, dIDs_size);
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
