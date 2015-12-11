/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.tests.modules.xcos.port.control;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.control.ControlPort;

/**
 * Unit test for {@link ControlPort}
 */
public class ControlPortTest {
    public static final int DEFAULT_PORTSIZE = 8;
    private JavaController controller;

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
        controller = new JavaController();
    }

    @Test
    public void checkType() {
        ControlPort port = new ControlPort(controller.createObject(Kind.PORT));
        assert port.getType() == null;
    }

    @Test
    public void checkDefaultOrientation() {
        ControlPort port = new ControlPort(controller.createObject(Kind.PORT));
        assert port.getOrientation() == Orientation.NORTH;
    }

    @Test
    public void checkStyle() {
        ControlPort port = new ControlPort(controller.createObject(Kind.PORT));
        assert port.getStyle().contains("ControlPort");
    }

    @Test
    public void checkClassHierarchy() {
        ControlPort port = new ControlPort(controller.createObject(Kind.PORT));
        assert port instanceof BasicPort;
    }

    @Test
    public void checkDefaultGeometry() {
        ControlPort port = new ControlPort(controller.createObject(Kind.PORT));
        assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
        assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
    }
}
