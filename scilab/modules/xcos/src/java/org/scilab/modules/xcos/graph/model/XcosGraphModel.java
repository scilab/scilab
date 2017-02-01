/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2017 - Scilab Enterprises - Clement DAVID
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

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGraphModel;
import java.rmi.server.UID;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfBool;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.block.BasicBlock;

/**
 * Implement a custom model for Xcos
 */
public class XcosGraphModel extends mxGraphModel {

    public XcosGraphModel(JavaController controller, long diagramId, Kind kind, String uid) {
        super(new mxCell());

        XcosCell parent = new XcosCell(controller, diagramId, kind, null, null, null, uid);
        ((mxCell) root).insert(parent);
    }

    @Override
    public final String createId(Object cell) {
        return new UID().toString();
    }

    /**
     * Common property change support for a generic type
     *
     * @param <T> the type
     */
    private static abstract class PropertyChange<T> extends mxAtomicGraphModelChange {

        private final long uid;
        private final ObjectProperties key;

        private T previous;

        public PropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, T value) {
            super(model);

            this.uid = cell.getUID();
            this.key = key;
            this.previous = value;
        }

        protected abstract T getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p);

        protected abstract void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, T value);

        @Override
        public void execute() {
            JavaController controller = new JavaController();

            T temp = getProperty(controller, uid, Kind.BLOCK, key);
            setProperty(controller, uid, Kind.LINK, key, previous);
            previous = temp;
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, double[] value) {
        execute(new DoublePropertyChange(this, cell, key, value));
    }

    private static final class DoublePropertyChange extends PropertyChange<double[]> {

        public DoublePropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, double[] value) {
            super(model, cell, key, value);
        }

        @Override
        protected double[] getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            double[] v = {0};
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, double[] value) {
            controller.setObjectProperty(uid, kind, p, value[0]);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, int[] value) {
        execute(new IntPropertyChange(this, cell, key, value));
    }

    private static class IntPropertyChange extends PropertyChange<int[]> {

        public IntPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, int[] value) {
            super(model, cell, key, value);
        }

        @Override
        protected int[] getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            int[] v = {0};
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, int[] value) {
            controller.setObjectProperty(uid, kind, p, value[0]);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, boolean[] value) {
        execute(new BoolPropertyChange(this, cell, key, value));
    }

    private static class BoolPropertyChange extends PropertyChange<boolean[]> {

        public BoolPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, boolean[] value) {
            super(model, cell, key, value);
        }

        @Override
        protected boolean[] getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            boolean[] v = {false};
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, boolean[] value) {
            controller.setObjectProperty(uid, kind, p, value[0]);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, String[] value) {
        execute(new StringPropertyChange(this, cell, key, value));
    }

    private static class StringPropertyChange extends PropertyChange<String[]> {

        public StringPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, String[] value) {
            super(model, cell, key, value);
        }

        @Override
        protected String[] getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            String[] v = {""};
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, String[] value) {
            controller.setObjectProperty(uid, kind, p, value[0]);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, long[] value) {
        execute(new LongPropertyChange(this, cell, key, value));
    }

    private static class LongPropertyChange extends PropertyChange<long[]> {

        public LongPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, long[] value) {
            super(model, cell, key, value);
        }

        @Override
        protected long[] getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            long[] v = {0};
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, long[] value) {
            controller.setObjectProperty(uid, kind, p, value[0]);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, VectorOfDouble value) {
        execute(new VectorOfDoublePropertyChange(this, cell, key, value));
    }

    private static class VectorOfDoublePropertyChange extends PropertyChange<VectorOfDouble> {

        public VectorOfDoublePropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, VectorOfDouble value) {
            super(model, cell, key, value);
        }

        @Override
        protected VectorOfDouble getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            VectorOfDouble v = new VectorOfDouble();
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, VectorOfDouble value) {
            controller.setObjectProperty(uid, kind, p, value);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, VectorOfInt value) {
        execute(new VectorOfIntPropertyChange(this, cell, key, value));
    }

    private static class VectorOfIntPropertyChange extends PropertyChange<VectorOfInt> {

        public VectorOfIntPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, VectorOfInt value) {
            super(model, cell, key, value);
        }

        @Override
        protected VectorOfInt getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            VectorOfInt v = new VectorOfInt();
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, VectorOfInt value) {
            controller.setObjectProperty(uid, kind, p, value);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, VectorOfBool value) {
        execute(new VectorOfBoolPropertyChange(this, cell, key, value));
    }

    private static class VectorOfBoolPropertyChange extends PropertyChange<VectorOfBool> {

        public VectorOfBoolPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, VectorOfBool value) {
            super(model, cell, key, value);
        }

        @Override
        protected VectorOfBool getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            VectorOfBool v = new VectorOfBool();
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, VectorOfBool value) {
            controller.setObjectProperty(uid, kind, p, value);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, VectorOfString value) {
        execute(new VectorOfStringPropertyChange(this, cell, key, value));
    }

    private static class VectorOfStringPropertyChange extends PropertyChange<VectorOfString> {

        public VectorOfStringPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, VectorOfString value) {
            super(model, cell, key, value);
        }

        @Override
        protected VectorOfString getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            VectorOfString v = new VectorOfString();
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, VectorOfString value) {
            controller.setObjectProperty(uid, kind, p, value);
        }
    }

    /**
     * Set a block property
     *
     * @param cell the block
     * @param key the property
     * @param value the value to assign
     */
    public void setProperty(BasicBlock cell, ObjectProperties key, VectorOfScicosID value) {
        execute(new VectorOfScicosIDPropertyChange(this, cell, key, value));
    }

    private static class VectorOfScicosIDPropertyChange extends PropertyChange<VectorOfScicosID> {

        public VectorOfScicosIDPropertyChange(XcosGraphModel model, BasicBlock cell, ObjectProperties key, VectorOfScicosID value) {
            super(model, cell, key, value);
        }

        @Override
        protected VectorOfScicosID getProperty(JavaController controller, long uid, Kind kind, ObjectProperties p) {
            VectorOfScicosID v = new VectorOfScicosID();
            controller.getObjectProperty(uid, kind, p, v);
            return v;
        }

        @Override
        protected void setProperty(JavaController controller, long uid, Kind kind, ObjectProperties p, VectorOfScicosID value) {

            // move the children to the parent
            if (ObjectProperties.CHILDREN == p) {
                final int size = value.size();
                for (int i = 0; i < size; i++) {
                    long child = value.get(i);
                    controller.referenceObject(child);
                    controller.setObjectProperty(child, controller.getKind(child), ObjectProperties.PARENT_BLOCK, uid);
                }
            }

            controller.setObjectProperty(uid, kind, p, value);
        }
    }
}
