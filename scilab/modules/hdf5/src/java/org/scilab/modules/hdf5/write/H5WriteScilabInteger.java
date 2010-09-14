/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
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
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabIntegerTypeEnum;


public class H5WriteScilabInteger {
	public static void writeInDataSet(int file_id, String dataSetName, ScilabInteger data) throws NullPointerException, HDF5Exception {
		Object[] dataToWrite = null; 
		ScilabIntegerTypeEnum prec = data.getPrec();
		int nativeType = 0;
		String classType = "";

		if (prec == ScilabIntegerTypeEnum.sci_int8 || prec == ScilabIntegerTypeEnum.sci_uint8) {
			nativeType = data.isUnsigned() ? HDF5Constants.H5T_NATIVE_UINT8 : HDF5Constants.H5T_NATIVE_INT8; 
			classType = data.isUnsigned() ? H5ScilabConstant.SCILAB_CLASS_UINT8 : H5ScilabConstant.SCILAB_CLASS_INT8; 
			dataToWrite = getDataToWrite8(data);
		}else if (prec == ScilabIntegerTypeEnum.sci_int16 || prec == ScilabIntegerTypeEnum.sci_uint16) {
			nativeType = data.isUnsigned() ? HDF5Constants.H5T_NATIVE_UINT16 : HDF5Constants.H5T_NATIVE_INT16; 
			classType = data.isUnsigned() ? H5ScilabConstant.SCILAB_CLASS_UINT16 : H5ScilabConstant.SCILAB_CLASS_INT16; 
			dataToWrite = getDataToWrite16(data);
		}else if (prec == ScilabIntegerTypeEnum.sci_int32 || prec == ScilabIntegerTypeEnum.sci_uint32) {
			nativeType = data.isUnsigned() ? HDF5Constants.H5T_NATIVE_UINT32 : HDF5Constants.H5T_NATIVE_INT32; 
			classType = data.isUnsigned() ? H5ScilabConstant.SCILAB_CLASS_UINT32 : H5ScilabConstant.SCILAB_CLASS_INT32; 
			dataToWrite = getDataToWrite32(data);
		}else if (prec == ScilabIntegerTypeEnum.sci_int64 || prec == ScilabIntegerTypeEnum.sci_uint64) {
			nativeType = data.isUnsigned() ? HDF5Constants.H5T_NATIVE_UINT64 : HDF5Constants.H5T_NATIVE_INT64; 
			classType = data.isUnsigned() ? H5ScilabConstant.SCILAB_CLASS_UINT64 : H5ScilabConstant.SCILAB_CLASS_INT64; 
			dataToWrite = getDataToWrite64(data);
		}

		long[] dims = {data.getHeight() * data.getWidth()};
		int dataspaceId = H5.H5Screate_simple(1, dims, null);
		
		int datasetId = H5.H5Dcreate(file_id, "/" + dataSetName, nativeType, dataspaceId, HDF5Constants.H5P_DEFAULT);
		H5.H5Dwrite(datasetId, nativeType, HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, HDF5Constants.H5P_DEFAULT, dataToWrite);
		
		H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS, H5ScilabConstant.SCILAB_CLASS_INT);
		H5Write.createAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_PREC, classType);

		H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_ROWS, data.getHeight());
		H5Write.createIntAttribute(datasetId, H5ScilabConstant.SCILAB_CLASS_COLS, data.getWidth());
		
		H5.H5Dclose(datasetId);
		H5.H5Sclose(dataspaceId);
	}

	public static Byte[] getDataToWrite8(ScilabInteger data){
		Byte[] dataToWrite = new Byte[data.getHeight() * data.getWidth()];
		byte[][] byteData = data.getDataAsByte();

		for(int i = 0 ; i < data.getHeight() ; i++) {
			for(int j = 0 ; j < data.getWidth() ; j++) {
				dataToWrite[i + data.getHeight() * j] = byteData[i][j];
			}
		}
		return dataToWrite;
	}

	public static Short[] getDataToWrite16(ScilabInteger data){
		Short[] dataToWrite = new Short[data.getHeight() * data.getWidth()];
		short[][] shortData = data.getDataAsShort();

		for(int i = 0 ; i < data.getHeight() ; i++) {
			for(int j = 0 ; j < data.getWidth() ; j++) {
				dataToWrite[i + data.getHeight() * j] = shortData[i][j];
			}
		}
		return dataToWrite;
	}

	public static Integer[] getDataToWrite32(ScilabInteger data){
		Integer[] dataToWrite = new Integer[data.getHeight() * data.getWidth()];
		int[][] intData = data.getDataAsInt();

		for(int i = 0 ; i < data.getHeight() ; i++) {
			for(int j = 0 ; j < data.getWidth() ; j++) {
				dataToWrite[i + data.getHeight() * j] = intData[i][j];
			}
		}
		return dataToWrite;
	}

	public static Long[] getDataToWrite64(ScilabInteger data){
		Long[] dataToWrite = new Long[data.getHeight() * data.getWidth()];
		long[][] longData = data.getDataAsLong();

		for(int i = 0 ; i < data.getHeight() ; i++) {
			for(int j = 0 ; j < data.getWidth() ; j++) {
				dataToWrite[i + data.getHeight() * j] = longData[i][j];
			}
		}
		return dataToWrite;
	}
}
