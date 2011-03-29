/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.pushbutton;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for SimplePushButton the associated object to Scilab GUIs PushButton
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface SimplePushButton extends Widget {

        /**
         * Sets the icon of a PushButton
         * @param filename the path to the icon image to set to the PushButton
         */
        void setIcon(String filename);

        /**
         * Set the string to be displayed when the mouse cursor is over the PushButton
         * TODO move this method in Widget or UIElement
         *
         * @param toolTipText the string to set
         */
        void setToolTipText(String toolTipText);

        /**
         * Get the base text used for the Menu
         * @return the text
         */
        String getBaseText();
}
