/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.utils;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;

/**
 * Borders for Widgets in Scilab GUIs definition
 * @author Vincent COUVERT
 */
public final class ScilabWidgetBorder {

    /**
     * Border used for FLAT relief property
     */
    public static final Border FLAT = BorderFactory.createEmptyBorder();

    /**
     * Border used for GROOVE relief property
     */
    public static final Border GROOVE = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);

    /**
     * Border used for RAISED relief property
     */
    public static final Border RAISED = BorderFactory.createRaisedBevelBorder();

    /**
     * Border used for RIDGE relief property
     */
    public static final Border RIDGE = BorderFactory.createEtchedBorder(EtchedBorder.RAISED);

    /**
     * Border used for SOLID relief property
     */
    public static final Border SOLID = BorderFactory.createLineBorder(Color.BLACK);

    /**
     * Border used for SUNKEN relief property
     */
    public static final Border SUNKEN = BorderFactory.createLoweredBevelBorder();

    /**
     * Constructor
     */
    private ScilabWidgetBorder() {
        throw new UnsupportedOperationException();
    }

}
