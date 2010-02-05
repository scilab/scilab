/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/


/**
 * This function forces the load of the JHDF library
 * On the first use of this function make sure we are calling the Java HDF5 
 * API. 
 * This a workaround for bug #5481
*/
void forceJHDF5load();
