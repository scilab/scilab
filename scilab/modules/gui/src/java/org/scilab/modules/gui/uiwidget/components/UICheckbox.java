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

package org.scilab.modules.gui.uiwidget.components;

import java.awt.event.ActionEvent;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Icon;
import javax.swing.JCheckBox;

import org.scilab.modules.gui.uiwidget.StringConverters;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * JCheckBox wrapper
 */
public class UICheckbox extends UIComponent {

    private JCheckBox checkbox;
    private String buttonGroup;
    private AbstractAction clicklistener;
    private UICallback onclickAction;
    private boolean onclickEnable = true;

    /**
     * {@inheritDoc}
     */
    public UICheckbox(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        checkbox = new JCheckBox();

        return checkbox;
    }

    @UIComponentAnnotation(attributes = {"text", "icon", "selected", "button-group"})
    public Object newInstance(String text, Icon icon, boolean selected, String group) {
        if (icon != null) {
            checkbox = new JCheckBox(text, icon, selected);
        } else {
            checkbox = new JCheckBox(text, selected);
        }

        setButtonGroup(group);

        return checkbox;
    }

    /**
     * Set the button group where this button belongs.
     * @param group the group name
     */
    public void setButtonGroup(String group) {
        if (group != null && !group.isEmpty()) {
            getRoot().addToButtonGroup(group, checkbox);
        } else {
            getRoot().removeFromButtonGroup(this.buttonGroup, checkbox);
        }
        this.buttonGroup = group;
    }

    /**
     * Get the button group where this button belongs.
     * @return the group name
     */
    public String getButtonGroup() {
        return buttonGroup;
    }

    /**
     * {@inheritDoc}
     */
    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        String al = style.get("alignment");
        if (al != null && !al.isEmpty()) {
            UIButton.Alignment a = StringConverters.getObjectFromValue(UIButton.Alignment.class, al);
            checkbox.setHorizontalAlignment(a.value());
            style.remove("alignment");
        }
        super.setUiStyle(style);
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setAlignment(UIButton.Alignment a) {
        checkbox.setHorizontalAlignment(a.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getAlignment() {
        return UIButton.Alignment.getAsString(checkbox.getHorizontalAlignment());
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setHorizontalAlignment(UIButton.Alignment alignment) {
        checkbox.setHorizontalAlignment(alignment.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getHorizontalAlignment() {
        return UIButton.Alignment.getAsString(checkbox.getHorizontalAlignment());
    }

    /**
     * Remove the action listener
     */
    public void removeActionListener() {
        if (clicklistener != null) {
            checkbox.removeActionListener(clicklistener);
            clicklistener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeActionListener();
        super.remove();
    }

    /**
     * Get the onclick action
     * @return the action
     */
    public UICallback getOnclick() {
        return onclickAction;
    }

    /**
     * Set the onclick action
     * @param the action
     */
    public void setOnclick(final String onclickAction) {
        if (this.onclickAction == null) {
            removeActionListener();
            clicklistener = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    if (onclickEnable) {
                        UIWidgetTools.execAction(UICheckbox.this.onclickAction);
                    }
                }
            };
            checkbox.addActionListener(clicklistener);
        }
        this.onclickAction = UICallback.newInstance(this, onclickAction);
    }

    /**
     * Check if the onclick is enabled
     * @return true if enabled
     */
    public boolean getOnclickEnable() {
        return onclickEnable;
    }

    /**
     * Set if the onclick is enabled
     * @param b true if enabled
     */
    public void setOnclickEnable(boolean b) {
        onclickEnable = b;
    }

    /**
     * Alias for setText
     */
    public void setString(String label) {
        checkbox.setText(label);
    }

    /**
     * Alias for getText
     */
    public String getString() {
        return checkbox.getText();
    }
}
