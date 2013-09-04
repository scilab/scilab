/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.popupmenu;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for PopupMenu associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface PopupMenu extends Widget {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimplePopupMenu getAsSimplePopupMenu();

    /**
     * Set the selected index of the PopupMenu
     * @param index the index of the item to be selected
     */
    void setUserSelectedIndex(int index);

    /**
     * Get the selected index of the PopupMenu
     * @return the index of the item selected
     */
    int getUserSelectedIndex();

    /**
     * Get the text of all the PopupMenu items
     * @return the text items
     */
    String[] getAllItemsText();

    /**
     * Get the number of items in the PopupMenu
     * @return the number of items
     */
    int getNumberOfItems();

    /**
     * Set the text of the PopupMenu items
     * @param text the text of the items
     */
    void setText(String[] text);

}
