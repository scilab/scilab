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

import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.JRadioButton;

import org.scilab.modules.uiwidget.StringConverters;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

public class UIRadio extends UIComponent {

    private JRadioButton radio;
    private String buttonGroup;

    public UIRadio(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        radio = new JRadioButton();

        return radio;
    }

    @UIComponentAnnotation(attributes = {"text", "icon", "selected", "button-group"})
    public Object newInstance(String text, Icon icon, boolean selected, String group) {
        if (icon != null) {
            radio = new JRadioButton(text, icon, selected);
        } else {
            radio = new JRadioButton(text, selected);
        }

        setButtonGroup(group);

        return radio;
    }

    public void setButtonGroup(String group) {
        if (group != null && !group.isEmpty()) {
            getRoot().addToButtonGroup(group, radio);
        } else {
            getRoot().removeFromButtonGroup(this.buttonGroup, radio);
        }
        this.buttonGroup = group;
    }

    public String getButtonGroup() {
        return buttonGroup;
    }

    public void setOnclick(Action action) {
        radio.addActionListener(action);
    }

    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        String al = style.get("alignment");
        if (al != null && !al.isEmpty()) {
            UIButton.Alignment a = StringConverters.getObjectFromValue(UIButton.Alignment.class, al);
            radio.setHorizontalAlignment(a.value());
            style.remove("alignment");
        }
        super.setUiStyle(style);
    }

    public void setHorizontalAlignment(UIButton.Alignment alignment) {
        radio.setHorizontalAlignment(alignment.value());
    }
}
