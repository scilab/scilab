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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.geom.Rectangle2D;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.callback.UICallback;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Base class for uicontrol compatibility
 */
public abstract class ScilabWidget extends UIComponent {

    protected String tooltipString;
    protected String hAlignment;
    protected String vAlignment;
    protected String relief;
    protected String callback;
    protected int callbackType;
    protected UICallback commonCallBack;
    protected double min;
    protected double max;
    protected double[] value;
    protected double[] sliderStep;
    protected int listboxTop;
    protected String[][] string;
    protected boolean usePreferredSize = true;

    public ScilabWidget(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public void setString(String[][] str) {
        this.string = str;
    }

    public String[][] getString() {
        return string;
    }

    public void setCallback(String callback) {
        this.callback = callback;
        this.callbackType = 0;
        if (callback != null && !callback.isEmpty()) {
            commonCallBack = UICallback.newInstance(this, callback, UICallback.Type.intToType(callbackType));
        } else {
            commonCallBack = null;
        }
    }

    public String getCallback() {
        return this.callback;
    }

    public void setCallbackType(int callbackType) {
        if (this.callbackType != callbackType) {
            this.callbackType = callbackType;
            if (this.callbackType != -1 && callback != null && !callback.isEmpty()) {
                commonCallBack = UICallback.newInstance(this, callback, UICallback.Type.intToType(callbackType));
            } else {
                commonCallBack = null;
            }
        }
    }

    public int getCallbackType() {
        return this.callbackType;
    }

    public void setTooltipString(String text) {
        this.tooltipString = text;
        try {
            setTooltip(text);
        } catch (UIWidgetException e) { }
    }

    public String getTooltipString() {
        return tooltipString;
    }

    public void setHorizontalAlignment(String halign) {
        this.hAlignment = halign;
    }

    public String getHorizontalAlignment() {
        return this.hAlignment;
    }

    public void setVerticalAlignment(String valign) {
        this.vAlignment = valign;
    }

    public String getVerticalAlignment() {
        return this.vAlignment;
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

    public void setValue(double[] value) {
        this.value = value;
    }

    public double[] getValue() {
        return this.value;
    }

    public void setSliderStep(double[] d) {
        this.sliderStep = d;
    }

    public double[] getSliderStep() {
        if (sliderStep == null || sliderStep.length < 2) {
            return new double[] {0.01  * (max - min), 0.1 * (max - min)};
        } else {
            return sliderStep;
        }
    }

    public void setListboxTop(int top) {
        listboxTop = top;
    }

    public int getListboxTop() {
        return listboxTop;
    }

    public void setUsePreferredSize(boolean usePreferredSize) {
        this.usePreferredSize = usePreferredSize;
    }

    public boolean setUsePreferredSize() {
        return this.usePreferredSize;
    }

    public void initialize() {
        super.initialize();
        try {
            setBackgroundColor(new Color(0.8f, 0.8f, 0.8f));
            setRelief(ScilabRelief.FLAT);
            //super.setPosition(new Rectangle2D.Double(20, 40, 40, 20));
            setFontName("helvetica");
            setFontAngle("normal");
            setFontWeight(UITools.FontWeight.REGULAR);
            setForegroundColor(new Color(0f, 0f, 0f));
            setFontUnits(UITools.FontUnit.PIXELS);
            setFontSize(12);
            setHorizontalAlignment("center");
            setVerticalAlignment("middle");
            setTooltipString(null);
            setMin(0);
            setMax(1);
            setValue(new double[] {0});
            setListboxTop(0);
        } catch (UIWidgetException e) { }
    }

    /**
     * {@inheritDoc}
     */
    public String getType() {
        return "uicontrol";
    }

    protected static String getSingleString(String[][] str) {
        if (str != null && str.length == 1 && str[0].length == 1) {
            return str[0][0];
        }

        return null;
    }

    protected static String[] getRowString(String[][] str) {
        if (str != null) {
            if (str.length == 1) {
                return str[0];
            } else if (str[0].length == 1) {
                String[] ret = new String[str.length];
                for (int i = 0; i < ret.length; i++) {
                    ret[i] = str[i][0];
                }

                return ret;
            }
        }

        return null;
    }
}
