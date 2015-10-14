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

package org.scilab.modules.xcos.graph.model;

import java.util.Collections;
import java.util.List;

import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.PortKind;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfScicosID;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;

public class XcosCell extends ScilabGraphUniqueObject {
    private static final long serialVersionUID = 1L;

    private transient ScicosObjectOwner owner;

    /**
     * Construct an Xcos graphical object.
     *
     * This Java object owns the corresponding MVC object and thus will unrefererence it on GC.
     *
     * @param uid
     *            the associated MVC identifier
     * @param kind
     *            the associated MVC kind
     */
    public XcosCell(long uid, Kind kind) {
        owner = new ScicosObjectOwner(uid, kind);

        // defensive programming
        if (uid == 0l) {
            throw new IllegalArgumentException();
        }

        JavaController controller = new JavaController();
        controller.referenceObject(uid);
    }

    /**
     * @return the MVC unique identifier
     */
    public long getUID() {
        return owner.getUID();
    }

    /**
     * @return the MVC kind of object
     */
    public Kind getKind() {
        return owner.getKind();
    }

    /*
     * Override setters and hierarchy modifiers to propagate changes to the model
     */

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#setValue(java.lang.Object)
     */
    @Override
    public void setValue(Object value) {
        super.setValue(value);
        if (owner == null) {
            return;
        }

        JavaController controller = new JavaController();
        switch (getKind()) {
            case ANNOTATION:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DESCRIPTION, String.valueOf(value));
                break;
            case BLOCK:
            case LINK:
            case PORT:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.LABEL, String.valueOf(value));
                break;
            default:
                break;
        }
    }

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#setId(java.lang.String)
     */
    @Override
    public void setId(String id) {
        super.setId(id);

        JavaController controller = new JavaController();
        switch (getKind()) {
            case ANNOTATION:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.UID, id);
                break;
            case BLOCK:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.UID, id);
                break;
            case LINK:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.UID, id);
                break;
            case PORT:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.UID, id);
                break;
            default:
                break;
        }
    }

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#setGeometry(com.mxgraph.model.mxGeometry)
     */
    @Override
    public void setGeometry(mxGeometry geometry) {
        super.setGeometry(geometry);
        if (owner == null) {
            return;
        }

        JavaController controller = new JavaController();
        switch (getKind()) {
            case ANNOTATION:
            case BLOCK: {
                VectorOfDouble v = new VectorOfDouble(4);
                v.set(0, geometry.getX());
                v.set(1, geometry.getY());
                v.set(2, geometry.getWidth());
                v.set(3, geometry.getHeight());
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.GEOMETRY, v);
                break;
            }
            case LINK: {
                /*
                 * try to find the origin of the source and target accordingly to the JGraphX implementation
                 */
                mxPoint sourcePoint = null;
                mxPoint targetPoint = null;

                mxICell sourceCell = getSource();
                mxICell targetCell = getTarget();
                if (sourceCell != null && sourceCell.getGeometry() != null) {
                    sourcePoint = new mxPoint(sourceCell.getGeometry().getCenterX(), sourceCell.getGeometry().getCenterY());
                }
                if (targetCell != null && targetCell.getGeometry() != null) {
                    targetPoint = new mxPoint(targetCell.getGeometry().getCenterX(), targetCell.getGeometry().getCenterY());
                }

                if (sourcePoint == null) {
                    sourcePoint = geometry.getSourcePoint();
                }
                if (targetPoint == null) {
                    targetPoint = geometry.getTargetPoint();
                }

                if (sourcePoint == null) {
                    sourcePoint = new mxPoint();
                }
                if (targetPoint == null) {
                    targetPoint = new mxPoint();
                }

                List<mxPoint> points = geometry.getPoints();
                if (points == null) {
                    points = Collections.emptyList();
                }

                /*
                 * At that point, the sourcePoint, targetPoint and points are valid values (but may be unknown) encode them to the the CONTROL_POINTS
                 */

                // Allocate some space to contains them all
                int nbOfPoints = 2 + points.size();
                VectorOfDouble v = new VectorOfDouble(2 * nbOfPoints);
                int i = 0;

                // then fill the allocation space
                v.set(2 * i, sourcePoint.getX());
                v.set(2 * i + 1, sourcePoint.getY());
                i++;

                for (; i < nbOfPoints - 1; i++) {
                    v.set(2 * i, points.get(i - 1).getX());
                    v.set(2 * i + 1, points.get(i - 1).getY());
                }

                v.set(2 * i, targetPoint.getX());
                v.set(2 * i + 1, targetPoint.getY());
                i++;

                /*
                 * Finally push the values to the model
                 */
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.CONTROL_POINTS, v);
                break;
            }
            default:
                break;
        }
    }

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#setStyle(java.lang.String)
     */
    @Override
    public void setStyle(String style) {
        super.setStyle(style);
        if (owner == null) {
            return;
        }

        JavaController controller = new JavaController();
        switch (getKind()) {
            case ANNOTATION:
            case BLOCK:
            case PORT:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.STYLE, String.valueOf(style));
                break;
            default:
                break;
        }

    }

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#setParent(com.mxgraph.model.mxICell)
     */
    @Override
    public void setParent(mxICell parent) {
        super.setParent(parent);

        if (parent instanceof XcosCell) {
            XcosCell p = (XcosCell) parent;
            JavaController controller = new JavaController();
            switch (getKind()) {
                case ANNOTATION:
                case BLOCK:
                case LINK:
                    if (p.getKind() == Kind.DIAGRAM) {
                        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.PARENT_DIAGRAM, p.getUID());
                    } else {
                        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.PARENT_BLOCK, p.getUID());

                        long[] root = new long[1];
                        controller.getObjectProperty(p.getUID(), p.getKind(), ObjectProperties.PARENT_DIAGRAM, root);
                        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.PARENT_DIAGRAM, root[0]);
                    }
                    break;
                case PORT:
                    controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SOURCE_BLOCK, p.getUID());
                    break;
                default:
                    break;
            }
        }
    }

    @Override
    public mxICell setTerminal(mxICell terminal, boolean isSource) {
        mxICell cell = super.setTerminal(terminal, isSource);

        if (cell == null) {
            return cell;
        }

        // a terminal of an XcosCell is always another XcosCell
        XcosCell t = (XcosCell) cell;
        JavaController controller = new JavaController();
        switch (getKind()) {
            case LINK:
                if (isSource) {
                    controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SOURCE_PORT, t.getUID());
                } else {
                    controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DESTINATION_PORT, t.getUID());
                }
                break;
            default:
                break;
        }

        return cell;
    }

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#insert(com.mxgraph.model.mxICell, int)
     */
    @Override
    public mxICell insert(mxICell child, int index) {
        mxICell inserted = super.insert(child, index);

        // the child might not be an XcosCell but just an mxCell label
        if (child instanceof XcosCell) {
            XcosCell c = (XcosCell) child;
            switch (getKind()) {
                case BLOCK:
                    if (c.getKind() == Kind.PORT) {
                        insertPort(c, index);
                    } else {
                        insertChild(c, index);
                    }
                    break;
                case DIAGRAM:
                    insertChild(c, index);
                    break;
                default:
                    break;
            }

            JavaController controller = new JavaController();
            controller.referenceObject(c.getUID());
        }

        return inserted;
    }

    private void insertPort(XcosCell c, int index) {
        JavaController controller = new JavaController();
        int[] v = new int[1];
        controller.getObjectProperty(c.getUID(), c.getKind(), ObjectProperties.PORT_KIND, v);

        VectorOfScicosID children = new VectorOfScicosID();
        final ObjectProperties property;
        switch (PortKind.values()[v[0]]) {
            case PORT_IN:
                property = ObjectProperties.INPUTS;
                break;
            case PORT_OUT:
                property = ObjectProperties.OUTPUTS;
                break;
            case PORT_EIN:
                property = ObjectProperties.EVENT_INPUTS;
                break;
            case PORT_EOUT:
                property = ObjectProperties.EVENT_OUTPUTS;
                break;
            default:
                property = null;
                break;
        }

        // FIXME manage the index argument, possibly by counting the JGraphX children by kind
        if (property != null) {
            controller.getObjectProperty(getUID(), getKind(), property, children);
            children.add(c.getUID());
            controller.setObjectProperty(getUID(), getKind(), property, children);
        }
    }

    private void insertChild(XcosCell c, int index) {
        JavaController controller = new JavaController();
        VectorOfScicosID children = new VectorOfScicosID();

        controller.getObjectProperty(getUID(), getKind(), ObjectProperties.CHILDREN, children);
        children.add(index, c.getUID());
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.CHILDREN, children);
    }

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#remove(com.mxgraph.model.mxICell)
     */
    @Override
    public mxICell remove(mxICell child) {
        mxICell removed = super.remove(child);

        // the child might not be an XcosCell but just an mxCell label
        if (child instanceof XcosCell) {
            XcosCell c = (XcosCell) child;
            switch (getKind()) {
                case BLOCK:
                    if (c.getKind() == Kind.PORT) {
                        removePort(c);
                    } else {
                        removeChild(c);
                    }
                    break;
                case DIAGRAM:
                    removeChild(c);
                    break;
                default:
                    break;
            }

            JavaController controller = new JavaController();
            controller.deleteObject(c.getUID());
        }
        return removed;
    }

    private void removePort(XcosCell c) {
        JavaController controller = new JavaController();
        int[] v = new int[1];
        controller.getObjectProperty(c.getUID(), c.getKind(), ObjectProperties.PORT_KIND, v);

        VectorOfScicosID children = new VectorOfScicosID();
        final ObjectProperties property;
        switch (PortKind.values()[v[0]]) {
            case PORT_IN:
                property = ObjectProperties.INPUTS;
                break;
            case PORT_OUT:
                property = ObjectProperties.OUTPUTS;
                break;
            case PORT_EIN:
                property = ObjectProperties.EVENT_INPUTS;
                break;
            case PORT_EOUT:
                property = ObjectProperties.EVENT_OUTPUTS;
                break;
            default:
                property = null;
                break;
        }

        controller.getObjectProperty(getUID(), getKind(), property, children);
        children.remove(c.getUID());
        controller.setObjectProperty(getUID(), getKind(), property, children);
    }

    private void removeChild(XcosCell c) {
        JavaController controller = new JavaController();
        VectorOfScicosID children = new VectorOfScicosID();

        controller.getObjectProperty(getUID(), getKind(), ObjectProperties.CHILDREN, children);
        children.remove(c.getUID());
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.CHILDREN, children);

    }

    /*
     * Override methods from Object
     */

    @Override
    public Object clone() throws CloneNotSupportedException {
        JavaController controller = new JavaController();
        XcosCell c = (XcosCell) super.clone();
        c.owner = new ScicosObjectOwner(controller.cloneObject(getUID(), false, false), getKind());
        return c;
    }

    @Override
    protected void finalize() throws Throwable {
        JavaController controller = new JavaController();
        controller.deleteObject(getUID());
    }
}
