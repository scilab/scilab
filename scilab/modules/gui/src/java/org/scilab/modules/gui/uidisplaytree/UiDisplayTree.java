/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
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

package org.scilab.modules.gui.uidisplaytree;

import org.scilab.modules.gui.text.Text;

/**
 * Interface for UiDisplayTree associated to objects in Scilab GUIs
 * @author Han DONG
 */
public interface UiDisplayTree extends Text {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleUiDisplayTree getAsSimpleUiDisplayTree();

    /**
     * Sets the tree data to be added to viewport
     * @param text the array of strings containing tree data
     */
    void setData(String[] text);
}
