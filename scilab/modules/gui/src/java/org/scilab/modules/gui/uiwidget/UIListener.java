/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget;

import java.awt.Component;

/**
 * Abstract class to handle EventListener
 */
public abstract class UIListener extends UIComponent {

    /**
     * Default constructor
     * @param parent the UIComponent which will have a listener
     */
    public UIListener(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * Add a listener to the given component
     * @param c the component
     */
    public abstract void addListenerToComponent(Component c);
}
