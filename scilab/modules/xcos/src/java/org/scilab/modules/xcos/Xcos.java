/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.awt.Component;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.logging.LogManager;
import java.util.logging.Logger;

import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.core.Scilab;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.actions.ExternalAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.graph.DiagramComparator;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.preferences.XcosConfiguration;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.view.mxStylesheet;

/**
 * Xcos entry point class
 */
// CSOFF: ClassFanOutComplexity
// CSOFF: ClassDataAbstractionCoupling
public final class Xcos {
    /**
     * The current Xcos version
     */
    public static final String VERSION = "1.0";
    /**
     * The current Xcos tradename
     */
    public static final String TRADENAME = "Xcos";
    public static final ImageIcon ICON = new ImageIcon(FindIconHelper.findIcon("utilities-system-monitor", "256x256"));

    private static final String LOAD_XCOS_LIBS_LOAD_SCICOS = "prot=funcprot(); funcprot(0); loadXcosLibs(); loadScicos(); funcprot(prot); clear prot";

    /*
     * Dependencies version
     */
    private static final List<String> MXGRAPH_VERSIONS = null;
    private static final List<String> BATIK_VERSIONS = Arrays.asList("1.7", "1.8pre", "1.8");

    private static final String UNABLE_TO_LOAD_JGRAPHX = Messages.gettext("Unable to load the jgraphx library.\nExpecting version %s ; Getting version %s .");
    private static final String UNABLE_TO_LOAD_BATIK = Messages.gettext("Unable to load the Batik library. \nExpecting version %s ; Getting version %s .");

    private static final String CALLED_OUTSIDE_THE_EDT_THREAD = "Called outside the EDT thread.";
    private static final Logger LOG = Logger.getLogger(Xcos.class.getSimpleName());

    /** common shared instance */
    private static volatile Xcos sharedInstance;

    static {
        Scilab.registerInitialHook(new Runnable() {
            @Override
            public void run() {
                /* load scicos libraries (macros) */
                InterpreterManagement.requestScilabExec(LOAD_XCOS_LIBS_LOAD_SCICOS);
            }
        });

        XConfiguration.addXConfigurationListener(new XcosConfiguration());
    }

    /*
     * Instance data
     */
    private final Map<File, Collection<XcosDiagram>> diagrams;
    private boolean onDiagramIteration = false;
    private String lastError = null;

    /*
     * Instance handlers
     */
    private final PaletteManager palette;
    private final ConfigurationManager configuration;
    private final mxStylesheet styleSheet;
    private final List<ExternalAction> externalActions;

    private final XcosTabFactory factory;

    /**
     * Construct an Xcos instance.
     *
     * There must be only one Xcos instance per Scilab application
     */
    private Xcos(final XcosTabFactory factory) {
        /*
         * Read the configuration to support dynamic (before Xcos launch)
         * settings.
         */
        try {
            LogManager.getLogManager().readConfiguration();
        } catch (final SecurityException e) {
            LOG.severe(e.toString());
        } catch (final IOException e) {
            LOG.severe(e.toString());
        }

        /* Check the dependencies at startup time */
        checkDependencies();

        /*
         * Allocate data
         */
        diagrams = new HashMap<File, Collection<XcosDiagram>>();
        // null is used for not saved diagrams
        addDiagram(null, null);

        /*
         * get the handlers instance
         */
        palette = PaletteManager.getInstance();
        configuration = ConfigurationManager.getInstance();
        styleSheet = new mxStylesheet();
        externalActions = new ArrayList<ExternalAction>();

        try {
            FileUtils.decodeStyle(styleSheet);
        } catch (final IOException e) {
            LOG.severe(e.toString());
        }

        /*
         * Register as an AbstractScilabTabFactory
         */
        if (factory == null) {
            this.factory = new XcosTabFactory(false);
        } else {
            this.factory = factory;
        }
        ScilabTabFactory.getInstance().addTabFactory(this.factory);
    }

