package org.scilab.modules.hdf5.read;
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import ncsa.hdf.hdf5lib.H5;
import ncsa.hdf.hdf5lib.HDF5Constants;
import ncsa.hdf.hdf5lib.exceptions.HDF5AttributeException;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;
import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabInteger;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;

public class H5Read {

    public static int openFile(String fileName) throws HDF5LibraryException, NullPointerException {
	return H5.H5Fopen(fileName, HDF5Constants.H5F_ACC_RDONLY, HDF5Constants.H5P_DEFAULT);
    }

    public static void closeFile(int fileID) throws HDF5LibraryException {
    	H5.H5Fclose(fileID);
    }

    public static String getRootType(int fileId) throws NullPointerException, HDF5LibraryException, HDF5Exception {
	return readAttribute(getRootId(fileId), H5ScilabConstant.SCILAB_CLASS);
    }

    public static int getRootId(int fileId) throws HDF5LibraryException, NullPointerException {
	return H5.H5Dopen(fileId, getGroupRootName(fileId, "/"));
    }

    private static String getGroupRootName(int fileId, String groupName) throws HDF5LibraryException, NullPointerException {
	int nb_objs = H5.H5Gn_members(fileId, groupName);
	String[] allObjectsName = new String[nb_objs];
	int[] allObjectsType = new int[nb_objs];
	H5.H5Gget_obj_info_all(fileId, groupName, allObjectsName, allObjectsType); 

	for (int i = 0 ; i < nb_objs ; ++i) {
	    //System.out.println("============================================");
	    //System.out.println(" Object numer "+i);
	    //System.out.println(" Name  = "+allObjectsName[i]);
	    if (allObjectsType[i] == HDF5Constants.H5G_DATASET) {
		//System.out.println(" Type  = DataSet");
		//int dataSetId = H5.H5Dopen(fileId, groupName+"/"+allObjectsName[i]);
		//readDataSet(indent, dataSetId);
		return allObjectsName[i];
	    }
	    //	    if (allObjectsType[i] == HDF5Constants.H5G_GROUP) {
	    //		System.out.println(" Type  = Group");
	    //		//expandGroup(indent + 1, fileId, groupName+"/"+allObjectsName[i]);
	    //	    }
	    //	    System.out.println("============================================");
	}
	return null;
    }

    /**
     * Get the number of dimensions of the dataset.
     * Ex : 1 means it is scalar or vector
     * 2 means it is a Matrix
     * 3 ...
     * 
     * @param dataSetId
     * @return the number of dimensions of the corresponding dataset
     * @throws HDF5LibraryException
     */
    public static int getNbDims(int dataSetId) throws HDF5LibraryException {
	return H5.H5Sget_simple_extent_ndims(H5.H5Dget_space(dataSetId));
    }

    /**
     * Get all the dimensions of a dataSet.
     * Ex : a 4 x 10 matrix will return {4, 10}
     * 
     * @param dataSetId
     * @return all dimensions in a row
     * @throws HDF5LibraryException
     */
    public static long[] getAllDims(int dataSetId) throws HDF5LibraryException {
	long[] dims = new long[getNbDims(dataSetId)];
	long[] maxDims = new long[getNbDims(dataSetId)];
	H5.H5Sget_simple_extent_dims(H5.H5Dget_space(dataSetId), dims, maxDims);
	return dims;
    }

    public static boolean isEmpty(int dataSetId) throws NullPointerException, HDF5Exception {
	// Only one attribute (ie CLASS) so the EMPTY attribute is not present
	if (H5.H5Aget_num_attrs(dataSetId) <= 1) {
	    return false;
	}
	if (readAttribute(dataSetId, H5ScilabConstant.SCILAB_EMPTY).compareTo(H5ScilabConstant.SCILAB_EMPTY_TRUE) == 0) {
	    return true;
	}
	return false;
    }
    
    public static boolean isComplex(int dataSetId) throws NullPointerException, HDF5Exception {
	// Only one attribute (ie CLASS) so the COMPLEX attribute is not present
	if (H5.H5Aget_num_attrs(dataSetId) <= 1) {
	    return false;
	}
	if (readAttribute(dataSetId, H5ScilabConstant.SCILAB_COMPLEX).compareTo(H5ScilabConstant.SCILAB_COMPLEX_TRUE) == 0) {
	    return true;
	}
	return false;
    }
    
    public static String getIntegerPrecision(int dataSetId) throws NullPointerException, HDF5Exception {
    	// Only one attribute (ie CLASS) so the SCILAB_CLASS_PREC attribute is not present
    	if (H5.H5Aget_num_attrs(dataSetId) <= 1) {
    		return "";
    	}
    	return readAttribute(dataSetId, H5ScilabConstant.SCILAB_CLASS_PREC);
    }

    private static String readAttribute(int dataSetId, String attributeName) throws NullPointerException, HDF5Exception {
    	int attributeId = -1;
    	try {
    		// If there is no attribue do not try to open it
    		// There _must_ be at least one : SCILAB_CLASS
    		if (H5.H5Aget_num_attrs(dataSetId) <= 0) {
    			return "";
    		}
    		attributeId = H5.H5Aopen_name(dataSetId, attributeName);
    	}
    	catch (HDF5AttributeException e) {
    		return "";
    	}
    	int stringLength = H5.H5Tget_size(H5.H5Aget_type(attributeId));
    	int tid = H5.H5Tcopy(HDF5Constants.H5T_C_S1);
    	H5.H5Tset_size(tid, stringLength);
    	byte[] data = new byte[stringLength];
    	H5.H5Aread(attributeId, tid, data);
    	String result = new String(data, 0, stringLength).trim();
    	H5.H5Aclose(attributeId);

    	return result;
    }

