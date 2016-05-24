/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.xcos.io.sax;

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
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.io.HandledElement;
import org.scilab.modules.xcos.io.ScilabTypeCoder;
import org.scilab.modules.xcos.io.scicos.DiagramElement;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.xml.sax.Attributes;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;

class RawDataHandler implements ScilabHandler {

    static class RawDataDescriptor {
        final ObjectProperties as;
        final HandledElement found;
        final String scilabClass;
        final Object value;

        public RawDataDescriptor(ObjectProperties as, HandledElement found, String scilabClass, Object container) {
            this.as = as;
            this.found = found;
            this.scilabClass = scilabClass;
            this.value = container;
        }
    }

    private final XcosSAXHandler saxHandler;
    private final Map<String, ObjectProperties> propertyMap;

    /**
     * Default constructor
     *
     * @param saxHandler
     *            the shared sax handler
     */
    RawDataHandler(XcosSAXHandler saxHandler) {
        this.saxHandler = saxHandler;

        Map<String, ObjectProperties> localPropertyMap = new HashMap<>();
        localPropertyMap.put("context", ObjectProperties.DIAGRAM_CONTEXT);
        localPropertyMap.put("exprs", ObjectProperties.EXPRS);
        localPropertyMap.put("realParameters", ObjectProperties.RPAR);
        localPropertyMap.put("integerParameters", ObjectProperties.IPAR);
        localPropertyMap.put("objectsParameters", ObjectProperties.OPAR);
        localPropertyMap.put("nbZerosCrossing", ObjectProperties.NZCROSS);
        localPropertyMap.put("nmode", ObjectProperties.NMODE);
        localPropertyMap.put("state", ObjectProperties.STATE);
        localPropertyMap.put("dState", ObjectProperties.DSTATE);
        localPropertyMap.put("oDState", ObjectProperties.ODSTATE);
        localPropertyMap.put("equations", ObjectProperties.EQUATIONS);
        localPropertyMap.put("points", ObjectProperties.CONTROL_POINTS);
        propertyMap = Collections.unmodifiableMap(localPropertyMap);
    }

    /*
     * Implement the ScilabHandler interface to decode the data
     */

    @Override
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

