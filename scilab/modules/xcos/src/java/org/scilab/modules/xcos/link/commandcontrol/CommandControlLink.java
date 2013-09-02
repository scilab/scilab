/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.link.commandcontrol;

import org.scilab.modules.xcos.link.BasicLink;

/**
 * An command control link connect an
 * {@link org.scilab.modules.xcos.port.control.ControlPort} to an
 * {@link org.scilab.modules.xcos.port.command.CommandPort}.
 */
public class CommandControlLink extends BasicLink {

    private static final long serialVersionUID = 5092221307994387276L;
    // 5 means red color
    // -1 means activation link
    private static final double[][] COLOR_AND_TYPE = { { 5.0, -1.0 } };

    /** Default constructor */
    public CommandControlLink() {
        super("CommandControlLink");
    }

    /** @return A red colored activation link identifier */
    @Override
    public double[][] getColorAndType() {
        return COLOR_AND_TYPE;
    }

}
