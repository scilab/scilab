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
import org.scilab.modules.types.scilabTypes.ScilabString;

public class H5WriteScilabString {

    public static void writeInDataSet(int file_id, String dataSetName, ScilabString data) throws NullPointerException, HDF5Exception {
	String[][] realData = data.getData();
	int rows = realData.length;
	int cols = realData[0].length;
	long[] dims = {rows *  cols};
	int dataLen = 0;

	StringBuffer dataOut = new StringBuffer();
	//get max length
	for(int i = 0 ; i < rows ; i++) {
	    for(int j = 0 ; j < cols ; j++) {
		dataLen = Math.max(dataLen, realData[i][j].length());
	    }
	}

	if(dataLen <= 0) {
	    dataLen = 1;
	}

	//Very ugly, TODO find another way to convert String[][] to byte[][] with homogeneous size
	for(int i = 0 ; i < cols ; i++) {
	    for(int j = 0 ; j < rows ; j++) {
		dataOut.append(realData[j][i]);
		for(int k = 0 ; k < (dataLen - realData[j][i].length()) ; k++) {
		    dataOut.append('\0');
		}
	    }
	}
	int space = H5.H5Screate_simple(1, dims, null);

	int typeId = H5.H5Tcopy (HDF5Constants.H5T_C_S1);
	H5.H5Tset_size (typeId, dataLen);

	int datasetId = H5.H5Dcreate(file_id, "/" + dataSetName,
		typeId, space, HDF5Constants.H5P_DEFAULT);

	H5.H5Dwrite(datasetId, typeId, HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, dataOut.toString().getBytes());
	H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_STRING);
	H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_ROWS, rows);
	H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_COLS, cols);

	H5.H5Dclose(datasetId);
	H5.H5Sclose(space);
	H5.H5Tclose(typeId);
    }
}
