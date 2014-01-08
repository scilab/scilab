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
import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.axes.TicksProperty;
import org.scilab.modules.graphic_objects.utils.LineType;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabHandle;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;

/**
 * Class to give the possibility to convert Scilab data into a "classical" Java Object.
 */
public final class ScilabTypeConverters {

    static final Map < Class<?>, ScilabTypeConverter > converters = new HashMap < Class<?>, ScilabTypeConverter > ();
    private static final Map < Class<?>, Class <? extends ScilabType >> preferred = new HashMap < Class<?>, Class <? extends ScilabType >> ();

    static {
        getPreferred().put(byte.class, ScilabInteger.class);
        getPreferred().put(byte[].class, ScilabInteger.class);
        getPreferred().put(byte[][].class, ScilabInteger.class);
        getPreferred().put(short.class, ScilabInteger.class);
        getPreferred().put(short[].class, ScilabInteger.class);
        getPreferred().put(short[][].class, ScilabInteger.class);
        getPreferred().put(int.class, ScilabInteger.class);
        getPreferred().put(int[].class, ScilabInteger.class);
        getPreferred().put(int[][].class, ScilabInteger.class);
        getPreferred().put(long.class, ScilabInteger.class);
        getPreferred().put(long[].class, ScilabInteger.class);
        getPreferred().put(long[][].class, ScilabInteger.class);
        getPreferred().put(boolean.class, ScilabBoolean.class);
        getPreferred().put(boolean[].class, ScilabBoolean.class);
        getPreferred().put(boolean[][].class, ScilabBoolean.class);
        getPreferred().put(String.class, ScilabString.class);
        getPreferred().put(String[].class, ScilabString.class);
        getPreferred().put(String[][].class, ScilabString.class);
        getPreferred().put(double.class, ScilabDouble.class);
        getPreferred().put(double[].class, ScilabDouble.class);
        getPreferred().put(double[][].class, ScilabDouble.class);
    }

    /**
     * Register a converter for a target class
     * @param clazz the target class
     * @param converter the converter
     */
    public static final void registerConverter(final Class<?> clazz, final ScilabTypeConverter converter) {
        converters.put(clazz, converter);
    }

    /**
     * Get a converter for a target class
     * @param clazz the target class
     * @return the corresponding converter or null
     */
    public static final ScilabTypeConverter getConverter(final Class<?> clazz) {
        return converters.get(clazz);
    }

    /**
     * Check if a converter for a target class is existing
     * @param clazz the target class
     * @return true if a converter is existing
     */
    public static final boolean containConverter(final Class<?> clazz) {
        return converters.containsKey(clazz);
    }

    /**
     * Get a Java object corresponding to the given target class
     * @param clazz the target class
     * @param value the Scilab value
     * @return the corresponding Java object
     */
    @SuppressWarnings("unchecked")
    public static final <T> T getObjectFromValue(final Class<T> clazz, final ScilabType value) {
        ScilabTypeConverter converter;
        if (value == null) {
            return StringConverters.getObjectFromValue(clazz, null);
        }

        if ((clazz != String.class && clazz != String[].class && clazz != String[][].class) && value.getType() == ScilabTypeEnum.sci_strings && value.getWidth() == 1 && value.getHeight() == 1) {
            // We have a Scilab string in input so we will use a String converter (since the target is not a string)
            converter = null;
        } else {
            converter = converters.get(clazz);
        }

        if (converter == null) {
            String str = null;
            if (value != null && value.getType() == ScilabTypeEnum.sci_strings && !value.isEmpty()) {
                str = ((ScilabString) value).getData()[0][0];
            }

            return StringConverters.getObjectFromValue(clazz, str);
        }

        return (T) converter.convert(value);
    }

