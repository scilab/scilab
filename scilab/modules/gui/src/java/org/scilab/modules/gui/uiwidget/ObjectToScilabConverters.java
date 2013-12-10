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

package org.scilab.modules.gui.uiwidget;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.LayoutManager;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;
import java.io.File;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.types.ScilabHandle;
import org.scilab.modules.types.ScilabStackPutter;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

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
            ScilabStackPutter.putEmpty(stackPos);
        } else {
            final Class clazz = o.getClass();
            ObjectConverter converter;
            if (o instanceof UIComponent) {
                converter = converters.get(UIComponent.class);
            } else {
                converter = converters.get(clazz);
            }

            if (converter == null) {
                converter = getConverter(clazz);
            }

            if (converter == null) {
                ScilabStackPutter.putEmpty(stackPos);
            } else {
                converter.convert(o, stackPos);
            }
        }
    }

    /**
     * Get a converter for derived class
     * @param clazz the class
     * @return a converter
     */
    private static final ObjectConverter getConverter(Class clazz) {
        for (Map.Entry<Class, ObjectConverter> entry : converters.entrySet()) {
            Class key = entry.getKey();
            if (key != Object.class && key.isAssignableFrom(clazz)) {
                return entry.getValue();
            }
        }

        return null;
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
                ScilabStackPutter.put(stackPos, (double) ((Integer) o).intValue());
            }
        });
        converters.put(int[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (int[]) o, false, false);
            }
        });
        converters.put(int[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (int[][]) o, false, true);
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
        converters.put(Double[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Double[] d = (Double[]) o;
                double[] dd = new double[d.length];
                for (int i = 0; i < dd.length; i++) {
                    dd[i] = d[i];
                }
                ScilabStackPutter.put(stackPos, dd, false);
            }
        });
        converters.put(double[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (double[][]) o, true);
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
                ScilabStackPutter.put(stackPos, (boolean[][]) o, true);
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
                ScilabStackPutter.put(stackPos, (String[][]) o, true);
            }
        });
        converters.put(Object.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, o.toString());
            }
        });
        converters.put(Object[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                String[] strs = new String[((Object[]) o).length];
                for (int i = 0; i < strs.length; i++) {
                    Object ob = ((Object[]) o)[i];
                    strs[i] = ob == null ? "" : ob.toString();
                }
                ScilabStackPutter.put(stackPos, strs, false);
            }
        });
        converters.put(Object[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                String[][] strs = new String[((Object[][]) o).length][((Object[][]) o)[0].length];
                for (int i = 0; i < strs.length; i++) {
                    for (int j = 0; j < strs[0].length; j++) {
                        Object ob = ((Object[][]) o)[i][j];
                        strs[i][j] = ob == null ? "" : ob.toString();
                    }
                }
                ScilabStackPutter.put(stackPos, strs, true);
            }
        });
        converters.put(Color.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Color c = (Color) o;
                float[] cc = new float[4];
                c.getRGBComponents(cc);
                ScilabStackPutter.put(stackPos, new double[] {(double) cc[0], (double) cc[1], (double) cc[2], (double) cc[3]} , false);
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
        converters.put(LayoutManager.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                if (o instanceof LayoutManager) {
                    ScilabStackPutter.put(stackPos, UILayoutFactory.getStringRepresentation((LayoutManager) o));
                } else {
                    ScilabStackPutter.put(stackPos, "Cannot convert " + o.toString());
                }
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
        converters.put(Rectangle2D.Double.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Rectangle2D.Double r = (Rectangle2D.Double) o;
                ScilabStackPutter.put(stackPos, new double[] {r.x, r.y, r.width, r.height} , false);
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
        converters.put(UIComponent.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                UIComponent ui = (UIComponent) o;
                ScilabHandle handle = new ScilabHandle((long) (-ui.getUid() - 1));
                ScilabStackPutter.put(stackPos, handle);
            }
        });
        converters.put(UIComponent[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                UIComponent[] uis = (UIComponent[]) o;
                long[][] hdls = new long[1][uis.length];
                for (int i = 0; i < uis.length; i++) {
                    hdls[0][i] = (long) (-uis[i].getUid() - 1);
                }
                ScilabHandle handle = new ScilabHandle(hdls);
                ScilabStackPutter.put(stackPos, handle);
            }
        });
        converters.put(File.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, o.toString());
            }
        });
        converters.put(URL.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, o.toString());
            }
        });
        converters.put(Cursor.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, UICursorFactory.getString((Cursor) o));
            }
        });
        converters.put(KeyStroke.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                KeyStroke ks = (KeyStroke) o;
                String s = o.toString();
                int pos = s.lastIndexOf(' ');
                if (pos != -1) {
                    String key = s.substring(pos + 1);
                    pos = s.lastIndexOf(' ', pos - 1);
                    if (pos != -1) {
                        s = s.substring(0, pos);
                        ScilabStackPutter.put(stackPos, s + " " + key);
                    } else {
                        ScilabStackPutter.put(stackPos, key);
                    }
                } else {
                    ScilabStackPutter.put(stackPos, new double[0], false);
                }
            }
        });
        converters.put(UICallback.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, o.toString());
            }
        });
    }
}