    /**
     * Check the dependencies and the version dependencies.
     *
     * This method use runtime class loading to handle ClassNotFoundException.
     *
     * This method catch any exception and rethrow it with a well defined
     * message. Thus it doesn't pass the IllegalCatch metrics.
     */
    // CSOFF: IllegalCatch
    // CSOFF: MagicNumber
    private void checkDependencies() {
        final ClassLoader loader = ClassLoader.getSystemClassLoader();

        /* JGraphx */
        String mxGraphVersion = "";
        try {
            final Class<?> klass = loader.loadClass("com.mxgraph.view.mxGraph");
            mxGraphVersion = (String) klass.getDeclaredField("VERSION").get(null);

            if (MXGRAPH_VERSIONS != null && !MXGRAPH_VERSIONS.contains(mxGraphVersion)) {
                throw new Exception();
            }
        } catch (final Throwable e) {
            throw new RuntimeException(String.format(UNABLE_TO_LOAD_JGRAPHX, MXGRAPH_VERSIONS.get(0), mxGraphVersion), e);
        }

        /* Batik */
        String batikVersion = null;
        try {
            final Class<?> klass = loader.loadClass("org.apache.batik.Version");
            batikVersion = klass.getPackage().getImplementationVersion().split("\\+")[0];

            if (!BATIK_VERSIONS.contains(batikVersion)) {
                throw new Exception();
            }

        } catch (final Throwable e) {
            throw new RuntimeException(String.format(UNABLE_TO_LOAD_BATIK, BATIK_VERSIONS.get(0), batikVersion), e);
        }
    }

    // CSON: MagicNumber
    // CSON: IllegalCatch

    /**
     * @return the per Scilab application, Xcos instance
     */
    public static synchronized Xcos getInstance() {
        return getInstance(null);
    }

    /**
     * @param factory
     *            the tab factory instance or null on creation
     * @return the per Scilab application, Xcos instance
     */
    private static synchronized Xcos getInstance(final XcosTabFactory factory) {
        if (sharedInstance == null) {
            sharedInstance = new Xcos(factory);

            LOG.finest("Session started");
        }

        return sharedInstance;
    }

    /**
     * Try to quit xcos
     */
    public void quit(boolean force) {
        if (sharedInstance == null) {
            return;
        }

    }

    /**
     * Clear the shared instance.
     */
    private static synchronized void clearInstance() {
        sharedInstance = null;
        LOG.finest("Session ended");
    }

    /**
     * All Opened diagrams
     *
     * @return the opened diagrams list
     */
    public List<XcosDiagram> openedDiagrams() {
        final List<XcosDiagram> opened = new ArrayList<XcosDiagram>();
        for (File f : diagrams.keySet()) {
            opened.addAll(openedDiagrams(f));
        }

        return opened;
    }

    /**
     * Opened diagrams
     *
     * @param f
     *            the file
     * @return the opened diagrams list
     */
    public List<XcosDiagram> openedDiagrams(File f) {
        final List<XcosDiagram> opened = new ArrayList<XcosDiagram>();
        for (XcosDiagram d : diagrams.get(f)) {
            if (d.isOpened()) {
                opened.add(d);
            }
        }

        return opened;
    }

    /**
     * Check if the in memory file representation is modified
     *
     * @param f
     *            the file
     * @return is modified
     */
    public boolean isModified(File f) {
        for (XcosDiagram d : diagrams.get(f)) {
            if (d.isModified()) {
                return true;
            }
        }

        return false;
    }

    /**
     * @return the global shared styleSheet
     */
    public mxStylesheet getStyleSheet() {
        return styleSheet;
    }

