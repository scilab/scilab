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

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.swing.JSpinner;
import javax.swing.SpinnerListModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

import org.scilab.modules.action_binding.InterpreterManagement;

public class UIListSpinner extends UIComponent {

    private JSpinner spinner;
    private List<Object> values;
    private SpinnerListModel model;
    private String action;

    public UIListSpinner(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        values = new ArrayList<Object>();
        model = new SpinnerListModel();
        spinner = new JSpinner(model);

        return spinner;
    }

    public void finish() {
        model.setList(values);
    }

    public void add(Object obj) {
        values.add(obj);
    }

    public void setOnchange(final String action) {
        if (this.action == null) {
            spinner.addChangeListener(new ChangeListener() {
                public void stateChanged(ChangeEvent e) {
                    InterpreterManagement.requestScilabExec(UIListSpinner.this.action + "(\"" + spinner.getValue().toString().replaceAll("\"", "\"\"").replaceAll("\'", "\'\'") + "\")");
                }
            });
        }
        this.action = action;
    }
}
