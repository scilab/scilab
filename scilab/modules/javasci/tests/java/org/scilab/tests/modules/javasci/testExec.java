/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.tests.modules.javasci;

import org.testng.annotations.*;
import java.io.File;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;

public class testExec {
    private Scilab sci;

    /* 
     * This method will be called for each test.
     * with @AfterMethod, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */ 
    @BeforeMethod
    public void open() throws NullPointerException, JavasciException {
        sci = new Scilab();
        assert sci.open() == true;
    }

    @Test(sequential = true)
    public void execAndReadTest() throws NullPointerException, JavasciException {

        /* Scalar test */
        assert sci.exec("a = 1+1") == true;
        ScilabType a = sci.get("a");
        double[][] aReal = ((ScilabDouble)a).getRealPart();

        assert a.getHeight() == 1;
        assert a.getWidth() == 1;

        assert ((ScilabDouble)a).getRealPart()[0][0] == 2.0;

        /* Matrix 10x10 */
        assert sci.exec("b = matrix(1:100,10,10)") == true;

        ScilabType b = sci.get("b");

        assert b.getHeight() == 10;
        assert b.getWidth() == 10;

        /* Check results of the addition of two matrixes */
        assert sci.exec("c = [42, 12; 32, 32] + [2, 1; 3, 2]; sumMatrix = sum(c);") == true;
        ScilabType c = sci.get("c");

        assert c.getHeight() == 2;

        assert c.getWidth() == 2;

        double sum = 0;
        /* Compute ourself the sum of all matrices elements */
        for (int i=0; i < c.getHeight(); i++) {
            for (int j=0; j < c.getWidth(); j++) {
                sum += ((ScilabDouble)c).getRealPart()[i][j];
            }
        }
        ScilabType sumMatrix = sci.get("sumMatrix");
        /* Compare if they match */
        assert ((ScilabDouble)sumMatrix).getRealPart()[0][0] == sum;
        sci.exec("b = matrix(1:100,10,10)") ;
        ScilabType b2 = sci.get("b");
        b2.getHeight(); // 10 
        b2.getWidth(); // 10
        ScilabDouble b3 = (ScilabDouble)sci.get("b");
        assert b3.equals(b2);
    }


    @Test(sequential = true)
    public void execFromFileTest() throws NullPointerException, JavasciException {
        sci.close();

        try {
            // Create temp file.
            File tempScript = File.createTempFile("tempScript", ".sci");
            
            // Write to temp file
            BufferedWriter out = new BufferedWriter(new FileWriter(tempScript));
            out.write("a=4+42;");
            out.close();
            
            assert sci.open(tempScript) == true;

            ScilabType a = sci.get("a");
            double[][] aReal = ((ScilabDouble)a).getRealPart();

            assert ((ScilabDouble)a).getRealPart()[0][0] == 46.0;
            tempScript.delete();

        } catch (IOException e) {
        }
    }

    @Test(sequential = true, expectedExceptions = FileNotFoundException.class)
    public void execFromNonExistingFileTest() throws NullPointerException, InitializationException, FileNotFoundException, JavasciException {
        sci.close();

        File nonExistingFile = new File("/wrong/path/file");

        sci.open(nonExistingFile);
    }

    @Test(sequential = true)
    public void execExecstrTest() throws NullPointerException, InitializationException, FileNotFoundException, JavasciException {
        sci.exec("execstr('toto = 111')");

        ScilabType a = sci.get("toto");
        double[][] aReal = ((ScilabDouble)a).getRealPart();
        
        assert ((ScilabDouble)a).getRealPart()[0][0] == 111.0;
    }

    /**
     * See #open()
     */
    @AfterMethod
    public void close() {
        sci.close();
        
    }
}