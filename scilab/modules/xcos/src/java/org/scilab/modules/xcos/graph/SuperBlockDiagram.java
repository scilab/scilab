/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.io.File;
import java.io.Serializable;
import java.util.Hashtable;
import java.util.List;
import java.util.logging.Logger;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;

public final class SuperBlockDiagram extends XcosDiagram implements Serializable, Cloneable {

    private static final String PARENT_DIAGRAM_WAS_NULL = "Parent diagram was null";
    private static final long serialVersionUID = -402918614723713301L;

    private SuperBlock container;

    /**
     * Constructor
     */
    public SuperBlockDiagram() {
        super();
    }

    /**
     * @param superBlock
     *            parent super block
     */
    public SuperBlockDiagram(SuperBlock superBlock) {
        super();
        setContainer(superBlock);
    }

    /**
     * @return parent super block
     */
    public SuperBlock getContainer() {
        return container;
    }

    /**
     * @param container
     *            parent super block
     */
    public void setContainer(SuperBlock container) {
        this.container = container;
    }

    @Override
    public File getSavedFile() {
        if (getContainer() != null) {
            XcosDiagram parent = getContainer().getParentDiagram();
            if (parent != null) {
                return parent.getSavedFile();
            }
        }

        return super.getSavedFile();
    }

    /**
     * Concatenate the context with the parent one
     *
     * @return the context
     * @see org.scilab.modules.xcos.graph.XcosDiagram#getContext()
     */
    @Override
    public String[] getContext() {
        final SuperBlock block = getContainer();
        XcosDiagram graph = block.getParentDiagram();
        if (graph == null) {
            block.setParentDiagram(Xcos.findParent(block));
            graph = block.getParentDiagram();
            Logger.getLogger(SuperBlockDiagram.class.getName()).finest(PARENT_DIAGRAM_WAS_NULL);
        }

        final String[] parent;
        if (graph == null) {
            parent = new String[] {};
        } else {
            parent = graph.getContext();
        }

        final String[] current = super.getContext();

        String[] full = new String[current.length + parent.length];
        System.arraycopy(parent, 0, full, 0, parent.length);
        System.arraycopy(current, 0, full, parent.length, current.length);
        return full;
    }

    /**
     * Validate I/O ports.
     *
     * /!\ No model modification should be made in this method, this is only a validation method.
     *
     * @param cell
     *            Cell that represents the cell to validate.
     * @param context
     *            Hashtable that represents the global validation state.
     */
    @SuppressWarnings("unchecked")
    @Override
    public String validateCell(final Object cell, final Hashtable<Object, Object> context) {
        String err = null;

        /*
         * Only validate I/O blocks
         */

        // get the key
        final String key;
        if (cell instanceof ExplicitInBlock || cell instanceof ImplicitInBlock) {
            key = IN;
        } else if (cell instanceof ExplicitOutBlock || cell instanceof ImplicitOutBlock) {
            key = OUT;
        } else if (cell instanceof EventInBlock) {
            key = EIN;
        } else if (cell instanceof EventOutBlock) {
            key = EOUT;
        } else {
            key = null;
        }

        final BasicBlock block;
        if (key != null) {
            block = (BasicBlock) cell;
        } else {
            return err;
        }

        /*
         * Prepare validation
         */

        // fill the context
        fillContext(context);

        /*
         * Validate with ipar
         */

        // get the real index
        final List <? extends BasicBlock > blocks = (List <? extends BasicBlock > ) context.get(key);
        final int realIndex = blocks.indexOf(block) + 1;

        // get the user index
        final ScilabDouble data = (ScilabDouble) block.getIntegerParameters();
        if (data.getWidth() < 1 || data.getHeight() < 1) {
            return err;
        }
        final int userIndex = (int) data.getRealPart()[0][0];

        // if the indexes are not equals, alert the user.
        if (realIndex != userIndex) {
            final StringBuilder str = new StringBuilder();
            str.append("<html><body><em>");
            str.append(XcosMessages.WRONG_PORT_NUMBER);
            str.append("</em><br/>");
            str.append(String.format(XcosMessages.EXPECTING_NUMBER, realIndex));
            str.append("</body></html>    ");

            err = str.toString();
        }

        return err;
    }

    /**
     * Listener for SuperBlock diagram events.
     */
    @SuppressWarnings(value = { "serial" })
    private static final class GenericSuperBlockListener implements mxIEventListener, Serializable {
        private static GenericSuperBlockListener instance;

        /**
         * Reduce constructor visibility
         */
        private GenericSuperBlockListener() {
            super();
        }

        /**
         * Mono-threaded singleton implementation getter
         *
         * @return The unique instance
         */
        public static GenericSuperBlockListener getInstance() {
            if (instance == null) {
                instance = new GenericSuperBlockListener();
            }
            return instance;
        }

