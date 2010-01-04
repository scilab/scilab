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

import junit.framework.Assert;
import junit.framework.TestCase;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;
import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.write.H5Write;

public class testScilabList extends TestCase {
    public void testEmptyList() throws NullPointerException, HDF5LibraryException, HDF5Exception {
	ScilabList data = new ScilabList();
	
	int fileId = H5Write.createFile("/tmp/emptyListFromJava.h5");
	H5Write.writeInDataSet(fileId, "EmptyList", data);
	H5Write.closeFile(fileId);
	
	data = new ScilabList();
	fileId = H5Read.openFile("/tmp/emptyListFromJava.h5");
	Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_LIST);
	H5Read.readDataFromFile(fileId, data);
	Assert.assertEquals(data.isEmpty(), true);
    }
    
    public void testStringList() throws NullPointerException, HDF5LibraryException, HDF5Exception {
	ScilabList data = new ScilabList();
	data.add(new ScilabString("hello"));
	String[][] stringData = {{"i","am","a"},{"string", "matrix", "!!!"}};
	data.add(new ScilabString(stringData));
	
	int fileId = H5Write.createFile("/tmp/stringListFromJava.h5");
	H5Write.writeInDataSet(fileId, "StringList", data);
	H5Write.closeFile(fileId);
    }
    
    public void testDoubleList() throws NullPointerException, HDF5LibraryException, HDF5Exception {
	ScilabList data = new ScilabList();
	data.add(new ScilabDouble(2));
	data.add(new ScilabDouble(51));
	data.add(new ScilabString("hello"));
	String[][] stringData = {{"i","am","a"},{"string", "matrix", "!!!"}};
	data.add(new ScilabString(stringData));
	
	int fileId = H5Write.createFile("/tmp/doubleListFromJava.h5");
	H5Write.writeInDataSet(fileId, "DoubleList", data);
	H5Write.closeFile(fileId);
    }

}
