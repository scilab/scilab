/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "CallScilabBridge.hxx"
extern "C"
{
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "Scierror.h"
#include "FileExist.h"
#include "IsAScalar.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "gw_gui.h"
#include "getScilabJavaVM.h"
#include "getFullFilename.h"
#include "loadOnUseClassPath.h"
}
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;
/*--------------------------------------------------------------------------*/
int sci_toprint(char *fname,unsigned long l)
{
    int l1 = 0, m1 = 0, n1 = 0;

    int *paramoutINT = new int[1];
    *paramoutINT=(int)FALSE;

    CheckRhs(1,2);
    CheckLhs(0,1);

    if (!loadedDep)
    {
      loadOnUseClassPath("pdf_ps_eps_graphic_export");
      loadedDep=TRUE;
    }

    /* File or figure print */
    if (Rhs == 1)
    {
        /* File print */
        if (GetType(1) == sci_strings)
        {
            char **params = NULL;

            GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&params);
            if ((m1*n1) == 1)
            {
                char *fullName = NULL;
                fullName = getFullFilename(params[0]);
                if (fullName)
                {
                    if (FileExist(fullName))
                    {
                        *paramoutINT = (int)CallScilabBridge::printFile(getScilabJavaVM(), fullName); 
                    }
                    else 
                    {
                        sciprint(_("%s: The file %s does not exist.\n"), fname, fullName);
                        *paramoutINT = (int)(FALSE);
                    }
                    FREE(fullName);
                    fullName = NULL;
                }
                else
                {
                    *paramoutINT = (int)(FALSE);
                }
                freeArrayOfString(params, 1);
            }
            else
            {
                freeArrayOfString(params, m1 * n1);
                Scierror(999,_("%s: Wrong size for input argument #%d: String expected.\n"),fname,1);
                return FALSE;
            }
        }
        else
        {
            /* Figure print */
            if (GetType(1) == sci_matrix)
            {
                if (IsAScalar(1))
                {
                    int num_win=-2;
                    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
                    num_win=*istk(l1);
                    if (num_win>=0)
                    {
                      try
                      {
                          *paramoutINT = (int)CallScilabBridge::printFigure(getScilabJavaVM(), num_win, FALSE, FALSE);
                      }
                      catch (const GiwsException::JniException& e)
                      {
                          Scierror(999,_("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
                      }

                    }
                    else
                    {
                        Scierror(999,_("%s: Wrong value for input argument #%d: Non-negative integers expected.\n"),fname);
                        return FALSE;
                    }
                }
                else
                {
                    Scierror(999,_("%s: Wrong type for input argument #%d: Non-negative integer expected.\n"),fname,1);
                    return FALSE;
                }
            }
            else
            {
                delete[] paramoutINT;
                Scierror(999,_("%s: Wrong type for input argument #%d: Non-negative integer expected.\n"),fname,1);
                return FALSE;
            }
        }
    }
    else
    {
        /* Character string print */
        if (GetType(1) == sci_strings)
        {
            /** TODO rewrite this part using CPP instructions */

            char **Str=NULL;
            char *param2=NULL;
            char *lines=NULL;

            if (GetType(2) == sci_strings)
            {
                GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

                if (n1 == 1)
                {
                    int i=0;

                    lines=(char*)MALLOC(m1*2048*sizeof(char));
                    for (i = 0; i<m1 ;i++)
                    {
                        if (i==0)
                        {
                            sprintf(lines,"%s\n",Str[i]);
                        }
                        else 
                        {
                            sprintf(lines,"%s%s\n",lines,Str[i]);
                        }
                    }
                    freeArrayOfString(Str,m1);
                }
                else
                    if (m1 == 1)
                    {
                        int i=0;

                        lines=(char*)MALLOC(n1*2048*sizeof(char));

                        for (i = 0; i<n1 ;i++)
                        {
                            if (i==0)
                            {
                                sprintf(lines,"%s\n",Str[i]);
                            }
                            else sprintf(lines,"%s%s\n",lines,Str[i]);
                        }
                        freeArrayOfString(Str,m1);
                    }
                    else
                    {
                        freeArrayOfString(Str,m1);
                        Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
                        return 0;
                    }

                    GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
                    param2=cstk(l1);

                    *paramoutINT = (int)CallScilabBridge::printString(getScilabJavaVM(), lines, param2);
                    if (lines){FREE(lines);lines=NULL;}
            }
            else
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,2);
                return FALSE;
            }
        }
        else if (GetType(1) == sci_matrix)
        {
            if (GetType(2) == sci_strings)
            {
                if (IsAScalar(1))
                {
                    int num_win=-2;
                    char *param=NULL;

                    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
                    num_win=*istk(l1);

                    GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
                    param=cstk(l1);

                    if ( (strcmp(param,"pos")==0) || (strcmp(param,"gdi")==0) )
                    {
                      try
                      {
                        if ( strcmp(param,"pos")==0 )
                        {
                            *paramoutINT = (int)CallScilabBridge::printFigure(getScilabJavaVM(), num_win, TRUE, FALSE);
                        }
                        else
                        {
                            *paramoutINT = (int)CallScilabBridge::printFigure(getScilabJavaVM(), num_win, FALSE, FALSE);
                        }
                      }
                      catch (const GiwsException::JniException& e)
                      {
                          Scierror(999,_("%s: An exception occurred: %s\n%s\n"), fname, e.getJavaDescription().c_str(), e.getJavaExceptionName().c_str());
                      }

                    }
                    else
                    {
                        Scierror(999,_("%s: Wrong input argument #%d: '%s' or '%s' expected"),fname,2,"pos","gdi");
                        return 0;
                    }
                }
                else
                {
                    Scierror(999,_("%s: Wrong type for input argument #%d: Non-negative integers expected\n"),fname,1);
                    return 0;
                }

            }
            else
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
                return FALSE;
            }
        }
        else
        {
            delete[] paramoutINT;
            Scierror(999,_("%s: Wrong type for input argument #%d: A string or a real expected.\n"),fname,1);
            return FALSE;
        }
    }

    n1=1;
    CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1,&paramoutINT);
    LhsVar(1)=Rhs+1;
    delete[] paramoutINT;
    C2F(putlhsvar)();

    return 0;
}
/*--------------------------------------------------------------------------*/
