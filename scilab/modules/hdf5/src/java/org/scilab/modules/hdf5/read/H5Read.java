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
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;

/**
 * H5Read
 *
 */
public final class H5Read {
    
    /**
     * Private Constructor for utility class.
     */
    private H5Read() { }

    /**
     * openFile
     * @param fileName the filename to open.
     * @return the ID of the opened file.
     * @throws NullPointerException 
     * @throws HDF5LibraryException 
     * @throws HDF5LibraryException .
     */
    public static int openFile(String fileName) throws HDF5LibraryException {
	return H5.H5Fopen(fileName, HDF5Constants.H5F_ACC_RDONLY, HDF5Constants.H5P_DEFAULT);
    }

    /**
     * closeFile
     * @param fileID the file ID to close
     * @throws HDF5LibraryException .
     */
    public static void closeFile(int fileID) throws HDF5LibraryException {
    	H5.H5Fclose(fileID);
    }

    /**
     * getRootType
     * @param fileId the file ID to look in.
     * @return the root element type.
     * @throws HDF5Exception .
     */
    public static String getRootType(int fileId) throws HDF5Exception {
	int root = getRootId(fileId);
	String attrib = readAttribute(root, H5ScilabConstant.SCILAB_CLASS);
	H5.H5Dclose(root);
	return attrib;
	
    }

    /**
     * getRootId
     * @param fileId the file ID to look in.
     * @return the ID of the root element.
     * @throws HDF5LibraryException .
     */
    public static int getRootId(int fileId) throws HDF5LibraryException {
	return H5.H5Dopen(fileId, getGroupRootName(fileId, "/"));
    }

    /**
     * getGroupRootName
     * @param fileId the file ID to look in.
     * @param groupName the groupName : "/" for root
     * @return the group name
     * @throws HDF5LibraryException .
     */
    private static String getGroupRootName(int fileId, String groupName) throws HDF5LibraryException {
	int nbObjs = H5.H5Gn_members(fileId, groupName);
	String[] allObjectsName = new String[nbObjs];
	int[] allObjectsType = new int[nbObjs];
	long[] refs = new long[nbObjs];
	H5.H5Gget_obj_info_all(fileId, groupName, allObjectsName, allObjectsType, refs); 

	for (int i = 0; i < nbObjs; ++i) {
	    if (allObjectsType[i] == HDF5Constants.H5G_DATASET) {
		return allObjectsName[i];
	    }
	}
	return null;
    }

    /**
     * Get the number of dimensions of the dataset.
     * Ex : 1 means it is scalar or vector
     * 2 means it is a Matrix
     * 3 ...
     * 
     * @param dataSetId the dataset ID to look in.
     * @return the number of dimensions of the corresponding dataset
     * @throws HDF5LibraryException .
     */
    public static int getNbDims(int dataSetId) throws HDF5LibraryException {
	int space = H5.H5Dget_space(dataSetId);
	int dim = H5.H5Sget_simple_extent_ndims(space);
	H5.H5Sclose(space);
	return dim;
    }

    /**
     * Get all the dimensions of a dataSet.
     * Ex : a 4 x 10 matrix will return {4, 10}
     * 
     * @param dataSetId the dataset ID to look in.
     * @return all dimensions in a row
     * @throws HDF5Exception . 
     */
    public static int[] getAllDims(int dataSetId) throws HDF5Exception {
	int[] dims = new int[2];
	
	if (isEmpty(dataSetId)) {
	    dims[0] = 0;
	    dims[1] = 0;
	} else {
	    dims[0] = readIntAttribute(dataSetId, H5ScilabConstant.SCILAB_CLASS_ROWS);
	    dims[1] = readIntAttribute(dataSetId, H5ScilabConstant.SCILAB_CLASS_COLS);
	}
	
	return dims;
    }

    /**
     * Get list item count.
     * 
     * @param dataSetId the dataset ID to look in.
     * @return List item count
     * @throws HDF5Exception .
     */
    public static int getListDim(int dataSetId) throws HDF5Exception {
	int itemCount = 0;
	
	if (isEmpty(dataSetId)) {
	    itemCount = 0;
	} else {
	    itemCount = readIntAttribute(dataSetId, H5ScilabConstant.SCILAB_CLASS_ITEMS);
	}
	
	return itemCount;
    }

