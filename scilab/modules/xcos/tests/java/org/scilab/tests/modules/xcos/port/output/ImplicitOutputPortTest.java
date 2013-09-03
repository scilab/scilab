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

package org.scilab.tests.modules.xcos.port.output;

import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.BasicPort.Type;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.junit.*;

/**
 * Unit test for {@link ImplicitOutputPort}
 */
public class ImplicitOutputPortTest {
    public static final int DEFAULT_PORTSIZE = 8;

    @Test
    public void checkType() {
        ImplicitOutputPort port = new ImplicitOutputPort();
        assert port.getType() == Type.IMPLICIT;
    }

    @Test
    public void checkDefaultOrientation() {
        ImplicitOutputPort port = new ImplicitOutputPort();
        assert port.getOrientation() == Orientation.EAST;
    }

    @Test
    public void checkStyle() {
        ImplicitOutputPort port = new ImplicitOutputPort();
        assert port.getStyle().contains("ImplicitOutputPort");
    }

    @Test
    public void checkTypeName() {
        ImplicitOutputPort port = new ImplicitOutputPort();
        assert port.getTypeName().equals("ImplicitOutputPort");
    }

    @Test
    public void checkClassHierarchy() {
        ImplicitOutputPort port = new ImplicitOutputPort();
        assert port instanceof OutputPort;
        assert port instanceof BasicPort;
    }

    @Test
    public void checkDefaultGeometry() {
        ImplicitOutputPort port = new ImplicitOutputPort();
        assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
        assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
    }
}
