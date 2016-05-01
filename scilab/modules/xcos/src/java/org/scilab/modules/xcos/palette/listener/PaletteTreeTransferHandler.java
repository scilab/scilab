/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.palette.listener;

import java.awt.Point;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.io.StringWriter;
import java.util.Arrays;
import java.util.logging.Logger;

import javax.swing.JComponent;
import javax.swing.JTree;
import javax.swing.TransferHandler;
import javax.swing.tree.TreePath;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;

import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;

/**
 * Used to modify the palette tree
 */
@SuppressWarnings(value = { "serial" })
public class PaletteTreeTransferHandler extends TransferHandler {

    private static final Logger LOG = Logger.getLogger(PaletteTreeTransferHandler.class.getName());

    /**
     * Default constructor
     */
    public PaletteTreeTransferHandler() {
        super();
    }

    /**
     * @param c
     *            the current component
     * @return {@code COPY_OR_MOVE}
     * @see javax.swing.TransferHandler#getSourceActions(javax.swing.JComponent)
     */
    @Override
    public int getSourceActions(final JComponent c) {
        return COPY_OR_MOVE;
    }

    /**
     * Reload the tree when the action is done
     *
     * @param source
     *            the source
     * @param data
     *            the data
     * @param action
     *            current action
     * @see javax.swing.TransferHandler#exportDone(javax.swing.JComponent,
     *      java.awt.datatransfer.Transferable, int)
     */
    @Override
    protected void exportDone(final JComponent source, final Transferable data, final int action) {
        final PaletteTransferable transferable = (PaletteTransferable) data;

        if (action == MOVE && transferable.isMoved()) {
            updateTree((JTree) source, data);
        }
    }

    /**
     * Update the palette tree after a drag
     *
     * @param t
     *            the source component
     * @param data
     *            the transferable
     */
    private void updateTree(final JTree t, final Transferable data) {
        final TreePath path = t.getSelectionPath();
        PaletteNode newNode;

        try {
            newNode = (PaletteNode) data.getTransferData(PaletteTransferable.PALETTE_FLAVOR);
        } catch (final UnsupportedFlavorException e) {
            LOG.severe(e.toString());
            return;
        } catch (final IOException e) {
            LOG.severe(e.toString());
            return;
        }

        PaletteNode.refreshView((PaletteNode) path.getParentPath().getLastPathComponent(), null);
        PaletteNode.refreshView(newNode.getParent(), newNode);
    }

    /**
     * @param c
     *            the current component
     * @return the new transferable
     * @see javax.swing.TransferHandler#createTransferable(javax.swing.JComponent)
     */
    @Override
    protected Transferable createTransferable(final JComponent c) {
        if (c == null) {
            return null;
        }

        final JTree tree = (JTree) c;
        final PaletteNode node = (PaletteNode) tree.getSelectionPath().getLastPathComponent();

        return new PaletteTransferable(node);
    }

    /**
     * @param support
     *            the current support
     * @return true for a Category area, false otherwise
     * @see javax.swing.TransferHandler#canImport(javax.swing.TransferHandler.TransferSupport)
     */
    @Override
    public boolean canImport(final TransferSupport support) {
        final JTree tree = (JTree) support.getComponent();
        final Point point = support.getDropLocation().getDropPoint();
        final TreePath path = tree.getPathForLocation(point.x, point.y);

        if (path != null) {
            return Arrays.asList(support.getDataFlavors()).contains(PaletteTransferable.PALETTE_FLAVOR);
        }

        return false;
    }

