/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

import java.util.ArrayList;
import org.scilab.modules.xcos.VectorOfDouble;

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
                        Kind parentKind = saxHandler.controller.getKind(parentUID[0]);

                        double x = 0.;
                        double y = 0.;
                        double width = 0.;
                        double height = 0.;

                        switch (parentKind) {
                            case BLOCK:
                            {
                                VectorOfDouble parentGeom = new VectorOfDouble(4);
                                saxHandler.controller.getObjectProperty(parentUID[0], parentKind, ObjectProperties.GEOMETRY, parentGeom);
                                if (parentGeom.size() < 4) {
                                     break;
                                }
                                x = g.getX() * parentGeom.get(2);
                                y = g.getY() * parentGeom.get(3);
                                width = 0;
                                height = 0;
                                break;
                            }
                            case LINK:
                            {
                                VectorOfDouble controlPoints = new VectorOfDouble();
                                saxHandler.controller.getObjectProperty(parentUID[0], parentKind, ObjectProperties.CONTROL_POINTS, controlPoints);
                                
                                // look for the max and min to center the label
                                double x_min = Double.MAX_VALUE;
                                double y_min = Double.MAX_VALUE;
                                double x_max = Double.MIN_VALUE;
                                double y_max = Double.MIN_VALUE;
                                final int nbOfPoints = controlPoints.size() / 2;
                                for (int i = 0; i < nbOfPoints; i++)
                                {
                                    x = controlPoints.get(2*i);
                                    y = controlPoints.get(2*i+1);
                                    x_min = Double.min(x_min, x);
                                    y_min = Double.min(y_min, y);
                                    x_max = Double.max(x_max, x);
                                    y_max = Double.max(y_max, y);
                                }
                                // center the label
                                x = x_min;
                                y = y_min;
                                width = x_max - x_min;
                                height = y_max - y_min;
                                break;
                            }
                        
                            default:
                                break;
                        }
                        
                        g.setX(x);
                        g.setY(y);
                        g.setWidth(width);
                        g.setHeight(height);
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
                    } else if ("offset".equals(v)) {
                        parent.setX(parent.getX() + p.getX());
                        parent.setY(parent.getY() + p.getY());
                    }
                } else if (localParent instanceof RawDataHandler.RawDataDescriptor) {
                    RawDataHandler.RawDataDescriptor parent = (RawDataHandler.RawDataDescriptor) localParent;
                    @SuppressWarnings("unchecked")
                    ArrayList<Object> list = (ArrayList<Object>) parent.value;
                    list.add(p);
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