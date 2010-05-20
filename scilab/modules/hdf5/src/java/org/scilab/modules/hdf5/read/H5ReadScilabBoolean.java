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

import org.scilab.modules.types.scilabTypes.ScilabBoolean;

/**
 * H5ReadScilabBoolean
 */
public final class H5ReadScilabBoolean {

    /**
     * Private Constructor for utility class
     */
    private H5ReadScilabBoolean() { }
    
    /**
     * readData
     * @param dataSetId the dataset ID to read.
     * @param data A data structure that will be filled.
     * @throws HDF5Exception .
     */
    public static void readData(int dataSetId, ScilabBoolean data) throws HDF5Exception {
	data.setData(getBooleanMatrix(dataSetId));
    }

    /**
     * getBooleanMatrix
     * @param dataSetId the dataset ID to read.
     * @return a boolean matrix
     * @throws HDF5Exception .
     */
    private static boolean[][] getBooleanMatrix(int dataSetId) throws HDF5Exception {
	int[] nbElems = H5Read.getAllDims(dataSetId); 
	int[] data = new int[nbElems[0] * nbElems[1]];
	boolean[][] result = new boolean[(int) nbElems[0]][(int) nbElems[1]];
	
	H5.H5Dread_int(dataSetId, HDF5Constants.H5T_NATIVE_INT, H5.H5Dget_space(dataSetId), 
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
	
	for (int i = 0; i < nbElems[0]; ++i) {
	    for (int j = 0; j < nbElems[1]; ++j) {
		if (data[i + j * nbElems[0]] == 0) {
		result[i][j] = false;
		} else {
		    result[i][j] = true;
		}
	    }
	}
	
	H5.H5Dclose(dataSetId);
	return result;
    }
}
