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

package org.scilab.modules.hdf5.write;

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;

public class H5WriteScilabBoolean {
    public static void writeInDataSet(int file_id, String dataSetName,ScilabBoolean data) throws NullPointerException, HDF5Exception {
	boolean[][] realData = data.getData();	
	int rows = realData.length;
	int cols = realData[0].length;
	int[] dataOut = new int[rows * cols];
	long[] dims = {rows * cols};

	for (int i = 0 ; i < rows ; ++i) { //rows
	    for (int j = 0 ; j < cols ; ++j) { //cols
		dataOut[i + j * rows] = realData[i][j] ? 1 : 0;
	    }
	}

	int dataspaceId = H5.H5Screate_simple(1, dims, null);
	int datasetId = H5.H5Dcreate(file_id, "/" + dataSetName,
		HDF5Constants.H5T_NATIVE_INT, dataspaceId,
		HDF5Constants.H5P_DEFAULT);
	H5.H5Dwrite(datasetId, HDF5Constants.H5T_NATIVE_INT,
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
		HDF5Constants.H5P_DEFAULT, dataOut);

	H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_BOOLEAN);
	H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_ROWS, rows);
	H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_COLS, cols);

	H5.H5Dclose(datasetId);
	H5.H5Sclose(dataspaceId);
    }
}