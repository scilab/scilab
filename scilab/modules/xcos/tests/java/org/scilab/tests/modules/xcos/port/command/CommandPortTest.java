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

package org.scilab.tests.modules.xcos.port.command;

import java.rmi.server.UID;
import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;

/**
 * Unit test for {@link CommandPort}
 */
public class CommandPortTest {
    public static final int DEFAULT_PORTSIZE = 8;
    private JavaController controller;

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
        controller = new JavaController();
    }

    @Test
    public void checkType() {
        CommandPort port = new CommandPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getType() == null;
    }

    @Test
    public void checkDefaultOrientation() {
        CommandPort port = new CommandPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getOrientation() == Orientation.SOUTH;
    }

    @Test
    public void checkStyle() {
        CommandPort port = new CommandPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getStyle().contains(null);
    }

    @Test
    public void checkClassHierarchy() {
        CommandPort port = new CommandPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port instanceof BasicPort;
    }

    @Test
    public void checkDefaultGeometry() {
        CommandPort port = new CommandPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());
        assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
        assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
    }
}
