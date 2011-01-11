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

import java.util.ArrayList;
import java.util.List;

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.types.ScilabString;

public class H5WriteScilabString {

	public static void writeInDataSet(int file_id, String dataSetName, ScilabString data) throws NullPointerException, HDF5Exception {
	String[][] realData = data.getData();
	int rows = realData.length;
	int cols = realData[0].length;
	long[] dims = {rows *  cols};
	int dataLen = 0;

	//get max length and store already encoded String
	final List<List<byte[]>> encodedString = new ArrayList<List<byte[]>>(cols);
	for(int i = 0 ; i < cols ; i++) {
		final List<byte[]> currentCol = new ArrayList<byte[]>(rows);
		encodedString.add(currentCol);
		
		for(int j = 0 ; j < rows ; j++) {
			final byte[] strBytes = realData[j][i].getBytes();
			dataLen = Math.max(dataLen, strBytes.length + 1);
			currentCol.add(strBytes);
		}
	}
	
	final byte[] buf;
	if(dataLen <= 0) {
		dataLen = 1;
		buf = new byte[] {0};
	} else {
		// TODO: avoid a huge allocation by using data slicing
		buf = new byte[rows * cols * dataLen];
	}

	for(int i = 0 ; i < cols ; i++) {
		for(int j = 0 ; j < rows ; j++) {
			final byte[] str = encodedString.get(i).get(j);
			System.arraycopy(str, 0, buf, (i * rows + j) * dataLen , str.length);
		}
	}
	int space = H5.H5Screate_simple(1, dims, null);

	int typeId = H5.H5Tcopy (HDF5Constants.H5T_C_S1);
	H5.H5Tset_size (typeId, dataLen);

	int datasetId = H5.H5Dcreate(file_id, "/" + dataSetName,
		typeId, space, HDF5Constants.H5P_DEFAULT);

	H5.H5Dwrite(datasetId, typeId, HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, buf);
	H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_STRING);
	H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_ROWS, rows);
	H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_COLS, cols);

	H5.H5Dclose(datasetId);
	H5.H5Sclose(space);
	H5.H5Tclose(typeId);
	}
}