    /**
     * isEmpty 
     * @param dataSetId the dataset ID to look in.
     * @return true if it is empty.
     * @throws HDF5Exception .
     */
    public static boolean isEmpty(int dataSetId) throws HDF5Exception {
	// Only one attribute (ie CLASS) so the EMPTY attribute is not present
	if (H5.H5Aget_num_attrs(dataSetId) <= 1) {
	    return false;
	}
	if (readAttribute(dataSetId, H5ScilabConstant.SCILAB_EMPTY).compareTo(H5ScilabConstant.SCILAB_EMPTY_TRUE) == 0) {
	    return true;
	}
	return false;
    }
    
    /**
     * isComplex
     * @param dataSetId the dataset ID to look in.
     * @return true if it is complex.
     * @throws HDF5Exception .
     */
    public static boolean isComplex(int dataSetId) throws HDF5Exception {
	// Only one attribute (ie CLASS) so the COMPLEX attribute is not present
	if (H5.H5Aget_num_attrs(dataSetId) <= 1) {
	    return false;
	}
	if (readAttribute(dataSetId, H5ScilabConstant.SCILAB_COMPLEX).compareTo(H5ScilabConstant.SCILAB_COMPLEX_TRUE) == 0) {
	    return true;
	}
	return false;
    }
    
    /**
     * getIntegerPrecision
     * @param dataSetId the dataset ID to look in.
     * @return the integer precision.
     * @throws HDF5Exception .
     */
    public static String getIntegerPrecision(int dataSetId) throws HDF5Exception {
    	// Only one attribute (ie CLASS) so the SCILAB_CLASS_PREC attribute is not present
    	if (H5.H5Aget_num_attrs(dataSetId) <= 1) {
    		return "";
    	}
    	return readAttribute(dataSetId, H5ScilabConstant.SCILAB_CLASS_PREC);
    }

    /**
     * readAttribute
     * @param dataSetId the dataset ID to look in.
     * @param attributeName the name of the attribute
     * @return the value of the corresponding attribute
     * @throws HDF5Exception .
     */
    private static String readAttribute(int dataSetId, String attributeName) throws HDF5Exception {
    	int attributeId = -1;
    	try {
    		int attrCount = H5.H5Aget_num_attrs(dataSetId); 
    		// If there is no attribue do not try to open it
    		// There _must_ be at least one : SCILAB_CLASS
    		if (attrCount <= 0) {
    			return "";
    		}

    		for (int i = 0; i < attrCount; i++) {
    			attributeId = H5.H5Aopen_idx(dataSetId, i);
    			String[] buf = new String[1];
    			H5.H5Aget_name(attributeId, 64, buf);
    			if (buf[0].compareTo(attributeName) == 0) {
    				break;
    			}
    			
    			H5.H5Aclose(attributeId);
    			attributeId = -1;
    		}
    		
    		if (attributeId == -1) {
    			//not found
    			return "";
    		}
    	} catch (HDF5AttributeException e) {
    		return "";
    	}
    	int stringLength = H5.H5Tget_size(H5.H5Aget_type(attributeId));
    	int tid = H5.H5Tcopy(HDF5Constants.H5T_C_S1);
    	H5.H5Tset_size(tid, stringLength);
    	byte[] data = new byte[stringLength];
    	H5.H5Aread(attributeId, tid, data);
    	String result = new String(data, 0, stringLength).trim();
    	H5.H5Tclose(tid);
    	H5.H5Aclose(attributeId);

    	return result;
    }

