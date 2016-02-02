/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.tests.modules.javasci;

import org.junit.*;
import static org.junit.Assert.*;

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

public class testExecException {
    private Scilab sci;

    /*
     * This method will be called for each test.
     * with @After, this ensures that all the time the engine is closed
     * especially in case of error.
     * Otherwise, the engine might be still running and all subsequent tests
     * would fail.
     */
    @Before
    public void open() throws NullPointerException, JavasciException {
        sci = new Scilab();
        assertTrue(sci.open());
    }

    @Test()
    public void execAndReadTest() throws NullPointerException, JavasciException {
        /* Scalar test */
        sci.execException("a = 1+1");
        ScilabType a = sci.get("a");
        double[][] aReal = ((ScilabDouble)a).getRealPart();

        assertEquals(a.getHeight(), 1);
        assertEquals(a.getWidth(), 1);

        assertEquals(((ScilabDouble)a).getRealPart()[0][0], 2.0, 1e-8);

        /* Matrix 10x10 */
        sci.execException("b = matrix(1:100,10,10)");

        ScilabType b = sci.get("b");

        assertEquals(b.getHeight(), 10);
        assertEquals(b.getWidth(), 10);

        /* Check results of the addition of two matrices */
        sci.execException("c = [42, 12; 32, 32] + [2, 1; 3, 2]; sumMatrix = sum(c);");
        ScilabType c = sci.get("c");

        assertEquals(c.getHeight(), 2);

        assertEquals(c.getWidth(), 2);

        double sum = 0;
        /* Compute ourself the sum of all matrices elements */
        for (int i = 0; i < c.getHeight(); i++) {
            for (int j = 0; j < c.getWidth(); j++) {
                sum += ((ScilabDouble)c).getRealPart()[i][j];
            }
        }
        ScilabType sumMatrix = sci.get("sumMatrix");
        /* Compare if they match */
        assertEquals(((ScilabDouble)sumMatrix).getRealPart()[0][0], sum, 1e-8);
        sci.execException("b = matrix(1:100,10,10)") ;
        ScilabType b2 = sci.get("b");
        b2.getHeight(); // 10
        b2.getWidth(); // 10
        ScilabDouble b3 = (ScilabDouble)sci.get("b");
        assertTrue(b3.equals(b2));
    }


    @Test()
    public void execFromFileTest() throws NullPointerException, JavasciException {
        sci.close();

        try {
            // Create temp file.
            File tempScript = File.createTempFile("tempScript", ".sci");

            // Write to temp file
            BufferedWriter out = new BufferedWriter(new FileWriter(tempScript));
            out.write("a=4+42;");
            out.close();

            assertTrue(sci.open(tempScript));

            ScilabType a = sci.get("a");
            double[][] aReal = ((ScilabDouble)a).getRealPart();

            assertEquals(((ScilabDouble)a).getRealPart()[0][0], 46.0, 1e-8);
            tempScript.delete();

        } catch (IOException e) {
        }
    }

    @Test( expected = FileNotFoundException.class)
    public void execFromNonExistingFileTest() throws NullPointerException, InitializationException, FileNotFoundException, JavasciException {
        sci.close();

        File nonExistingFile = new File("/wrong/path/file");

        sci.open(nonExistingFile);
    }

    @Test()
    public void execExecstrTest() throws NullPointerException, InitializationException, FileNotFoundException, JavasciException {
        sci.execException("execstr('toto = 111')");

        ScilabType a = sci.get("toto");
        double[][] aReal = ((ScilabDouble)a).getRealPart();

        assertEquals(((ScilabDouble)a).getRealPart()[0][0], 111.0, 1e-8);
    }

    @Test()
    public void execExecFileTest() throws NullPointerException, InitializationException, FileNotFoundException, JavasciException {

        try {
            // Create temp file.
            File tempScript = File.createTempFile("tempScript", ".sci");

            // Write to temp file
            BufferedWriter out = new BufferedWriter(new FileWriter(tempScript));
            out.write("a=4+42;");
            out.close();

            sci.execException(tempScript);

            ScilabType a = sci.get("a");
            double[][] aReal = ((ScilabDouble)a).getRealPart();

            assertEquals(aReal[0][0], 46, 1e-8);
        } catch (IOException e) {
        }

    }

    @Test( expected = FileNotFoundException.class)
    public void execExecNonFileTest() throws NullPointerException, InitializationException, FileNotFoundException, JavasciException {

        sci.execException(new File("Doesnotexist"));



    }
    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }
}