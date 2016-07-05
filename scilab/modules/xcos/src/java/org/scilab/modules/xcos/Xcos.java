/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.xcos;

import java.awt.Component;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
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
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.graph.DiagramComparator;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.preferences.XcosConfiguration;
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
    public static final String VERSION = "2.0";
    /**
     * The current Xcos tradename
     */
    public static final String TRADENAME = Xcos.class.getSimpleName();
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
    private static final Logger LOG = Logger.getLogger(Xcos.class.getName());

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
    private final Map<Long, List<XcosDiagram>> diagrams;
    private XcosView view;
    private BrowserView browser;
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
         * Read the configuration to support dynamic (before Xcos launch) settings.
         */
        try {
            LogManager.getLogManager().readConfiguration();
        } catch (final SecurityException | IOException e) {
            LOG.severe(e.toString());
        }

        /* Check the dependencies at startup time */
        checkDependencies();

        /*
         * Allocate data
         */
        diagrams = new HashMap<>();

        view = new XcosView();
        JavaController.register_view(Xcos.class.getName(), view);
        // allocate and install the browser view on demand to avoid any cost
        browser = null;

        /*
         * get the handlers instance
         */
        palette = PaletteManager.getInstance();
        configuration = ConfigurationManager.getInstance();
        styleSheet = new mxStylesheet();
        externalActions = new ArrayList<>();

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

    @Override
    protected void finalize() throws Throwable {
        if (browser != null) {
            JavaController.unregister_view(browser);
        }

        JavaController.unregister_view(view);

        super.finalize();
    }

    /**
     * Check the dependencies and the version dependencies.
     *
     * This method use runtime class loading to handle ClassNotFoundException.
     *
     * This method catch any exception and rethrow it with a well defined message. Thus it doesn't pass the IllegalCatch metrics.
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
            try {
                if (!SwingUtilities.isEventDispatchThread()) {
                    SwingUtilities.invokeAndWait(() -> {
                        sharedInstance = new Xcos(factory);
                    });
                } else {
                    sharedInstance = new Xcos(factory);
                }
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

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

        // TODO : perform something ?
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
        for (Long l : diagrams.keySet()) {
            opened.addAll(openedDiagrams(l));
        }

        return opened;
    }

    /**
     * Opened diagrams
     *
     * @param l
     *            the root diagram uid
     * @return the opened diagrams list
     */
    public List<XcosDiagram> openedDiagrams(Long l) {
        final List<XcosDiagram> opened = new ArrayList<>();
        for (XcosDiagram d : diagrams.get(l)) {
            if (d.isOpened()) {
                opened.add(d);
            }
        }

        return opened;
    }

    public Long openedDiagramUID(File f) {
        Long opened = 0l;
        if (f == null) {
            return opened;
        }

        for (Long diagUID : diagrams.keySet()) {
            List<XcosDiagram> diags = diagrams.getOrDefault(diagUID, Collections.emptyList());

            if (!diags.isEmpty() && f.equals(diags.get(0).getSavedFile())) {
                opened = diagUID;
                break;
            }
        }

        return opened;
    }

    /**
     * Check if the in memory file representation is modified
     *
     * @param l
     *            the root diagram UID
     * @return is modified
     */
    public boolean isModified(Long l) {
        for (XcosDiagram d : diagrams.get(l)) {
            if (d.isModified()) {
                return true;
            }
        }

        return false;
    }

    /**
     * Popup a dialog to ask for a file creation
     *
     * @param f
     *            the file to create
     * @return true if creation is has been performed
     */
    public boolean askForFileCreation(final XcosDiagram diag, final File f) {
        AnswerOption answer;
        try {
            answer = ScilabModalDialog.show(diag.getAsComponent(), new String[] { String.format(XcosMessages.FILE_DOESNT_EXIST, f.getCanonicalFile()) },
                                            XcosMessages.XCOS, IconType.QUESTION_ICON, ButtonType.YES_NO);
        } catch (final IOException e) {
            LOG.severe(e.toString());
            answer = AnswerOption.YES_OPTION;
        }

        if (answer == AnswerOption.YES_OPTION) {
            return diag.saveDiagramAs(f);
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
     * This method must be called on the EDT thread. For other use, please use the {@link #xcos(String, String)} method.
     *
     * @param file
     *            the file to open. If null an empty diagram is created.
     * @param diagramId
     *            the MVC ID to track. If 0 no association is performed.
     */
    public void open(final String file, final long diagramId) {
        if (!SwingUtilities.isEventDispatchThread()) {
            LOG.severe(CALLED_OUTSIDE_THE_EDT_THREAD);
        }

        /*
         * If it is the first window opened, then open the palette first.
         */
        if (openedDiagrams().isEmpty()) {
            PaletteManager.setVisible(true);
        }

        JavaController controller = new JavaController();
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
        final Long rootUID = openedDiagramUID(f);
        if (rootUID != 0l) {
            diag = diagrams.get(rootUID).iterator().next();
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

        final long currentId;
        if (diagramId != 0) {
            currentId = diagramId;
        } else {
            currentId = controller.createObject(Kind.DIAGRAM);
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
            diag = new XcosDiagram(controller, currentId, Kind.DIAGRAM, "");
            diag.installListeners();

            /*
             * Ask for file creation
             */
            if (f != null && !f.exists()) {
                if (!askForFileCreation(diag, f)) {
                    // loading disabled, unlock
                    synchronized (this) {
                        setLastError("");
                        notify();
                    }

                    // return now, to avoid tab creation
                    controller.deleteObject(diag.getUID());
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
            diag.transformAndLoadFile(controller, file);

            addDiagram(diag.getUID(), diag);
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
     * @return the Xcos view
     */
    public XcosView getXcosView() {
        return view;
    }

    /**
     * @return the Browser view
     */
    public BrowserView getBrowser() {
        if (browser == null) {
            browser = new BrowserView();
            JavaController.register_view(BrowserView.class.getSimpleName(), browser);
        }
        return browser;
    }

    /**
     * Clear the browser state and unregister the current view.
     */
    public void clearBrowser() {
        if (browser != null) {
            JavaController.unregister_view(browser);
            browser = null;
        }
    }

    /**
     * Get an unmodifiable view of the diagrams for an UID
     *
     * @param l
     *            the root diagram UID
     * @return the diagram collection
     */
    public Collection<XcosDiagram> getDiagrams(final long l) {
        final Collection<XcosDiagram> diags = diagrams.get(l);
        if (diags == null) {
            return null;
        }
        return Collections.unmodifiableCollection(diags);
    }

    /**
     * Add a diagram to the diagram list for a file. Be sure to set the right opened status on the diagram before calling this method.
     *
     * @param l
     *            the root diagram UID
     * @param diag
     *            the diag
     */
    public void addDiagram(final long l, final XcosDiagram diag) {
        if (onDiagramIteration) {
            throw new RuntimeException();
        }
        if (l == 0l) {
            throw new IllegalArgumentException();
        }

        /*
         * Create the collection if it does not exist
         */
        List<XcosDiagram> diags = diagrams.get(l);
        if (diags == null) {
            diags = createDiagramCollection();
            diagrams.put(l, diags);
        }

        // insert the diagram
        diags.add(diag);
    }

    /**
     * Add a diagram to the opened  list
     *
     * This method manage both super-block and root diagrams.
     * @param diag the diagram to add
     */
    public void addDiagram(final XcosDiagram diag) {
        if (diag.getKind() == Kind.DIAGRAM) {
            addDiagram(diag.getUID(), diag);
        } else {
            long[] root = new long[1];
            new JavaController().getObjectProperty(diag.getUID(), diag.getKind(), ObjectProperties.PARENT_DIAGRAM, root);

            addDiagram(root[0], diag);
        }

    }



    /**
     * Create a diagram collections (sorted List)
     *
     * @return the diagram collection
     */
    @SuppressWarnings("serial")
    public List<XcosDiagram> createDiagramCollection() {
        return new ArrayList<XcosDiagram>() {
            @Override
            public boolean add(XcosDiagram element) {
                final boolean status = super.add(element);
                DiagramComparator.sort(this);
                return status;
            }

            @Override
            public boolean addAll(Collection<? extends XcosDiagram> c) {
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

        JavaController controller = new JavaController();
        long[] rootDiagram = new long[1];
        controller.getObjectProperty(graph.getUID(), graph.getKind(), ObjectProperties.PARENT_DIAGRAM, rootDiagram);
        if (rootDiagram[0] == 0l) {
            rootDiagram[0] = graph.getUID();
        }

        final boolean wasLastOpened = openedDiagrams(rootDiagram[0]).size() <= 1;
        final boolean isModified = isModified(rootDiagram[0]);
        if (!(wasLastOpened && isModified)) {
            canClose = true;
        }

        if (!canClose) {
            final AnswerOption ans = ScilabModalDialog.show(XcosTab.get(graph), XcosMessages.DIAGRAM_MODIFIED, XcosMessages.XCOS, IconType.QUESTION_ICON,
                                     ButtonType.YES_NO_CANCEL);

            switch (ans) {
                case YES_OPTION:
                    canClose = diagrams.get(rootDiagram[0]).iterator().next().saveDiagram();
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
        JavaController controller = new JavaController();
        long[] rootDiagram = new long[1];
        controller.getObjectProperty(graph.getUID(), graph.getKind(), ObjectProperties.PARENT_DIAGRAM, rootDiagram);
        if (rootDiagram[0] == 0l) {
            rootDiagram[0] = graph.getUID();
        }

        final boolean wasLastOpenedForFile = openedDiagrams(rootDiagram[0]).size() <= 1;
        if (wasLastOpenedForFile) {
            diagrams.remove(rootDiagram[0]);
        } else {
            diagrams.get(rootDiagram[0]).remove(graph);
        }

        if (openedDiagrams().size() <= 0) {
            JavaController.end_simulation();
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
     * This method must be called on the EDT thread. For other use, please use the {@link #closeXcosFromScilab()} method.
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

            /* terminate any remaining simulation */
            JavaController.end_simulation();

            /* Saving modified data */
            instance.palette.saveConfig();
            instance.configuration.saveConfig();
        }
    }

    /*
     * Scilab exported methods.
     *
     * All the following methods must use SwingUtilities method to assert that the operations will be called on the EDT thread.
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
     * @param diagramId
     *            The Xcos DIAGRAM model ID (can be 0)
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcos(final String file, final long diagramId) {
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
                    instance.open(file, diagramId);
                }
            });
        }
        if (instance.lastError != null && !instance.lastError.isEmpty()) {
            throw new RuntimeException(instance.lastError);
        }
    }

    /**
     * Load or Save an xcos diagram without using Scilab at all.
     *
     * <P>
     * This support a reduced number of format and should be mainly used to test
     *
     * @param file
     *            the file
     * @param diagramId
     *            the diagram to load into
     * @param export
     *            flag used to indicate an export (true == export ; false == import)
     * @throws Exception
     *             on loading error
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcosDiagramToScilab(String file, long diagramId, boolean export) throws Exception {
        XcosFileType filetype = XcosFileType.findFileType(file);
        if (filetype == null) {
            throw new IllegalArgumentException("not handled filetype");
        }

        if (XcosFileType.getAvailableSaveFormats().contains(filetype)) {
            if (export) {
                filetype.save(file, new XcosDiagram(new JavaController(), diagramId, Kind.DIAGRAM, ""));
            } else {
                filetype.load(file, new XcosDiagram(new JavaController(), diagramId, Kind.DIAGRAM, ""));
            }
        } else {
            throw new IllegalArgumentException("not handled filetype");
        }
    }

    /**
     * Close the current xcos session from any thread.
     *
     * This method invoke Xcos operation on the EDT thread. Please prefer using {@link #closeSession()} when the caller is on the EDT thread.
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
     * Look in each diagram to find the block corresponding to the given uid and display a warning message.
     *
     * This method invoke Xcos operation on the EDT thread.
     *
     * @param uid
     *            A String as UID.
     * @param message
     *            The message to display.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void warnCellByUID(final String[] uids, final String message) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    getInstance().warnCell(uids, message);
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

    private void warnCell(final String[] uids, final String message) {

        final mxCell[] cells = lookupForCells(uids);
        for (int i = cells.length - 1; i >= 0; --i) {
            mxCell cell = cells[i];

            // perform the action on the last visible block
            if (cell != null) {
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

                return;
            }

        }
    }

    public mxCell[] lookupForCells(final String[] uid) {
        mxCell[] found = new mxCell[uid.length];
        XcosView view = (XcosView) JavaController.lookup_view(Xcos.class.getName());

        final String[] sortedUIDs = Arrays.copyOf(uid, uid.length);
        Arrays.sort(sortedUIDs);

        view.getVisibleObjects().values().stream()
        // look for the visible objects in the UID set
        .filter(o -> o instanceof mxCell).map(o -> (mxCell) o).filter(o -> Arrays.binarySearch(sortedUIDs, o.getId()) >= 0)

        // push the results to the resulting array
        .forEach(o -> found[Arrays.asList(uid).indexOf(o.getId())] = o);

        return found;
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

        for (Collection<XcosDiagram> diags : instance.diagrams.values()) {
            for (XcosDiagram diag : diags) {
                final mxGraphModel model = (mxGraphModel) diag.getModel();

                // use the O(1) lookup
                if (cell instanceof mxICell && model.getCell(((mxICell) cell).getId()) != null) {
                    return diag;
                }
            }
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
            final Xcos instance = getInstance();
            XcosDiagram graph = null;

            if (isTab) {
                // load a new diagram
                graph = getInstance().configuration.loadDiagram(cachedDocumentType);
            } else if (isViewport) {
                // get the cached diagram
                final File f = instance.configuration.getFile(cachedDocumentType);
                final Long rootUID = getInstance().openedDiagramUID(f);

                Collection<XcosDiagram> diags = instance.diagrams.getOrDefault(rootUID, Collections.emptyList());
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
