/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.tests.modules.xcos.utils;

import java.io.File;
import java.io.IOException;
import java.awt.GraphicsEnvironment;

import org.junit.Test;
import org.junit.Assume;

import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;


/**
 * Test the {@link XcosFileType} class.
 */
public class FileTypeTest {
    private static final String XcosFileHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

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
        XcosFileType.XCOS.save(tmp.getCanonicalPath(), new XcosDiagram());

        assert XcosFileType.XCOS == XcosFileType.findFileType(tmp);

        tmp.delete();
    }

    @Test
    public void validateZcosFindFileType() throws Exception {
        Assume.assumeTrue(!GraphicsEnvironment.isHeadless());
        File tmp = File.createTempFile("xcosTest", XcosFileType.ZCOS.getDottedExtension());
        XcosFileType.ZCOS.save(tmp.getCanonicalPath(), new XcosDiagram());

        assert XcosFileType.ZCOS == XcosFileType.findFileType(tmp);

        tmp.delete();
    }
}