                if (binary &&
                        saxHandler.dictionary != null &&
                        (0 <= position && position < saxHandler.dictionary.size())) {
                    final ObjectProperties asProp = propertyMap.get(as);
                    return new RawDataDescriptor(asProp, found, null, saxHandler.dictionary.get(position));
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

                // allocate the right class and push it to a descriptor
                final Object container = allocateDataType(found, atts, height, width, scilabClass);
                final ObjectProperties asProp = propertyMap.get(as);
                return new RawDataDescriptor(asProp, found, scilabClass, container);
            }
            case add: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof RawDataDescriptor)) {
                    return null;
                }
                RawDataDescriptor fieldValue = (RawDataDescriptor) saxHandler.parents.peek();

                switch (fieldValue.as) {
                    case DIAGRAM_CONTEXT: {
                        ArrayList container = ((ArrayList) fieldValue.value);
                        container.add(atts.getValue("value"));
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
                            if (localScilabValue.isReal()) {
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
                break;
            }
            default:
                throw new IllegalArgumentException();
        }

        return null;
    }

    /**
     * Allocate a {@link ScilabType} datatype accordingly to the decoded descriptors
     *
     * @param found
     *            the decoded element
     * @param atts
     *            the attributes of the element
     * @param height
     *            decoded height
     * @param width
     *            decoded width
     * @param scilabClass
     *            decoded scilabClass
     * @return the container
     */
    @SuppressWarnings("fallthrough")
    private Object allocateDataType(HandledElement found, Attributes atts, int height, int width, String scilabClass) {
        String v;
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
                if ("ScilabMList".equals(scilabClass)) {
                    container = new ScilabMList();
                } else if ("ScilabTList".equals(scilabClass)) {
                    container = new ScilabTList();
                } else if ("ScilabList".equals(scilabClass)) {
                    container = new ScilabList();
                } else {
                    container = new ScilabList();
                }
                break;
        }
        return container;
    }

    @Override
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

                // when we are decoding data into a list "as" is null
                if (fieldValue.as == null) {
                    if (!(parent instanceof RawDataDescriptor)) {
                        return;
                    }

                    ArrayList<Object> parentList = (ArrayList<Object>) ((RawDataDescriptor) parent).value;
                    parentList.add(fieldValue.value);
                    return;
                }

                switch (fieldValue.as) {
                    case CONTROL_POINTS: {
                        // defensive programming
                        if (!(parent instanceof mxGeometry)) {
                            return;
                        }
                        mxGeometry geom = (mxGeometry) parent;

                        @SuppressWarnings("unchecked")
                        ArrayList<mxPoint> value = (ArrayList<mxPoint>) fieldValue.value;
                        geom.setPoints(value);
                        break;
                    }
                    case DIAGRAM_CONTEXT: {
                        // defensive programming
                        if (!(parent instanceof XcosCell)) {
                            return;
                        }
                        XcosCell diagramOrSuperBlockRoot = (XcosCell) parent;

                        @SuppressWarnings("unchecked")
                        ArrayList value = (ArrayList) fieldValue.value;
                        VectorOfString ctx = new VectorOfString(value.size());
                        for (int i = 0; i < value.size(); i++) {
                            ctx.set(i, (String) value.get(i));
                        }
                        saxHandler.controller.setObjectProperty(diagramOrSuperBlockRoot.getUID(), diagramOrSuperBlockRoot.getKind(), ObjectProperties.DIAGRAM_CONTEXT, ctx);
                        break;
                    }
                    case STATE:
                    case DSTATE:
                    case RPAR: {
                        // defensive programming
                        if (!(parent instanceof XcosCell)) {
                            return;
                        }
                        XcosCell cell = (XcosCell) parent;
                        if (fieldValue.as == ObjectProperties.RPAR && fieldValue.value instanceof ScilabMList) {
                            // CORNER CASE for partially decoded sub-diagram hierarchy
                            // decode the rpar as a subdiagram using the legacy decoders
                            // when there is no children
                            VectorOfScicosID children = new VectorOfScicosID();
                            saxHandler.controller.getObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.CHILDREN, children);
                            if (children.size() == 0) {
                                try {
                                    new DiagramElement(saxHandler.controller).decode((ScilabMList) fieldValue.value, new XcosDiagram(saxHandler.controller, cell.getUID(), cell.getKind(), cell.getId()));
                                } catch (ScicosFormatException e) {
                                }
                            }
                            return;
                        }

                        VectorOfDouble vec;
                        if (fieldValue.value instanceof ScilabDouble) {
                            // defensive programming against old schema
                            ScilabDouble value = (ScilabDouble) fieldValue.value;

                            vec = new VectorOfDouble(value.getHeight() * value.getWidth());
                            for (int i = 0; i < value.getWidth(); i++) {
                                for (int j = 0; j < value.getHeight(); j++) {
                                    vec.set(value.getHeight() * i + j, value.getRealElement(j, i));
                                }
                            }
                            saxHandler.controller.setObjectProperty(cell.getUID(), cell.getKind(), fieldValue.as, vec);
                        }
                        break;
                    }
                    case NZCROSS:
                    case NMODE:
                    case IPAR: {
                        // defensive programming
                        if (!(parent instanceof XcosCell)) {
                            return;
                        }
                        XcosCell cell = (XcosCell) parent;

                        VectorOfInt vec = null;
                        if (fieldValue.value instanceof ScilabDouble) {
                            // defensive programming against old schema
                            ScilabDouble value = (ScilabDouble) fieldValue.value;

                            vec = new VectorOfInt(value.getHeight() * value.getWidth());
                            for (int i = 0; i < value.getWidth(); i++) {
                                for (int j = 0; j < value.getHeight(); j++) {
                                    vec.set(value.getHeight() * i + j, (int) value.getRealElement(j, i));
                                }
                            }
                        } else if (fieldValue.value instanceof ScilabInteger) {
                            // defensive programming against old schema
                            ScilabInteger value = (ScilabInteger) fieldValue.value;

                            vec = new VectorOfInt(value.getHeight() * value.getWidth());
                            for (int i = 0; i < value.getWidth(); i++) {
                                for (int j = 0; j < value.getHeight(); j++) {
                                    int index = value.getHeight() * i + j;
                                    switch (value.getPrec()) {
                                        case sci_int8:
                                        case sci_uint8:
                                            vec.set(index, value.getByteElement(j, i));
                                            break;
                                        case sci_int16:
                                        case sci_uint16:
                                            vec.set(index, value.getShortElement(j, i));
                                            break;
                                        case sci_int32:
                                        case sci_uint32:
                                            vec.set(index, value.getIntElement(j, i));
                                            break;
                                        case sci_int64:
                                        case sci_uint64:
                                            vec.set(index, (int) value.getLongElement(j, i));
                                            break;
                                    }
                                }
                            }
                        }
                        if (vec != null) {
                            saxHandler.controller.setObjectProperty(cell.getUID(), cell.getKind(), fieldValue.as, vec);
                        }
                        break;

                    }
                    case EXPRS:
                    case EQUATIONS:
                    case OPAR:
                    case ODSTATE: {
                        // defensive programming
                        if (!(parent instanceof XcosCell)) {
                            return;
                        }
                        XcosCell cell = (XcosCell) parent;
                        ScilabType value = (ScilabType) fieldValue.value;

                        VectorOfDouble vec = new ScilabTypeCoder().var2vec(value);
                        saxHandler.controller.setObjectProperty(cell.getUID(), cell.getKind(), fieldValue.as, vec);
                        break;
                    }
                    default:
                        XcosSAXHandler.LOG.warning("RawDataHandler not handled: " + fieldValue.as);
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
