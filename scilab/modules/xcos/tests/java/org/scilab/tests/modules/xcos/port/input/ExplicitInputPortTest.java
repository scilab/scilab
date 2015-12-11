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

package org.scilab.tests.modules.xcos.port.input;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Type;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;

/**
 * Unit test for {@link ExplicitInputPort}
 */
public class ExplicitInputPortTest {
    public static final int DEFAULT_PORTSIZE = 8;
    private JavaController controller;

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
        controller = new JavaController();
    }

    @Test
    public void checkType() {
        ExplicitInputPort port = new ExplicitInputPort(controller.createObject(Kind.PORT));
        assert port.getType() == Type.EXPLICIT;
    }

    @Test
    public void checkDefaultOrientation() {
        ExplicitInputPort port = new ExplicitInputPort(controller.createObject(Kind.PORT));
        assert port.getOrientation() == Orientation.WEST;
    }

    @Test
    public void checkStyle() {
        ExplicitInputPort port = new ExplicitInputPort(controller.createObject(Kind.PORT));
        assert port.getStyle().contains("ExplicitInputPort");
    }

    @Test
    public void checkClassHierarchy() {
        ExplicitInputPort port = new ExplicitInputPort(controller.createObject(Kind.PORT));
        assert port instanceof InputPort;
        assert port instanceof BasicPort;
    }

    @Test
    public void checkDefaultGeometry() {
        ExplicitInputPort port = new ExplicitInputPort(controller.createObject(Kind.PORT));
        assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
        assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
    }
}