    /**
     * @param support
     *            the current support
     * @return true if the import has been successfully performed, false
     *         otherwise.
     * @see javax.swing.TransferHandler#importData(javax.swing.TransferHandler.TransferSupport)
     */
    @Override
    public boolean importData(final TransferSupport support) {
        if (PaletteManagerView.get() == null) {
            PaletteManagerView.restore(null);
        }
        final JTree tree = PaletteManagerView.get().getTree();
        final JTree.DropLocation location = tree.getDropLocation();

        final Category newParent = (Category) location.getPath().getLastPathComponent();
        final int index = location.getChildIndex();

        final Transferable t = support.getTransferable();

        try {
            final PaletteNode child = (PaletteNode) t.getTransferData(PaletteTransferable.PALETTE_FLAVOR);

            final Category oldParent = child.getParent();
            oldParent.getNode().remove(child);
            child.setParent(newParent);

            if (index == -1) {
                newParent.getNode().add(child);
            } else {
                newParent.getNode().add(index, child);
            }
        } catch (final UnsupportedFlavorException e) {
            LOG.severe(e.toString());
            return false;
        } catch (final IOException e) {
            LOG.severe(e.toString());
            return false;
        }

        return true;
    }

    /**
     * Implement transferable for any Palette node.
     */
    private static class PaletteTransferable implements Transferable {
        /**
         * The palette data flavor
         */
        public static final DataFlavor PALETTE_FLAVOR = new DataFlavor(PaletteNode.class, null);
        private static final DataFlavor STRING_FLAVOR = DataFlavor.stringFlavor;

        private final PaletteNode node;
        private boolean moved;

        /**
         * Default constructor
         *
         * @param node
         *            the selected node
         */
        public PaletteTransferable(final PaletteNode node) {
            this.node = node;
            moved = false;
        }

        /**
         * @return the node or null if it has been moved
         */
        public final PaletteNode getNode() {
            return node;
        }

        /**
         * @return the moved flag
         */
        public final boolean isMoved() {
            return moved;
        }

        /**
         * @param flavor
         *            the current flavor
         * @return the associated {@link PaletteNode}
         * @throws UnsupportedFlavorException
         *             when the flavor is not supported
         * @throws IOException
         *             when deserialization fails
         * @see java.awt.datatransfer.Transferable#getTransferData(java.awt.datatransfer.DataFlavor)
         */
        @Override
        public Object getTransferData(final DataFlavor flavor) throws UnsupportedFlavorException, IOException {
            Object ret = null;

            if (flavor.equals(STRING_FLAVOR) && getNode() instanceof Category) {
                try {
                    final JAXBContext jaxbContext = JAXBContext.newInstance(PaletteManager.MODEL_CLASS_PACKAGE);
                    final Marshaller marshaller = jaxbContext.createMarshaller();
                    marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);

                    final StringWriter writer = new StringWriter();
                    marshaller.marshal(node, writer);
                    ret = writer.toString();
                } catch (final JAXBException e) {
                    throw new IOException(e);
                }
            } else if (flavor.equals(PALETTE_FLAVOR)) {
                ret = node;
                moved = true;
            }

            if (ret == null) {
                throw new UnsupportedFlavorException(flavor);
            } else {
                return ret;
            }
        }

        /**
         * @return the valid data flavors
         * @see java.awt.datatransfer.Transferable#getTransferDataFlavors()
         */
        @Override
        public DataFlavor[] getTransferDataFlavors() {
            final Category parent = getNode().getParent();
            if (parent.getParent() == null && parent.getChildAt(0) == getNode()) {
                return new DataFlavor[] { STRING_FLAVOR };
            } else if (getNode() instanceof Category) {
                return new DataFlavor[] { PALETTE_FLAVOR, STRING_FLAVOR };
            } else {
                return new DataFlavor[] { PALETTE_FLAVOR };
            }
        }

        /**
         * @param flavor
         *            the flavor
         * @return true if the flavor match the right mime type; false
         *         otherwise.
         * @see java.awt.datatransfer.Transferable#isDataFlavorSupported(java.awt.datatransfer.DataFlavor)
         */
        @Override
        public boolean isDataFlavorSupported(final DataFlavor flavor) {
            return Arrays.asList(getTransferDataFlavors()).contains(flavor);
        }
    }
}
