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

package org.scilab.modules.hdf5.read;

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.scilabTypes.ScilabInteger;

public class H5ReadScilabInteger {
	public static void readData(int dataSetId, ScilabInteger data) throws NullPointerException, HDF5Exception {
		boolean bUnsigned = false;
		String prec = H5Read.getIntegerPrecision(dataSetId);

		if(prec.compareTo("8") == 0 || prec.compareTo("u8") == 0) {
			bUnsigned = prec.compareTo("u8") == 0 ? true : false;
			data.setData(getIntegerMatrix8(dataSetId, bUnsigned), bUnsigned );
		}
		else if(prec.compareTo("16") == 0 || prec.compareTo("u16") == 0){
			bUnsigned = prec.compareTo("u16") == 0 ? true : false;
			data.setData(getIntegerMatrix16(dataSetId, bUnsigned), bUnsigned );
		}
		else if(prec.compareTo("32") == 0 || prec.compareTo("u32") == 0){
			bUnsigned = prec.compareTo("u32") == 0 ? true : false;
			data.setData(getIntegerMatrix32(dataSetId, bUnsigned), bUnsigned );
		}
		else if(prec.compareTo("64") == 0 || prec.compareTo("u64") == 0){
			bUnsigned = prec.compareTo("u64") == 0 ? true : false;
			data.setData(getIntegerMatrix64(dataSetId, bUnsigned), bUnsigned );
		}
	}

	private static byte[][] getIntegerMatrix8(int dataSetId, boolean bUnsigned) throws NullPointerException, HDF5Exception {
		long[] nbElems = H5Read.getAllDims(dataSetId); 
		byte[][] data = new byte[(int)nbElems[0]][(int)nbElems[1]];
		int prec = bUnsigned ? HDF5Constants.H5T_NATIVE_UINT8 : HDF5Constants.H5T_NATIVE_INT8; 

		H5.H5Dread(dataSetId, prec, H5.H5Dget_space(dataSetId), HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
		H5.H5Dclose(dataSetId);

		return data;
	}
	
	private static short[][] getIntegerMatrix16(int dataSetId, boolean bUnsigned) throws NullPointerException, HDF5Exception {
		long[] nbElems = H5Read.getAllDims(dataSetId); 
		short[][] data = new short[(int)nbElems[0]][(int)nbElems[1]];
		int prec = bUnsigned ? HDF5Constants.H5T_NATIVE_UINT16 : HDF5Constants.H5T_NATIVE_INT16; 

		H5.H5Dread(dataSetId, prec, H5.H5Dget_space(dataSetId), HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
		H5.H5Dclose(dataSetId);

		return data;
	}
	
	private static int[][] getIntegerMatrix32(int dataSetId, boolean bUnsigned) throws NullPointerException, HDF5Exception {
		long[] nbElems = H5Read.getAllDims(dataSetId); 
		int[][] data = new int[(int)nbElems[0]][(int)nbElems[1]];
		int prec = bUnsigned ? HDF5Constants.H5T_NATIVE_UINT32 : HDF5Constants.H5T_NATIVE_INT32; 

		H5.H5Dread(dataSetId, prec, H5.H5Dget_space(dataSetId), HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
		H5.H5Dclose(dataSetId);

		return data;
	}
	
	private static long[][] getIntegerMatrix64(int dataSetId, boolean bUnsigned) throws NullPointerException, HDF5Exception {
		long[] nbElems = H5Read.getAllDims(dataSetId); 
		long[][] data = new long[(int)nbElems[0]][(int)nbElems[1]];
		int prec = bUnsigned ? HDF5Constants.H5T_NATIVE_UINT64 : HDF5Constants.H5T_NATIVE_INT64; 

		H5.H5Dread(dataSetId, prec, H5.H5Dget_space(dataSetId), HDF5Constants.H5S_ALL, HDF5Constants.H5S_ALL, data);
		H5.H5Dclose(dataSetId);

		return data;
	}
}
