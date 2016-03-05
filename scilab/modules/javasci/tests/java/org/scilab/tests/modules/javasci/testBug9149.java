/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.

 */
package org.scilab.tests.modules.javasci;

import org.junit.*;
import static org.junit.Assert.*;

import java.io.File;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.ScilabErrorException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.commons.ScilabCommons;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.SwingUtilities;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class testBug9149 {
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

    @Test( expected = ScilabErrorException.class)
    public void nonRegBug9149Working() throws NullPointerException, ScilabErrorException {
        assertEquals(sci.isGraphicOpened(), false);
        sci.execException("plot3d();");
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }

}
