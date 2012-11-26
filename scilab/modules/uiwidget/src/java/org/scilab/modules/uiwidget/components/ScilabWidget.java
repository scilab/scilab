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

import java.awt.Rectangle;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;

import org.scilab.modules.gui.widget.Widget;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

public abstract class ScilabWidget extends UIComponent {

    protected Widget widget;
    protected double min;
    protected double max;
    protected double value;

    public ScilabWidget(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    // public void setPosition(Rectangle r) {
    // 	widget.setDims(new Size(r.width, r.height));
    // 	widget.setPosition(new Position(r.x, r.y));
    // }

    public void setString(String str) {
        widget.setText(str);
    }

    public String getString() {
        return widget.getText();
    }

    public void setMin(double min) {
        this.min = min;
    }

    public double getMin() {
        return this.min;
    }

    public void setMax(double max) {
        this.max = max;
    }

    public double getMax() {
        return this.max;
    }

    public void setValue(double value) {
        this.value = value;
    }

    public double getValue() {
        return this.value;
    }
}