    /**
     * Get a Java object corresponding to the given target class
     * @param clazz the target class
     * @param value the Scilab value
     * @param defaultValue the default value to return
     * @return the corresponding Java object
     */
    @SuppressWarnings("unchecked")
    public static final <T> T getObjectFromValue(final Class<T> clazz, final ScilabType value, final T defaultValue) {
        ScilabTypeConverter converter;
        if (value == null) {
            return StringConverters.getObjectFromValue(clazz, null);
        }

        if (clazz != String.class && value.getType() == ScilabTypeEnum.sci_strings) {
            converter = null;
        } else {
            converter = converters.get(clazz);
        }

        if (converter == null) {
            String str = null;
            if (value != null && value.getType() == ScilabTypeEnum.sci_strings && !value.isEmpty()) {
                str = ((ScilabString) value).getData()[0][0];
            }

            return StringConverters.getObjectFromValue(clazz, str, defaultValue);
        }

        return (value == null) ? defaultValue : (T) converter.convert(value);
    }

    public static Map < Class<?>, Class <? extends ScilabType >> getPreferred() {
        return preferred;
    }

    /**
     * Interface to implement to parse an attribute String into a Java object
     */
    public static interface ScilabTypeConverter {

        /**
         * Convert a ScilabType into a "classic" Java object (e.g., a row of 4 doubles into a Color object)
         * @param value the Scilab value to convert
         * @return the corresponding Object
         */
        public Object convert(ScilabType value);
    }

