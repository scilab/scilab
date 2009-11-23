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
import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.hdf5.scilabTypes.ScilabString;

public class H5ReadScilabString {

    public static void readData(int dataSetId, ScilabString data) throws NullPointerException, HDF5Exception {
	data.setData(getStringMatrix(dataSetId));
    }
    
    private static String[][] getStringMatrix(int dataSetId) throws NullPointerException, HDF5Exception {
	int[] nbElems = H5Read.getAllDims(dataSetId); 
	int rows = nbElems[0];
	int cols = nbElems[1];
	int space, filetype, memtype;
	String[] dataRead = null;
	String[][] result = null;
	
	//Get the data type and its size
	filetype = H5.H5Dget_type(dataSetId);
	int dataLen = H5.H5Tget_size(filetype);
	dataLen++;
	
	space = H5.H5Dget_space(dataSetId);

	memtype = H5.H5Tcopy (HDF5Constants.H5T_C_S1);
	H5.H5Tset_size (memtype, dataLen);

	//alloc temp byte array
	dataRead = new String[rows * cols];
	//alloc return data
	result = new String[rows][cols];

	H5.H5Dread_string (dataSetId, memtype, HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, dataRead);
	
	for(int i = 0 ; i < rows ; i++) {
	    for(int j = 0 ; j < cols ; j++) {
		result[i][j] = dataRead[i+ j * rows];	
	    }
	}

	H5.H5Tclose(filetype);
	H5.H5Tclose(memtype);
	H5.H5Sclose(space);
	H5.H5Dclose(dataSetId);
	return result;
    }
    
}
