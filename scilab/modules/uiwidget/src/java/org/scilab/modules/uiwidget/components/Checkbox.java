/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
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

import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

public class Checkbox extends ScilabWidget {

    public Checkbox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        widget = new SwingScilabCheckBox();
        setMin(0);
        setMax(1);

        return widget;
    }

    public double getValue() {
        if (((SwingScilabCheckBox) widget).isSelected()) {
            return getMin();
        } else {
            return getMax();
        }
    }
}
