/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block.io;

import com.mxgraph.model.mxGeometry;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.Serializable;
import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;
import java.util.regex.Pattern;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosView;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxICell;
import java.rmi.server.UID;
import org.scilab.modules.xcos.ObjectProperties;

/**
 * Common class for the SuperBlock I/O blocks (represent ports)
 */
public abstract class ContextUpdate extends BasicBlock {

    private static final Logger LOG_LOCAL = Logger.getLogger(ContextUpdate.class.getName());
    private static final long serialVersionUID = 6076826729067963560L;

    private static final double DEFAULT_WIDTH = 20.0;
    private static final double DEFAULT_HEIGHT = 20.0;

    /**
     * Implement a listener which update the value and refresh the view when the
     * index of the port change.
     */
    @SuppressWarnings(value = { "serial" })
    private static final class IndexChangeAdapter implements PropertyChangeListener, Serializable {
        private static IndexChangeAdapter instance;

        /**
         * Default constructor.
         */
        private IndexChangeAdapter() {
        }

        /**
         * @return the instance
         */
        public static synchronized IndexChangeAdapter getInstance() {
            if (instance == null) {
                instance = new IndexChangeAdapter();
            }
            return instance;
        }

        /**
         * Update the value and refresh the graph view.
         *
         * @param evt
         *            the event
         * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
         */
        @Override
        public void propertyChange(PropertyChangeEvent evt) {
            ScilabType data = (ScilabType) evt.getNewValue();
            ContextUpdate ioBlock = (ContextUpdate) evt.getSource();

            if (!data.isEmpty()) {
                int newIndex = (int) ((ScilabDouble) data).getRealPart()[0][0];

                int oldIndex;
                if (evt.getOldValue() instanceof ScilabDouble && !((ScilabDouble) evt.getOldValue()).isEmpty()) {
                    oldIndex = (int) ((ScilabDouble) evt.getOldValue()).getRealPart()[0][0];
                } else {
                    oldIndex = -1;
                }

                ioBlock.setValue(newIndex);
            }
        }

    }

    /**
     * Implement a listener to update the
     * {@link ContextUpdate#isContextDependent} flag.
     *
     * @SuppressWarnings(value = { "serial" })
     */
    private static final class ExprsChangeAdapter implements PropertyChangeListener, Serializable {
        private static final Pattern INTEGER_PATTERN = Pattern.compile("\\d+");

        private static ExprsChangeAdapter instance;

        /**
         * Default constructor
         */
        public ExprsChangeAdapter() {
        }

        /**
         * @return the shared instance
         */
        public static ExprsChangeAdapter getInstance() {
            if (instance == null) {
                instance = new ExprsChangeAdapter();
            }
            return instance;
        }

        /**
         * isContextDependant field
         *
         * @param evt
         *            the event
         * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
         */
        @Override
        public void propertyChange(final PropertyChangeEvent evt) {
            final ScilabType data = (ScilabType) evt.getNewValue();
            final ContextUpdate ioBlock = (ContextUpdate) evt.getSource();

            if (!data.isEmpty()) {
                final String newIndex = ((ScilabString) data).getData()[0][0];

                if (!INTEGER_PATTERN.matcher(newIndex).matches()) {
                    ioBlock.isContextDependent = true;
                } else {
                    ioBlock.isContextDependent = false;
                }
            }
        }
    }

    /**
     * This enum represent all the subclasses of ContextUpdate .
     *
     * It is used to easily loop over a BasicBlock I/O blocks
     */
    public static enum IOBlocks {
        /** Map a control port to an event input block */
        EventInBlock(EventInBlock.class, ControlPort.class, CommandPort.class, ControlPort.class),
        /** Map a command port to an event output block */
        EventOutBlock(EventOutBlock.class, CommandPort.class, ControlPort.class, CommandPort.class),
        /** Map an explicit input port to an explicit input block */
        ExplicitInBlock(ExplicitInBlock.class, ExplicitInputPort.class, ExplicitOutputPort.class, InputPort.class),
        /** Map an explicit output port to an explicit output block */
        ExplicitOutBlock(ExplicitOutBlock.class, ExplicitOutputPort.class, ExplicitInputPort.class, OutputPort.class),
        /** Map an implicit input port to an implicit input block */
        ImplicitInBlock(ImplicitInBlock.class, ImplicitInputPort.class, ImplicitOutputPort.class, InputPort.class),
        /** Map an implicit output port to an implicit output block */
        ImplicitOutBlock(ImplicitOutBlock.class, ImplicitOutputPort.class, ImplicitInputPort.class, OutputPort.class);

        private final Class<? extends ContextUpdate> ioBlock;
        private final Class<? extends BasicPort> port;
        private final Class<? extends BasicPort> opposite;
        private final Class<? extends BasicPort> assignement;

        /**
         * @param ioBlock
         *            input/output block
         * @param port
         *            the associated port class
         * @param opposite
         *            the opposite port class
         */
        private IOBlocks(Class<? extends ContextUpdate> ioBlock, Class<? extends BasicPort> port, Class<? extends BasicPort> opposite,
        Class<? extends BasicPort> assignement) {
            this.ioBlock = ioBlock;
            this.port = port;
            this.opposite = opposite;
            this.assignement = assignement;
        }

