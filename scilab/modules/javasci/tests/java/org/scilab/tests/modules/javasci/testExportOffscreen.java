/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Sylvestre LEDRU
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

import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.types.ScilabTypeEnum;

public class testExportOffscreen {
    private Scilab sci;
    private String tmpDir = System.getProperty("java.io.tmpdir");

    @Before
    public void open() throws NullPointerException, JavasciException {
        sci = new Scilab(true);
        assertTrue(sci.open());
    }

    private String generateExportCommand(String type, File filename) {
        String exportToFile = "driver('" + type + "'); xinit('" + filename.getAbsolutePath() + "'); plot3d(); xend()";
        return exportToFile;
    }

    @Test()
    public void exportToPSTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.ps");
        String exportToFile = generateExportCommand("ps", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    @Test()
    public void exportToPOSTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.ps");
        String exportToFile = generateExportCommand("Pos", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    @Test()
    public void exportToGIFTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.gif");
        String exportToFile = generateExportCommand("GIF", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    @Test()
    public void exportToPPMTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.ppm");
        String exportToFile = generateExportCommand("PPM", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    @Test()
    public void exportToJPGTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.jpg");
        String exportToFile = generateExportCommand("JPG", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    @Test()
    public void exportToPDFTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.pdf");
        String exportToFile = generateExportCommand("PDF", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    @Test()
    public void exportToSVGTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.svg");
        String exportToFile = generateExportCommand("SVG", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    @Test()
    public void exportToPNGTest() throws NullPointerException, JavasciException {
        File export = new File(tmpDir + "/foo.svg");
        String exportToFile = generateExportCommand("PNG", export);
        sci.exec(exportToFile);
        assertTrue(export.exists());
        assertTrue(export.length() > 0);
        assertTrue(export.delete());
    }

    /**
     * See #open()
     */
    @After
    public void close() {
        sci.close();

    }

}