    static {
        converters.put(int.class, new ScilabTypeConverter() {
            public Integer convert(ScilabType value) {
                if (value == null ||
                        (value.getType() != ScilabTypeEnum.sci_matrix &&
                         value.getType() != ScilabTypeEnum.sci_handles) ||
                value.isEmpty()) {
                    return Integer.MAX_VALUE;
                }

                if (value.getType() == ScilabTypeEnum.sci_handles) {
                    long handle = ((ScilabHandle) value).getData()[0][0];
                    return ScilabNativeView.ScilabNativeView__getObjectFromHandle(handle);
                }
                return (int) ((ScilabDouble) value).getRealPart()[0][0];
            }
        });
        converters.put(double.class, new ScilabTypeConverter() {
            public Double convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return Double.NaN;
                }
                return ((ScilabDouble) value).getRealPart()[0][0];
            }
        });
        converters.put(float.class, new ScilabTypeConverter() {
            public Float convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return Float.NaN;
                }

                return (float) ((ScilabDouble) value).getRealPart()[0][0];
            }
        });
        converters.put(boolean.class, new ScilabTypeConverter() {
            public Boolean convert(ScilabType value) {
                switch (value.getType()) {
                    case sci_boolean :
                        return ((ScilabBoolean) value).getData()[0][0];
                    case sci_strings :
                        String sz = ((ScilabString)value).getData()[0][0];
                        if (sz.equalsIgnoreCase("off")) {
                            return false;
                        } else if (sz.equalsIgnoreCase("on")) {
                            return true;
                        } else {
                            return null;
                        }
                    default :
                        return null;
                }
            }
        });
        converters.put(boolean[].class, new ScilabTypeConverter() {
            public boolean[] convert(ScilabType value) {
                switch (value.getType()) {
                    case sci_boolean : {
                        ScilabBoolean bool = (ScilabBoolean) value;
                        int iSize = bool.getHeight() * bool.getWidth();
                        boolean[] result = new boolean[iSize];
                        for (int i = 0 ; i < iSize ; i++) {
                            result[i] = bool.getData()[i][0];
                        }
                        return result;
                    }
                    case sci_strings : {
                        ScilabString string = (ScilabString)value;
                        int iSize = string.getHeight() * string.getWidth();
                        boolean[] result = new boolean[iSize];

                        for (int i = 0 ; i < iSize ; i++) {
                            String sz = string.getData()[i][0];
                            if (sz.equalsIgnoreCase("on")) {
                                result[i] = true;
                            } else {
                                result[i] = false;
                            }
                        }

                        return result;
                    }
                    default :
                        return null;
                }
            }
        });
        converters.put(long.class, new ScilabTypeConverter() {
            public Long convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return 0L;
                }
                return (long) ((ScilabDouble) value).getRealPart()[0][0];
            }
        });
        converters.put(String.class, new ScilabTypeConverter() {
            public String convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_strings || value.isEmpty()) {
                    return null;
                }
                return ((ScilabString) value).getData()[0][0];
            }
        });
        converters.put(String[].class, new ScilabTypeConverter() {
            public String[] convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_strings || value.isEmpty()) {
                    return null;
                }
                String[][] s = ((ScilabString) value).getData();
                int r = s.length;
                int c = s[0].length;
                String[] ret = new String[r * c];
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        ret[j + c * i] = s[i][j];
                    }
                }
                return ret;
            }
        });
        converters.put(String[][].class, new ScilabTypeConverter() {
            public String[][] convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_strings || value.isEmpty()) {
                    return null;
                }

                String[][] s = ((ScilabString) value).getData();
                String[][] ss = new String[s[0].length][s.length];
                for (int i = 0; i < s[0].length; i++) {
                    for (int j = 0; j < s.length; j++) {
                        ss[i][j] = s[j][i];
                    }
                }

                return ss;
            }
        });
        converters.put(int[].class, new ScilabTypeConverter() {
            public int[] convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return null;
                }
                double[][] s = ((ScilabDouble) value).getRealPart();
                int r = s.length;
                int c = s[0].length;
                int[] ret = new int[r * c];
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        ret[j + c * i] = (int) s[i][j];
                    }
                }
                return ret;
            }
        });
        converters.put(Integer[].class, new ScilabTypeConverter() {
            public Integer[] convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return null;
                }
                double[][] s = ((ScilabDouble) value).getRealPart();
                int r = s.length;
                int c = s[0].length;
                Integer[] ret = new Integer[r * c];
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        ret[j + c * i] = (int) s[i][j];
                    }
                }
                return ret;
            }
        });
        converters.put(double[].class, new ScilabTypeConverter() {
            public double[] convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return null;
                }
                double[][] s = ((ScilabDouble) value).getRealPart();
                int r = s.length;
                int c = s[0].length;
                double[] ret = new double[r * c];
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        ret[j + c * i] = s[i][j];
                    }
                }
                return ret;
            }
        });
        converters.put(Double[].class, new ScilabTypeConverter() {
            public Double[] convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return null;
                }
                double[][] s = ((ScilabDouble) value).getRealPart();
                int r = s.length;
                int c = s[0].length;
                Double[] ret = new Double[r * c];
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        ret[j + c * i] = s[i][j];
                    }
                }
                return ret;
            }
        });
        converters.put(Color.class, new ScilabTypeConverter() {
            public Color convert(ScilabType value) {
                if (value == null || value.isEmpty()) {
                    return null;
                }

                if (value.getType() == ScilabTypeEnum.sci_matrix) {
                    // We expect to have [a,b,c,d] or [a;b;c;d]
                    // So the structure of the ScilabDouble is depending if we have a row or a col
                    double[][] data = ((ScilabDouble) value).getRealPart();
                    try {
                        if (data.length >= 3 && data[0].length == 1) {
                            return new Color((float) data[0][0], (float) data[1][0], (float) data[2][0], (float) ((data.length == 4) ? data[3][0] : 1));
                        } else if (data.length == 1 && data[0].length >= 3) {
                            return new Color((float) data[0][0], (float) data[0][1], (float) data[0][2], (float) ((data[0].length == 4) ? data[0][3] : 1));
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                } else if (value.getType() == ScilabTypeEnum.sci_ints) {
                    // Data can be integers too
                    long[][] data = ((ScilabInteger) value).getData();
                    try {
                        if (data.length >= 3 && data[0].length == 1) {
                            return new Color((int) data[0][0], (int) data[1][0], (int) data[2][0], (int) ((data.length == 4) ? data[3][0] : 255));
                        } else if (data.length == 1 && data[0].length >= 3) {
                            return new Color((int) data[0][0], (int) data[0][1], (int) data[0][2], (int) ((data[0].length == 4) ? data[0][3] : 255));
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                }

                return null;
            }
        });
        converters.put(Insets.class, new ScilabTypeConverter() {
            public Insets convert(ScilabType value) {
                if (value == null || value.isEmpty()) {
                    return null;
                }

                if (value.getType() == ScilabTypeEnum.sci_matrix) {
                    double[][] data = ((ScilabDouble) value).getRealPart();
                    try {
                        if (data.length >= 4 && data[0].length == 1) {
                            return new Insets((int) data[0][0], (int) data[1][0], (int) data[2][0], (int) data[3][0]);
                        } else if (data.length == 1 && data[0].length >= 4) {
                            return new Insets((int) data[0][0], (int) data[0][1], (int) data[0][2], (int) data[0][3]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                } else if (value.getType() == ScilabTypeEnum.sci_ints) {
                    long[][] data = ((ScilabInteger) value).getData();
                    try {
                        if (data.length >= 4 && data[0].length == 1) {
                            return new Insets((int) data[0][0], (int) data[1][0], (int) data[2][0], (int) data[3][0]);
                        } else if (data.length == 1 && data[0].length >= 4) {
                            return new Insets((int) data[0][0], (int) data[0][1], (int) data[0][2], (int) data[0][3]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                }

                return null;
            }
        });
        converters.put(Rectangle.class, new ScilabTypeConverter() {
            public Rectangle convert(ScilabType value) {
                if (value == null || value.isEmpty()) {
                    return null;
                }

                if (value.getType() == ScilabTypeEnum.sci_matrix) {
                    double[][] data = ((ScilabDouble) value).getRealPart();
                    try {
                        if (data.length >= 4 && data[0].length == 1) {
                            return new Rectangle((int) data[0][0], (int) data[1][0], (int) data[2][0], (int) data[3][0]);
                        } else if (data.length == 1 && data[0].length >= 4) {
                            return new Rectangle((int) data[0][0], (int) data[0][1], (int) data[0][2], (int) data[0][3]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                } else if (value.getType() == ScilabTypeEnum.sci_ints) {
                    long[][] data = ((ScilabInteger) value).getData();
                    try {
                        if (data.length >= 4 && data[0].length == 1) {
                            return new Rectangle((int) data[0][0], (int) data[1][0], (int) data[2][0], (int) data[3][0]);
                        } else if (data.length == 1 && data[0].length >= 4) {
                            return new Rectangle((int) data[0][0], (int) data[0][1], (int) data[0][2], (int) data[0][3]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                }

                return null;
            }
        });
        converters.put(Rectangle2D.Double.class, new ScilabTypeConverter() {
            public Rectangle2D.Double convert(ScilabType value) {
                if (value == null || value.isEmpty()) {
                    return null;
                }

                if (value.getType() == ScilabTypeEnum.sci_matrix) {
                    double[][] data = ((ScilabDouble) value).getRealPart();
                    try {
                        if (data.length >= 4 && data[0].length == 1) {
                            return new Rectangle2D.Double(data[0][0], data[1][0], data[2][0], data[3][0]);
                        } else if (data.length == 1 && data[0].length >= 4) {
                            return new Rectangle2D.Double(data[0][0], data[0][1], data[0][2], data[0][3]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                } else if (value.getType() == ScilabTypeEnum.sci_ints) {
                    long[][] data = ((ScilabInteger) value).getData();
                    try {
                        if (data.length >= 4 && data[0].length == 1) {
                            return new Rectangle2D.Double((double) data[0][0], (double) data[1][0], (double) data[2][0], (double) data[3][0]);
                        } else if (data.length == 1 && data[0].length >= 4) {
                            return new Rectangle2D.Double((double) data[0][0], (double) data[0][1], (double) data[0][2], (double) data[0][3]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                }

                return null;
            }
        });
        converters.put(Dimension.class, new ScilabTypeConverter() {
            public Dimension convert(ScilabType value) {
                if (value == null || value.isEmpty()) {
                    return null;
                }

                if (value.getType() == ScilabTypeEnum.sci_matrix) {
                    double[][] data = ((ScilabDouble) value).getRealPart();
                    try {
                        if (data.length >= 2 && data[0].length == 1) {
                            return new Dimension((int) data[0][0], (int) data[1][0]);
                        } else if (data.length == 1 && data[0].length >= 2) {
                            return new Dimension((int) data[0][0], (int) data[0][1]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                } else if (value.getType() == ScilabTypeEnum.sci_ints) {
                    long[][] data = ((ScilabInteger) value).getData();
                    try {
                        if (data.length >= 2 && data[0].length == 1) {
                            return new Dimension((int) data[0][0], (int) data[1][0]);
                        } else if (data.length == 1 && data[0].length >= 2) {
                            return new Dimension((int) data[0][0], (int) data[0][1]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                }

                return null;
            }
        });
        converters.put(Point.class, new ScilabTypeConverter() {
            public Point convert(ScilabType value) {
                if (value == null || value.isEmpty()) {
                    return null;
                }

                if (value.getType() == ScilabTypeEnum.sci_matrix) {
                    double[][] data = ((ScilabDouble) value).getRealPart();
                    try {
                        if (data.length >= 2 && data[0].length == 1) {
                            return new Point((int) data[0][0], (int) data[1][0]);
                        } else if (data.length == 1 && data[0].length >= 2) {
                            return new Point((int) data[0][0], (int) data[0][1]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                } else if (value.getType() == ScilabTypeEnum.sci_ints) {
                    long[][] data = ((ScilabInteger) value).getData();
                    try {
                        if (data.length >= 2 && data[0].length == 1) {
                            return new Point((int) data[0][0], (int) data[1][0]);
                        } else if (data.length == 1 && data[0].length >= 2) {
                            return new Point((int) data[0][0], (int) data[0][1]);
                        }
                    } catch (IllegalArgumentException e) {
                        return null;
                    }
                }

                return null;
            }
        });
        converters.put(LineType.class, new ScilabTypeConverter() {
            public LineType convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_matrix || value.isEmpty()) {
                    return LineType.SOLID;
                }
                return LineType.intToEnum((int) ((ScilabDouble)value).getRealPart()[0][0]);
            }
        });

        converters.put(TicksProperty.class, new ScilabTypeConverter() {
            public TicksProperty convert(ScilabType value) {
                if (value == null || value.getType() != ScilabTypeEnum.sci_tlist || value.isEmpty()) {
                    return null;
                }

                ScilabTList tlist = (ScilabTList)value;

                //extract data from TList

                //locations
                if (tlist.get(1).getType() != ScilabTypeEnum.sci_matrix) {
                    return null;
                }

                ScilabDouble dblLoc = (ScilabDouble)tlist.get(1);
                double[][] dd = dblLoc.getRealPart();
                Double[] locations = new Double[dd[0].length];
                for (int i = 0 ; i < dd[0].length ; i++) {
                    locations[i] = dd[0][i];
                }

                //labels
                if (tlist.get(2).getType() != ScilabTypeEnum.sci_strings) {
                    return null;
                }

                ScilabString stLab = (ScilabString)tlist.get(2);
                String[] labels = stLab.getData()[0];

                //check data sizes
                if (labels.length != locations.length) {
                    return null;
                }


                TicksProperty ticks = new TicksProperty();
                ticks.setLocations(locations);
                ticks.setLabelsStrings(labels);
                return ticks;
            }
        });
    }
}
