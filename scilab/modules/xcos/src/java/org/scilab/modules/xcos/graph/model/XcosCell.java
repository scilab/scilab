package org.scilab.modules.xcos.graph.model;

import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.PortKind;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfScicosID;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;


public class XcosCell extends ScilabGraphUniqueObject {
    private static final long serialVersionUID = 1L;

    private long uid;
    private Kind kind;

    /**
     * Construct an Xcos graphical object.
     *
     * This Java object owns the corresponding MVC object and thus will unrefererence it on GC.
     *
     * @param uid the associated MVC identifier
     * @param kind the associated MVC kind
     */
    public XcosCell(long uid, Kind kind) {
        this.uid = uid;
        this.kind = kind;
    }

    /**
     * @return the MVC unique identifier
     */
    public long getUID() {
        return uid;
    }

    /**
     * @return the MVC kind of object
     */
    public Kind getKind() {
        return kind;
    }

    /*
     * Override setters and hierarchy modifiers to propagate changes to the model
     */

    /* (non-Javadoc)
     * @see com.mxgraph.model.mxCell#setValue(java.lang.Object)
     */
    @Override
    public void setValue(Object value) {
        super.setValue(value);
        if (getKind() == null) {
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

    /* (non-Javadoc)
     * @see com.mxgraph.model.mxCell#setId(java.lang.String)
     */
    @Override
    public void setId(String id) {
        super.setId(id);

        JavaController controller = new JavaController();
        switch (getKind()) {
            case BLOCK:
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.UID, id);
                break;
            default:
                break;
        }
    }

    /* (non-Javadoc)
     * @see com.mxgraph.model.mxCell#setGeometry(com.mxgraph.model.mxGeometry)
     */
    @Override
    public void setGeometry(mxGeometry geometry) {
        super.setGeometry(geometry);
        if (getKind() == null) {
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
                VectorOfDouble v = new VectorOfDouble(4);
                v.set(0, geometry.getX());
                v.set(1, geometry.getY());
                v.set(2, geometry.getWidth());
                v.set(3, geometry.getHeight());
                controller.setObjectProperty(getUID(), getKind(), ObjectProperties.GEOMETRY, v);
                break;
            }
            default:
                break;
        }
    }

    /* (non-Javadoc)
     * @see com.mxgraph.model.mxCell#setStyle(java.lang.String)
     */
    @Override
    public void setStyle(String style) {
        super.setStyle(style);
        if (getKind() == null) {
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

    /* (non-Javadoc)
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

    /* (non-Javadoc)
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

        // FIXME manage the index argument
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

    /* (non-Javadoc)
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
        c.uid = controller.cloneObject(getUID(), false);
        c.kind = getKind();
        return c;
    }

    protected void finalize() throws Throwable {
        JavaController controller = new JavaController();
        controller.deleteObject(getUID());
    }
}
