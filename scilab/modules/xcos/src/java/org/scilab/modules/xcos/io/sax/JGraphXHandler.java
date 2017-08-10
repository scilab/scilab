/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2017 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2017 - ESI Group - Clement DAVID
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

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.io.HandledElement;
import org.xml.sax.Attributes;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;
import java.util.ArrayList;

class JGraphXHandler implements ScilabHandler {

    private final XcosSAXHandler saxHandler;

    JGraphXHandler(XcosSAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }



    @Override
    public Object startElement(HandledElement found, Attributes atts) {
        String v;

        switch (found) {
            case mxCell: {
                v = atts.getValue("parent");
                if (v != null) {
                    long parentUID = saxHandler.allChildren.peek().getOrDefault(v, 0l);
                    if (parentUID != 0) {
                        return decodeCellAnnotation(parentUID, saxHandler.controller.getKind(parentUID), atts);
                    }
                } else {
                    Object parent = saxHandler.parents.peek();
                    if (parent instanceof XcosCell) {
                        XcosCell cell = ((XcosCell) parent);
                        if (cell.getUID() != 0) {
                            return decodeCellAnnotation(cell.getUID(), cell.getKind(), atts);
                        }
                    }
                }
                return null;
            }
            case mxGeometry: {
                mxGeometry g = new mxGeometry();

                v = atts.getValue("height");
                if (v != null) {
                    g.setHeight(Double.valueOf(v));
                }
                v = atts.getValue("width");
                if (v != null) {
                    g.setWidth(Double.valueOf(v));
                }
                v = atts.getValue("x");
                if (v != null) {
                    g.setX(Double.valueOf(v));
                }
                v = atts.getValue("y");
                if (v != null) {
                    g.setY(Double.valueOf(v));
                }

                return g;
            }
            case mxPoint: {
                mxPoint p = new mxPoint();

                v = atts.getValue("x");
                if (v != null) {
                    p.setX(Double.valueOf(v));
                }
                v = atts.getValue("y");
                if (v != null) {
                    p.setY(Double.valueOf(v));
                }

                if (saxHandler.parents.peek() instanceof mxGeometry) {
                    mxGeometry parent = (mxGeometry) saxHandler.parents.peek();
                    v = atts.getValue("as");
                    if ("sourcePoint".equals(v)) {
                        parent.setSourcePoint(p);
                    } else if ("targetPoint".equals(v)) {
                        parent.setTargetPoint(p);
                    }
                } else if (saxHandler.parents.peek() instanceof RawDataHandler.RawDataDescriptor) {
                    RawDataHandler.RawDataDescriptor parent = (RawDataHandler.RawDataDescriptor) saxHandler.parents.peek();
                    ((ArrayList) parent.value).add(p);
                }
                return p;
            }
            default:
                throw new IllegalArgumentException();
        }
    }

    private XcosCell decodeCellAnnotation(long parentUID, Kind parentKind, Attributes atts) {
        Kind kind = Kind.ANNOTATION;
        final long uid = saxHandler.controller.createObject(kind);
        String value = atts.getValue("value");
        if (value != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.DESCRIPTION, value);
        }
        String style = atts.getValue("style");
        if (style != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.STYLE, style);
        }
        String id = atts.getValue("id");
        if (id != null) {
            saxHandler.allChildren.peek().put(id, uid);
        }

        XcosCell label = new XcosCell(saxHandler.controller, uid, kind, value, null, style, id);
        saxHandler.controller.setObjectProperty(parentUID, parentKind, ObjectProperties.LABEL, label.getUID());
        saxHandler.controller.setObjectProperty(label.getUID(), label.getKind(), ObjectProperties.RELATED_TO, parentUID);

        return label;
    }

    @Override
    public void endElement(HandledElement found) {
        switch (found) {
            case mxCell:
                break;
            case mxGeometry: {
                // defensive programming
                if (!(saxHandler.parents.peek() instanceof mxGeometry)) {
                    return;
                }
                mxGeometry g = (mxGeometry) saxHandler.parents.peek();
                if (!(saxHandler.parents.peek(1) instanceof XcosCell)) {
                    return;
                }
                XcosCell cell = (XcosCell) saxHandler.parents.peek(1);

                cell.setGeometry(g);
            }
            break;
            case mxPoint:
                break;
            default:
                throw new IllegalArgumentException();
        }
    }
}