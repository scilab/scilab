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
import java.awt.Image;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.net.URL;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.NumberFormat;
import java.text.ParseException;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.KeyStroke;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
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
import org.scilab.modules.graphic_objects.utils.MarkSizeUnitType;
import org.scilab.modules.graphic_objects.utils.PixelDrawingMode;
import org.scilab.modules.graphic_objects.utils.RotationType;
import org.scilab.modules.graphic_objects.utils.TextBoxMode;
import org.scilab.modules.graphic_objects.utils.TicksDirection;
import org.scilab.modules.graphic_objects.utils.TicksStyle;
import org.scilab.modules.graphic_objects.utils.ViewType;

/**
 * Main class which handle conversion between String and basic Java objects
 */
public final class StringConverters {

    private static final Map < Class<?>, StringConverter > converters = new HashMap < Class<?>, StringConverter > ();
    private static final DateFormat dateformat = DateFormat.getDateInstance(DateFormat.LONG);

    /**
     * Register a converter
     * @param clazz the resulting class
     * @param converter the converter to use
     */
    public static final void registerConverter(final Class<?> clazz, final StringConverter converter) {
        converters.put(clazz, converter);
    }

    /**
     * Get the converter to use for a given class
     * @param clazz the class result
     * @return the corresponding converter
     */
    public static final StringConverter getConverter(final Class<?> clazz) {
        return converters.get(clazz);
    }

    /**
     * Check if a converter is existing
     * @param clazz the class result
     * @return true if a converter is existing
     */
    public static final boolean containConverter(final Class<?> clazz) {
        return converters.containsKey(clazz);
    }

    /**
     * Get an object from a value
     * @param clazz the class of the resulting object
     * @param value the value to convert
     * @return the converted object
     */
    @SuppressWarnings("unchecked")
    public static final <T> T getObjectFromValue(final Class<T> clazz, final String value) {
        final StringConverter converter = converters.get(clazz);
        if (converter == null) {
            // throw
        }

        return (T) converter.convert(value);
    }

    /**
     * Get an object from a value
     * @param clazz the class of the resulting object
     * @param value he value to convert
     * @param defaultValue the default value
     * @return the converted object
     */
    @SuppressWarnings("unchecked")
    public static final <T> T getObjectFromValue(final Class<T> clazz, final String value, final T defaultValue) {
        final StringConverter converter = converters.get(clazz);
        if (converter == null) {
            // throw
        }

        return (value == null || value.isEmpty()) ? defaultValue : (T) converter.convert(value);
    }

    /**
     * Interface to implement to parse an attribute String into a Java object
     */
    public static interface StringConverter {

        /**
         * Parse a string
         * @param str the string to parse
         * @return the corresponding Object
         */
        public Object convert(String str);
    }

