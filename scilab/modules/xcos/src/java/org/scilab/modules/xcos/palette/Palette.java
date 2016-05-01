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

package org.scilab.modules.xcos.palette;

import java.awt.GraphicsEnvironment;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Deque;
import java.util.LinkedList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.SwingUtilities;

import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.javasci.JavasciException;
import org.scilab.modules.javasci.Scilab;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
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
import org.scilab.modules.xcos.graph.model.XcosCellFactory;

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

                        PaletteNode.refreshView(cat, pal);
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
     * Push a block list into Scilab.
     *
     * The block list is pushed into a "pal" variable, a pseudo palette.
     *
     * @param path the path used to export the palette tree
     * @throws JavasciException on pushing error
     * @throws InterruptedException on wait error
     * @throws InvocationTargetException on palette creation
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void get(final String[] path) throws JavasciException, InvocationTargetException, InterruptedException {
        SwingUtilities.invokeAndWait(new Runnable() {
            @Override
            public void run() {
                PaletteNode root = getPathNode(path, false);

                /*
                 * Create a pseudo palette
                 */
                final PreLoaded pal;
                if (root instanceof PreLoaded) {
                    pal = (PreLoaded) root;
                } else if (root instanceof Category) {
                    LinkedList<Category> stash = new LinkedList<>();
                    stash.add((Category) root);

                    pal = new PreLoaded();
                    pal.setName(root.getName());
                    pal.getBlock().addAll(list(stash, pal));
                } else {
                    pal = null;
                }


                /*
                 * Encode the pseudo-palette into a ScilabType
                 */
                final ScilabType element;
                if (pal != null) {
                    final PreLoadedElement encoder = new PreLoadedElement();
                    element = encoder.encode(pal, null);
                } else {
                    element = new ScilabDouble();
                }

                try {
                    Scilab.putInCurrentScilabSession("pal", element);
                } catch (JavasciException e) {
                    throw new RuntimeException(e);
                }

            }
        });
    }

    private static List<PaletteBlock> list(Deque<Category> stash, PreLoaded pal) {
        final ArrayList<PaletteBlock> blocks = new ArrayList<>();
        while (!stash.isEmpty()) {
            final Category c = stash.pop();
            for (PaletteNode n : c.getNode()) {
                if (n instanceof Category) {
                    stash.add((Category) n);
                } else if (n instanceof PreLoaded) {
                    final PreLoaded p = (PreLoaded) n;
                    blocks.addAll(p.getBlock());
                }

            }

        }
        return blocks;
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

                    PaletteNode.refreshView(node.getParent(), node);
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

                    PaletteNode.refreshView(node.getParent(), node);
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

                    PaletteNode.refreshView(toBeReloaded[0], null);
                    PaletteNode.refreshView(toBeReloaded[1], src);
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
     * Generate a palette block image from a block instance stored into scilab
     * (need a valid style).
     *
     * @param iconPath
     *            the output file path use to save the palette block.
     * @throws Exception
     *             on error
     */
    @ScilabExported(module = XCOS, filename = PALETTE_GIWS_XML)
    public static void generatePaletteIcon(final long uid, final String iconPath) throws Exception {
        /*
         * If the env. is headless does nothing
         */
        if (GraphicsEnvironment.isHeadless()) {
            LOG.warning("Headless environment detected, do not generate icons");
            return;
        }

        JavaController controller = new JavaController();
        Kind kind = controller.getKind(uid);

        final BasicBlock block = XcosCellFactory.createBlock(controller, uid, kind);
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

        JavaController controller = new JavaController();

        final XcosDiagram graph = new XcosDiagram(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM, "");
        graph.installListeners();

        graph.addCell(block);
        graph.selectAll();

        BlockPositioning.updateBlockView(graph, block);

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
}
