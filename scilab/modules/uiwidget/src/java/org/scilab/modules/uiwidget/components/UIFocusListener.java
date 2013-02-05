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

import java.awt.Component;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;
import org.scilab.modules.uiwidget.UIListener;

public class UIFocusListener extends UIListener {

    public enum Type {
        GAIN, LOSS;
    }

    Component component;

    FocusAdapter adapter;
    FocusAction gain;
    FocusAction loss;

    boolean gainEnable = true;
    boolean lossEnable = true;
    boolean listenerAdded;

    public UIFocusListener(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        adapter = new FocusAdapter() {
            public void focusGained(FocusEvent e) {
                if (gain != null && gainEnable) {
                    gain.action(e);
                }
            }

            public void focusLost(FocusEvent e) {
                if (loss != null && lossEnable) {
                    loss.action(e);
                }
            }
        };

        return adapter;
    }

    public void addListenerToComponent(Component c) {
        if (this.component != c) {
            if (this.component != null) {
                this.component.removeFocusListener(adapter);
            }
            if (listenerAdded && c != null) {
                c.addFocusListener(adapter);
            }

            this.component = c;
        }

        if (!listenerAdded && this.component != null && (gain != null || loss != null)) {
            this.component.addFocusListener(adapter);
            listenerAdded = true;
        }
    }

    public void setOnfocusgain(String command) {
        gain = getFocusAction(Type.GAIN, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnfocusloss(String command) {
        loss = getFocusAction(Type.LOSS, getParent(), command);
        addListenerToComponent(component);
    }

    public void setOnfocusgainEnable(boolean b) {
        gainEnable = b;
    }

    public boolean getOnfocusgainEnable() {
        return gainEnable;
    }

    public void setOnfocuslossEnable(boolean b) {
        lossEnable = b;
    }

    public boolean getOnfocuslossEnable() {
        return lossEnable;
    }

    public String getOnfocusgain() {
        if (gain != null) {
            return gain.command;
        }

        return null;
    }

    public String getOnfocusloss() {
        if (loss != null) {
            return loss.command;
        }

        return null;
    }

    public static FocusAdapter getFocusAdapter(Type type, UIComponent uicomp, String command) {
        final FocusAction action = getFocusAction(type, uicomp, command);

        switch (type) {
            case GAIN:
                return new FocusAdapter() {
                    public void focusGained(FocusEvent e) {
                        action.action(e);
                    }
                };
            case LOSS:
                return new FocusAdapter() {
                    public void focusLost(FocusEvent e) {
                        action.action(e);
                    }
                };
            default:
                return null;
        }
    }

    public static FocusAction getFocusAction(Type type, UIComponent uicomp, String command) {
        switch (type) {
            case GAIN:
                return new FocusAction(uicomp, command) {
                    public void action(FocusEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command);
                    }
                };
            case LOSS:
                return new FocusAction(uicomp, command) {
                    public void action(FocusEvent e) {
                        UIWidgetTools.execAction(this.uicomp, this.command);
                    }
                };
            default:
                return null;
        }
    }

    private static abstract class FocusAction {

        String command;
        UIComponent uicomp;

        FocusAction(UIComponent uicomp, String command) {
            this.uicomp = uicomp;
            this.command = command;
        }

        public abstract void action(FocusEvent e);
    }
}
