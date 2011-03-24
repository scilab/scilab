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
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;
import ncsa.hdf.hdf5lib.exceptions.HDF5LibraryException;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.testng.Assert;
import org.testng.annotations.Test;

import java.io.File;

public class testScilabList {
    private final static String tempDir = System.getProperty("java.io.tmpdir");

    @Test
    public void testEmptyList() throws NullPointerException, HDF5LibraryException, HDF5Exception {
        ScilabList data = new ScilabList();
        String fileName=tempDir + "/emptyListFromJava.h5";

        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "EmptyList", data);
        H5Write.closeFile(fileId);
    
        data = new ScilabList();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_LIST);
        H5Read.readDataFromFile(fileId, data);
        Assert.assertEquals(data.isEmpty(), true);
        new File(fileName).delete();
    }
    
    @Test
    public void testStringList() throws NullPointerException, HDF5LibraryException, HDF5Exception {
        ScilabList dataList = new ScilabList();
        dataList.add(new ScilabString("hello"));
        String[][] stringData = {{"i","am","a"},{"string", "matrix", "!!!"}};
        String fileName = tempDir + "/stringListFromJava.h5";

        dataList.add(new ScilabString(stringData));
        
        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "StringList", dataList);
        H5Write.closeFile(fileId);
        
        ScilabList data = new ScilabList();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_LIST);
        H5Read.readDataFromFile(fileId, data);
        
        Assert.assertEquals(data.getHeight(), dataList.getHeight());
        Assert.assertEquals(data.getWidth(), dataList.getWidth());
        
        Assert.assertEquals(data, dataList); // deep equals
        new File(fileName).delete();
    }
    
    @Test
    public void testDoubleList() throws NullPointerException, HDF5LibraryException, HDF5Exception {
        ScilabList dataList = new ScilabList();
        String fileName = tempDir + "/doubleListFromJava.h5";
        dataList.add(new ScilabDouble(2));
        dataList.add(new ScilabDouble(51));
    
        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "DoubleList", dataList);
        H5Write.closeFile(fileId);
        
        ScilabList data = new ScilabList();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_LIST);
        H5Read.readDataFromFile(fileId, data);
        
        Assert.assertEquals(data.getHeight(), dataList.getHeight());
        Assert.assertEquals(data.getWidth(), dataList.getWidth());
        
        Assert.assertEquals(data, dataList); // deep equals
        new File(fileName).delete();
    }

    @Test
    public void testMixedList() throws HDF5Exception {
        String fileName=tempDir + "/mixedListFromJava.h5";
        ScilabList dataList = new ScilabList();
        dataList.add(new ScilabDouble(2));
        dataList.add(new ScilabDouble(51));
        String[][] stringData = {{"i","am","a"},{"string", "matrix", "!!!"}};
        dataList.add(new ScilabString(stringData));
    
        int fileId = H5Write.createFile(fileName);
        H5Write.writeInDataSet(fileId, "MixedList", dataList);
        H5Write.closeFile(fileId);
        
        ScilabList data = new ScilabList();
        fileId = H5Read.openFile(fileName);
        Assert.assertEquals(H5Read.getRootType(fileId), H5ScilabConstant.SCILAB_CLASS_LIST);
        H5Read.readDataFromFile(fileId, data);
        
        Assert.assertEquals(data.getHeight(), dataList.getHeight());
        Assert.assertEquals(data.getWidth(), dataList.getWidth());
        
        Assert.assertEquals(data, dataList); // deep equals
        new File(fileName).delete();
    }
    
    /**
     * Call all public methods through introspection
     * @param args not used
     * @throws Exception on error
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
