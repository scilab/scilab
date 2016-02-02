/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.separator;

import org.scilab.modules.gui.bridge.separator.SwingScilabSeparator;

/**
 * Bridge for Scilab Separators in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabSeparatorBridge {

    /**
     * Constructor
     */
    protected ScilabSeparatorBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a new Scilab Separator
     * @return the created Separator
     */
    public static Separator createSeparator() {
        return new SwingScilabSeparator();
    }

}
