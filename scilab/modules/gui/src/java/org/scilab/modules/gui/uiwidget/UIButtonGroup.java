/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;

/**
 * The main interest of this class is to have the method getSelected() to get the selected
 * UIComponent which belongs to a ButtonGroup
 */
public class UIButtonGroup extends ButtonGroup {

    private Map<AbstractButton, UIComponent> comps = new HashMap<AbstractButton, UIComponent>();

    /**
     * Default constructor
     */
    public UIButtonGroup() {
        super();
    }

    /**
     * Add an UIComponent to this Group
     * @param c the UIComponent to add
     */
    public void add(UIComponent c) {
        if (c != null && (c.component instanceof AbstractButton)) {
            comps.put((AbstractButton) c.component, c);
            add((AbstractButton) c.component);
        }
    }

    /**
     * Remove an UIComponent from this Group
     * @param c the UIComponent to remove
     */
    public void remove(UIComponent c) {
        if (c != null && (c.component instanceof AbstractButton)) {
            comps.remove((AbstractButton) c.component);
            remove((AbstractButton) c.component);
        }
    }

    /**
     * Get the selected UIComponent in this Group
     * @return the selected UIComponent
     */
    public UIComponent getSelected() {
        Enumeration<AbstractButton> e = getElements();
        while (e.hasMoreElements()) {
            AbstractButton button = e.nextElement();
            if (button.isSelected()) {
                return comps.get(button);
            }
        }

        return null;
    }
}
