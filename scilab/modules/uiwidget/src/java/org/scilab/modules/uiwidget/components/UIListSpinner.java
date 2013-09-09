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

import java.util.ArrayList;
import java.util.List;

import javax.swing.JSpinner;
import javax.swing.SpinnerListModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;
import org.scilab.modules.uiwidget.callback.UICallback;

/**
 * JSpinner wrapper
 */
public class UIListSpinner extends UIComponent {

    private JSpinner spinner;
    private ChangeListener listener;
    private List<Object> values;
    private SpinnerListModel model;
    private UICallback action;
    private boolean onchangeEnable = true;

    /**
     * {@inheritDoc}
     */
    public UIListSpinner(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        values = new ArrayList<Object>();
        model = new SpinnerListModel();
        spinner = new JSpinner(model);

        return spinner;
    }

    /**
     * {@inheritDoc}
     */
    public void finish() {
        model.setList(values);
    }

    /**
     * Add an object
     * @param obj the object to add
     */
    public void add(Object obj) {
        values.add(obj);
    }

    /**
     * Remove a listener
     */
    protected void removeListener() {
        if (listener != null) {
            spinner.removeChangeListener(listener);
            listener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeListener();
        super.remove();
    }

    /**
     * Get the onchange action
     * @return the action
     */
    public UICallback getOnchange() {
        return action;
    }

    /**
     * Set the onchange action
     * @param the action
     */
    public void setOnchange(final String action) {
        if (this.action == null) {
            removeListener();
            listener = new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    if (onchangeEnable && spinner.getValue() != null) {
                        UIWidgetTools.execAction(UIListSpinner.this.action, "\"" + spinner.getValue().toString().replaceAll("\"", "\"\"").replaceAll("\'", "\'\'") + "\"");
                    }
                }
            };
            spinner.addChangeListener(listener);
        }
        this.action = UICallback.newInstance(this, action);
    }

    /**
     * Check if the onchange is enabled
     * @return true if enabled
     */
    public boolean getOnchangeEnable() {
        return onchangeEnable;
    }

    /**
     * Set if the onchange is enabled
     * @param b true if enabled
     */
    public void setOnchangeEnable(boolean b) {
        onchangeEnable = b;
    }
}