    /**
     * Open a file from it's filename.
     *
     * This method must be called on the EDT thread. For other use, please use
     * the {@link #xcos(String, String)} method.
     *
     * @param file
     *            the file to open. If null an empty diagram is created.
     * @param variable
     *            the variable to decode. If null no decode is performed.
     */
    public void open(final String file, final String variable) {
        if (!SwingUtilities.isEventDispatchThread()) {
            LOG.severe(CALLED_OUTSIDE_THE_EDT_THREAD);
        }

        /*
         * If it is the first window opened, then open the palette first.
         */
        if (file == null && variable == null && openedDiagrams().isEmpty()) {
            PaletteManager.setVisible(true);
        }

        XcosDiagram diag = null;
        final File f;
        if (file != null) {
            f = new File(file);
        } else {
            f = null;
        }

        if (f != null && f.exists()) {
            configuration.addToRecentFiles(f);
        }

        /*
         * looking for an already opened diagram
         */
        final Collection<XcosDiagram> diags = diagrams.get(f);
        if (diags != null && !diags.isEmpty()) {
            diag = diags.iterator().next();
        }
        // if unsaved and empty, reuse it. Allocate otherwise.
        if (f == null && diag != null && diag.getModel().getChildCount(diag.getDefaultParent()) > 0) {
            diag = null;
        }
        // if reuse then request focus
        if (diag != null) {
            XcosTab tab = XcosTab.get(diag);
            if (tab != null) {
                tab.setCurrent();
                tab.requestFocus();
            }
        }

        if (diag != null) {
            // loading disabled, unlock
            synchronized (this) {
                setLastError("");
                notify();
            }
        } else {
            // loading enable, unlock will be performed later, on another thread

            /*
             * Allocate and setup a new diagram
             */
            diag = new XcosDiagram();
            diag.installListeners();

            /*
             * Ask for file creation
             */
            if (f != null && !f.exists()) {
                if (!diag.askForFileCreation(f)) {
                    // loading disabled, unlock
                    synchronized (this) {
                        setLastError("");
                        notify();
                    }

                    // return now, to avoid tab creation
                    return;
                }
            }

            /*
             * Create a visible window before loading
             */
            if (XcosTab.get(diag) == null) {
                XcosTab.restore(diag);
            }

            /*
             * Load the file
             */
            diag.transformAndLoadFile(file, variable);

            if (diag != null) {
                addDiagram(diag.getSavedFile(), diag);
            }
        }

        if (diag != null) {
            diag.updateTabTitle();
        }
    }

    /**
     * Log a loading error
     *
     * @param lastError
     *            the error description
     */
    public void setLastError(String error) {
        this.lastError = error;
    }

    /**
     * Get an unmodifiable view of the diagrams for a specific file
     *
     * @param f
     *            the file
     * @return the diagram collection
     */
    public Collection<XcosDiagram> getDiagrams(final File f) {
        final Collection<XcosDiagram> diags = diagrams.get(f);
        if (diags == null) {
            return null;
        }
        return Collections.unmodifiableCollection(diags);
    }

    /**
     * Add a diagram to the diagram list for a file. Be sure to set the right
     * opened status on the diagram before calling this method.
     *
     * @param f
     *            the file
     * @param diag
     *            the diag
     */
    public void addDiagram(final File f, final XcosDiagram diag) {
        if (onDiagramIteration) {
            throw new RuntimeException();
        }

        /*
         * Create the collection if it does not exist
         */
        Collection<XcosDiagram> diags = diagrams.get(f);
        if (diags == null) {
            diags = createDiagramCollection();
            diagrams.put(f, diags);
        }

        if (diag != null) {
            /*
             * Remove the diagram (and any child)
             */
            final Collection<XcosDiagram> toBeMoved = removeChildren(diag);

            /*
             * Add the diagram to the collection
             */
            diags.addAll(toBeMoved);
        }
    }

