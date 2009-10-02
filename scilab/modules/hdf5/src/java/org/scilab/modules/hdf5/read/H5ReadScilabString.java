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
	long[] nbElems = H5Read.getAllDims(dataSetId); 
	int rows = (int) nbElems[0];
	int cols = (int) nbElems[1];
	byte[][] dataRefs = new byte[rows * cols][8];
	
	data.setData(new String[rows][cols]);
	
	H5.H5Dread(dataSetId, HDF5Constants.H5T_STD_REF_OBJ,
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, dataRefs);
	
	for( int i = 0 ; i < rows ; ++i) {
	    for( int j = 0 ; j < cols ; ++j) {
		data.getData()[i][j] = getString(H5.H5Rdereference(dataSetId, HDF5Constants.H5R_OBJECT, dataRefs[i * cols +j]));
	    }
	}
	H5.H5Dclose(dataSetId);
    }
    
    private static String getString(int dataSetId) throws HDF5LibraryException {
	int stringLength = H5.H5Tget_size(H5.H5Dget_type(dataSetId));
	byte[] data = new byte[stringLength];
	H5.H5Dread(dataSetId, H5.H5Dget_type(dataSetId),
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, data);
	String result = new String(data, 0, stringLength).trim();
	H5.H5Dclose(dataSetId);
	
	return result;
    }

}
