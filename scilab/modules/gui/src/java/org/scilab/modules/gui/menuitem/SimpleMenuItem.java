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

package org.scilab.modules.gui.menuitem;

import org.scilab.modules.gui.menu.SimpleMenu;

/**
 * Interface for SimpleMenuItem the associated object to Scilab GUIs MenuItem
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuItem extends SimpleMenu {
    /**
     * set a mnemonic to a MenuItem
     * @param mnemonic the mnemonic for  the MenuItem
     */
    void setMnemonic(char mnemonic);
}
