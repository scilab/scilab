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

import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.KeyStroke;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UIKeystroke extends UIComponent {

    private KeyStroke key;
    private Action action;
    private String actionCommand;
    private boolean enable = true;

    public UIKeystroke(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        return this;
    }

    @UIComponentAnnotation(attributes = {"key", "action"})
    public Object newInstance(KeyStroke key, String action) {
        this.key = key;
        this.actionCommand = action;
        this.action = new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                if (enable && actionCommand != null  && !actionCommand.isEmpty()) {
                    UIWidgetTools.execAction(UIKeystroke.this, actionCommand);
                }
            }
        };

        return this;
    }

    Action getRealAction() {
        return action;
    }

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

    public KeyStroke getKey() {
        return key;
    }

    public void setActionEnable(boolean b) {
        this.enable = b;
    }

    public boolean getActionEnable() {
        return enable;
    }

    public void setAction(String action) {
        this.actionCommand = action;
    }

    public String getAction() {
        return actionCommand;
    }
}
