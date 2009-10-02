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

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;

public class H5ReadScilabDouble {
    public static void readData(int dataSetId, ScilabDouble data) throws NullPointerException, HDF5Exception {
	/* Empty Matrix */
	if (H5Read.isEmpty(dataSetId)) {
	    data.setRealPart(null);
	    data.setImaginaryPart(null);
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
	long[] nbElems = H5Read.getAllDims(dataSetId); 
	double[][] data = new double[(int) nbElems[0]][(int) nbElems[1]];
	H5.H5Dread(dataSetId, H5.H5Dget_type(dataSetId),
		H5.H5Dget_space(dataSetId), HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
	H5.H5Dclose(dataSetId);
	return data;
    }
}
