/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.synchronousScilabExec;

import java.awt.GraphicsEnvironment;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.view.mxGraphView;
import com.mxgraph.view.mxStylesheet;

/**
 * Utility class which is the entry point from Scilab for palette related
 * functions.
 */
public final class Palette {
    /** the "name" argument */
    public static final String NAME = "name";

    /** Error message used on invalid path */
    public static final String WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH = Messages.gettext("Wrong input argument \"%s\": invalid tree path.\n");
    /** Error message used on invalid node */
    public static final String WRONG_INPUT_ARGUMENT_S_INVALID_NODE = Messages
            .gettext("Wrong input argument \"%s\": invalid node, use 'xcosPalDisable' instead.\n");
    /** "Unable to import" string */
    public static final String UNABLE_TO_IMPORT = Messages.gettext("Unable to import %s .\n");

    private static final Logger LOG = Logger.getLogger(Palette.class.getName());
    private static final String XCOS = "xcos";
    private static final String PALETTE_GIWS_XML = "Palette.giws.xml";

    /**
     * Default hidden constructor
     */
    private Palette() {
    }

    /**
     * Get the {@link PaletteNode} of the path.
     *
     * @param path
     *            the path
     * @param create
     *            if <code>true</code> the Category path will be created,
     *            otherwise it will not.
     * @return the selected node
     */
    private static PaletteNode getPathNode(final String[] path, final boolean create) {

        if (!SwingUtilities.isEventDispatchThread()) {
            throw new RuntimeException("Unable to manipulate palette outside the EDT thread.");
        }

        Category node = PaletteManager.getInstance().getRoot();

        if (path == null || path.length == 0 || (path.length == 1 && path[0].isEmpty())) {
            return node;
        }

        for (int categoryCounter = 0; categoryCounter < path.length; categoryCounter++) {

            for (final PaletteNode next : node.getNode()) {
                if (next.getName().equals(path[categoryCounter]) && next instanceof Category) {
                    node = (Category) next;
                    break;
                } else if (next.getName().equals(path[categoryCounter]) && (categoryCounter == path.length - 1)) {
                    return next; // found the terminal Palette instance
                }
            }

            if (!node.toString().equals(path[categoryCounter])) {
                if (create) {
                    final Category cat = new Category();
                    cat.setName(path[categoryCounter]);
                    cat.setEnable(create);

                    cat.setParent(node);
                    node.getNode().add(cat);

                    node = cat;
                } else {
                    return null;
                }
            }
        }
        return node;
    }

    /**
     * Load an xcos palette into the palette manager
     *
     * @param name
     *            the scilab exported palette variable name
     * @param category
     *            TreePath of the palette
     * @throws JavasciException
     *             on invocation error
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void loadPal(final String name, final String[] category) throws JavasciException {
        /*
         * If the env. is headless only perform fake loading to assert data
         * integrity.
         */
        if (GraphicsEnvironment.isHeadless()) {
            LOG.warning("Headless environment detected, only perform sanity check");
            loadPalHeadless(name);
            return;
        }

        /*
         * Import the palette
         */
        final ScilabTList data = (ScilabTList) Scilab.getInCurrentScilabSession(name);