    /**
     * readIntAttribute
     * @param dataSetId the dataset ID to look in.
     * @param attributeName the name of the attribute
     * @return the value of the corresponding attribute
     * @throws HDF5Exception .
     */
    private static int readIntAttribute(int dataSetId, String attributeName) throws HDF5Exception {
    	int attributeId = -1;
    	int[] data = new int[1];
    	try {
    		int attrCount = H5.H5Aget_num_attrs(dataSetId); 
    		// If there is no attribue do not try to open it
    		// There _must_ be at least one : SCILAB_CLASS
    		if (attrCount <= 0) {
    			return 0;
    		}

    		for (int i = 0; i < attrCount; i++) {
    			attributeId = H5.H5Aopen_idx(dataSetId, i);
    			String[] buf = new String[1];
    			H5.H5Aget_name(attributeId, 64, buf);
    			if (buf[0].compareTo(attributeName) == 0) {
    				break;
    			}
    			
    			H5.H5Aclose(attributeId);
    			attributeId = -1;
    		}
    		
    		if (attributeId == -1) {
    			//not found
    			return 0;
    		}
    	} catch (HDF5AttributeException e) {
    		return 0;
    	}
    	H5.H5Aread(attributeId, HDF5Constants.H5T_NATIVE_INT, data);
    	H5.H5Aclose(attributeId);

    	return data[0];
    }

    /**
     * Check if the dataset is made of double.
     * 
     * @param dataSetId the dataset ID to look in.
     * @return true if the dataset is of double data type
     * @throws HDF5LibraryException .
     */
    public static boolean isDouble(int dataSetId) throws HDF5LibraryException {
	int type = H5.H5Dget_type(dataSetId);
	
	boolean result = false;
	if (H5.H5Tget_class(type) == HDF5Constants.H5T_FLOAT && H5.H5Tget_precision(type) == 64) {
	    result = true;
	}
	
	H5.H5Tclose(type);
	return result;
    }

    /**
     * Check if the dataset is made of float.
     * 
     * @param dataSetId the dataset ID to look in.
     * @return true if the dataset is of float data type
     * @throws HDF5LibraryException .
     */
    public static boolean isFloat(int dataSetId) throws HDF5LibraryException {
	int type = H5.H5Dget_type(dataSetId);
	
	boolean result = false;
	if (H5.H5Tget_class(type) == HDF5Constants.H5T_FLOAT && H5.H5Tget_precision(type) == 32) {
	    result = true;
	}
	H5.H5Tclose(type);
	return result;
    } 

    /**
     * Check if the dataset is made of integer.
     * 
     * @param dataSetId the dataset ID to look in.
     * @return true if the dataset is of int data type
     * @throws HDF5LibraryException .
     */
    public static boolean isInt(int dataSetId) throws HDF5LibraryException {
	int type = H5.H5Dget_type(dataSetId);
	
	boolean result = false;
	if (H5.H5Tget_class(type) == HDF5Constants.H5T_INTEGER) {
	    result = true;
	}
	H5.H5Tclose(type);
	return result;
    } 

    /**
     * Check if the dataset is made of integer.
     * 
     * @param dataSetId the dataset ID to look in.
     * @return true if the dataset is of int data type
     * @throws HDF5LibraryException .
     */
    public static boolean isString(int dataSetId) throws HDF5LibraryException {
	int type = H5.H5Dget_type(dataSetId);
	
	boolean result = false;
	if (H5.H5Tget_class(type) == HDF5Constants.H5T_STRING) {
	    result = true;
	}
	H5.H5Tclose(type);
	return result;
    }
    
    /**
     * Check if the dataset is made of references.
     * 
     * @param dataSetId the dataset ID to look in.
     * @return true if the dataset is of reference data type
     * @throws HDF5LibraryException .
     */
    public static boolean isList(int dataSetId) throws HDF5LibraryException {
	int type = H5.H5Dget_type(dataSetId);
	
	boolean result = false;
	if (type == HDF5Constants.H5T_REFERENCE) {
	    result = true;
	}
	H5.H5Tclose(type);
	return result;
    } 

    /**
     * readDataFromFile
     * @param fileId the file ID where data will be read.
     * @param data a typed object to store the data.
     * @throws HDF5Exception .
     */
    public static void readDataFromFile(int fileId, ScilabDouble data) throws HDF5Exception {
	H5ReadScilabDouble.readData(H5Read.getRootId(fileId), data);
    }
    
    /**
     * readDataFromFile
     * @param fileId the file ID where data will be read.
     * @param data a typed object to store the data.
     * @throws HDF5Exception .
     */
    public static void readDataFromFile(int fileId, ScilabBoolean data) throws HDF5Exception {
	H5ReadScilabBoolean.readData(H5Read.getRootId(fileId), data);
    }
    
