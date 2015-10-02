/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.sax;

import java.beans.BeanDescriptor;
import java.beans.IntrospectionException;
import java.beans.PropertyDescriptor;
import java.beans.SimpleBeanInfo;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.xml.sax.Attributes;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;

class RawDataHandler implements ScilabHandler {

    static class RawDataDescriptor {
        final ObjectProperties as;
        final HandledElement found;
        final String scilabClass;
        final Object value;

        public RawDataDescriptor(ObjectProperties as, HandledElement found, String scilabClass, Object container) {
            // defensive programming
            if (as == null) {
                throw new IllegalAccessError();
            }

            this.as = as;
            this.found = found;
            this.scilabClass = scilabClass;
            this.value = container;
        }
    }

    private final SAXHandler saxHandler;
    private final Map<String, ObjectProperties> propertyMap;

    /**
     * Default constructor
     * @param saxHandler the shared sax handler
     */
    RawDataHandler(SAXHandler saxHandler) {
        this.saxHandler = saxHandler;

        Map<String, ObjectProperties> localPropertyMap = new HashMap<>();
        localPropertyMap.put("context", ObjectProperties.DIAGRAM_CONTEXT);
        localPropertyMap.put("equations", ObjectProperties.EQUATIONS);
        localPropertyMap.put("exprs", ObjectProperties.EXPRS);
        localPropertyMap.put("integerParameters", ObjectProperties.IPAR);
        localPropertyMap.put("nbZerosCrossing", ObjectProperties.NZCROSS);
        localPropertyMap.put("nmode", ObjectProperties.NMODE);
        localPropertyMap.put("objectsParameters", ObjectProperties.OPAR);
        localPropertyMap.put("oDState", ObjectProperties.ODSTATE);
        localPropertyMap.put("points", ObjectProperties.CONTROL_POINTS);
        localPropertyMap.put("realParameters", ObjectProperties.RPAR);
        // TODO might be incomplete
        propertyMap = Collections.unmodifiableMap(localPropertyMap);
    }

    public Object startElement(HandledElement found, Attributes atts) {
        String v;

        switch (found) {
            case ScilabBoolean:
            // no break on purpose
            case ScilabDouble:
            // no break on purpose
            case ScilabInteger:
            // no break on purpose
            case ScilabString:
            // no break on purpose
            case Array: {
                String as = atts.getValue("as");

                /*
                 * first : try to retrieve the already decoded binary data
                 */
                boolean binary = false;
                int position = -1;

                v = atts.getValue("binary");
                if (v != null) {
                    binary = Boolean.valueOf(v);
                }
                v = atts.getValue("position");
                if (v != null) {
                    position = Integer.valueOf(v);
                }

                if (binary && (0 <= position && position < saxHandler.dictionary.size())) {
                    return new RawDataDescriptor(propertyMap.get(as), found, null, saxHandler.dictionary.get(position));
                }

                /*
                 * otherwise : this is not a binary, decode it
                 */
                int height = 0;
                int width = 0;
                String scilabClass = "ScilabDouble";

                v = atts.getValue("height");
                if (v != null) {
                    height = Integer.valueOf(v);
                }
                v = atts.getValue("width");
                if (v != null) {
                    width = Integer.valueOf(v);
                }
                v = atts.getValue("scilabClass");
                if (v != null) {
                    scilabClass = v;
                }

                // allocate the right class
                final Object container;
                switch (found) {
                    case ScilabBoolean:
                        container = new ScilabBoolean(new boolean[height][width]);
                        break;
                    case ScilabDouble:
                        container = new ScilabDouble(new double[height][width]);
                        break;
                    case ScilabInteger:
                        v = atts.getValue("intPrecision");
                        if (v != null) {
                            boolean unsigned = true;
                            switch (ScilabIntegerTypeEnum.valueOf(v)) {
                                case sci_int8:
                                    unsigned = false;
                                // no break on purpose
                                case sci_uint8:
                                    container = new ScilabInteger(new byte[height][width], unsigned);
                                    break;
                                case sci_int16:
                                    unsigned = false;
                                // no break on purpose
                                case sci_uint16:
                                    container = new ScilabInteger(new short[height][width], unsigned);
                                    break;
                                case sci_int32:
                                    unsigned = false;
                                // no break on purpose
                                case sci_uint32:
                                    container = new ScilabInteger(new int[height][width], unsigned);
                                    break;
                                case sci_int64:
                                    unsigned = false;
                                // no break on purpose
                                case sci_uint64:
                                    container = new ScilabInteger(new long[height][width], unsigned);
                                    break;
                                default:
                                    container = new ScilabInteger(new long[height][width], false);
                                    break;
                            }
                        } else {
                            container = new ScilabInteger(new long[height][width], false);
                        }
                        break;
                    case ScilabString:
                        container = new ScilabString(new String[height][width]);
                        break;
                    default: // case Array
                        if (scilabClass.equalsIgnoreCase("ScilabMList")) {
                            container = new ScilabMList();
                        } else if (scilabClass.equalsIgnoreCase("ScilabTList")) {
                            container = new ScilabTList();
                        } else if (scilabClass.equalsIgnoreCase("ScilabList")) {
                            container = new ScilabList();
                        } else {
                            container = new ArrayList<>();
                        }
                        break;
                }

                return new RawDataDescriptor(propertyMap.get(as), found, scilabClass, container);
            }
            case add: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof RawDataDescriptor)) {
                    return null;
                }
                RawDataDescriptor fieldValue = (RawDataDescriptor) saxHandler.parents.peek();

