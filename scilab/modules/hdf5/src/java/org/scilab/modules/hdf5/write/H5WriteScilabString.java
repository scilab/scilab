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
import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;

public class H5WriteScilabString {

    public static void writeInDataSet(int file_id, String dataSetName, ScilabString data) throws NullPointerException, HDF5Exception {
	String[][] realData = data.getData();
	int rows = realData.length;
	int cols = realData[0].length;
	long[] dims = {rows, cols};
	byte[][] refs = new byte[rows * cols][8];

	/*
	 * Create a group named #<dataSetName where '/' became '_'>#
	 */
	int group_id = H5.H5Gcreate(file_id, "/#" + dataSetName.replace('/', '_') +"#", HDF5Constants.H5P_DEFAULT);
	H5.H5Gclose(group_id);

	/*
	 * Then for each object, create a dataset and a Reference
	 */
	for (int i = 0 ; i < rows ; ++i) {
	    for (int j = 0 ; j < cols ; ++j) {
		String currentName = "#"+dataSetName.replace('/', '_')+"#/#"+(i+rows*j)+"#";
		writeInDataSet(file_id, currentName, realData[i][j]);
		refs[i * cols + j] = H5.H5Rcreate(file_id, currentName, HDF5Constants.H5R_OBJECT, -1);
	    }
	}

	int dataspaceId = H5.H5Screate_simple(2, dims, null);
	int datasetId = H5.H5Dcreate(file_id, "/" + dataSetName,
		HDF5Constants.H5T_STD_REF_OBJ, dataspaceId,
		HDF5Constants.H5P_DEFAULT);
	H5.H5Dwrite(datasetId, HDF5Constants.H5T_STD_REF_OBJ,
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
		HDF5Constants.H5P_DEFAULT, refs);
	H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_STRING);
	H5.H5Dclose(datasetId);
	H5.H5Sclose(dataspaceId);
    }


    public static void writeInDataSet(int file_id, String dataSetName, String data) throws NullPointerException, HDF5Exception {
	long[] dims = {1};
	int size = 0;
	if (data.length() != 0) {
	    size = data.length();
	}
	else {
	    size = 1;
	}

	int tid = H5.H5Tcopy(HDF5Constants.H5T_C_S1);
	H5.H5Tset_size(tid, size);
	int dataspaceId = H5.H5Screate_simple(1, dims, null);
	int datasetId = H5.H5Dcreate(file_id, "/" + dataSetName,
		tid, dataspaceId,
		HDF5Constants.H5P_DEFAULT);
	H5.H5Dwrite(datasetId, tid,
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
		HDF5Constants.H5P_DEFAULT, data.getBytes());
	H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_STRING);
	H5.H5Dclose(datasetId);
	H5.H5Sclose(dataspaceId);
    }
}
