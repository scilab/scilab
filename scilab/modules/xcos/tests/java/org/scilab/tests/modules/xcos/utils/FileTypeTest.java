/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
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

package org.scilab.tests.modules.xcos.utils;

import java.awt.GraphicsEnvironment;
import java.io.File;
import java.io.IOException;
import java.rmi.server.UID;

import org.junit.Assume;
import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;

/**
 * Test the {@link XcosFileType} class.
 */
public class FileTypeTest {
    private static final String XcosFileHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
    }

    @Test
    public void checkSupportedType() {
        assert XcosFileType.values().length == 4;
        assert XcosFileType.getAvailableSaveFormats().contains(XcosFileType.ZCOS);
        assert XcosFileType.getAvailableSaveFormats().contains(XcosFileType.XCOS);
    }

    @Test
    public void checkNullField() {
        for (XcosFileType type : XcosFileType.values()) {
            assert type.getExtension() != null;
            assert type.getDescription() != null;
        }
    }

    @Test
    public void checkExtension() {
        for (XcosFileType type : XcosFileType.values()) {
            assert type.getDottedExtension().compareTo("." + type.getExtension()) == 0;
            assert type.getFileMask().compareTo("*" + type.getDottedExtension()) == 0;
        }
    }

    @Test
    public void validateFindFileType() throws IOException {
        for (XcosFileType type : XcosFileType.values()) {
            File tmp = File.createTempFile("xcosTest", type.getDottedExtension());

            if (type != XcosFileType.XCOS) {
                assert type == XcosFileType.findFileType(tmp.getAbsolutePath());
            } else {
                assert XcosFileType.findFileType(tmp.getAbsolutePath()) == null;
            }

            tmp.delete();
        }
    }

    @Test
    public void validateXcosFindFileType() throws Exception {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());
        File tmp = File.createTempFile("xcosTest", XcosFileType.XCOS.getDottedExtension());

        JavaController controller = new JavaController();
        XcosFileType.XCOS.save(tmp.getCanonicalPath(), new XcosDiagram(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM, new UID().toString()));

        assert XcosFileType.XCOS == XcosFileType.findFileType(tmp);

        tmp.delete();
    }

    @Test
    public void validateZcosFindFileType() throws Exception {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());
        File tmp = File.createTempFile("xcosTest", XcosFileType.ZCOS.getDottedExtension());

        JavaController controller = new JavaController();
        XcosFileType.ZCOS.save(tmp.getCanonicalPath(), new XcosDiagram(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM, new UID().toString()));

        assert XcosFileType.ZCOS == XcosFileType.findFileType(tmp);

        tmp.delete();
    }
}
