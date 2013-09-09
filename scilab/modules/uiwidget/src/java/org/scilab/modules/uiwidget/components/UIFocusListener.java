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
import org.scilab.modules.uiwidget.callback.UICallback;

/**
 * FocusListener wrapper
 */
public class UIFocusListener extends UIListener {

    public enum Type {
        GAIN, LOSS;
    }

    private Component component;

    private FocusAdapter adapter;
    private FocusAction gain;
    private FocusAction loss;

    private boolean gainEnable = true;
    private boolean lossEnable = true;
    private boolean listenerAdded;

    /**
     * {@inheritDoc}
     */
    public UIFocusListener(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
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

    /**
     * {@inheritDoc}
     */
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

    /**
     * Set the onfocusgain command
     * @param command the command
     */
    public void setOnfocusgain(String command) {
        gain = getFocusAction(Type.GAIN, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Get the onfocusgain command
     * @return the command
     */
    public UICallback getOnfocusgain() {
        if (gain != null) {
            return gain.callback;
        }

        return null;
    }

    /**
     * Set the onfocusloss command
     * @param command the command
     */
    public void setOnfocusloss(String command) {
        loss = getFocusAction(Type.LOSS, getParent(), command);
        addListenerToComponent(component);
    }

    /**
     * Get the onfocusloss command
     * @return the command
     */
    public UICallback getOnfocusloss() {
        if (loss != null) {
            return loss.callback;
        }

        return null;
    }

    /**
     * Enable onfocusgain
     * @param b true to enable
     */
    public void setOnfocusgainEnable(boolean b) {
        gainEnable = b;
    }

    /**
     * Check if onfocusgain is enabled
     * @return true if enabled
     */
    public boolean getOnfocusgainEnable() {
        return gainEnable;
    }

    /**
     * Enable onfocusloss
     * @param b true to enable
     */
    public void setOnfocuslossEnable(boolean b) {
        lossEnable = b;
    }

    /**
     * Check if onfocusloss is enabled
     * @return true if enabled
     */
    public boolean getOnfocuslossEnable() {
        return lossEnable;
    }

    /**
     * Get a FocusAdapter corresponding to the event type
     * @param type GAIN or LOSS
     * @param uicomp the component where to put the listener
     * @param command the command
     * @return the listener
     */
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

    /**
     * Get a focus action according to the type
     * @param type GAIN or LOSS
     * @param uicomp the component where to put the listener
     * @param command the command
     * @return the action
     */
    public static FocusAction getFocusAction(Type type, UIComponent uicomp, String command) {
        switch (type) {
            case GAIN:
                return new FocusAction(uicomp, command) {
                    public void action(FocusEvent e) {
                        UIWidgetTools.execAction(this.callback);
                    }
                };
            case LOSS:
                return new FocusAction(uicomp, command) {
                    public void action(FocusEvent e) {
                        UIWidgetTools.execAction(this.callback);
                    }
                };
            default:
                return null;
        }
    }

    /**
     * Inner class to have a Focus action
     */
    protected static abstract class FocusAction {

        UICallback callback;
        UIComponent uicomp;

        /**
         * Default constructor
         * @param uicomp the component which will have this action
         * @param command the command
         */
        FocusAction(UIComponent uicomp, String command) {
            this.uicomp = uicomp;
            this.callback = UICallback.newInstance(uicomp, command);
        }

        /**
         * The action
         * @param e event
         */
        public abstract void action(FocusEvent e);
    }
}