    /**
     * Check if the dataset is made of double.
     * 
     * @param dataSetId
     * @return true if the dataset is of double data type
     * @throws HDF5LibraryException
     */
    public static boolean isDouble(int dataSetId) throws HDF5LibraryException {
	return (H5.H5Tget_class(H5.H5Dget_type(dataSetId)) ==  HDF5Constants.H5T_FLOAT) &&
	(H5.H5Tget_precision(H5.H5Dget_type(dataSetId)) == 64);
    }

    /**
     * Check if the dataset is made of float.
     * 
     * @param dataSetId
     * @return true if the dataset is of float data type
     * @throws HDF5LibraryException
     */
    public static boolean isFloat(int dataSetId) throws HDF5LibraryException {
	return (H5.H5Tget_class(H5.H5Dget_type(dataSetId)) ==  HDF5Constants.H5T_FLOAT) &&
	(H5.H5Tget_precision(H5.H5Dget_type(dataSetId)) == 32);
    } 

    /**
     * Check if the dataset is made of integer.
     * 
     * @param dataSetId
     * @return true if the dataset is of int data type
     * @throws HDF5LibraryException
     */
    public static boolean isInt(int dataSetId) throws HDF5LibraryException {
	return (H5.H5Tget_class(H5.H5Dget_type(dataSetId)) ==  HDF5Constants.H5T_INTEGER);
    } 

    /**
     * Check if the dataset is made of integer.
     * 
     * @param dataSetId
     * @return true if the dataset is of int data type
     * @throws HDF5LibraryException
     */
    public static boolean isString(int dataSetId) throws HDF5LibraryException {
	return H5.H5Tget_class(H5.H5Dget_type(dataSetId)) ==  HDF5Constants.H5T_STRING;
    }
    /**
     * Check if the dataset is made of references.
     * 
     * @param dataSetId
     * @return true if the dataset is of reference data type
     * @throws HDF5LibraryException
     */
    public static boolean isList(int dataSetId) throws HDF5LibraryException {
	return (H5.H5Tget_class(H5.H5Dget_type(dataSetId)) ==  HDF5Constants.H5T_REFERENCE);
    } 

    public static void readDataFromFile(int fileId, ScilabDouble data) throws NullPointerException, HDF5Exception {
	H5ReadScilabDouble.readData(H5Read.getRootId(fileId), data);
    }
    
    public static void readDataFromFile(int fileId, ScilabBoolean data) throws NullPointerException, HDF5Exception {
	H5ReadScilabBoolean.readData(H5Read.getRootId(fileId), data);
    }
    
    public static void readDataFromFile(int fileId, ScilabString data) throws NullPointerException, HDF5Exception {
	H5ReadScilabString.readData(H5Read.getRootId(fileId), data);
    }

    public static void readDataFromFile(int fileId, ScilabList data) throws NullPointerException, HDF5LibraryException, HDF5Exception {
	H5ReadScilabList.readData(H5Read.getRootId(fileId), data);
    }
    
    public static void readDataFromFile(int fileId, ScilabTList data) throws NullPointerException, HDF5LibraryException, HDF5Exception {
	H5ReadScilabTList.readData(H5Read.getRootId(fileId), data);
    }
    
    public static void readDataFromFile(int fileId, ScilabMList data) throws NullPointerException, HDF5LibraryException, HDF5Exception {
    	H5ReadScilabTList.readData(H5Read.getRootId(fileId), data);
    }
    

    public static ScilabType getData(int dataSetId) throws NullPointerException, HDF5Exception {
	ScilabType data = null;
	String dataType = readAttribute(dataSetId, H5ScilabConstant.SCILAB_CLASS);
	if(isScilabString(dataType)) { 
	    data = new ScilabString(); 
	    H5ReadScilabString.readData(dataSetId, (ScilabString) data);    
	}
	if(isScilabDouble(dataType)) { 
	    data = new ScilabDouble(); 
	    H5ReadScilabDouble.readData(dataSetId, (ScilabDouble) data);    
	}
	if(isScilabList(dataType)) { 
	    data = new ScilabList(); 
	    H5ReadScilabList.readData(dataSetId, (ScilabList) data);    
	}
	if(isScilabTList(dataType)) { 
	    data = new ScilabTList(); 
	    H5ReadScilabTList.readData(dataSetId, (ScilabTList) data);    
	}
	if(isScilabMList(dataType)) { 
	    data = new ScilabMList(); 
	    H5ReadScilabMList.readData(dataSetId, (ScilabMList) data);    
	}
	if(isScilabBoolean(dataType)) { 
	    data = new ScilabBoolean(); 
	    H5ReadScilabBoolean.readData(dataSetId, (ScilabBoolean) data);    
	}
	if(isScilabInteger(dataType)) { 
	    data = new ScilabInteger(); 
	    H5ReadScilabInteger.readData(dataSetId, (ScilabInteger) data);    
	}
	
	return data;
    }
    
    private static boolean isScilabString(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_STRING) == 0; }
    private static boolean isScilabDouble(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_DOUBLE) == 0; }
    private static boolean isScilabList(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_LIST) == 0; }
    private static boolean isScilabMList(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_MLIST) == 0; }
    private static boolean isScilabTList(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_TLIST) == 0; }
    private static boolean isScilabBoolean(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_BOOLEAN) == 0; }
    private static boolean isScilabInteger(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_INT) == 0; }
}
