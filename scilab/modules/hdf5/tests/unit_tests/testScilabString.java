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

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.write.H5Write;


public class testScilabString extends TestCase {

    public final static String myString = "myString";

    public void testEmptyString() throws NullPointerException, HDF5Exception {
	ScilabString emptyString = new ScilabString("");
	
	int fileId = H5Write.createFile("/tmp/emptyStringFromJava.h5");
	H5Write.writeInDataSet(fileId, "EmptyString", emptyString);
	H5Write.closeFile(fileId);
	
    }
    
    public void testSingleString() throws NullPointerException, HDF5Exception {
	ScilabString scilabSingleString = new ScilabString(myString);

	int fileId = H5Write.createFile("/tmp/singleStringFromJava.h5");
	H5Write.writeInDataSet(fileId, "SingleString", scilabSingleString);
	H5Write.closeFile(fileId);

	ScilabString data = new ScilabString();
	fileId = H5Read.openFile("/tmp/singleStringFromJava.h5");
	Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_STRING);
	H5Read.readDataFromFile(fileId, data);
	Assert.assertEquals(data.getData().length, 1);
	Assert.assertEquals(data.getData()[0].length, 1);
	Assert.assertEquals(data.getData()[0][0], myString);
    }

    public void testStringMatrix() throws NullPointerException, HDF5Exception {
	String[][] dataStringMatix = {
		{"MatrixString(1,1)", "MatrixString(1,2)"},
		{"MatrixString(2,1)", "MatrixString(2,2)"}, 
		{"MatrixString(3,1)", "MatrixString(3,2)"}
	};
	
	int ROWS = dataStringMatix.length;
	int COLS = dataStringMatix[0].length;

	ScilabString scilabMatrixString = new ScilabString(dataStringMatix);

	int fileId = H5Write.createFile("/tmp/matrixStringFromJava.h5");
	H5Write.writeInDataSet(fileId, "MatrixString", scilabMatrixString);
	H5Write.closeFile(fileId);

	ScilabString data = new ScilabString();
	fileId = H5Read.openFile("/tmp/matrixStringFromJava.h5");
	Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_STRING);
	H5Read.readDataFromFile(fileId, data);
	Assert.assertEquals(data.getData().length, ROWS);
	Assert.assertEquals(data.getData()[0].length, COLS);
	for (int i = 0 ; i < ROWS ; ++i) {
	    for (int j = 0 ; j < COLS ; ++j) {
		Assert.assertEquals(data.getData()[i][j], dataStringMatix[i][j]);
	    }
	}

    }

}
