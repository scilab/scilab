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

package org.scilab.modules.uiwidget.components;

import java.util.Map;

import javax.swing.JSeparator;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

/**
 * JSeparator wrapper
 */
public class UISeparator extends UIComponent {

    /**
     * {@inheritDoc}
     */
    public UISeparator(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        return new JSeparator();
    }

    public static UISeparator getNewSeparator(UIComponent parent) throws UIWidgetException {
        UISeparator sep = new UISeparator(parent);
        sep.setComponent(sep.newInstance());

        return sep;
    }
}
