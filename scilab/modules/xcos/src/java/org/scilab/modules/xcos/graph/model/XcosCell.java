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

package org.scilab.modules.xcos.graph.model;

import java.util.Collections;
import java.util.List;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.PortKind;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfScicosID;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;
import java.rmi.server.UID;
import java.util.regex.Pattern;

/**
 * An Xcos cell is a JGraphX cell that store most of its information into the
 * Scicos MVC.
 *
 * The reference to the Scicos MVC object is taken at construction time and
 * released by the destructor. There is thus no need to manage parent /
 * children, block / port or port / link association referencing as the JVM will
 * GC the object if needed.
 */
public class XcosCell extends mxCell {
    private static final long serialVersionUID = 1L;
    private static Pattern validCIdentifier = Pattern.compile("[a-zA-Z][a-zA-Z0-9_]+");

    private transient ScicosObjectOwner owner;

    /**
     * Construct an Xcos graphical object.
     *
     * @param controller
     *            the shared controller
     * @param uid
     *            the associated MVC identifier
     * @param kind
     *            the associated MVC kind
     */
    public XcosCell(final JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super();

        // defensive programming
        if (uid == 0l) {
            throw new IllegalArgumentException();
        }

        owner = new ScicosObjectOwner(controller, uid, kind);
        setValue(controller, value);
        setGeometry(controller, geometry);
        setStyle(controller, style);
        setId(controller, id);
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
        setValue(new JavaController(), value);
    }

    public final void setValue(JavaController controller, Object value) {
        super.setValue(value);
        setMVCValue(controller, value);
    }

    private void setMVCValue(JavaController controller, Object value) {
        if (value == null) {
            return;
        }

        switch (getKind()) {
            case BLOCK:
                if (validCIdentifier.matcher(String.valueOf(value)).matches()) {
                    controller.setObjectProperty(getUID(), getKind(), ObjectProperties.LABEL, String.valueOf(value));
                }
            // no break on purpose
            case ANNOTATION:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DESCRIPTION, String.valueOf(value));
                break;
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
        setId(new JavaController(), id);
    }

    public final void setId(JavaController controller, String id) {
        if (id == null || id.isEmpty()) {
            id = new UID().toString();
        }

        super.setId(id);
        setMVCId(controller, id);
    }

    private void setMVCId(JavaController controller, String id) {
        if (id == null) {
            return;
        }

        switch (getKind()) {
            case ANNOTATION:
            case BLOCK:
            case LINK:
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
        setGeometry(new JavaController(), geometry);
    }

    public final void setGeometry(JavaController controller, mxGeometry geometry) {
        super.setGeometry(geometry);
        setMVCGeometry(controller, geometry);
    }

    private void setMVCGeometry(JavaController controller, mxGeometry geometry) {
        if (geometry == null) {
            return;
        }

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
        setStyle(new JavaController(), style);
    }

    public final void setStyle(JavaController controller, String style) {
        super.setStyle(style);
        setMVCStyle(controller, style);
    }

    private void setMVCStyle(JavaController controller, String style) {
        if (style == null) {
            return;
        }

        switch (getKind()) {
            case BLOCK:
            case LINK:
            case ANNOTATION:
            case PORT:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.STYLE, style);
                break;
            default:
                break;
        }
    }

    /*
     * (non-Javadoc)
     *
     * @see com.mxgraph.model.mxCell#removeFromParent()
     */
    @Override
    public void removeFromParent() {
        super.removeFromParent();

        // do not remove from parent on SUPER_f diagram creation : there is an MVC parent but no JGraphX one
        if (parent == null) {
            return;
        }

        JavaController controller = new JavaController();
        switch (getKind()) {
            case ANNOTATION:
            case BLOCK:
            case LINK: {
                /*
                 * Retrieve the parent
                 */
                long[] parent = new long[1];
                Kind parentKind = Kind.BLOCK;
                ObjectProperties prop = ObjectProperties.PARENT_BLOCK;
                controller.getObjectProperty(getUID(), getKind(), prop, parent);
                if (parent[0] == 0l) {
                    parentKind = Kind.DIAGRAM;
                    prop = ObjectProperties.PARENT_DIAGRAM;
                    controller.getObjectProperty(getUID(), getKind(), prop, parent);
                }

                /*
                 * If there is a parent, clear it
                 */
                if (parent[0] != 0l) {
                    VectorOfScicosID children = new VectorOfScicosID();
                    controller.getObjectProperty(parent[0], parentKind, ObjectProperties.CHILDREN, children);
                    children.remove(getUID());
                    controller.setObjectProperty(parent[0], parentKind, ObjectProperties.CHILDREN, children);

                    controller.setObjectProperty(getUID(), getKind(), prop, 0l);
                }
                break;
            }
            case PORT: {
                long[] parent = new long[1];
                Kind parentKind = Kind.BLOCK;
                controller.getObjectProperty(getUID(), getKind(), ObjectProperties.SOURCE_BLOCK, parent);

                int[] portKind = new int[1];
                controller.getObjectProperty(getUID(), getKind(), ObjectProperties.PORT_KIND, portKind);
                ObjectProperties property = relatedPortKindProperty(portKind[0]);

                VectorOfScicosID ports = new VectorOfScicosID();
                controller.getObjectProperty(parent[0], parentKind, property, ports);
                ports.remove(getUID());
                controller.setObjectProperty(parent[0], parentKind, property, ports);

                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SOURCE_BLOCK, 0l);
                break;
            }
            default:
                break;
        }
    }

    private ObjectProperties relatedPortKindProperty(int portKind) {
        ObjectProperties property;
        switch (PortKind.values()[portKind]) {
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
        return property;
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
        final long uid = ((XcosCell) cell).getUID();
        final Kind kind = ((XcosCell) cell).getKind();

        JavaController controller = new JavaController();
        switch (getKind()) {
            case LINK:
                if (isSource) {
                    controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SOURCE_PORT, uid);
                } else {
                    controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DESTINATION_PORT, uid);
                }
                if (uid != 0l) {
                    controller.setObjectProperty(uid, kind, ObjectProperties.CONNECTED_SIGNALS, getUID());
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
        }

        return inserted;
    }

    private void insertPort(XcosCell c, int index) {
        JavaController controller = new JavaController();
        int[] v = new int[1];
        controller.getObjectProperty(c.getUID(), c.getKind(), ObjectProperties.PORT_KIND, v);

        VectorOfScicosID children = new VectorOfScicosID();
        final ObjectProperties property = relatedPortKindProperty(v[0]);

        if (property != null) {
            controller.getObjectProperty(getUID(), getKind(), property, children);
            // do no use the index argument as it is not possible to insert out of order on the MVC (as we have kind of port)
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

        c.owner = new ScicosObjectOwner(controller.cloneObject(getUID(), true, false), getKind());
        return c;
    }
}
