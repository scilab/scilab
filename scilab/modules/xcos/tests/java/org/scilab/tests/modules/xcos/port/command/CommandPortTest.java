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

package org.scilab.tests.modules.xcos.port.command;

import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.junit.*;

/**
 * Unit test for {@link CommandPort}
 */
public class CommandPortTest {
    public static final int DEFAULT_PORTSIZE = 8;

    @Test
    public void checkType() {
        CommandPort port = new CommandPort();
        assert port.getType() == null;
    }

    @Test
    public void checkDefaultOrientation() {
        CommandPort port = new CommandPort();
        assert port.getOrientation() == Orientation.SOUTH;
    }

    @Test
    public void checkStyle() {
        CommandPort port = new CommandPort();
        assert port.getStyle().contains("CommandPort");
    }

    @Test
    public void checkTypeName() {
        CommandPort port = new CommandPort();
        assert port.getTypeName().equals("CommandPort");
    }

    @Test
    public void checkClassHierarchy() {
        CommandPort port = new CommandPort();
        assert port instanceof BasicPort;
    }

    @Test
    public void checkDefaultGeometry() {
        CommandPort port = new CommandPort();
        assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
        assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
    }
}
