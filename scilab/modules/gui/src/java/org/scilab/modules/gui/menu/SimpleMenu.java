/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for SimpleMenu the associated object to Scilab GUIs Menu
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenu extends Widget {

    /**
     * Append a MenuItem to the Menu
     * @param newMenuItem the MenuItem to add to the Menu
     */
    void add(MenuItem newMenuItem);

    /**
     * Append a CheckBoxMenuItem to the Menu
     * @param newCheckBoxMenuItem the CheckBoxMenuItem to add to the Menu
     */
    void add(CheckBoxMenuItem newCheckBoxMenuItem);

    /**
     * Append a subMenu to the Menu
     * @param newSubMenu the subMenu to append to the Menu
     */
    void add(Menu newSubMenu);

    /**
     * set a mnemonic to the Menu
     * @param mnemonic the new mnemonic of the Menu
     */
    void setMnemonic(int mnemonic);

    /**
     * Add a Separator to a Menu
     */
    void addSeparator();

    /**
     * Set if the Menu is checked or not
     * @param status true if the Menu is checked
     */
    void setChecked(boolean status);

    /**
     * Get if the Menu is checked or not
     * @return true if the Menu is checked
     */
    boolean isChecked();

    /**
     * Retrieve the CallBack associated to this Menu
     * @return the CallBack
     */
    CommonCallBack getCallback();

    /**
     * Get the base text used for the Menu
     * @return the text
     */
    String getBaseText();
}
