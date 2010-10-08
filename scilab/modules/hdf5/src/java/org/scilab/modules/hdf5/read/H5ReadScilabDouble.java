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

import org.scilab.modules.types.ScilabDouble;

public class H5ReadScilabDouble {
    public static void readData(int dataSetId, ScilabDouble data) throws NullPointerException, HDF5Exception {
	/* Empty Matrix */
	if (H5Read.isEmpty(dataSetId)) {
	    data.setRealPart(null);
	    data.setImaginaryPart(null);
	    H5.H5Dclose(dataSetId);
	    return;
	}
	/* 
	 * Complex Matrix have 2 refs
	 * Real Matrix only have one.
	 */
	byte[][] dataRefs = new byte[2][8];

	H5.H5Dread(dataSetId, HDF5Constants.H5T_STD_REF_OBJ,
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, dataRefs);
	
	data.setRealPart(getDoubleMatrix(H5.H5Rdereference(dataSetId, HDF5Constants.H5R_OBJECT, dataRefs[0])));
	if (H5Read.isComplex(dataSetId)) {
	    data.setImaginaryPart(getDoubleMatrix(H5.H5Rdereference(dataSetId, HDF5Constants.H5R_OBJECT, dataRefs[1])));
	    }
	else {
	    data.setImaginaryPart(null);
	}
	H5.H5Dclose(dataSetId);
    }

    private static double[][] getDoubleMatrix(int dataSetId) throws NullPointerException, HDF5Exception {
	int[] nbElems = H5Read.getAllDims(dataSetId); 
	double[] data = new double[nbElems[0] * nbElems[1]];
	double[][] result = new double[nbElems[0]][nbElems[1]];
	
	int space = H5.H5Dget_space(dataSetId);
	H5.H5Dread_double(dataSetId, HDF5Constants.H5T_NATIVE_DOUBLE, space, 
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
	
	
	for(int i = 0 ; i < nbElems[0] ; i++) {
	    for(int j = 0 ; j < nbElems[1] ; j++) {
		result[i][j] = data[i+ j * nbElems[0]];
	    }
	}
	
	H5.H5Sclose(space);
	H5.H5Dclose(dataSetId);
	return result;
    }
}
