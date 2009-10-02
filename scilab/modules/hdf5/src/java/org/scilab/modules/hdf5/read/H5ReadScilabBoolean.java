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

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;

public class H5ReadScilabBoolean {

    public static void readData(int dataSetId, ScilabBoolean data) throws NullPointerException, HDF5Exception {
	data.setData(getBooleanMatrix(dataSetId));
    }

    private static boolean[][] getBooleanMatrix(int dataSetId) throws NullPointerException, HDF5Exception {
	long[] nbElems = H5Read.getAllDims(dataSetId); 
	int[][] data = new int[(int) nbElems[0]][(int) nbElems[1]];
	boolean[][] result = new boolean[(int) nbElems[0]][(int) nbElems[1]];
	
	H5.H5Dread(dataSetId, HDF5Constants.H5T_NATIVE_INT,
		H5.H5Dget_space(dataSetId), HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
	H5.H5Dclose(dataSetId);
	
	for(int i = 0 ; i < nbElems[0] ; ++i) {
	    for (int j = 0 ; j < nbElems[1] ; ++j) {
		result[i][j] = data[i][j] == 0 ? false : true;
	    }
	}
	
	return result;
    }
    
}
