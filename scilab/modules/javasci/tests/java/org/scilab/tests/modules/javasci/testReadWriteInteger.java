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

import org.junit.*;
import static org.junit.Assert.*;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabInteger;

public class testReadWriteInteger {
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
    public void putAndGetInteger8UnsignedTest() throws NullPointerException, JavasciException {

        byte [][]a = {{21, 22, 42, 39}, {23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */
        sci.put("b", aOriginal);
        //        assertTrue(sci.exec("somme = sum(a);"));

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assertTrue(aFromScilab.equals(aOriginal));

        // Test values
        byte [][]z = {{ -1, -128, 0}};
        ScilabInteger zMatrix = new ScilabInteger(z, true);
        sci.put("z", zMatrix);
        sci.exec("sumElements=sum(z);");

        ScilabInteger zFromScilab = (ScilabInteger)sci.get("z");
        assertTrue(zFromScilab.equals(zMatrix));
    }

    @Test()
    public void putAndGetInteger8SignedTest() throws NullPointerException, JavasciException {
        byte [][]a = {{ -21, 22, -42, 39}, {23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b", aOriginal);
        //        assertTrue(sci.exec("somme = sum(a);"));

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        byte [][]z = {{ -1, -128, 0}};
        ScilabInteger zMatrix = new ScilabInteger(z, false);
        sci.put("z", zMatrix);
        ScilabInteger zFromScilab = (ScilabInteger)sci.get("z");
        assertTrue(zFromScilab.equals(zMatrix));

        assertTrue(aFromScilab.equals(aOriginal));

    }

    @Test()
    public void putAndGetInteger16UnsignedTest() throws NullPointerException, JavasciException {
        short [][]a = {{21, 22, 42, 39}, {23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */
        sci.put("b", aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assertTrue(aFromScilab.equals(aOriginal));

    }

    @Test()
    public void putAndGetInteger16SignedTest() throws NullPointerException, JavasciException {
        short [][]a = {{ -21, 22, -42, 39}, {23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b", aOriginal);
        //        assertTrue(sci.exec("somme = sum(a);"));

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assertTrue(aFromScilab.equals(aOriginal));

    }

    @Test()
    public void putAndGetInteger32UnsignedTest() throws NullPointerException, JavasciException {

        int [][]a = {{21, 22, 42, 39}, {23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */

        sci.put("b", aOriginal);
        //        assertTrue(sci.exec("somme = sum(a);"));

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assertTrue(aFromScilab.equals(aOriginal));

    }

    @Test()
    public void putAndGetInteger32SignedTest() throws NullPointerException, JavasciException {
        int [][]a = {{ -21, 22, -42, 39}, {23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b", aOriginal);
        //        assertTrue(sci.exec("somme = sum(a);"));

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assertTrue(aFromScilab.equals(aOriginal));

    }

    @Test( expected = UnsupportedTypeException.class)
    // Will be unblocked for Scilab 6
    public void putAndGetInteger64UnsignedTest() throws NullPointerException, JavasciException {
        long [][]a = {{21, 22, 42, 39}, {23, 24, 44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, true); /* unsigned */
        sci.put("b", aOriginal); /* Exception launched */

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assertTrue(aFromScilab.equals(aOriginal));
    }

    @Test( expected = UnsupportedTypeException.class)
    // Will be unblocked for Scilab 6
    public void putAndGetInteger64SignedTest() throws NullPointerException, JavasciException {
        long [][]a = {{ -21, 22, -42, 39}, {23, -24, -44, 40}};
        ScilabInteger aOriginal = new ScilabInteger(a, false); /* signed */
        sci.put("b", aOriginal); /* Exception launched */

        ScilabInteger aFromScilab = (ScilabInteger)sci.get("b");

        assertTrue(aFromScilab.equals(aOriginal));

    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }
}