        /**
         * Get all the port of the SuperBlock parent.
         *
         * @param parent
         *            the parent
         * @return the port list mapped by port type
         */
        public static Map<IOBlocks, List<mxICell>> getAllPorts(SuperBlock parent) {
            final EnumMap<IOBlocks, List<mxICell>> ret = new EnumMap<IOBlocks, List<mxICell>>(IOBlocks.class);

            /* Allocation */
            for (IOBlocks b : IOBlocks.values()) {
                ret.put(b, new ArrayList<mxICell>());
            }

            /* Loop all over the children */
            final int childCount = parent.getChildCount();

            for (int i = 0; i < childCount; i++) {
                final mxICell child = parent.getChildAt(i);

                /* if compatible add it to the list */
                for (IOBlocks b : IOBlocks.values()) {
                    if (child.getClass().equals(b.getReferencedPortClass())) {
                        ret.get(b).add(child);
                    }
                }
            }

            return ret;
        }

        /**
         * Get the ports of the super blocks with kind klass
         *
         * @param parent
         *            the parent {@link SuperBlock}
         * @param klass
         *            the filter klass
         * @return the list of ports
         */
        public static List<mxICell> getPorts(SuperBlock parent, Class<? extends ContextUpdate> klass) {
            List<mxICell> ret = new ArrayList<mxICell>();

            /* Get the corresponding klass */
            Class<? extends BasicPort> portKlass = null;
            for (IOBlocks b : IOBlocks.values()) {
                if (b.getReferencedClass().equals(klass)) {
                    portKlass = b.getAssignementCompatiblePortClass();
                    break;
                }
            }

            /* Loop all over the children */
            final int childCount = parent.getChildCount();

            for (int i = 0; i < childCount; i++) {
                final mxICell child = parent.getChildAt(i);

                if (portKlass.isInstance(child)) {
                    ret.add(child);
                }
            }

            return ret;
        }

        /**
         * Return the opposite of the port
         *
         * @param klass
         *            the klass
         * @return the opposite of klass
         */
        public static Class<? extends BasicPort> getOpposite(Class<? extends BasicPort> klass) {
            for (IOBlocks b : IOBlocks.values()) {
                if (b.getReferencedPortClass() == klass) {
                    return b.getOppositeClass();
                }
            }
            return null;
        }

        /**
         * Create a corresponding I/O block
         *
         * @param port
         *            the port used as an output
         * @return the corresponding block
         */
        public static ContextUpdate createBlock(BasicPort port) {
            for (IOBlocks io : IOBlocks.values()) {
                if (io.getReferencedPortClass().isInstance(port)) {
                    try {
                        JavaController controller = new JavaController();

                        // create the Input/Output block
                        Constructor<? extends ContextUpdate> blockCstr = io.getReferencedClass().getConstructor(JavaController.class);
                        ContextUpdate block = blockCstr.newInstance(controller);

                        // create the single port of the block
                        Constructor<? extends BasicPort> portCstr = io.getOppositeClass().getConstructor(
                                    JavaController.class, Long.TYPE, Kind.class, Object.class, String.class, String.class);
                        BasicPort blockPort = portCstr.newInstance(
                                                  controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());

                        // inser the port into the newly created block
                        block.insert(blockPort);

                        return block;
                    } catch (ReflectiveOperationException e) {
                        Logger.getLogger(IOBlocks.class.getName()).severe(e.toString());
                    }
                }
            }

            return null;
        }

        /**
         * @return referenced class
         */
        public Class<? extends ContextUpdate> getReferencedClass() {
            return ioBlock;
        }

        /**
         * @return the port referenced class
         */
        public Class<? extends BasicPort> getReferencedPortClass() {
            return port;
        }

        public Class<? extends BasicPort> getAssignementCompatiblePortClass() {
            return assignement;
        }

        /**
         * @return the port opposite class
         */
        public Class<? extends BasicPort> getOppositeClass() {
            return opposite;
        }
    }

    private transient boolean isContextDependent;

    /**
     * Constructor
     */
    public ContextUpdate(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);
    }

    /**
     * Constructor used to allocate a new block on the Java side
     *
     * The caller should add a port child and setup the simulation function accordingly to the style.
     * @param controller the controller used to allocate the block
     * @param blockName the interface function and style applied on the block
     */
    protected ContextUpdate(JavaController controller, String blockName) {
        super(controller, controller.createObject(Kind.BLOCK), Kind.BLOCK, null, new mxGeometry(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT), blockName, new UID().toString());

        controller.setObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, blockName);
    }



    /**
     * @param context
     *            new context
     */
    public void onContextChange(String[] context) {
        // prevent to open twice
        if (isLocked()) {
            return;
        }

        // do not evaluate context is the block is not context dependent.
        if (!isContextDependent) {
            return;
        }

        LOG_LOCAL.finest("Update the I/O value from the context");

        // final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        // if (handler == null) {
        // return;
        // }
        //
        // try {
        // // Write scs_m
        // handler.writeBlock(this);
        // // Write context
        // handler.writeContext(context);
        //
        // String cmd = ScilabInterpreterManagement.buildCall("blk =
        // xcosBlockEval", getInterfaceFunctionName().toCharArray(),
        // ScilabDirectHandler.BLK.toCharArray(),
        // ScilabDirectHandler.CONTEXT.toCharArray());
        //
        // try {
        // ScilabInterpreterManagement.synchronousScilabExec(cmd);
        // } catch (InterpreterException e) {
        // e.printStackTrace();
        // }
        // BasicBlock modifiedBlock = handler.readBlock();
        // updateBlockSettings(modifiedBlock);
        //
        // } catch (ScicosFormatException e) {
        // LOG_LOCAL.severe(e.toString());
        // } finally {
        // handler.release();
        // }
    }
}
