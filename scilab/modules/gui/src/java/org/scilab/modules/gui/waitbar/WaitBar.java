/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
