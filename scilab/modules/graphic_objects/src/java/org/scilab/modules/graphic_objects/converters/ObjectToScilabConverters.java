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

package org.scilab.modules.graphic_objects.converters;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;
import java.io.File;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

import org.scilab.modules.graphic_objects.axes.TicksProperty;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.utils.Alignment;
import org.scilab.modules.graphic_objects.utils.Antialiasing;
import org.scilab.modules.graphic_objects.utils.ArcDrawingMethod;
import org.scilab.modules.graphic_objects.utils.AxisLocation;
import org.scilab.modules.graphic_objects.utils.BoxType;
import org.scilab.modules.graphic_objects.utils.ClipStateType;
import org.scilab.modules.graphic_objects.utils.DataMapping;
import org.scilab.modules.graphic_objects.utils.GridPosition;
import org.scilab.modules.graphic_objects.utils.LegendLocation;
import org.scilab.modules.graphic_objects.utils.LightType;
import org.scilab.modules.graphic_objects.utils.LineType;
import org.scilab.modules.graphic_objects.utils.MarkSizeUnitType;
import org.scilab.modules.graphic_objects.utils.PixelDrawingMode;
import org.scilab.modules.graphic_objects.utils.RotationType;
import org.scilab.modules.graphic_objects.utils.TextBoxMode;
import org.scilab.modules.graphic_objects.utils.TicksDirection;
import org.scilab.modules.graphic_objects.utils.TicksStyle;
import org.scilab.modules.graphic_objects.utils.TipOrientation;
import org.scilab.modules.graphic_objects.utils.ViewType;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabHandle;
import org.scilab.modules.types.ScilabStackPutter;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;

/**
 * Class to provide easy way to convert a Java object into a Scilab variable and to put the result
 * on the Scilab stack at the given position
 */
public final class ObjectToScilabConverters {

    private static final Map < Class<?>, ObjectConverter > converters = new HashMap < Class<?>, ObjectConverter > ();