        /*
         * handle shared data on the EDT thread
         */
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    try {
                        /*
                         * Decode the style part of the palette
                         */
                        final mxStylesheet styleSheet = Xcos.getInstance().getStyleSheet();
                        try {
                            new StyleElement().decode(data, styleSheet);
                        } catch (final ScicosFormatException e) {
                            throw new RuntimeException(e);
                        }

                        // reload all the opened diagram (clear states)
                        for (final XcosDiagram d : Xcos.getInstance().openedDiagrams()) {
                            if (d != null) {
                                final mxGraphView view = d.getView();
                                if (view != null) {
                                    view.reload();
                                }

                                final mxGraphComponent comp = d.getAsComponent();
                                if (comp != null) {
                                    comp.refresh();
                                }
                            }
                        }

                        final PaletteNode node = getPathNode(category, true);
                        if (!(node instanceof Category)) {
                            throw new RuntimeException(String.format(WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, "category"));
                        }
                        final Category cat = (Category) node;

                        /*
                         * Adding the palette tree part of the palette
                         */
                        PreLoaded pal;
                        try {
                            pal = new PreLoadedElement().decode(data, new PreLoaded.Dynamic());
                        } catch (final ScicosFormatException e) {
                            throw new RuntimeException(e);
                        }
                        cat.getNode().add(pal);
                        pal.setParent(cat);

                        PaletteNode.refreshView(pal);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        } catch (final InterruptedException e) {
            LOG.severe(e.toString());
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    private static final void loadPalHeadless(final String name) throws JavasciException {
        try {
            final ScilabTList data = (ScilabTList) Scilab.getInCurrentScilabSession(name);

            // style check
            new StyleElement().decode(data, new mxStylesheet());

            // palette data check
            new PreLoadedElement().decode(data, new PreLoaded.Dynamic());

        } catch (ScicosFormatException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Load an xcos palette into the palette manager at the root category.
     *
     * @param name
     *            the scilab exported palette variable name
     * @throws JavasciException
     *             on invocation error
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void loadPal(final String name) throws JavasciException {
        loadPal(name, null);
    }

    /**
     * Add a category into the palette manager
     *
     * @param name
     *            TreePath of the palette
     * @param visible
     *            default visibility of the palette
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void addCategory(final String[] name, final boolean visible) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    final PaletteNode node = getPathNode(name, true);
                    if (node instanceof Category) {
                        node.setEnable(visible);
                    } else {
                        throw new RuntimeException(String.format(WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, NAME));
                    }

                    PaletteNode.refreshView(node.getParent());
                }
            });
        } catch (final InterruptedException e) {
            Logger.getLogger(Palette.class.getName()).severe(e.toString());
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Remove a palette or a category of the palette manager
     *
     * @param name
     *            TreePath of the palette
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void remove(final String[] name) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    final PaletteNode node = getPathNode(name, false);
                    PaletteNode.remove(node);
                }
            });
        } catch (final InterruptedException e) {
            LOG.severe(e.toString());
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Remove a palette or a category of the palette manager
     *
     * @param name
     *            TreePath of the palette or category
     * @param status
     *            True to set the palette visible, false otherwise
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void enable(final String[] name, final boolean status) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    final PaletteNode node = getPathNode(name, false);
                    if (node == null) {
                        throw new RuntimeException(String.format(WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, NAME));
                    }

                    node.setEnable(status);

                    PaletteNode.refreshView(node.getParent());
                }
            });
        } catch (final InterruptedException e) {
            LOG.severe(e.toString());
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Move a palette or a category of the palette manager
     *
     * @param source
     *            TreePath of the palette or category
     * @param target
     *            TreePath of the destination
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void move(final String[] source, final String[] target) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {

                    final PaletteNode src = getPathNode(source, false);
                    if (src == null) {
                        throw new RuntimeException(String.format(WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, "source"));
                    }

                    final PaletteNode trg = getPathNode(target, true);
                    if (trg == null || !(trg instanceof Category)) {
                        throw new RuntimeException(String.format(WRONG_INPUT_ARGUMENT_S_INVALID_TREE_PATH, "target"));
                    }
                    final Category destination = (Category) trg;

                    final Category[] toBeReloaded = new Category[] { src.getParent(), destination };

                    if (toBeReloaded[0] != null) {
                        toBeReloaded[0].getNode().remove(src);
                    }
                    destination.getNode().add(src);
                    src.setParent(destination);

                    PaletteNode.refreshView(toBeReloaded[0]);
                    PaletteNode.refreshView(toBeReloaded[1]);
                }
            });
        } catch (final InterruptedException e) {
            LOG.severe(e.toString());
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Generate a palette block image from a block saved instance (need a valid
     * style).
     *
     * @param blockName
     *            the block name
     * @param iconPath
     *            the output file path use to save the palette block.
     * @throws IOException
     *             in case of write errors
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void generatePaletteIcon(final String blockName, final String iconPath) throws IOException {
        BasicBlock block;

        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        try {
            synchronousScilabExec(ScilabDirectHandler.BLK + " = " + buildCall(blockName, "define"));
            block = handler.readBlock();
        } catch (ScicosFormatException e) {
            throw new IOException(e);
        } catch (InterpreterException e) {
            throw new IOException(e);
        } finally {
            handler.release();
        }

        generateIcon(block, iconPath);

        if (LOG.isLoggable(Level.FINEST)) {
            LOG.finest(iconPath + " updated.");
        }
    }

    private static void generateIcon(BasicBlock block, final String iconPath) throws IOException {
        if (block == null || block.getGeometry() == null) {
            return;
        }
        block.getGeometry().setX(XcosConstants.PALETTE_BLOCK_WIDTH);
        block.getGeometry().setY(XcosConstants.PALETTE_BLOCK_HEIGHT);

        final XcosDiagram graph = new XcosDiagram();
        graph.installListeners();

        graph.addCell(block);
        graph.selectAll();

        BlockPositioning.updateBlockView(block);

        /*
         * Render
         */
        final mxGraphComponent graphComponent = graph.getAsComponent();
        graphComponent.refresh();

        final mxRectangle bounds = graph.getPaintBounds(new Object[] { block });
        final double width = bounds.getWidth();
        final double height = bounds.getHeight();

        final double scale;
        if (width > XcosConstants.PALETTE_BLOCK_WIDTH || height > XcosConstants.PALETTE_BLOCK_HEIGHT) {
            scale = Math.min(XcosConstants.PALETTE_BLOCK_WIDTH / width, XcosConstants.PALETTE_BLOCK_HEIGHT / height) / XcosConstants.PALETTE_BLOCK_ICON_RATIO;
        } else {
            scale = 1.0;
        }

        final BufferedImage image = mxCellRenderer.createBufferedImage(graph, null, scale, graphComponent.getBackground(), graphComponent.isAntiAlias(), null,
                                    graphComponent.getCanvas());

        final String extension = iconPath.substring(iconPath.lastIndexOf('.') + 1);
        ImageIO.write(image, extension, new File(iconPath));
    }

    /**
     * Helper function used to regenerate palette block images from block
     * instance.
     *
     * This method is not export to scilab but is needed for internal purpose
     * (palette block image generation)
     */
    public static void generateAllPaletteImages() {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                final PaletteManager current = PaletteManager.getInstance();
                iterate(current.getRoot().getNode());
                current.saveConfig();
            }

            private void iterate(final List<PaletteNode> children) {
                for (final PaletteNode next : children) {
                    if (next instanceof Category) {
                        iterate(((Category) next).getNode());
                    } else if (next instanceof PreLoaded) {
                        generatePreLoadedIcon((PreLoaded) next);
                    }
                }
            }

            /**
             * Generate PreLoaded icon file.
             *
             * @param current
             *            the current node
             */
            private void generatePreLoadedIcon(final PreLoaded current) {
                final List<PaletteBlock> blocks = current.getBlock();
                for (final PaletteBlock paletteBlock : blocks) {

                    try {
                        final BasicBlock block = new PaletteBlockCtrl(paletteBlock).loadBlock();
                        generateIcon(block, paletteBlock.getIcon().getEvaluatedPath());
                    } catch (IOException e) {
                        e.printStackTrace();
                    } catch (ScicosFormatException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        LOG.finest("All images has been generated.");
    }
}
