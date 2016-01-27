/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
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
