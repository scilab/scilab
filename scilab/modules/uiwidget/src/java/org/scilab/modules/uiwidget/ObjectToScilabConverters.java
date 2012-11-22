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

package org.scilab.modules.uiwidget;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.net.URL;
import java.net.MalformedURLException;
import java.text.DateFormat;
import java.text.ParseException;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;
import javax.swing.border.Border;

import org.scilab.modules.uiwidget.components.UILabel;
import org.scilab.modules.uiwidget.components.UIMenuItem;
import org.scilab.modules.uiwidget.components.UITab;
import org.scilab.modules.uiwidget.components.UISplit;
import org.scilab.modules.uiwidget.components.UIButton;
import org.scilab.modules.uiwidget.components.UIToolBar;
import org.scilab.modules.uiwidget.components.UITools;
import org.scilab.modules.uiwidget.components.UIPanel;
import org.scilab.modules.uiwidget.components.UISlider;
import org.scilab.modules.uiwidget.components.UIProgressBar;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.types.ScilabStackPutter;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class to provide easy way to convert a Java object into a Scilab variable and to put the result
 * on the Scilab stack at the given position
 */
public final class ObjectToScilabConverters {

    private static final Map<Class, ObjectConverter> converters = new HashMap<Class, ObjectConverter>();

    /**
     * Put the Java object o on the Scilab stack at the position stackPos
     * An ObjetConverter is searched and if none has been found an empty matrix is put
     * @param o the Java object to convert and put.
     * @param stackPos the Scilab stack position.
     */
    public static void putOnScilabStack(Object o, int stackPos) {
        if (o == null) {
            ScilabStackPutter.put(stackPos, new double[0], false);
        } else {
            ObjectConverter converter = converters.get(o.getClass());
            if (converter == null) {
                ScilabStackPutter.put(stackPos, new double[0], false);
            } else {
                converter.convert(o, stackPos);
            }
        }
    }

    /**
     * Interface to implement to parse an attribute String into a Java object
     */
    public static interface ObjectConverter {

        /**
         * Convert an object
         * @param o the Object to put on the Scilab stack
         */
        public void convert(Object o, int stackPos);
    }

    static {
        converters.put(Integer.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, ((Integer) o).intValue(), false);
            }
        });
        converters.put(int[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (int[]) o, false, false);
            }
        });
        converters.put(int[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (int[][]) o, false, false);
            }
        });
        converters.put(Double.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, ((Double) o).doubleValue());
            }
        });
        converters.put(double[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (double[]) o, false);
            }
        });
        converters.put(double[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (double[][]) o, false);
            }
        });
        converters.put(Boolean.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, ((Boolean) o).booleanValue());
            }
        });
        converters.put(boolean[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (boolean[]) o, false);
            }
        });
        converters.put(boolean[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (boolean[][]) o, false);
            }
        });
        converters.put(Float.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (double) ((Float) o).floatValue());
            }
        });
        converters.put(String.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (String) o);
            }
        });
        converters.put(String[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (String[]) o, false);
            }
        });
        converters.put(String[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (String[][]) o, false);
            }
        });
        converters.put(Color.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Color c = (Color) o;
                ScilabStackPutter.put(stackPos, new double[] {(double) c.getRed(), (double) c.getGreen(), (double) c.getBlue(), (double) c.getAlpha()} , false);
            }
        });
        converters.put(Icon.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                if (o instanceof ImageIcon) {
                    ScilabStackPutter.put(stackPos, ((ImageIcon) o).getDescription());
                } else {
                    ScilabStackPutter.put(stackPos, "Cannot convert " + o.toString());
                }
            }
        });
        converters.put(ImageIcon.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, ((ImageIcon) o).getDescription());
            }
        });
        converters.put(Insets.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Insets i = (Insets) o;
                ScilabStackPutter.put(stackPos, new double[] {(double) i.top, (double) i.left, (double) i.bottom, (double) i.right} , false);
            }
        });
        converters.put(Rectangle.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Rectangle r = (Rectangle) o;
                ScilabStackPutter.put(stackPos, new double[] {(double) r.x, (double) r.y, (double) r.width, (double) r.height} , false);
            }
        });
        converters.put(Dimension.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Dimension d = (Dimension) o;
                ScilabStackPutter.put(stackPos, new double[] {(double) d.width, (double) d.height} , false);
            }
        });
        converters.put(Point.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Point p = (Point) o;
                ScilabStackPutter.put(stackPos, new double[] {(double) p.x, (double) p.y} , false);
            }
        });
        converters.put(Position.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Position p = (Position) o;
                ScilabStackPutter.put(stackPos, new double[] {(double) p.getX(), (double) p.getY()} , false);
            }
        });
        converters.put(Size.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Size s = (Size) o;
                ScilabStackPutter.put(stackPos, new double[] {(double) s.getWidth(), (double) s.getHeight()} , false);
            }
        });
        converters.put(CommonCallBack.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                CommonCallBack cb = (CommonCallBack) o;
                ScilabStackPutter.put(stackPos, cb.getCommand());
            }
        });
    }
}