    /**
     * readDataFromFile
     * @param fileId the file ID where data will be read.
     * @param data a typed object to store the data.
     * @throws HDF5Exception .
     */
    public static void readDataFromFile(int fileId, ScilabString data) throws HDF5Exception {
	H5ReadScilabString.readData(H5Read.getRootId(fileId), data);
    }

    /**
     * readDataFromFile
     * @param fileId the file ID where data will be read.
     * @param data a typed object to store the data.
     * @throws HDF5Exception .
     */
    public static void readDataFromFile(int fileId, ScilabList data) throws HDF5Exception {
	H5ReadScilabList.readData(H5Read.getRootId(fileId), data);
    }
    
    /**
     * readDataFromFile
     * @param fileId the file ID where data will be read.
     * @param data a typed object to store the data.
     * @throws HDF5Exception .
     */
    public static void readDataFromFile(int fileId, ScilabTList data) throws HDF5Exception {
	H5ReadScilabTList.readData(H5Read.getRootId(fileId), data);
    }
    
    /**
     * readDataFromFile
     * @param fileId the file ID where data will be read.
     * @param data a typed object to store the data.
     * @throws HDF5Exception .
     */
    public static void readDataFromFile(int fileId, ScilabMList data) throws HDF5Exception {
    	H5ReadScilabTList.readData(H5Read.getRootId(fileId), data);
    }
    

    /**
     * getData
     * @param dataSetId the dataset ID where data will be read.
     * @return a typed object to store the data.
     * @throws HDF5Exception .
     */
    public static ScilabType getData(int dataSetId) throws HDF5Exception {
	ScilabType data = null;
	String dataType = readAttribute(dataSetId, H5ScilabConstant.SCILAB_CLASS);
	if (isScilabString(dataType)) { 
	    data = new ScilabString(); 
	    H5ReadScilabString.readData(dataSetId, (ScilabString) data);    
	}
	if (isScilabDouble(dataType)) { 
	    data = new ScilabDouble(); 
	    H5ReadScilabDouble.readData(dataSetId, (ScilabDouble) data);    
	}
	if (isScilabList(dataType)) { 
	    data = new ScilabList(); 
	    H5ReadScilabList.readData(dataSetId, (ScilabList) data);    
	}
	if (isScilabTList(dataType)) { 
	    data = new ScilabTList(); 
	    H5ReadScilabTList.readData(dataSetId, (ScilabTList) data);    
	}
	if (isScilabMList(dataType)) { 
	    data = new ScilabMList(); 
	    H5ReadScilabMList.readData(dataSetId, (ScilabMList) data);    
	}
	if (isScilabBoolean(dataType)) { 
	    data = new ScilabBoolean(); 
	    H5ReadScilabBoolean.readData(dataSetId, (ScilabBoolean) data);    
	}
	if (isScilabInteger(dataType)) { 
	    data = new ScilabInteger(); 
	    H5ReadScilabInteger.readData(dataSetId, (ScilabInteger) data);    
	}
	
	return data;
    }
    
    /**
     * @param dataType the datatype found as attribute
     * @return true if it is a string
     */
    private static boolean isScilabString(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_STRING) == 0; }
    
    /**
     * @param dataType the datatype found as attribute
     * @return true if it is a double
     */
    private static boolean isScilabDouble(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_DOUBLE) == 0; }
    
    /**
     * @param dataType the datatype found as attribute
     * @return true if it is a list
     */
    private static boolean isScilabList(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_LIST) == 0; }
    
    /**
     * @param dataType the datatype found as attribute
     * @return true if it is a mlist
     */
    private static boolean isScilabMList(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_MLIST) == 0; }
    
    /**
     * @param dataType the datatype found as attribute
     * @return true if it is a tlist
     */
    private static boolean isScilabTList(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_TLIST) == 0; }
    
    /**
     * @param dataType the datatype found as attribute
     * @return true if it is a boolean
     */
    private static boolean isScilabBoolean(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_BOOLEAN) == 0; }
    
    /**
     * @param dataType the datatype found as attribute
     * @return true if it is a integer
     */
    private static boolean isScilabInteger(String dataType) { return dataType.compareTo(H5ScilabConstant.SCILAB_CLASS_INT) == 0; }
}
