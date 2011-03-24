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

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTypeEnum;

public class testBug6651 {
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
    public void nonRegBug6651() throws NullPointerException, JavasciException {
        double[][] a={{1,2,3},{4,5,6},{7,8,9},{10,11,12}};

        ScilabDouble A=new ScilabDouble(a);
        sci.put("a",A);
        assert sci.exec("b = a;") == true;

        ScilabDouble B=(ScilabDouble)sci.get("b");
        assert B.equals(A);

        assert sci.exec("c = a;") == true;
        ScilabDouble C=(ScilabDouble)sci.get("c");
        assert C.equals(A);
    }

    /**
     * See #open()
     */
    @AfterMethod
    public void close() {
        sci.close();
        
    }
}