        /**
         * Update the IOPorts colors and values.
         *
         * @param arg0
         *            the source
         * @param arg1
         *            the event data
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(Object arg0, mxEventObject arg1) {
            final SuperBlockDiagram graph = ((SuperBlockDiagram) arg0);
            final SuperBlock block = graph.getContainer();
            if (block != null) {
                block.updateExportedPort();
            }

            // validate display errors
            graph.getAsComponent().clearCellOverlays();
            graph.getAsComponent().validateGraph();
        }
    }

    /**
     * Listener for SuperBlock diagram events.
    @SuppressWarnings(value = { "serial" })
     */
    private static final class ContentChangedListener implements mxIEventListener, Serializable {
        private static ContentChangedListener instance;

        /**
         * Reduce constructor visibility
         */
        private ContentChangedListener() {
            super();
        }

        /**
         * Mono-threaded singleton implementation getter
         *
         * @return The unique instance
         */
        public static ContentChangedListener getInstance() {
            if (instance == null) {
                instance = new ContentChangedListener();
            }
            return instance;
        }

        /**
         * Update the IOPorts colors and values.
         *
         * @param arg0
         *            the source
         * @param arg1
         *            the event data
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(Object arg0, mxEventObject arg1) {
            final SuperBlockDiagram graph = ((SuperBlockDiagram) arg0);
            final SuperBlock block = graph.getContainer();
            if (block != null) {
                block.invalidateRpar();
            }
        }
    }

    /**
    @SuppressWarnings(value = { "serial" })
     * Update the diagram labels
     */
    private static final class LabelBlockListener implements mxIEventListener, Serializable {
        private static LabelBlockListener instance;

        /**
         * Default Constructor
         */
        private LabelBlockListener() {
            super();
        }

        /**
         * @return the instance
         */
        public static LabelBlockListener getInstance() {
            if (instance == null) {
                instance = new LabelBlockListener();
            }
            return instance;
        }

        /** {@inheritDoc} */
        @Override
        public void invoke(Object sender, mxEventObject evt) {
            final String value = (String) evt.getProperty("value");
            final Object parent = evt.getProperty("parent");
            if (parent instanceof ContextUpdate) {
                final ContextUpdate block = (ContextUpdate) parent;
                final ScilabDouble data = (ScilabDouble) block.getIntegerParameters();

                final int index;
                if (data.getHeight() > 0 && data.getWidth() > 0) {
                    index = (int) data.getRealPart()[0][0];
                } else {
                    index = 1;
                }

                final SuperBlock container = ((SuperBlockDiagram) sender).getContainer();
                if (container == null) {
                    return;
                }

                container.sortChildren();
                final List<mxICell> ports = IOBlocks.getPorts(container, block.getClass());

                XcosDiagram graph = container.getParentDiagram();
                if (graph == null) {
                    container.setParentDiagram(Xcos.findParent(container));
                    graph = container.getParentDiagram();
                    Logger.getLogger(SuperBlockDiagram.class.getName()).finest(PARENT_DIAGRAM_WAS_NULL);
                }

                if (index > 0 && index <= ports.size()) {
                    container.getParentDiagram().cellLabelChanged(ports.get(index - 1), value, false);
                }
            }
        }
    }

    /**
     * Install the specific listeners for {@link SuperBlockDiagram}.
     */
    public void installSuperBlockListeners() {
        removeListener(GenericSuperBlockListener.getInstance());
        removeListener(LabelBlockListener.getInstance());
        removeListener(ContentChangedListener.getInstance());

        addListener(mxEvent.CELLS_ADDED, GenericSuperBlockListener.getInstance());
        addListener(mxEvent.CELLS_REMOVED, GenericSuperBlockListener.getInstance());
        addListener(XcosEvent.IO_PORT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());
        addListener(mxEvent.LABEL_CHANGED, LabelBlockListener.getInstance());

        addListener(mxEvent.CELLS_ADDED, ContentChangedListener.getInstance());
        addListener(mxEvent.CELLS_MOVED, ContentChangedListener.getInstance());
        addListener(mxEvent.CELLS_REMOVED, ContentChangedListener.getInstance());
        addListener(mxEvent.CELLS_RESIZED, ContentChangedListener.getInstance());
    }

    /**
     * This function set the SuperBlock diagram and all its parents in a
     * modified state or not.
     *
     * @param modified
     *            status
     */
    @Override
    public void setModified(boolean modified) {
        super.setModified(modified);

        if (getContainer() != null && getContainer().getParentDiagram() != null) {
            getContainer().getParentDiagram().setModified(modified);
        }
    }

    /**
     * This function set the SuperBlock diagram in a modified state or not.
     *
     * It doesn't perform recursively on the parent diagrams. If you want such a
     * behavior use setModified instead.
     *
     * @param modified
     *            status
     * @see #setModified
     */
    public void setModifiedNonRecursively(boolean modified) {
        super.setModified(modified);
    }

    /** {@inheritDoc} */
    // CSOFF: SuperClone
    @Override
    public Object clone() throws CloneNotSupportedException {
        final SuperBlockDiagram clone = new SuperBlockDiagram();
        clone.installListeners();
        clone.installSuperBlockListeners();

        clone.setScicosParameters((ScicosParameters) getScicosParameters().clone());
        clone.addCells(cloneCells(getChildCells(getDefaultParent())), clone.getDefaultParent());

        return clone;
    }
    // CSON: SuperClone
}
