/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 *  Copyright (C) 2011 - Scilab Enterprises - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include "SciDocMain.hxx"
#include "GiwsException.hxx"
#include "BuildDocObject.hxx"

extern "C"
{
    /*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "gw_helptools.h"
#include "Scierror.h"
#include "sci_path.h"
#include "localization.h"
#include "setgetlanguage.h"
#include "getScilabJavaVM.h"
#include "sci_malloc.h"
#ifdef _MSC_VER
#include "ConvertSlash.h"
#endif
    /*--------------------------------------------------------------------------*/
#define PATHTOCSS "/modules/helptools/css/javahelp.css"
#define PATHTOBUILDDOC "/modules/helptools/build/doc/scilab_%s_help/"
#define PATHTOMASTERXML "/modules/helptools/master_%s_help.xml"
#define DEFAULTEXPORT "JH"
#ifdef _MSC_VER
    static void __slashToAntislash(std::string * in)
    {
        size_t found = in->rfind("/");

        while (found != std::string::npos)
        {
            in->replace(found, 1, "\\");
            found = in->rfind("/");
        }
    }
#endif
    /*--------------------------------------------------------------------------*/
    int sci_buildDoc(char *fname, void* pvApiCtx)
    {
        std::string exportFormat;
        std::string SciPath; /* Scilab path */
        std::string masterXML; /* Which file contains all the doc stuff */
        std::string masterXMLTMP;
        std::string outputDirectory;    /* Working directory */
        std::string outputDirectoryTMP;
        std::string language;
        std::string styleSheet; /* the CSS */
        char * fileToExec = NULL;
        char * temp;
        SciErr sciErr;
        int *piAddr = NULL;
        int iRet = 0;

        CheckRhs(0, 4);
        CheckLhs(1, 1);

        temp = getSCI();
        SciPath = std::string(temp);
        free(temp);
        styleSheet = SciPath + PATHTOCSS;

        if (Rhs < 1)
        {
            exportFormat = DEFAULTEXPORT;
        }
        else
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if (!isStringType(pvApiCtx, piAddr))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
                return 0;
                // Wrong type string
            }

            iRet = getAllocatedSingleString(pvApiCtx, piAddr, &pstData);
            if (iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }
            exportFormat = std::string(pstData);
            freeAllocatedSingleString(pstData);


        }

        if (Rhs < 3)            /* Language not provided */
        {
            wchar_t* l = getlanguage();
            temp = wide_string_to_UTF8(l);
            language = std::string(temp);
            FREE(temp);
            free(l);
        }
        else
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

            if (!isStringType(pvApiCtx, piAddr))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
                return 0;
                // Wrong type string
            }

            if (!isScalar(pvApiCtx, piAddr))
            {
                wchar_t* pwstLang = getlanguage();
                temp = wide_string_to_UTF8(pwstLang);
                language = std::string(temp);
                FREE(temp);
                free(pwstLang);
            }
            else
            {
                iRet = getAllocatedSingleString(pvApiCtx, piAddr, &pstData);
                if (iRet)
                {
                    freeAllocatedSingleString(pstData);
                    return iRet;
                }
                language = std::string(pstData);
                freeAllocatedSingleString(pstData);

            }

        }

        if (Rhs < 2)
        {
            /* Update the path with the localization */
            masterXMLTMP = std::string("/modules/helptools/master_") + language + std::string("_help.xml");
            masterXML = SciPath + masterXMLTMP;
        }
        else
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }
            if (!isStringType(pvApiCtx, piAddr))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
                return 0;
                // Wrong type string
            }


            iRet = getAllocatedSingleString(pvApiCtx, piAddr, &pstData);
            if (iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }
            masterXML = std::string(pstData);
            freeAllocatedSingleString(pstData);
        }

        if (Rhs == 4)
        {
            char* pstData = NULL;
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
                return 0;
            }
            if (!isStringType(pvApiCtx, piAddr))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 4);
                return 0;
                // Wrong type string
            }

            iRet = getAllocatedSingleString(pvApiCtx, piAddr, &pstData);
            if (iRet)
            {
                freeAllocatedSingleString(pstData);
                return iRet;
            }
            outputDirectory = std::string(pstData) + std::string("/scilab_") + language + std::string("_help/");
            freeAllocatedSingleString(pstData);

        }
        else                    /* Scilab help */
        {
            /* Update the path with the localization */
            if (exportFormat != "jar-only")
            {
                outputDirectoryTMP =
                    std::string("/modules/helptools/") + std::string(exportFormat) + std::string("/scilab_") + language + std::string("_help/");
            }
            else
            {
                outputDirectoryTMP =
                    std::string("/modules/helptools/") + std::string("javaHelp") + std::string("/scilab_") + language + std::string("_help/");
            }

            outputDirectory = SciPath + outputDirectoryTMP;
        }

#ifdef _MSC_VER
        __slashToAntislash(&outputDirectory);
        __slashToAntislash(&styleSheet);
        __slashToAntislash(&masterXML);
#endif

        try
        {
            org_scilab_modules_helptools::SciDocMain * doc = new org_scilab_modules_helptools::SciDocMain(getScilabJavaVM());

            if (doc->setOutputDirectory((char *)outputDirectory.c_str()))
            {
                doc->setWorkingLanguage((char *)language.c_str());
                doc->setExportFormat((char *)exportFormat.c_str());
                doc->setIsToolbox(Rhs == 4);
                fileToExec = doc->process((char *)masterXML.c_str(), (char *)styleSheet.c_str());
            }
            else
            {
                Scierror(999, _("%s: Could find or create the working directory %s.\n"), fname, outputDirectory.c_str());
                return FALSE;
            }
            if (doc != NULL)
            {
                delete doc;
            }
        }
        catch (GiwsException::JniException ex)
        {
            Scierror(999, _("%s: Error while building documentation: %s.\n"), fname, ex.getJavaDescription().c_str());
            Scierror(999, _("%s: Execution Java stack: %s.\n"), fname, ex.getJavaStackTrace().c_str());
            Scierror(999,
                     _
                     ("If Scilab is started in a chroot, you might want to try to set the two environment variables: SCI_DISABLE_TK=1 SCI_JAVA_ENABLE_HEADLESS=1\n"));
            return FALSE;
        }

        if (fileToExec)
        {
            createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, &fileToExec);
            delete [] fileToExec;
        }
        else
        {
            createEmptyMatrix(pvApiCtx, Rhs + 1);
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    /*--------------------------------------------------------------------------*/
}
