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

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.H5ScilabConstant;

public class H5WriteScilabCommonList {
    protected static void writeInDataSet(int file_id, String dataSetName, ArrayList data, String ListType) throws NullPointerException, HDF5Exception {
	long[] dims = {data.size()};
	byte[][] refs = new byte[data.size()][8];

	/*
	 * Create a group named #<dataSetName where '/' became '_'>#
	 */
	int group_id = H5.H5Gcreate(file_id, "/#" + dataSetName.replace('/', '_') +"#", HDF5Constants.H5P_DEFAULT);
	H5.H5Gclose(group_id);

	/*
	 * Then for each object, create a dataset and a Reference
	 */
	for (int i = 0 ; i < data.size() ; ++i) {
	    String currentName = "#"+dataSetName.replace('/', '_')+"#/#"+i+"#";
	    H5Write.writeInDataSet(file_id, currentName, data.get(i));
	    refs[i] = H5.H5Rcreate(file_id, currentName, HDF5Constants.H5R_OBJECT, -1);
	}
	/* Special Empty List case */
	if(data.size() == 0) {
	    dims[0] = 1;
	    refs = new byte[1][8];
	}
	int dataspaceId = H5.H5Screate_simple(1, dims, null);
	int datasetId = H5.H5Dcreate(file_id, "/" + dataSetName,
		HDF5Constants.H5T_STD_REF_OBJ, dataspaceId,
		HDF5Constants.H5P_DEFAULT);
	H5.H5Dwrite(datasetId, HDF5Constants.H5T_STD_REF_OBJ,
		HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
		HDF5Constants.H5P_DEFAULT, refs);
	/* Special Empty List case */
	if(data.size() == 0) {
	    H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_EMPTY, H5ScilabConstant.SCILAB_EMPTY_TRUE);
	}
	H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, ListType);

	H5.H5Dclose(datasetId);
	H5.H5Sclose(dataspaceId);
    }
}
