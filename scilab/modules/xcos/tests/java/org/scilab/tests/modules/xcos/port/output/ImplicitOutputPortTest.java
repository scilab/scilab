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
 *
 */

package org.scilab.tests.modules.xcos.port.output;

import java.rmi.server.UID;
import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Type;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

/**
 * Unit test for {@link ImplicitOutputPort}
 */
public class ImplicitOutputPortTest {
    public static final int DEFAULT_PORTSIZE = 8;
    private JavaController controller;

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
        controller = new JavaController();
    }

    @Test
    public void checkType() {
        ImplicitOutputPort port = new ImplicitOutputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getType() == Type.IMPLICIT;
    }

    @Test
    public void checkDefaultOrientation() {
        ImplicitOutputPort port = new ImplicitOutputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getOrientation() == Orientation.EAST;
    }

    @Test
    public void checkStyle() {
        ImplicitOutputPort port = new ImplicitOutputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getStyle().contains("ImplicitOutputPort");
    }

    @Test
    public void checkClassHierarchy() {
        ImplicitOutputPort port = new ImplicitOutputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port instanceof OutputPort;
        assert port instanceof BasicPort;
    }

    @Test
    public void checkDefaultGeometry() {
        ImplicitOutputPort port = new ImplicitOutputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
        assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
    }
}