    static {
        converters.put(int.class, new StringConverter() {
            public Integer convert(String str) {
                if (str == null || str.isEmpty()) {
                    return Integer.MAX_VALUE;
                }
                try {
                    return Integer.parseInt(str);
                } catch (NumberFormatException e) {
                    try {
                        return (int) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Integer(0);
                    }
                }
            }
        });
        converters.put(char.class, new StringConverter() {
            public Object convert(String str) {
                if (str.length() > 0) {
                    return str.charAt(0);
                } else {
                    return new Character((char) 0);
                }
            }
        });
        converters.put(byte.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Byte.parseByte(str);
                } catch (NumberFormatException e) {
                    try {
                        return (byte) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Byte((byte) 0);
                    }
                }
            }
        });
        converters.put(short.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Short.parseShort(str);
                } catch (NumberFormatException e) {
                    try {
                        return (short) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Short((short) 0);
                    }
                }
            }
        });
        converters.put(double.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return Double.NaN;
                }

                try {
                    return Double.parseDouble(str);
                } catch (NumberFormatException ee) {
                    return new Double((double) 0);
                }
            }
        });
        converters.put(float.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Float.parseFloat(str);
                } catch (NumberFormatException ee) {
                    return new Float((float) 0);
                }
            }
        });
        converters.put(boolean.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return false;
                }

                str = str.toLowerCase();
                if (str.equals("on")) {
                    return true;
                }

                if (str.equals("off")) {
                    return false;
                }

                return null;
            }
        });
        converters.put(long.class, new StringConverter() {
            public Object convert(String str) {
                try {
                    return Long.parseLong(str);
                } catch (NumberFormatException e) {
                    try {
                        return (long) Double.parseDouble(str);
                    } catch (NumberFormatException ee) {
                        return new Long((long) 0);
                    }
                }
            }
        });
        converters.put(String.class, new StringConverter() {
            public Object convert(String str) {
                return str;
            }
        });
        converters.put(Object.class, new StringConverter() {
            public Object convert(String str) {
                return str;
            }
        });
        converters.put(Color.class, new StringConverter() {
            public Object convert(String str) {
                return UITools.getColor(str);
            }
        });
        converters.put(KeyStroke.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ +]");
                StringBuilder buffer = new StringBuilder();
                for (int i = 0; i < toks.length - 1; i++) {
                    if (toks[i].equalsIgnoreCase("OSSCKEY")) {
                        buffer.append("OSSCKEY ");
                    } else {
                        buffer.append(toks[i].toLowerCase());
                        buffer.append(" ");
                    }
                }
                buffer.append(toks[toks.length - 1].toUpperCase());
                return ScilabKeyStroke.getKeyStroke(buffer.toString());
            }
        });
        converters.put(ImageIcon.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                ImageIcon icon = null;

                if (str.lastIndexOf('.') == -1) {
                    String path = str;
                    if (path != null) {
                        try {
                            BufferedImage img = ImageIO.read(new File(path));
                            icon = new ImageIcon(img);
                        } catch (Exception e) {
                            return null;
                        }
                    }
                }

                if (icon == null) {
                    try {
                        URL url = new URL(str);
                        BufferedImage img = ImageIO.read(url);
                        icon = new ImageIcon(img);
                    } catch (Exception e) { }
                }

                if (icon == null) {
                    try {
                        BufferedImage img = ImageIO.read(new File(FileTools.getFile(str).getAbsolutePath()));
                        icon = new ImageIcon(img);
                    } catch (Exception e) {
                        return null;
                    }
                }

                if (icon != null) {
                    icon.setDescription(str);
                }

                return icon;
            }
        });
        converters.put(Icon.class, converters.get(ImageIcon.class));
        converters.put(Image.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                if (str.lastIndexOf('.') == -1) {
                    String path = str;
                    if (path != null) {
                        try {
                            return ImageIO.read(new File(path));
                        } catch (Exception e) {
                            return null;
                        }
                    }
                }

                try {
                    URL url = new URL(str);
                    return ImageIO.read(url);
                } catch (Exception e) { }

                try {
                    return ImageIO.read(new File(FileTools.getFile(str).getAbsolutePath()));
                } catch (Exception e) {
                    return null;
                }
            }
        });
        converters.put(Date.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }
                try {
                    return dateformat.parse(str);
                } catch (ParseException e) {
                    return null;
                }
            }
        });
        converters.put(Action.class, new StringConverter() {
            @SuppressWarnings("serial")
            public Object convert(final String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }
                return new AbstractAction() {
                    public void actionPerformed(ActionEvent e) {
                        InterpreterManagement.requestScilabExec(str + "()");
                    }
                };
            }
        });
        converters.put(UITools.FontWeight.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                str = str.toLowerCase();
                return UITools.mapWeight.get(str);
            }
        });
        converters.put(Insets.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[4];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 4; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;
                        }
                    }
                }

                return new Insets(ints[0], ints[1], ints[2], ints[3]);
            }
        });
        converters.put(Rectangle.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[4];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 4; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;
                        }
                    }
                }

                return new Rectangle(ints[0], ints[1], ints[2], ints[3]);
            }
        });
        converters.put(Rectangle2D.Double.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                final double[] d = new double[4];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 4; i++) {
                    try {
                        d[i] = (int) Double.parseDouble(toks[i]);
                    } catch (NumberFormatException ee) {
                        d[i] = 0;
                    }
                }

                return new Rectangle2D.Double(d[0], d[1], d[2], d[3]);
            }
        });
        converters.put(Dimension.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[2];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 2; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;;
                        }
                    }
                }

                return new Dimension(ints[0], ints[1]);
            }
        });
        converters.put(Point.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int[] ints = new int[2];
                String[] toks = str.split("[ ,;]");
                for (int i = 0; i < toks.length && i < 2; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;;
                        }
                    }
                }

                return new Point(ints[0], ints[1]);
            }
        });
        converters.put(int[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ ,;]");
                int[] ints = new int[toks.length];
                for (int i = 0; i < toks.length; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;
                        }
                    }
                }

                return ints;
            }
        });
        converters.put(Integer[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ ,;]");
                Integer[] ints = new Integer[toks.length];
                for (int i = 0; i < toks.length; i++) {
                    try {
                        ints[i] = Integer.parseInt(toks[i]);
                    } catch (NumberFormatException e) {
                        try {
                            ints[i] = (int) Double.parseDouble(toks[i]);
                        } catch (NumberFormatException ee) {
                            ints[i] = 0;
                        }
                    }
                }

                return ints;
            }
        });
        converters.put(int[][].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int rows = 0;
                for (int i = 0; i < str.length(); i++) {
                    if (str.charAt(i) == ';') {
                        rows++;
                    }
                }

                String[] toks = str.split("[ ,;]");
                int[][] ints = new int[rows][toks.length / rows];
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < toks.length / rows; j++) {
                        try {
                            ints[i][j] = Integer.parseInt(toks[i * rows + j]);
                        } catch (NumberFormatException e) {
                            try {
                                ints[i][j] = (int) Double.parseDouble(toks[i * rows + j]);
                            } catch (NumberFormatException ee) {
                                ints[i][j] = 0;
                            }
                        }
                    }
                }

                return ints;
            }
        });
        converters.put(double[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ ,;]");
                double[] dbls = new double[toks.length];
                for (int i = 0; i < toks.length; i++) {
                    try {
                        dbls[i] = Double.parseDouble(toks[i]);
                    } catch (NumberFormatException e) {
                        dbls[i] = 0;
                    }
                }

                return dbls;
            }
        });
        converters.put(Double[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ ,;]");
                Double[] dbls = new Double[toks.length];
                for (int i = 0; i < toks.length; i++) {
                    try {
                        dbls[i] = Double.parseDouble(toks[i]);
                    } catch (NumberFormatException e) {
                        dbls[i] = new Double(0);
                    }
                }

                return dbls;
            }
        });
        converters.put(double[][].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                int rows = 0;
                for (int i = 0; i < str.length(); i++) {
                    if (str.charAt(i) == ';') {
                        rows++;
                    }
                }

                String[] toks = str.split("[ ,;]");
                double[][] dbls = new double[rows][toks.length / rows];
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < toks.length / rows; j++) {
                        try {
                            dbls[i][j] = Double.parseDouble(toks[i * rows + j]);
                        } catch (NumberFormatException e) {
                            dbls[i][j] = 0;
                        }
                    }
                }

                return dbls;
            }
        });
        converters.put(String[].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[ ,]");

                return toks;
            }
        });
        converters.put(String[][].class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                String[] toks = str.split("[;]");
                String[][] strs = new String[toks.length][];
                for (int i = 0; i < toks.length; i++) {
                    strs[i] = toks[i].split("[ ,]");
                }

                return strs;
            }
        });
        converters.put(DecimalFormat.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null) {
                    return null;
                }

                DecimalFormat format;
                if (str.isEmpty()) {
                    format = new DecimalFormat();
                }

                try {
                    format = new DecimalFormat(str);
                } catch (IllegalArgumentException e) {
                    format = (DecimalFormat) NumberFormat.getNumberInstance();
                }

                DecimalFormatSymbols symbols = format.getDecimalFormatSymbols();
                symbols.setDecimalSeparator('.');
                symbols.setGroupingSeparator('\u00A0');

                format.setDecimalFormatSymbols(symbols);

                return format;
            }
        });
        converters.put(File.class, new StringConverter() {
            public Object convert(String str) {
                if (str == null || str.isEmpty()) {
                    return null;
                }

                return new File(str);
            }
        });
        converters.put(UITools.FontUnit.class, new StringConverter() {
            public Object convert(String str) {
                return UITools.FontUnit.fromString(str);
            }
        });
        converters.put(PixelDrawingMode.class, new StringConverter() {
            public PixelDrawingMode convert(String str) {
                return PixelDrawingMode.stringToEnum(str);
            }
        });
        converters.put(RotationType.class, new StringConverter() {
            public RotationType convert(String str) {
                return RotationType.stringToEnum(str);
            }
        });
        converters.put(Antialiasing.class, new StringConverter() {
            public Antialiasing convert(String str) {
                return Antialiasing.stringToEnum(str);
            }
        });
        converters.put(ArcDrawingMethod.class, new StringConverter() {
            public ArcDrawingMethod convert(String str) {
                return ArcDrawingMethod.stringToEnum(str);
            }
        });
        converters.put(AxisLocation.class, new StringConverter() {
            public AxisLocation convert(String str) {
                return AxisLocation.stringToEnum(str);
            }
        });
        converters.put(BoxType.class, new StringConverter() {
            public BoxType convert(String str) {
                return BoxType.stringToEnum(str);
            }
        });
        converters.put(GridPosition.class, new StringConverter() {
            public GridPosition convert(String str) {
                return GridPosition.stringToEnum(str);
            }
        });
        converters.put(MarkSizeUnitType.class, new StringConverter() {
            public MarkSizeUnitType convert(String str) {
                return MarkSizeUnitType.stringToEnum(str);
            }
        });
        converters.put(ClipStateType.class, new StringConverter() {
            public ClipStateType convert(String str) {
                return ClipStateType.stringToEnum(str);
            }
        });
        converters.put(ViewType.class, new StringConverter() {
            public ViewType convert(String str) {
                return ViewType.stringToEnum(str);
            }
        });
        converters.put(TicksDirection.class, new StringConverter() {
            public TicksDirection convert(String str) {
                return TicksDirection.stringToEnum(str);
            }
        });
        converters.put(TicksStyle.class, new StringConverter() {
            public TicksStyle convert(String str) {
                return TicksStyle.stringToEnum(str);
            }
        });
        converters.put(DataMapping.class, new StringConverter() {
            public DataMapping convert(String str) {
                return DataMapping.stringToEnum(str);
            }
        });
        converters.put(Alignment.class, new StringConverter() {
            public Alignment convert(String str) {
                return Alignment.stringToEnum(str);
            }
        });
        converters.put(TextBoxMode.class, new StringConverter() {
            public TextBoxMode convert(String str) {
                return TextBoxMode.stringToEnum(str);
            }
        });
        converters.put(LegendLocation.class, new StringConverter() {
            public LegendLocation convert(String str) {
                return LegendLocation.stringToEnum(str);
            }
        });
        converters.put(LightType.class, new StringConverter() {
            public LightType convert(String str) {
                return LightType.stringToEnum(str);
            }
        });
    }
}
