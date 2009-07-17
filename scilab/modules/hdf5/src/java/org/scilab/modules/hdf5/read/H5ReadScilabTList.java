/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

package org.scilab.modules.hdf5.read;

import java.util.ArrayList;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.scilabTypes.ScilabTList;

public class H5ReadScilabTList extends H5ReadScilabCommonList {

    public static void readData(int dataSetId, ScilabTList scilabList) throws NullPointerException, HDF5Exception {
	readData(dataSetId, (ArrayList) scilabList);
    }

}
