package org.scilab.tests.modules.hdf5;
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
import org.testng.annotations.*;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.write.H5Write;


public class testScilabString {

    public final static String myString = "myString";
	private final static String tempDir = System.getProperty("java.io.tmpdir");

	@Test
    public void emptyStringTest() throws NullPointerException, HDF5Exception {
		ScilabString emptyString = new ScilabString("");
	
		int fileId = H5Write.createFile(tempDir + "/emptyStringFromJava.h5");
		H5Write.writeInDataSet(fileId, "EmptyString", emptyString);
		H5Write.closeFile(fileId);	
    }

	@Test    
    public void testSingleString() throws NullPointerException, HDF5Exception {
		ScilabString scilabSingleString = new ScilabString(myString);

		int fileId = H5Write.createFile(tempDir + "/singleStringFromJava.h5");
		H5Write.writeInDataSet(fileId, "SingleString", scilabSingleString);
		H5Write.closeFile(fileId);

		ScilabString data = new ScilabString();
		fileId = H5Read.openFile(tempDir + "/singleStringFromJava.h5");
		assert H5Read.getRootType(fileId).equals(H5ScilabConstant.SCILAB_CLASS_STRING);
		H5Read.readDataFromFile(fileId, data);
		assert data.getData().length == 1;
		assert data.getData()[0].length == 1;
		assert data.getData()[0][0].equals(myString);
    }

	@Test
    public void testStringMatrix() throws NullPointerException, HDF5Exception {
		String[][] dataStringMatix = {
			{"MatrixString(1,1)", "MatrixString(1,2)"},
			{"MatrixString(2,1)", "MatrixString(2,2)"}, 
			{"MatrixString(3,1)", "MatrixString(3,2)"}
		};
	
		int ROWS = dataStringMatix.length;
		int COLS = dataStringMatix[0].length;

		ScilabString scilabMatrixString = new ScilabString(dataStringMatix);

		int fileId = H5Write.createFile(tempDir + "/matrixStringFromJava.h5");
		H5Write.writeInDataSet(fileId, "MatrixString", scilabMatrixString);
		H5Write.closeFile(fileId);

		ScilabString data = new ScilabString();
		fileId = H5Read.openFile(tempDir + "/matrixStringFromJava.h5");
		assert H5Read.getRootType(fileId).equals(H5ScilabConstant.SCILAB_CLASS_STRING);
		H5Read.readDataFromFile(fileId, data);
		assert data.getData().length == ROWS;
		assert data.getData()[0].length == COLS;
		for (int i = 0 ; i < ROWS ; ++i) {
			for (int j = 0 ; j < COLS ; ++j) {
				assert data.getData()[i][j].equals(dataStringMatix[i][j]);
			}
		}

    }

}
