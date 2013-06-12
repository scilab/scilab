/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __COMPLETIONDICTIONARY_H__
#define __COMPLETIONDICTIONARY_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * get all symbols dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : all symbols dictionary
    */
    char **getAllDictionary(int *sizeArrayReturned);

    /**
    * get functions dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : functions dictionary
    */
    char **getFunctionsDictionary(int *sizeArrayReturned);

    /**
    * get variables dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : variables dictionary
    */
    char **getVariablesDictionary(int *sizeArrayReturned, int bWithoutMacros);

    /**
    * get keywords (command keys) dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : keywords dictionary
    */
    char **getKeywordsDictionary(int *sizeArrayReturned);

    /**
    * get macros files dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : macros files dictionary
    */
    char **getMacrosFilesDictionary(int *sizeArrayReturned);

    /**
    * get macros variables dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : macros variables dictionary
    */
    char **getMacrosVariablesDictionary(int *sizeArrayReturned);

    /**
    * get all macros (macros variables and files) dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : all macros dictionary
    */
    char **getMacrosAllDictionary(int *sizeArrayReturned);

    /**
    * get graphic properties dictionary
    * @param[out] int *sizeArrayReturned: size of returned array
    * @return a string array : graphic properties dictionary
    */
    char **getGraphicPropertiesDictionary(int *sizeArrayReturned);

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif /* __COMPLETIONDICTIONARY_H__ */
