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

import java.util.Arrays;
import java.util.Map;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabBooleanSparse;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabPolynomial;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabSparse;
import org.scilab.modules.types.ScilabTypeEnum;

public class testReadWriteBuf {
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
    public void putAndGetRefDoubleTest() throws NullPointerException, JavasciException {
        double [][]a = {{21.2, 22.0, 42.0, 39.0}, {23.2, 24.0, 44.0, 40.0}};
        ScilabDouble aOriginal = new ScilabDouble(a);
        sci.put("a", aOriginal);

        ScilabDouble aFromScilab = (ScilabDouble)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=12345;"));
        assertTrue(aFromScilab.getRealElement(1, 2) == 12345);
        aFromScilab.setRealElement(1, 2, 3.14159);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabDouble bFromScilab = (ScilabDouble)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabDouble(3.14159)));
    }

    @Test()
    public void putAndGetRefComplexDoubleTest() throws NullPointerException, JavasciException {
        double [][]a = {{21.2, 22.0, 42.0, 39.0}, {23.2, 24.0, 44.0, 40.0}};
        double [][]aImg = {{212.2, 221.0, 423.0, 393.0}, {234.2, 244.0, 441.0, 407.0}};
        ScilabDouble aOriginal = new ScilabDouble(a, aImg);
        sci.put("a", aOriginal);

        ScilabDouble aFromScilab = (ScilabDouble)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=12345+%i*5.4321;"));
        assertTrue(aFromScilab.getRealElement(1, 2) == 12345 && aFromScilab.getImaginaryElement(1, 2) == 5.4321);
        aFromScilab.setRealElement(1, 2, 3.14159);
        aFromScilab.setImaginaryElement(1, 2, 2.71828);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabDouble bFromScilab = (ScilabDouble)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabDouble(3.14159, 2.71828)));
    }

    @Test()
    public void putAndGetRefInt8Test() throws NullPointerException, JavasciException {
        byte[][] a = {{1, 2, 3, 4}, {5, 6, 7, 8}};
        ScilabInteger aOriginal = new ScilabInteger(a, false);
        sci.put("a", aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=123;"));
        assertTrue(aFromScilab.getElement(1, 2) == 123);
        aFromScilab.setElement(1, 2, (byte) - 98);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabInteger bFromScilab = (ScilabInteger)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabInteger((byte) - 98, false)));
    }

    @Test()
    public void putAndGetRefUInt8Test() throws NullPointerException, JavasciException {
        byte[][] a = {{1, 2, 3, 4}, {5, 6, 7, 8}};
        ScilabInteger aOriginal = new ScilabInteger(a, true);
        sci.put("a", aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=253;"));
        assertTrue(aFromScilab.getElement(1, 2) == (byte)253);
        aFromScilab.setElement(1, 2, (byte)189);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabInteger bFromScilab = (ScilabInteger)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabInteger((byte)189, true)));
    }

    @Test()
    public void putAndGetRefInt16Test() throws NullPointerException, JavasciException {
        short[][] a = {{1, 2, 3, 4}, {5, 6, 7, 8}};
        ScilabInteger aOriginal = new ScilabInteger(a, false);
        sci.put("a", aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=123;"));
        assertTrue(aFromScilab.getElement(1, 2) == 123);
        aFromScilab.setElement(1, 2, (short) - 98);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabInteger bFromScilab = (ScilabInteger)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabInteger((short) - 98, false)));
    }

    @Test()
    public void putAndGetRefUInt16Test() throws NullPointerException, JavasciException {
        short[][] a = {{1, 2, 3, 4}, {5, 6, 7, 8}};
        ScilabInteger aOriginal = new ScilabInteger(a, true);
        sci.put("a", aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=253;"));
        assertTrue(aFromScilab.getElement(1, 2) == (short)253);
        aFromScilab.setElement(1, 2, (short)189);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabInteger bFromScilab = (ScilabInteger)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabInteger((short)189, true)));
    }

    @Test()
    public void putAndGetRefInt32Test() throws NullPointerException, JavasciException {
        int[][] a = {{1, 2, 3, 4}, {5, 6, 7, 8}};
        ScilabInteger aOriginal = new ScilabInteger(a, false);
        sci.put("a", aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=123;"));
        assertTrue(aFromScilab.getElement(1, 2) == 123);
        aFromScilab.setElement(1, 2, (int) - 98);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabInteger bFromScilab = (ScilabInteger)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabInteger((int) - 98, false)));
    }

    @Test()
    public void putAndGetRefUInt32Test() throws NullPointerException, JavasciException {
        int[][] a = {{1, 2, 3, 4}, {5, 6, 7, 8}};
        ScilabInteger aOriginal = new ScilabInteger(a, true);
        sci.put("a", aOriginal);

        ScilabInteger aFromScilab = (ScilabInteger)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=253;"));
        assertTrue(aFromScilab.getElement(1, 2) == (int)253);
        aFromScilab.setElement(1, 2, (int)189);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabInteger bFromScilab = (ScilabInteger)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabInteger((int)189, true)));
    }

    @Test()
    public void putAndGetRefBooleanTest() throws NullPointerException, JavasciException {
        boolean[][] a = {{true, false, true, true}, {false, false, true, true}};
        ScilabBoolean aOriginal = new ScilabBoolean(a);
        sci.put("a", aOriginal);

        ScilabBoolean aFromScilab = (ScilabBoolean)sci.getByReference("a");
        assertTrue(aFromScilab.equals(aOriginal));
        assertTrue(sci.exec("a(2,3)=%f;"));
        assertTrue(aFromScilab.getElement(1, 2) == false);
        aFromScilab.setElement(1, 2, true);
        assertTrue(sci.exec("b=a(2,3);"));
        ScilabBoolean bFromScilab = (ScilabBoolean)sci.get("b");
        assertTrue(bFromScilab.equals(new ScilabBoolean(true)));
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();
    }
}