    private Collection<XcosDiagram> removeChildren(XcosDiagram diag) {
        final Collection<XcosDiagram> removed = new HashSet<XcosDiagram>();
        removed.add(diag);

        for (Collection<XcosDiagram> it : diagrams.values()) {
            if (!it.contains(diag)) {
                continue;
            }

            /*
             * Add all children to the removed collection.
             */
            for (XcosDiagram graph : it) {
                if (graph instanceof SuperBlockDiagram) {
                    final XcosDiagram parent = ((SuperBlockDiagram) graph).getContainer().getParentDiagram();

                    // As "it" is sorted according to the hierarchy, "removed"
                    // is also ordered.
                    if (removed.contains(parent)) {
                        removed.add(graph);
                    }
                }

            }

            /*
             * really remove them all
             */
            it.removeAll(removed);

        }

        return removed;
    }

    /**
     * Create a diagram collections (sorted List)
     *
     * @return the diagram collection
     */
    @SuppressWarnings("serial")
    public Collection<XcosDiagram> createDiagramCollection() {
        return new ArrayList<XcosDiagram>() {
            @Override
            public boolean add(XcosDiagram element) {
                final boolean status = super.add(element);
                DiagramComparator.sort(this);
                return status;
            }

            @Override
            public boolean addAll(Collection <? extends XcosDiagram > c) {
                final boolean status = super.addAll(c);
                DiagramComparator.sort(this);
                return status;
            }
        };
    }

    /**
     * Try to close the graph (popup save dialog)
     *
     * @param graph
     *            the graph to close
     * @return if we can (or not) close the graph
     */
    public boolean canClose(final XcosDiagram graph) {
        boolean canClose = false;
        final File f = graph.getSavedFile();

        final boolean wasLastOpened = openedDiagrams(f).size() <= 1;
        final boolean isModified = isModified(f);
        if (!(wasLastOpened && isModified)) {
            canClose = true;
        }

        if (!canClose) {
            final AnswerOption ans = ScilabModalDialog.show(XcosTab.get(graph), XcosMessages.DIAGRAM_MODIFIED, XcosMessages.XCOS, IconType.QUESTION_ICON,
                                     ButtonType.YES_NO_CANCEL);

            switch (ans) {
                case YES_OPTION:
                    canClose = diagrams.get(f).iterator().next().saveDiagram();
                    break;
                case NO_OPTION:
                    canClose = true; // can close
                    break;
                default:
                    canClose = false; // operation canceled
                    break;
            }
        }

        /*
         * Update configuration before the destroy call to validate the uuid
         */
        if (canClose) {
            configuration.addToRecentTabs(graph);
            configuration.saveConfig();
        }
        return canClose;
    }

    /**
     * Close a diagram.
     *
     * This method must be called on the EDT thread.
     *
     * @param graph
     *            the diagram to close
     */
    public void destroy(XcosDiagram graph) {
        final File f = graph.getSavedFile();
        final boolean wasLastOpenedForFile = openedDiagrams(f).size() <= 1;

        if (!onDiagramIteration && wasLastOpenedForFile) {
            diagrams.remove(f);
        }

        if (openedDiagrams().size() <= 0) {
            /* halt scicos (stop the simulation) */
            InterpreterManagement.requestScilabExec("if isdef('haltscicos'), haltscicos(), end;");
        }
    }

    /**
     * @param graph
     *            the graph to handle
     * @param list
     *            the diagram to check
     * @return diagram name for the "Are your sure ?" dialog
     */
    public String askForClosing(final XcosDiagram graph, final List<SwingScilabDockablePanel> list) {
        final String msg;

        if (wasLastOpened(list)) {
            msg = TRADENAME;
        } else {
            msg = null;
        }

        return msg;
    }

    /**
     * Does Xcos will close or not ?
     *
     * @param list
     *            the list to be closed
     * @return true if all files will be close on tabs close.
     */
    public boolean wasLastOpened(final List<SwingScilabDockablePanel> list) {
        final HashSet<String> opened = new HashSet<String>();
        for (XcosDiagram diag : openedDiagrams()) {
            opened.add(diag.getGraphTab());
        }

        final HashSet<String> tabs = new HashSet<String>();
        for (SwingScilabDockablePanel tab : list) {
            if (tab != null) {
                tabs.add(tab.getPersistentId());
            }
        }

        opened.removeAll(tabs);

        return opened.isEmpty();
    }

