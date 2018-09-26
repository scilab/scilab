/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2017 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2017-2018 - ESI Group - Clement DAVID
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
import com.mxgraph.util.mxUtils;

import java.nio.DoubleBuffer;
import java.nio.LongBuffer;
import java.util.ArrayList;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfScicosID;

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

                v = atts.getValue("x");
                if (v != null) {
                    g.setX(Double.valueOf(v));
                }
                v = atts.getValue("y");
                if (v != null) {
                    g.setY(Double.valueOf(v));
                }
                v = atts.getValue("width");
                if (v != null) {
                    g.setWidth(Double.valueOf(v));
                }
                v = atts.getValue("height");
                if (v != null) {
                    g.setHeight(Double.valueOf(v));
                }

                /*
                 * the MVC only store absolute values, resolve the "relative" geometry flag for Scilab 5.5.2 annotation
                 */
                v = atts.getValue("relative");
                if (v != null && v.charAt(0) == '1') {
                    Object parent = saxHandler.parents.peek();
                    if (parent instanceof XcosCell) {
                        XcosCell cell = (XcosCell) parent;
                        long[] parentUID = {0};
                        saxHandler.controller.getObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.RELATED_TO, parentUID);

                        VectorOfDouble parentGeom = new VectorOfDouble(4);
                        saxHandler.controller.getObjectProperty(parentUID[0], saxHandler.controller.getKind(parentUID[0]), ObjectProperties.GEOMETRY, parentGeom);
                        g.setX(g.getX() * parentGeom.get(2));
                        g.setY(g.getY() * parentGeom.get(3));
                    }
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

                Object localParent = saxHandler.parents.peek();
                if (localParent instanceof mxGeometry) {
                    mxGeometry parent = (mxGeometry) localParent;
                    v = atts.getValue("as");
                    if ("sourcePoint".equals(v)) {
                        parent.setSourcePoint(p);
                    } else if ("targetPoint".equals(v)) {
                        parent.setTargetPoint(p);
                    }
                } else if (localParent instanceof RawDataHandler.RawDataDescriptor) {
                    RawDataHandler.RawDataDescriptor parent = (RawDataHandler.RawDataDescriptor) localParent;
                    ((ArrayList) parent.value).add(p);
                } else if (localParent instanceof XcosCell) {
                    // Diagram origin, translate each children
                    XcosCell parent = (XcosCell) localParent;

                    VectorOfScicosID children = new VectorOfScicosID();
                    saxHandler.controller.getObjectProperty(parent.getUID(), parent.getKind(), ObjectProperties.CHILDREN, children);

                    VectorOfDouble geometry = new VectorOfDouble(4);
                    DoubleBuffer geom = geometry.asByteBuffer(0, 4).asDoubleBuffer();

                    LongBuffer childrenUIDs = children.asByteBuffer(0, children.size()).asLongBuffer();
                    while (childrenUIDs.hasRemaining()) {
                        long uid = childrenUIDs.get();
                        Kind kind = saxHandler.controller.getKind(uid);

                        saxHandler.controller.getObjectProperty(uid, kind, ObjectProperties.GEOMETRY, geometry);
                        geom.put(0, geom.get(0) + p.getX());
                        geom.put(1, geom.get(1) + p.getY());
                        saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.GEOMETRY, geometry);

                        // translate the annotation
                        long[] annotation = { 0 };
                        saxHandler.controller.getObjectProperty(uid, kind, ObjectProperties.LABEL, annotation);
                        if (annotation[0] != 0) {
                            saxHandler.controller.getObjectProperty(annotation[0], Kind.ANNOTATION, ObjectProperties.GEOMETRY, geometry);
                            geom.put(0, geom.get(0) + p.getX());
                            geom.put(1, geom.get(1) + p.getY());
                            saxHandler.controller.setObjectProperty(annotation[0], Kind.ANNOTATION, ObjectProperties.GEOMETRY, geometry);
                        }

                        // translate control-points
                        if (kind == Kind.LINK) {
                            VectorOfDouble controlPoints = new VectorOfDouble();
                            saxHandler.controller.getObjectProperty(uid, kind, ObjectProperties.CONTROL_POINTS, controlPoints);

                            DoubleBuffer points = controlPoints.asByteBuffer(0, controlPoints.size()).asDoubleBuffer();
                            for (int i = 0; i < controlPoints.size(); i += 2) {
                                points.put(i, points.get(i) + p.getX());
                                points.put(i + 1, points.get(i + 1) + p.getY());
                            }
                            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.CONTROL_POINTS, controlPoints);
                        }
                    }

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
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.DESCRIPTION, mxUtils.getBodyMarkup(value, false));
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

                cell.setGeometry(saxHandler.controller, g);
            }
            break;
            case mxPoint:
                break;
            default:
                throw new IllegalArgumentException();
        }
    }
}