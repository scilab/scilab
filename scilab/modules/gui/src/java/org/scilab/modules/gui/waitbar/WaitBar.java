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

package org.scilab.modules.gui.waitbar;

import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for WaitBars/ProgressBars used by Scilab
 * @author Vincent COUVERT
 */
public interface WaitBar extends UIElement {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleWaitBar getAsSimpleWaitBar();

    /**
     * Set the title of the WaitBar
     * @param title the title to set
     */
    void setTitle(String title);

    /**
     * Set the message of the WaitBar
     * @param message the message to set
     */
    void setMessage(String[] message);

    /**
     * Set the current value of the WaitBar
     * @param value the value to set
     */
    void setValue(int value);

    /**
     * Close the WaitBar
     */
    void close();

    /**
     * Indicates if the total execution time is known
     * @param status true if the total progress time in unknown
     */
    void setIndeterminateMode(boolean status);

}