    /**
     * @return the external action list
     */
    public List<ExternalAction> getExternalActions() {
        return externalActions;
    }

    /**
     * Close the current xcos session.
     *
     * This method must be called on the EDT thread. For other use, please use
     * the {@link #closeXcosFromScilab()} method.
     */
    public static synchronized void closeSession(final boolean ask) {
        if (!SwingUtilities.isEventDispatchThread()) {
            LOG.severe(CALLED_OUTSIDE_THE_EDT_THREAD);
        }

        /* Doesn't instantiate xcos on close operation */
        if (sharedInstance == null) {
            return;
        }

        /*
         * Try to close all opened files
         */
        final Xcos instance = sharedInstance;

        // get all tabs
        final List<SwingScilabDockablePanel> tabs = new ArrayList<SwingScilabDockablePanel>();
        for (final Collection<XcosDiagram> diags : instance.diagrams.values()) {
            for (final XcosDiagram diag : diags) {
                final SwingScilabDockablePanel tab = XcosTab.get(diag);
                if (tab != null) {
                    tabs.add(tab);
                }
            }
        }

        // ask to close
        final boolean status = ClosingOperationsManager.startClosingOperation(tabs, ask, ask);

        // clear states
        if (status) {
            /* reset the shared instance state */
            instance.diagrams.keySet().clear();
            instance.addDiagram(null, null);

            /* terminate any remaining simulation */
            InterpreterManagement.putCommandInScilabQueue("if isdef('haltscicos'), haltscicos(), end;");

            /* Saving modified data */
            instance.palette.saveConfig();
            instance.configuration.saveConfig();
        }
    }

    /*
     * Scilab exported methods.
     *
     * All the following methods must use SwingUtilities method to assert that
     * the operations will be called on the EDT thread.
     *
     * @see modules/xcos/src/jni/Xcos.giws.xml
     *
     * @see sci_gateway/xcos_gateway.xml
     *
     * @see modules/xcos/sci_gateway/cpp/sci_*.cpp
     */

    /**
     * Main entry point
     *
     * This method invoke Xcos operation on the EDT thread.
     *
     * @param file
     *            The filename (can be null)
     * @param variable
     *            The Scilab variable to load (can be null)
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcos(final String file, final String variable) {
        final Xcos instance = getInstance();
        instance.lastError = null;

        /* load scicos libraries (macros) */
        InterpreterManagement.requestScilabExec(LOAD_XCOS_LIBS_LOAD_SCICOS);

