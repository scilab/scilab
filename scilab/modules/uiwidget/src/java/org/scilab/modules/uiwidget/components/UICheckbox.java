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

import java.util.Map;

import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.JCheckBox;

import org.scilab.modules.uiwidget.StringConverters;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UICheckbox extends UIComponent {

    private JCheckBox checkbox;

    public UICheckbox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        checkbox = new JCheckBox();

        return checkbox;
    }

    @UIComponentAnnotation(attributes = {"text", "icon", "selected"})
    public Object newInstance(String text, Icon icon, boolean selected) {
        if (icon != null) {
            checkbox = new JCheckBox(text, icon, selected);
        } else {
            checkbox = new JCheckBox(text, selected);
        }

        return checkbox;
    }

    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        String al = style.get("alignment");
        if (al != null && !al.isEmpty()) {
            UIButton.Alignment a = StringConverters.getObjectFromValue(UIButton.Alignment.class, al);
            checkbox.setHorizontalAlignment(a.value());
            style.remove("alignment");
        }
        super.setUiStyle(style);
    }

    public void setOnclick(Action action) {
        checkbox.addActionListener(action);
    }

    public void setHorizontalAlignment(UIButton.Alignment alignment) {
        checkbox.setHorizontalAlignment(alignment.value());
    }
}
