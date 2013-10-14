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

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;

/**
 * KeyStroke wrapper
 */
public class UIKeystroke extends UIComponent {

    private KeyStroke key;
    private Action action;
    private String actionCommand;
    private boolean enable = true;

    /**
     * {@inheritDoc}
     */
    public UIKeystroke(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        return this;
    }

    @UIComponentAnnotation(attributes = {"key", "action"})
    public Object newInstance(KeyStroke key, String action) {
        this.key = key;
        this.actionCommand = action;
        this.action = new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                if (enable && actionCommand != null && !actionCommand.isEmpty()) {
                    UIWidgetTools.execAction(UIKeystroke.this, actionCommand);
                }
            }
        };

        return this;
    }

    /**
     * Get the associated action
     * @return the action
     */
    Action getRealAction() {
        return action;
    }

    /**
     * Set the keystroke
     * @param key the key
     */
    public void setKey(KeyStroke key) {
        if (this.key != null && (key == null || !this.key.equals(key))) {
            UIComponent p = getParent();
            if (p instanceof UIKeymap) {
                UIKeymap map = (UIKeymap) p;
                map.replaceKey(this.key, key);
            }
            this.key = key;
        }
    }

    /**
     * Get the keystroke
     * @return the key
     */
    public KeyStroke getKey() {
        return key;
    }

    /**
     * Enable the action
     * @param b true to enable
     */
    public void setActionEnable(boolean b) {
        this.enable = b;
    }

    /**
     * Check if the action is enabled
     * @return true if enabled
     */
    public boolean getActionEnable() {
        return enable;
    }

    /**
     * Set the action
     * @param action the action
     */
    public void setAction(String action) {
        this.actionCommand = action;
    }

    /**
     * Get the action
     * @return the action
     */
    public String getAction() {
        return actionCommand;
    }
}
