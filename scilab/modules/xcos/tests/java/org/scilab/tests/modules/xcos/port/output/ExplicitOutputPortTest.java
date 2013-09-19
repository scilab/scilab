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
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.junit.*;

/**
 * Unit test for {@link ExplicitOutputPort}
 */
public class ExplicitOutputPortTest {
    public static final int DEFAULT_PORTSIZE = 8;

    @Test
    public void checkType() {
        ExplicitOutputPort port = new ExplicitOutputPort();
        assert port.getType() == Type.EXPLICIT;
    }

    @Test
    public void checkDefaultOrientation() {
        ExplicitOutputPort port = new ExplicitOutputPort();
        assert port.getOrientation() == Orientation.EAST;
    }

    @Test
    public void checkStyle() {
        ExplicitOutputPort port = new ExplicitOutputPort();
        assert port.getStyle().contains("ExplicitOutputPort");
    }

    @Test
    public void checkTypeName() {
        ExplicitOutputPort port = new ExplicitOutputPort();
        assert port.getTypeName().equals("ExplicitOutputPort");
    }

    @Test
    public void checkClassHierarchy() {
        ExplicitOutputPort port = new ExplicitOutputPort();
        assert port instanceof OutputPort;
        assert port instanceof BasicPort;
    }

    @Test
    public void checkDefaultGeometry() {
        ExplicitOutputPort port = new ExplicitOutputPort();
        assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
        assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
    }
}
