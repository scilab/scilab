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
package org.scilab.modules.xcos.io.writer;

import javax.xml.stream.XMLStreamException;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;

public class JGraphXWriter extends ScilabWriter {

    public JGraphXWriter(XcosWriter writer) {
        super(writer);
    }

    @Override
    public void write(long uid, Kind kind) throws XMLStreamException {
        VectorOfDouble vDouble = new VectorOfDouble();

        switch (kind) {
            case ANNOTATION:
            case BLOCK:
                shared.controller.getObjectProperty(uid, kind, ObjectProperties.GEOMETRY, vDouble);

                shared.stream.writeEmptyElement("mxGeometry");
                shared.stream.writeAttribute("as", "geometry");
                shared.stream.writeAttribute("x", Double.toString(vDouble.get(0)));
                shared.stream.writeAttribute("y", Double.toString(vDouble.get(1)));
                shared.stream.writeAttribute("width", Double.toString(vDouble.get(2)));
                shared.stream.writeAttribute("height", Double.toString(vDouble.get(3)));
                break;
            case LINK:
                shared.controller.getObjectProperty(uid, kind, ObjectProperties.CONTROL_POINTS, vDouble);
                int i = 0;
                final int nbOfPoints = vDouble.size() / 2;

                shared.stream.writeStartElement("mxGeometry");
                shared.stream.writeAttribute("as", "geometry");

                if (i < nbOfPoints) {
                    shared.stream.writeEmptyElement("mxPoint");
                    shared.stream.writeAttribute("as", "sourcePoint");
                    shared.stream.writeAttribute("x", Double.toString(vDouble.get(2 * i)));
                    shared.stream.writeAttribute("y", Double.toString(vDouble.get(2 * i + 1)));
                }

                shared.stream.writeStartElement("Array");
                shared.stream.writeAttribute("as", "points");
                for (; i < nbOfPoints - 1; i++) {
                    shared.stream.writeEmptyElement("mxPoint");
                    shared.stream.writeAttribute("x", Double.toString(vDouble.get(2 * i)));
                    shared.stream.writeAttribute("y", Double.toString(vDouble.get(2 * i + 1)));
                }
                shared.stream.writeEndElement(); // Array

                if (i < nbOfPoints) {
                    shared.stream.writeEmptyElement("mxPoint");
                    shared.stream.writeAttribute("as", "targetPoint");
                    shared.stream.writeAttribute("x", Double.toString(vDouble.get(2 * i)));
                    shared.stream.writeAttribute("y", Double.toString(vDouble.get(2 * i + 1)));
                }

                shared.stream.writeEndElement(); // mxGeometry
                break;
            case PORT: // no break on purpose
            case DIAGRAM:
                break;
        }
    }
}
