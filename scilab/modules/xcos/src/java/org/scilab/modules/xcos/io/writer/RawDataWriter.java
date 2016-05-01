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
package org.scilab.modules.xcos.io.writer;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.logging.Level;

import javax.xml.stream.XMLStreamException;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.io.ScilabTypeCoder;

public class RawDataWriter extends ScilabWriter {

    /*
     * Shared map information used to avoid duplicated entry on the dictionary
     */
    private final HashMap<ScilabType, Integer> dictionaryMap;

    public RawDataWriter(XcosWriter writer) {
        super(writer);

        dictionaryMap = new HashMap<>();
    }

    /**
     * Fill the shared dictinary with the encoded mapping
     */
    public void fillSharedDictionary() {
        if (shared.dictionary == null) {
            return;
        }

        // reserve the size with empty values
        shared.dictionary.addAll(Collections.nCopies(dictionaryMap.size(), new ScilabDouble()));

        // push all the values
        for (Entry<ScilabType, Integer> entry : dictionaryMap.entrySet()) {
            shared.dictionary.set(entry.getValue(), entry.getKey());
        }
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {

        switch (kind) {
            case BLOCK:
                writeBlock(uid, kind);
                break;
            case DIAGRAM:
                writeContext(uid, kind);
                break;
            case LINK: // no break on purpose
            case ANNOTATION:  // no break on purpose
            default:
                break;
        }
    }

    private void writeContext(long uid, Kind kind) throws XMLStreamException {
        VectorOfString context = new VectorOfString();
        shared.controller.getObjectProperty(uid, kind, ObjectProperties.DIAGRAM_CONTEXT, context);

        String[] value = new String[context.size()];
        for (int i = 0; i < value.length; i++) {
            value[i] = context.get(i);
        }
        write(value, "context");
    }

    private void writeBlock(long uid, Kind kind) throws XMLStreamException {
        VectorOfDouble vDouble = new VectorOfDouble();
        VectorOfInt vInt = new VectorOfInt();

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.EXPRS, vDouble);
        write(new ScilabTypeCoder().vec2var(vDouble), "exprs");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.RPAR, vDouble);
        write(vDouble, "realParameters");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.IPAR, vInt);
        write(vInt, "integerParameters");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.OPAR, vDouble);
        write(new ScilabTypeCoder().vec2var(vDouble), "objectsParameters");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.NZCROSS, vInt);
        write(vInt, "nbZerosCrossing");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.NMODE, vInt);
        write(vInt, "nmode");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.STATE, vDouble);
        write(vDouble, "state");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.DSTATE, vDouble);
        write(vDouble, "dState");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.ODSTATE, vDouble);
        write(new ScilabTypeCoder().vec2var(vDouble), "oDState");

        shared.controller.getObjectProperty(uid, kind, ObjectProperties.EQUATIONS, vDouble);
        write(new ScilabTypeCoder().vec2var(vDouble), "equations");
    }

    /*
     * Low-level serialization methods
     */

    private void write(String[] value, String as) throws XMLStreamException {
        shared.stream.writeStartElement("Array");
        shared.stream.writeAttribute("as", as);
        shared.stream.writeAttribute("scilabClass", "String[]");

        for (String v : value) {
            shared.stream.writeEmptyElement("add");
            shared.stream.writeAttribute("value", v);
        }

        shared.stream.writeEndElement(); // Array
    }

    private void write(ScilabType value, String as) throws XMLStreamException {
        if (shared.dictionary == null) {
            writeAsXml(value, as);
        } else {
            writeAsBinary(value, as);
        }
    }

    private void writeAsXml(ScilabType value, String as) throws XMLStreamException {
        ScilabTypeEnum type = value.getType();
        final String localName;
        final String scilabClass;
        final String intPrecision;

        /*
         * Get the right descriptor
         */
        switch (type) {
            case sci_boolean:
                localName = "ScilabBoolean";
                scilabClass = null;
                intPrecision = null;
                break;
            case sci_ints:
                localName = "ScilabInteger";
                scilabClass = null;
                intPrecision = ((ScilabInteger) value).getPrec().name();
                break;
            case sci_matrix:
                localName = "ScilabDouble";
                scilabClass = null;
                intPrecision = null;
                break;
            case sci_strings:
                localName = "ScilabString";
                scilabClass = null;
                intPrecision = null;
                break;
            case sci_list:
                localName = "Array";
                scilabClass = "ScilabList";
                intPrecision = null;
                break;
            case sci_mlist:
                localName = "Array";
                scilabClass = "ScilabMList";
                intPrecision = null;
                break;
            case sci_tlist:
                localName = "Array";
                scilabClass = "ScilabTList";
                intPrecision = null;
                break;
            default:
                throw new XMLStreamException("Not handled ScilabType : " + type.name());
        }

        /*
         * Emit the elements
         */
        final boolean isNotEmptyElement = value.getHeight() > 0 &&  value.getWidth() > 0;
        if (isNotEmptyElement) {
            shared.stream.writeStartElement(localName);
        } else {
            shared.stream.writeEmptyElement(localName);
        }
        if (as != null) {
            shared.stream.writeAttribute("as", as);
        }

        if (value instanceof ArrayList) {
            writeList(value, scilabClass);
        } else {
            writeMatrix(value, type, intPrecision);
        }

        if (isNotEmptyElement) {
            shared.stream.writeEndElement();
        }
    }

    private void writeList(ScilabType value, final String scilabClass) throws XMLStreamException {
        shared.stream.writeAttribute("scilabClass", scilabClass);

        @SuppressWarnings("unchecked")
        ArrayList<ScilabType> values = (ArrayList<ScilabType>) value;
        for (ScilabType subValue : values) {
            write(subValue, null);
        }
    }

    @SuppressWarnings("incomplete-switch")
    private void writeMatrix(ScilabType value, ScilabTypeEnum type, final String intPrecision) throws XMLStreamException {
        shared.stream.writeAttribute("height", Integer.toString(value.getHeight()));
        shared.stream.writeAttribute("width", Integer.toString(value.getWidth()));
        if (intPrecision != null) {
            shared.stream.writeAttribute("intPrecision", intPrecision);
        }

        for (int i = 0; i < value.getHeight(); i++) {
            for (int j = 0; j < value.getWidth(); j++) {
                shared.stream.writeEmptyElement("data");
                shared.stream.writeAttribute("line", Integer.toString(i));
                shared.stream.writeAttribute("column", Integer.toString(j));

                switch (type) {
                    case sci_boolean:
                        boolean bValue = ((ScilabBoolean)value).getElement(i, j);
                        shared.stream.writeAttribute("value", Boolean.toString(bValue));
                        break;
                    case sci_ints:
                        long lValue = ((ScilabInteger) value).getElement(i, j);
                        shared.stream.writeAttribute("value", Long.toString(lValue));
                        break;
                    case sci_matrix:
                        double dValue = ((ScilabDouble) value).getRealElement(i, j);
                        shared.stream.writeAttribute("realPart", Double.toString(dValue));
                        if (!((ScilabDouble) value).isReal()) {
                            dValue  = ((ScilabDouble) value).getImaginaryElement(i, j);
                            shared.stream.writeAttribute("imaginaryPart", Double.toString(dValue));
                        }
                        break;
                    case sci_strings:
                        String strValue = ((ScilabString) value).getData()[i][j];
                        shared.stream.writeAttribute("value", strValue);
                        break;
                }
            }
        }
    }

    private void writeAsBinary(ScilabType value, String as) throws XMLStreamException {
        ScilabTypeEnum type = value.getType();
        final String localName;

        /*
         * Get the right descriptor
         */
        switch (type) {
            case sci_boolean:
                localName = "ScilabBoolean";
                break;
            case sci_ints:
                localName = "ScilabInteger";
                break;
            case sci_matrix:
                localName = "ScilabDouble";
                break;
            case sci_strings:
                localName = "ScilabString";
                break;
            case sci_list: // no break on purpose
            case sci_mlist: // no break on purpose
            case sci_tlist:
                localName = "Array";
                break;
            default:
                throw new XMLStreamException("Not handled ScilabType : " + type.name());
        }

        /*
         * Emit the elements
         */

        // if the value has not been mapped, then use the next position (map.size())
        // else re-use the same position
        Integer expectedPosition = dictionaryMap.size();
        Integer position = dictionaryMap.putIfAbsent(value, expectedPosition);
        if (position == null) {
            position = expectedPosition;
        }

        if (XcosWriter.LOG.isLoggable(Level.FINE)) {
            XcosWriter.LOG.fine("put:" + value.toString() + ":" + position.toString());
        }

        shared.stream.writeEmptyElement(localName);
        shared.stream.writeAttribute("as", as);
        shared.stream.writeAttribute("binary", "true");
        shared.stream.writeAttribute("position", position.toString());
    }

    private void write(VectorOfDouble value, String as) throws XMLStreamException {
        // recreate a ScilabDouble for the encoding
        final int length = value.size();
        if (length > 0) {
            double[][] data = new double[1][length];
            value.asByteBuffer(0, length).asDoubleBuffer().get(data[0]);
            write(new ScilabDouble(data), as);
        } else {
            write(new ScilabDouble(), as);
        }
    }

    private void write(VectorOfInt value, String as) throws XMLStreamException {
        // recreate a ScilabInteger for the encoding
        final int length = value.size();
        if (length > 0) {
            int[][] data = new int[1][length];
            value.asByteBuffer(0, length).asIntBuffer().get(data[0]);
            write(new ScilabInteger(data, false), as);
        } else {
            write(new ScilabDouble(), as);
        }
    }
}
