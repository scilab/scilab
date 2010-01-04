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
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.write.H5Write;


public class testScilabDouble extends TestCase {

    public final static int ROWS = 123;
    public final static int COLS = 456;

    public void testEmptyMatrix() throws NullPointerException, HDF5Exception {
	ScilabDouble scilabEmptyDouble = new ScilabDouble();

	int fileId = H5Write.createFile("/tmp/emptyDoubleFromJava.h5");
	H5Write.writeInDataSet(fileId, "EmptyDouble", scilabEmptyDouble);
	H5Write.closeFile(fileId);

	ScilabDouble data = new ScilabDouble();
	fileId = H5Read.openFile("/tmp/emptyDoubleFromJava.h5");
	Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_DOUBLE);
	H5Read.readDataFromFile(fileId, data);
	Assert.assertEquals(data.isEmpty(), true);
    }

    public void testRealMatrix() throws NullPointerException, HDF5Exception {
	double [][]realPart = new double[ROWS][COLS];

	for (int i = 0 ; i < ROWS ; ++i) {
	    for (int j = 0 ; j < COLS ; ++j) {
		realPart[i][j] = Math.random();
	    }
	}

	ScilabDouble scilabRealDouble = new ScilabDouble(realPart);

	int fileId = H5Write.createFile("/tmp/realDoubleFromJava.h5");
	H5Write.writeInDataSet(fileId, "RealDouble", scilabRealDouble);
	H5Write.closeFile(fileId);

	ScilabDouble data = new ScilabDouble();

	fileId = H5Read.openFile("/tmp/realDoubleFromJava.h5");
	Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_DOUBLE);
	H5Read.readDataFromFile(fileId, data);
	Assert.assertEquals(true, data.isReal());
	for (int i = 0 ; i < ROWS ; ++i) {
	    for (int j = 0 ; j < COLS ; ++j) {
		Assert.assertEquals(realPart[i][j],data.getRealPart()[i][j]);
	    }
	}
    }

    public void testComplexMatrix() throws NullPointerException, HDF5Exception {
	double [][]realPart = new double[ROWS][COLS];
	double [][]imagPart = new double[ROWS][COLS];

	for (int i = 0 ; i < ROWS ; ++i) {
	    for (int j = 0 ; j < COLS ; ++j) {
		realPart[i][j] = Math.random();
		imagPart[i][j] = Math.random();
	    }
	}

	ScilabDouble scilabComplexDouble = new ScilabDouble(realPart, imagPart);

	int fileId = H5Write.createFile("/tmp/complexDoubleFromJava.h5");
	H5Write.writeInDataSet(fileId, "ComplexDouble", scilabComplexDouble);
	H5Write.closeFile(fileId);

	ScilabDouble data = new ScilabDouble();

	fileId = H5Read.openFile("/tmp/complexDoubleFromJava.h5");
	Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_DOUBLE);
	H5Read.readDataFromFile(fileId, data);
	for (int i = 0 ; i < ROWS ; ++i) {
	    for (int j = 0 ; j < COLS ; ++j) {
		Assert.assertEquals(realPart[i][j], data.getRealPart()[i][j]);
		Assert.assertEquals(imagPart[i][j], data.getImaginaryPart()[i][j]);
	    }
	}
    }

}
