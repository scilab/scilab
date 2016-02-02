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

package org.scilab.modules.gui.text;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for TextBridge the associated object to Scilab GUIs Text
 * @author Marouane BEN JELLOUL
 */
public interface SimpleText extends Widget {

    /**
     * Sets the text of a Text Widget
     * @param newText the text to set to the Text Widget
     */
    void setText(String newText);

    /**
     * Gets the text of a Text Widget
     * @return the text of the Text Widget
     */
    String getText();
}