        synchronized (instance) {
            /*
             * Open the file
             */
            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    // open on EDT
                    instance.open(file, variable);
                }
            });

            /*
             * Wait loading and fail on error only if the variable is readeable
             */
            try {
                while (variable != null && instance.lastError == null) {
                    instance.wait();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        if (instance.lastError != null && !instance.lastError.isEmpty()) {
            throw new RuntimeException(instance.lastError);
        }
    }

    /**
     * Close the current xcos session from any thread.
     *
     * This method invoke Xcos operation on the EDT thread. Please prefer using
     * {@link #closeSession()} when the caller is on the EDT thread.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void closeXcosFromScilab() {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    closeSession(false);
                    clearInstance();
                }
            });
        } catch (final InterruptedException e) {
            e.printStackTrace();
        } catch (final InvocationTargetException e) {
            e.printStackTrace();

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
     * Look in each diagram to find the block corresponding to the given uid and
     * display a warning message.
     *
     * This method invoke Xcos operation on the EDT thread.
     *
     * @param uid
     *            A String as UID.
     * @param message
     *            The message to display.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void warnCellByUID(final String[] uid, final String message) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    getInstance().warnCell(uid, message);
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

    private void warnCell(final String[] uid, final String message) {
        final mxCell cell = (mxCell) lookupForCell(uid);

        // We are unable to find the block with the right id
        if (cell == null) {
            return;
        }

        // finally perform the action on the last block
        final XcosDiagram parent = findParent(cell);
        parent.warnCellByUID(cell.getId(), message);

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                /*
                 * Focus on an existing diagram
                 */
                XcosTab.get(parent).setCurrent();
            }
        });
    }

    public Object lookupForCell(final String[] uid) {
        final ArrayDeque<String> deque = new ArrayDeque<String>(Arrays.asList(uid));

        // specific case with an empty array
        if (deque.isEmpty()) {
            return null;
        }

        // first element
        Object cell = null;
        Collection<XcosDiagram> diags = null;
        String id = deque.pop();
        try {
            onDiagramIteration = true;

            for (Collection<XcosDiagram> ds : diagrams.values()) {
                if (ds.isEmpty()) {
                    continue;
                }

                final XcosDiagram root = ds.iterator().next();

                cell = ((mxGraphModel) root.getModel()).getCell(id);
                if (cell != null) {
                    diags = ds;
                    break;
                }
            }
        } finally {
            onDiagramIteration = false;
        }

        // loop to get only the last diagram
        while (cell instanceof SuperBlock && !deque.isEmpty()) {
            final SuperBlock block = (SuperBlock) cell;

            block.getParentDiagram().warnCellByUID(block.getId(), XcosMessages.ERROR_UNABLE_TO_COMPILE_THIS_SUPER_BLOCK);

            id = deque.pop();

            if (!diags.contains(block.getChild()) || !block.getChild().isOpened()) {
                block.openBlockSettings(null);
            }

            final mxGraphModel model = ((mxGraphModel) block.getChild().getModel());
            cell = model.getCell(id);
        }

        return cell;
    }

    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void updateBlock(final String h5File) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    getInstance().updateBlockInstance();
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

    private void updateBlockInstance() {
        // get the cell
        BasicBlock modifiedBlock;

        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        try {
            modifiedBlock = handler.readBlock();
        } catch (ScicosFormatException e) {
            throw new RuntimeException(e);
        } finally {
            handler.release();
        }
        if (modifiedBlock == null) {
            return;
        }

        // diagram lookup
        final XcosDiagram diag = findParent(modifiedBlock);
        if (diag == null) {
            throw new RuntimeException(Messages.gettext("parent diagram not found."));
        }

        // finally update the instance
        final mxGraphModel model = (mxGraphModel) diag.getModel();
        final BasicBlock block = (BasicBlock) model.getCell(modifiedBlock.getId());
        assert block != null;

        block.updateBlockSettings(modifiedBlock);
        block.setInterfaceFunctionName(modifiedBlock.getInterfaceFunctionName());
        block.setSimulationFunctionName(modifiedBlock.getSimulationFunctionName());
        block.setSimulationFunctionType(modifiedBlock.getSimulationFunctionType());
        if (block instanceof SuperBlock) {
            ((SuperBlock) block).setChild(null);
        }

        block.setStyle(block.getStyle() + ";blockWithLabel");
        block.setValue(block.getSimulationFunctionName());
        BlockPositioning.updateBlockView(block);
    }

    /**
     * This function convert a Xcos diagram to Scilab variable.
     *
     * This method invoke Xcos operation on the EDT thread.
     *
     * @param xcosFile
     *            The xcos diagram file
     * @return Not used (compatibility)
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static int xcosDiagramToScilab(final String xcosFile) {
        final File file = new File(xcosFile);

        if (!file.exists()) {
            return 1;
        }

        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    LOG.finest("xcosDiagramToScilab: entering");
                    final XcosDiagram diagram = new XcosDiagram();

                    final XcosFileType filetype = XcosFileType.findFileType(file);
                    if (filetype != null) {
                        try {
                            LOG.finest("xcosDiagramToScilab: initialized");
                            filetype.load(xcosFile, diagram);
                            LOG.finest("xcosDiagramToScilab: loaded");

                            final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
                            if (handler == null) {
                                return;
                            }

                            try {
                                handler.writeDiagram(diagram);
                            } finally {
                                handler.release();
                            }
                        } catch (Exception e) {
                            throw new RuntimeException(e);
                        }
                    }
                    LOG.finest("xcosDiagramToScilab: exiting");
                }
            });
        } catch (final InterruptedException e) {
            throw new RuntimeException(e);
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }

        return 0;
    }

    /**
     * Add a menu into xcos
     *
     * @param label
     *            the label to use
     * @param command
     *            the callback (as a Scilab executable String)
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void addToolsMenu(final String label, final String command) {
        final ExternalAction action = new ExternalAction(null, command);
        action.putValue(Action.NAME, label);
        final Xcos instance = Xcos.getInstance();

        /*
         * Store for future tabs
         */
        instance.externalActions.add(action);

        /*
         * Update opened tabs
         */
        for (final XcosDiagram d : instance.openedDiagrams()) {
            final String uuid = d.getGraphTab();
            final SwingScilabDockablePanel tab = ScilabTabFactory.getInstance().getFromCache(uuid);

            if (tab != null) {
                final SwingScilabMenuBar bar = ((SwingScilabMenuBar) tab.getMenuBar().getAsSimpleMenuBar());

                final Component[] comps = bar.getComponents();
                for (Component component : comps) {
                    if (component instanceof SwingScilabMenu) {
                        final SwingScilabMenu menu = (SwingScilabMenu) component;

                        if (menu.getText() == XcosMessages.TOOLS) {
                            menu.add(new ExternalAction(action, d));
                        }
                    }
                }

                // Also update the parent window toolbar
                BarUpdater.updateBars(tab.getParentWindowId(), tab.getMenuBar(), tab.getToolBar(), tab.getInfoBar(), tab.getName(), tab.getWindowIcon());
            }
        }
    }

    /**
     * Open a diagram by uid.
     *
     * This method invoke Xcos operation on the EDT thread.
     *
     * @param uid
     *            UID path to a block.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    @Deprecated
    public static void xcosDiagramOpen(final String[] uid) {
        throw new UnsupportedOperationException();
    }

    /**
     * Close a diagram by uid.
     *
     * This method invoke Xcos operation on the EDT thread.
     *
     * @param uid
     *            The diagram id path
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    @Deprecated
    public static void xcosDiagramClose(final String[] uid) {
        throw new UnsupportedOperationException();
    }

    /**
     * Inform Xcos the simulator has just started
     *
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcosSimulationStarted() {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                GraphActionManager.setEnable(StopAction.class, true);
            }
        });
    }

    /**
     * Look for the parent diagram of the cell in the diagram hierarchy.
     *
     * @param cell
     *            the cell to search for
     * @return the associated diagram
     */
    public static XcosDiagram findParent(Object cell) {
        final Xcos instance = getInstance();
        try {
            instance.onDiagramIteration = true;

            for (Collection<XcosDiagram> diags : instance.diagrams.values()) {
                for (XcosDiagram diag : diags) {
                    final mxGraphModel model = (mxGraphModel) diag.getModel();

                    // use the O(1) lookup
                    if (cell instanceof mxICell && model.getCell(((mxICell) cell).getId()) != null) {
                        if (cell instanceof BasicBlock) {
                            ((BasicBlock) cell).setParentDiagram(diag);
                        }
                        return diag;
                    }
                }
            }
        } finally {
            instance.onDiagramIteration = false;
        }

        return null;
    }

    /*
     * @see org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory
     */
    public static class XcosTabFactory extends AbstractScilabTabFactory {

        /*
         * Cache
         */
        private DocumentType cachedDocumentType;

        /**
         * Default constructor
         */
        public XcosTabFactory() {
            this(true);
        }

        private XcosTabFactory(boolean instanciateXcos) {
            if (instanciateXcos) {
                getInstance(this);
            }
        }

        /**
         * Create/restore a tab for a given uuid
         *
         * @param uuid
         *            the specific uuid
         * @return the tab instance
         */
        @Override
        public synchronized SwingScilabDockablePanel getTab(final String uuid) {
            if (uuid == null) {
                return null;
            }

            SwingScilabDockablePanel tab = ScilabTabFactory.getInstance().getFromCache(uuid);

            // Palette manager restore
            if (tab == null) {
                if (PaletteManagerView.DEFAULT_TAB_UUID.equals(uuid)) {
                    PaletteManagerView.restore(null, false);
                    tab = PaletteManagerView.get();
                }
            }

            // diagram (tab or viewport) restore
            if (tab == null) {
                cache(uuid);
                if (cachedDocumentType == null) {
                    return null;
                }

                final boolean isTab = uuid.equals(cachedDocumentType.getUuid());
                final boolean isViewport = uuid.equals(cachedDocumentType.getViewport());

                final XcosDiagram graph = getDiagram(isTab, isViewport);
                if (graph != null && isTab) {
                    XcosTab.restore(graph, false);
                    graph.fireEvent(new mxEventObject(mxEvent.ROOT));
                    tab = XcosTab.get(graph);
                } else if (graph != null && isViewport) {
                    ViewPortTab.restore(graph, false);
                    tab = ViewPortTab.get(graph);

                    ClosingOperationsManager.addDependency(XcosTab.get(graph), tab);
                    WindowsConfigurationManager.makeDependency(graph.getGraphTab(), tab.getPersistentId());
                } else {
                    return null;
                }
            }

            WindowsConfigurationManager.restorationFinished(tab);
            ScilabTabFactory.getInstance().addToCache(tab);

            return tab;
        }

        private XcosDiagram getDiagram(boolean isTab, boolean isViewport) {
            XcosDiagram graph = null;
            if (isTab) {
                // load a new diagram
                graph = getInstance().configuration.loadDiagram(cachedDocumentType);
            } else if (isViewport) {
                // get the cached diagram
                final File f = getInstance().configuration.getFile(cachedDocumentType);
                final Collection<XcosDiagram> diags = getInstance().diagrams.get(f);

                for (XcosDiagram d : diags) {
                    final String id = d.getGraphTab();
                    if (id != null && id.equals(cachedDocumentType.getUuid())) {
                        graph = d;
                        break;
                    }
                }
            }

            return graph;
        }

        @Override
        public synchronized boolean isAValidUUID(String uuid) {
            // check the Palette manager view (static uuid)
            if (PaletteManagerView.DEFAULT_TAB_UUID.equals(uuid)) {
                return true;
            }

            /*
             * Cache and check against cache to ease next getTab(uuid) call
             */
            cache(uuid);
            return cachedDocumentType != null;
        }

        /**
         * Cache the {@link DocumentType} for the specific uuid
         *
         * @param uuid
         *            the uuid
         */
        private void cache(String uuid) {
            /*
             * Handle a non null cache
             */
            if (cachedDocumentType != null) {
                final boolean isTab = uuid.equals(cachedDocumentType.getUuid());
                final boolean isViewport = uuid.equals(cachedDocumentType.getViewport());

                if (isTab || isViewport) {
                    return;
                } else {
                    cachedDocumentType = null;
                }
            }

            /*
             * Invalid cache, look for the right one
             */
            final ConfigurationManager config = getInstance().configuration;
            final List<DocumentType> docs = config.getSettings().getTab();
            for (DocumentType d : docs) {
                final boolean isTab = uuid.equals(d.getUuid());
                final boolean isViewport = uuid.equals(d.getViewport());

                if (isTab || isViewport) {
                    cachedDocumentType = d;
                    break;
                }
            }
        }

        @Override
        public String getPackage() {
            return TRADENAME;
        }

        @Override
        public String getClassName() {
            return XcosTabFactory.class.getName();
        }

        @Override
        public String getApplication() {
            return TRADENAME;
        }
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