    /**
     * Put the Java object o on the Scilab stack at the position stackPos
     * An ObjetConverter is searched and if none has been found an empty matrix is put
     * @param o the Java object to convert and put.
     * @param stackPos the Scilab stack position.
     */
    public static void putOnScilabStack(Object o, int stackPos) {
        if (o == null) {
            ScilabStackPutter.putEmpty(stackPos);
        } else if (o instanceof ScilabType) {
            ScilabStackPutter.put(stackPos, (ScilabType)o);
        } else {
            final Class <? extends Object > clazz = o.getClass();
            ObjectConverter converter = null;

            if (o instanceof GraphicObject) {
                converter = converters.get(GraphicObject.class);
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
    private static final ObjectConverter getConverter(Class<?> clazz) {
        for (Entry < Class<?>, ObjectConverter > entry : converters.entrySet()) {
            Class<?> key = entry.getKey();
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
        converters.put(Integer[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Integer[] d = (Integer[]) o;
                double[] dd = new double[d.length];
                for (int i = 0; i < dd.length; i++) {
                    dd[i] = (double)d[i];
                }
                ScilabStackPutter.put(stackPos, dd, true);
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
                ScilabStackPutter.put(stackPos, dd, true);
            }
        });
        converters.put(double[][].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (double[][]) o, true);
            }
        });
        converters.put(Boolean.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                if ((Boolean)o == true) {
                    ScilabStackPutter.put(stackPos, "on");
                } else {
                    ScilabStackPutter.put(stackPos, "off");
                }
            }
        });
        converters.put(Boolean[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                Boolean[] b = (Boolean[])o;
                String[] str = new String[b.length];
                for (int i = 0 ; i < b.length ; i++) {
                    if (b[i] == true) {
                        str[i] = "on";
                    } else {
                        str[i] = "off";
                    }
                }
                ScilabStackPutter.put(stackPos, str, true);
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
                ScilabStackPutter.put(stackPos, (String[]) o, true);
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
        converters.put(GraphicObject.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                GraphicObject ui = (GraphicObject) o;
                ScilabHandle handle = new ScilabHandle((long) (ui.getIdentifier()));
                ScilabStackPutter.put(stackPos, handle);
            }
        });
        converters.put(GraphicObject[].class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                GraphicObject[] uis = (GraphicObject[]) o;
                long[][] hdls = new long[1][uis.length];
                for (int i = 0; i < uis.length; i++) {
                    hdls[0][i] = (long) (uis[i].getIdentifier());
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
        converters.put(KeyStroke.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
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
        converters.put(PixelDrawingMode.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, PixelDrawingMode.enumToString((PixelDrawingMode)o));
            }
        });
        converters.put(RotationType.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, RotationType.enumToString((RotationType)o));
            }
        });
        converters.put(ColorMap.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ColorMap color = (ColorMap)o;
                Double[] data = color.getData();
                double[][] dd = new double[3][data.length / 3];
                for (int j = 0; j < dd.length; j++) {
                    for (int i = 0; i < dd[0].length; i++) {
                        dd[j][i] = data[j * dd[0].length + i];
                    }
                }
                ScilabStackPutter.put(stackPos, dd, false);
            }
        });
        converters.put(Antialiasing.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, Antialiasing.enumToString((Antialiasing)o));
            }
        });
        converters.put(ArcDrawingMethod.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, ArcDrawingMethod.enumToString((ArcDrawingMethod)o));
            }
        });
        converters.put(AxisLocation.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, AxisLocation.enumToString((AxisLocation)o));
            }
        });
        converters.put(BoxType.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, BoxType.enumToString((BoxType)o));
            }
        });
        converters.put(GridPosition.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, GridPosition.enumToString((GridPosition)o));
            }
        });
        converters.put(MarkSizeUnitType.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, MarkSizeUnitType.enumToString((MarkSizeUnitType)o));
            }
        });
        converters.put(LineType.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, (double) LineType.enumToInt((LineType)o).intValue());
            }
        });
        converters.put(ClipStateType.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, ClipStateType.enumToString((ClipStateType)o));
            }
        });
        converters.put(ViewType.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, ViewType.enumToString((ViewType)o));
            }
        });
        converters.put(TicksProperty.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                TicksProperty ticks = (TicksProperty)o;

                //create a TList with :
                // - fields names as String[]
                // - locations as ScilabDouble ( Nx1 )
                // - labels as ScilabString ( Nx1 )

                //fields list
                String[] fields = new String[3];
                fields[0] = "ticks";
                fields[1] = "locations";
                fields[2] = "labels";

                //locations
                Double[] locations = ticks.getLocations();

                //convert Double[] to double[][]
                double[][] dd = new double[1][locations.length];
                for (int i = 0 ; i < locations.length ; i++) {
                    dd[0][i] = locations[i];
                }

                ScilabDouble dblLoc = new ScilabDouble(dd);

                //labels
                String[] labels = ticks.getLabelsStrings();
                ScilabString stLab = new ScilabString(labels);

                List<ScilabType> data = new ArrayList<ScilabType>();
                data.add(dblLoc);
                data.add(stLab);
                ScilabTList tlist = new ScilabTList(fields, data);
                ScilabStackPutter.put(stackPos, tlist);
            }
        });
        converters.put(TicksDirection.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, TicksDirection.enumToString((TicksDirection)o));
            }
        });
        converters.put(TicksStyle.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, TicksStyle.enumToString((TicksStyle)o));
            }
        });
        converters.put(DataMapping.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, DataMapping.enumToString((DataMapping)o));
            }
        });
        converters.put(Alignment.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, Alignment.enumToString((Alignment)o));
            }
        });
        converters.put(TextBoxMode.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, TextBoxMode.enumToString((TextBoxMode)o));
            }
        });
        converters.put(LegendLocation.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, LegendLocation.enumToString((LegendLocation)o));
            }
        });
        converters.put(LightType.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, LightType.enumToString((LightType)o));
            }
        });
        converters.put(TipOrientation.class, new ObjectConverter() {
            public void convert(Object o, int stackPos) {
                ScilabStackPutter.put(stackPos, TipOrientation.enumToString((TipOrientation)o));
            }
        });
    }
}
