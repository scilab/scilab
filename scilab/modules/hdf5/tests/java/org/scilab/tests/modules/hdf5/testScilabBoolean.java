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
import org.testng.Assert;
import org.testng.annotations.*;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.hdf5.write.H5Write;

import java.io.File;

public class testScilabBoolean {

    public final static boolean myBoolean = true;
    private final static String tempDir = System.getProperty("java.io.tmpdir");

    @Test  
    public void testSingleBoolean() throws NullPointerException, HDF5Exception {
        ScilabBoolean scilabSingleboolean = new ScilabBoolean(myBoolean);
        String fileName = tempDir+"/singleBooleanFromJava.h5";

        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "SingleBoolean", scilabSingleboolean);
        H5Write.closeFile(fileId);

        ScilabBoolean data = new ScilabBoolean();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_BOOLEAN);
        H5Read.readDataFromFile(fileId, data);
        Assert.assertEquals(data.getData().length, 1);
        Assert.assertEquals(data.getData()[0].length, 1);
        Assert.assertEquals(data.getData()[0][0], myBoolean);
        new File(fileName).delete();
    }

    @Test
    public void testBooleanMatrix() throws NullPointerException, HDF5Exception {
        boolean[][] dataBooleanMatix = {
            {true, true},
            {false, true}, 
            {false, false}
        };
        String fileName = tempDir+"/matrixbooleanFromJava.h5";
        int ROWS = dataBooleanMatix.length;
        int COLS = dataBooleanMatix[0].length;

        ScilabBoolean scilabMatrixboolean = new ScilabBoolean(dataBooleanMatix);

        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "Matrixboolean", scilabMatrixboolean);
        H5Write.closeFile(fileId);

        ScilabBoolean data = new ScilabBoolean();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_BOOLEAN);
        H5Read.readDataFromFile(fileId, data);
        Assert.assertEquals(data.getData().length, ROWS);
        Assert.assertEquals(data.getData()[0].length, COLS);
        for (int i = 0 ; i < ROWS ; ++i) {
            for (int j = 0 ; j < COLS ; ++j) {
                Assert.assertEquals(data.getData()[i][j], dataBooleanMatix[i][j]);
            }
        }
        new File(fileName).delete();
    }

}
