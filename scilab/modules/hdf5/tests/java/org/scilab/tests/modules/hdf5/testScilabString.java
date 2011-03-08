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

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import org.testng.Assert;
import org.testng.annotations.*;


import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.types.ScilabString;
import org.testng.Assert;
import org.testng.annotations.Test;

import java.io.File;

public class testScilabString {

    public final static String myString = "myString";
    private final static String tempDir = System.getProperty("java.io.tmpdir");

    @Test
    public void emptyStringTest() throws NullPointerException, HDF5Exception {
        ScilabString emptyString = new ScilabString("");
        String fileName = tempDir + "/emptyStringFromJava.h5";
    
        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "EmptyString", emptyString);
        H5Write.closeFile(fileId);
        new File(fileName).delete();
    }

    @Test    
    public void testSingleString() throws NullPointerException, HDF5Exception {
        ScilabString scilabSingleString = new ScilabString(myString);
        String fileName = tempDir + "/singleStringFromJava.h5";
        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "SingleString", scilabSingleString);
        H5Write.closeFile(fileId);

        ScilabString data = new ScilabString();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_STRING);
        H5Read.readDataFromFile(fileId, data);
        Assert.assertEquals(data.getData().length, 1);
        Assert.assertEquals(data.getData()[0].length, 1);
        Assert.assertEquals(data.getData()[0][0], myString);
        new File(fileName).delete();
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
        String fileName = tempDir + "/matrixStringFromJava.h5";
        ScilabString scilabMatrixString = new ScilabString(dataStringMatix);

        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "MatrixString", scilabMatrixString);
        H5Write.closeFile(fileId);

        ScilabString data = new ScilabString();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_STRING);
        H5Read.readDataFromFile(fileId, data);
        Assert.assertEquals(data.getData().length, ROWS);
        Assert.assertEquals(data.getData()[0].length, COLS);
        for (int i = 0 ; i < ROWS ; ++i) {
            for (int j = 0 ; j < COLS ; ++j) {
                Assert.assertEquals(data.getData()[i][j], dataStringMatix[i][j]);
            }
        }
        new File(fileName).delete();
    }
     
    //@Test(dependsOnMethods={"testStringMatrix"})
    // Commented because of bug #8863
    public void testMultiByteString() throws NullPointerException, HDF5Exception {
        String[][] dataStringMatix = {
                {"éàᐛ@(1,1)", "éàᐛ@(1,1)"},
                {"สวัสดี", "ァクゾ"}, 
                {"Բարեւ Ձեզ", "העלא"}
            };
        
        int ROWS = dataStringMatix.length;
        int COLS = dataStringMatix[0].length;
        String fileName=tempDir + "/matrixMultiByteStringFromJava.h5";
        ScilabString scilabMatrixString = new ScilabString(dataStringMatix);

        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "MatrixString", scilabMatrixString);
        H5Write.closeFile(fileId);

        ScilabString data = new ScilabString();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_STRING);
        H5Read.readDataFromFile(fileId, data);
        Assert.assertEquals(data.getData().length, ROWS);
        Assert.assertEquals(data.getData()[0].length, COLS);
        for (int i = 0 ; i < ROWS ; ++i) {
            for (int j = 0 ; j < COLS ; ++j) {
                Assert.assertEquals(data.getData()[i][j], dataStringMatix[i][j]);
            }
        }
        new File(fileName).delete();
    }

    /**
     * Call all public methods through introspection
     * @param args not used
     * @throws InvocationTargetException 
     * @throws IllegalAccessException 
     * @throws IllegalArgumentException 
     */
    public static void main(String[] args) throws Exception {
        final Class< ? > myClass = Class.forName(new Throwable().getStackTrace()[0].getClassName());
        
        Object obj = myClass.newInstance();
        java.lang.reflect.Method[] tests = myClass.getDeclaredMethods();
        for (java.lang.reflect.Method method : tests) {
            if (method.getAnnotation(Test.class) != null) {
                method.invoke(obj, (Object[]) null);
            }
        }
    }
}
