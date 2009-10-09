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
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;

public class H5WriteScilabDouble {
	public static void writeInDataSet(int fileId, String dataSetName, ScilabDouble data) throws NullPointerException, HDF5Exception {
		/* Empty Matrix */
		if (data.getRealPart() == null && data.getImaginaryPart() == null) {
			writeEmptyMatrix(fileId, dataSetName);
			return;
		}

		/* Real Matrix */
		if (data.isReal()) {
			writeRealMatrix(fileId, dataSetName, data.getRealPart());
			return;
		}

		/* Complex Matrix */
		writeComplexMatrix(fileId, dataSetName, data.getRealPart(), data.getImaginaryPart());
	}

	private static int writeDoubleDataSet(int fileId, String dataSetName, double[][] data, boolean empty) throws NullPointerException, HDF5Exception {
		long[] dims = {data.length, data[0].length};
		int dataspaceId = H5.H5Screate_simple(2, dims, null);
		int datasetId = H5.H5Dcreate(fileId, "/" + dataSetName,
				HDF5Constants.H5T_NATIVE_DOUBLE, dataspaceId,
				HDF5Constants.H5P_DEFAULT);
		H5.H5Dwrite(datasetId, HDF5Constants.H5T_NATIVE_DOUBLE,
				HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
				HDF5Constants.H5P_DEFAULT, data);
		H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_DOUBLE);
		if (empty) {
			H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_EMPTY, H5ScilabConstant.SCILAB_EMPTY_TRUE);
		}
		H5.H5Dclose(datasetId);
		H5.H5Sclose(dataspaceId);

		return datasetId;
	}

	private static void writeEmptyMatrix(int fileId, String dataSetName) throws NullPointerException, HDF5Exception {
		double [][]data = new double[1][1];
		data[0][0] = 0;

		/*
		 * Create a group named #<dataSetName where '/' became '_'>#
		 */
		int group_id = H5.H5Gcreate(fileId, "/#" + dataSetName.replace('/', '_') +"#", HDF5Constants.H5P_DEFAULT);
		H5.H5Gclose(group_id);
		writeDoubleDataSet(fileId, dataSetName, data, true);
	}

	private static void writeRealMatrix(int fileId, String dataSetName, double[][]data) throws NullPointerException, HDF5Exception {
		long[] dims = {1};
		byte[][] refs = new byte[1][8];

		/*
		 * Create a group named #<dataSetName where '/' became '_'>#
		 */
		int group_id = H5.H5Gcreate(fileId, "/#" + dataSetName.replace('/', '_') +"#", HDF5Constants.H5P_DEFAULT);
		H5.H5Gclose(group_id);

		/*
		 * Then for each object, create a dataset and a Reference
		 */
		String currentName = "#"+dataSetName.replace('/', '_')+"#/#0#";
		writeDoubleDataSet(fileId, currentName, data, false);
		refs[0] = H5.H5Rcreate(fileId, currentName, HDF5Constants.H5R_OBJECT, -1);

		int dataspaceId = H5.H5Screate_simple(1, dims, null);
		int datasetId = H5.H5Dcreate(fileId, "/" + dataSetName,
				HDF5Constants.H5T_STD_REF_OBJ, dataspaceId,
				HDF5Constants.H5P_DEFAULT);
		H5.H5Dwrite(datasetId, HDF5Constants.H5T_STD_REF_OBJ,
				HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
				HDF5Constants.H5P_DEFAULT, refs);
		H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_DOUBLE);
		H5.H5Dclose(datasetId);
		H5.H5Sclose(dataspaceId);
	}

	private static void writeComplexMatrix(int fileId, String dataSetName, double[][]realData, double[][] imagData) throws NullPointerException, HDF5Exception {
		long[] dims = {2};
		byte[][] refs = new byte[2][8];

		/*
		 * Create a group named #<dataSetName where '/' became '_'>#
		 */
		int group_id = H5.H5Gcreate(fileId, "/#" + dataSetName.replace('/', '_') +"#", HDF5Constants.H5P_DEFAULT);
		H5.H5Gclose(group_id);

		String currentName = "#"+dataSetName.replace('/', '_')+"#/#0#";
		writeDoubleDataSet(fileId, currentName, realData, false);
		refs[0] = H5.H5Rcreate(fileId, currentName, HDF5Constants.H5R_OBJECT, -1);

		currentName = "#"+dataSetName.replace('/', '_')+"#/#1#";
		writeDoubleDataSet(fileId, currentName, imagData, false);
		refs[1] = H5.H5Rcreate(fileId, currentName, HDF5Constants.H5R_OBJECT, -1);

		int dataspaceId = H5.H5Screate_simple(1, dims, null);
		int datasetId = H5.H5Dcreate(fileId, "/" + dataSetName,
				HDF5Constants.H5T_STD_REF_OBJ, dataspaceId,
				HDF5Constants.H5P_DEFAULT);
		H5.H5Dwrite(datasetId, HDF5Constants.H5T_STD_REF_OBJ,
				HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL,
				HDF5Constants.H5P_DEFAULT, refs);
		H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_DOUBLE);
		H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_COMPLEX, H5ScilabConstant.SCILAB_COMPLEX_TRUE);
		H5.H5Dclose(datasetId);
		H5.H5Sclose(dataspaceId);
	}
}