                switch (fieldValue.as) {
                    case DIAGRAM_CONTEXT: {
                        ArrayList<String> container = ((ArrayList<String>) fieldValue.value);
                        container.add(atts.getValue("value"));
                        break;
                    }
                    case CONTROL_POINTS: {
                        // FIXME not implemented yet
                        break;
                    }
                    default:
                        break;
                }

                break;
            }
            case data: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof RawDataDescriptor)) {
                    return null;
                }
                RawDataDescriptor fieldValue = (RawDataDescriptor) saxHandler.parents.peek();

                /*
                 * Decode the position and then switch per container klass
                 */

                int column = 0;
                int line = 0;

                v = atts.getValue("column");
                if (v != null) {
                    column = Integer.valueOf(v);
                }
                v = atts.getValue("line");
                if (v != null) {
                    line = Integer.valueOf(v);
                }

                switch (fieldValue.found) {
                    case ScilabBoolean: {
                        ScilabBoolean localScilabValue = ((ScilabBoolean) fieldValue.value);
                        boolean[][] data = localScilabValue.getData();

                        v = atts.getValue("value");
                        if (v != null) {
                            data[line][column] = Boolean.valueOf(v);
                        }
                        break;
                    }
                    case ScilabDouble: {
                        ScilabDouble localScilabValue = ((ScilabDouble) fieldValue.value);
                        double[][] realPartData = localScilabValue.getRealPart();
                        double[][] imaginaryPartData = localScilabValue.getImaginaryPart();

                        v = atts.getValue("realPart");
                        if (v != null) {
                            realPartData[line][column] = Double.valueOf(v);
                        }

                        v = atts.getValue("imaginaryPart");
                        if (v != null) {
                            // allocate the imaginary part on demand
                            if (imaginaryPartData == null) {
                                imaginaryPartData = new double[localScilabValue.getHeight()][localScilabValue.getWidth()];
                                localScilabValue.setImaginaryPart(imaginaryPartData);
                            }

                            imaginaryPartData[line][column] = Double.valueOf(v);
                        }
                        break;
                    }
                    case ScilabInteger: {
                        ScilabInteger localScilabValue = ((ScilabInteger) fieldValue.value);
                        ScilabIntegerTypeEnum precision = localScilabValue.getPrec();

                        v = atts.getValue("value");
                        if (v != null) {
                            switch (precision) {
                                case sci_int8:
                                case sci_uint8:
                                    localScilabValue.getDataAsByte()[line][column] = Byte.valueOf(v);
                                    break;
                                case sci_int16:
                                case sci_uint16:
                                    localScilabValue.getDataAsShort()[line][column] = Short.valueOf(v);
                                    break;
                                case sci_int32:
                                case sci_uint32:
                                    localScilabValue.getDataAsInt()[line][column] = Integer.valueOf(v);
                                    break;
                                case sci_int64:
                                case sci_uint64:
                                    localScilabValue.getDataAsLong()[line][column] = Long.valueOf(v);
                                    break;
                            }
                        }
                        break;
                    }
                    case ScilabString: {
                        ScilabString localScilabValue = ((ScilabString) fieldValue.value);
                        String[][] data = localScilabValue.getData();
                        data[line][column] = atts.getValue("value");
                        break;
                    }
                    default:
                        break;
                }
            }
            default:
                throw new IllegalArgumentException();
        }

        return null;
    }

    public void endElement(HandledElement found) {
        switch (found) {
            case Array:
            // no break on purpose
            case ScilabBoolean:
            // no break on purpose
            case ScilabDouble:
            // no break on purpose
            case ScilabInteger:
            // no break on purpose
            case ScilabString: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof RawDataDescriptor)) {
                    return;
                }
                RawDataDescriptor fieldValue = (RawDataDescriptor) saxHandler.parents.peek();
                Object parent = saxHandler.parents.peek(1);

                switch (fieldValue.as) {
                    case CONTROL_POINTS: {
                        // defensive programming
                        if (!(parent instanceof mxGeometry)) {
                            return;
                        }
                        mxGeometry geom = (mxGeometry) parent;
                        ArrayList<mxPoint> value = (ArrayList<mxPoint>) fieldValue.value;
                        geom.setPoints(value);
                        break;
                    }
                    case DIAGRAM_CONTEXT: {
                        // defensive programming
                        if (!(parent instanceof ScicosObjectOwner)) {
                            return;
                        }
                        ScicosObjectOwner diagram = (ScicosObjectOwner) parent;

                        ArrayList<String> value = (ArrayList<String>) fieldValue.value;
                        VectorOfString ctx = new VectorOfString(value.size());
                        for (int i = 0; i < value.size(); i++) {
                            ctx.set(i, value.get(i));
                        }
                        saxHandler.controller.setObjectProperty(diagram.getUID(), diagram.getKind(), ObjectProperties.DIAGRAM_CONTEXT, ctx);
                        break;
                    }
                    default:
                        System.err.println("RawDataHandler not handled: " + fieldValue.as);
                        break;

                }
                break;
            }
            case add:
                break;
            case data:
                break;
            default:
                throw new IllegalArgumentException();
        }
    }

}