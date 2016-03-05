/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.gui.utils;

import java.awt.Dimension;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Rectangle;
import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

import javax.swing.SwingUtilities;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.docking.state.LayoutNode;
import org.flexdock.perspective.persist.xml.LayoutNodeSerializer;
import org.flexdock.perspective.persist.xml.PersistenceConstants;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.xml.ScilabXPathFactory;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.localization.Messages;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

/**
 *
 * Save the windows properties.
 *
 * @author Calixte DENIZET
 */
public class WindowsConfigurationManager implements XConfigurationListener {

    private static final String RESTORATION_ERROR = Messages.gettext("Restoration error");
    private static final String ERROR_IN_LOADING = Messages.gettext("The configuration file has been corrupted and reset to the default one.");
    private static final String FATAL_ERROR = Messages.gettext("The configuration file has been corrupted. Scilab needs to restart.");

    private static final int DEFAULTX = 0;
    private static final int DEFAULTY = 0;
    private static final int DEFAULTHEIGHT = 500;
    private static final int DEFAULTWIDTH = 500;

    private static final String LAYOUT_PATH = "//general/desktop-layout/body/layouts";

    private static final String SCI = "SCI";
    private static final String WINDOWS_CONFIG_FILE = System.getenv(SCI) + "/modules/gui/etc/windowsConfiguration.xml";
    private static final String DEFAULT_WINDOWS_CONFIG_FILE = System.getenv(SCI) + "/modules/gui/etc/integratedConfiguration.xml";
    private static final String NULLUUID = new UUID(0L, 0L).toString();
    private static final Map<SwingScilabDockablePanel, EndedRestoration> endedRestoration = new HashMap<SwingScilabDockablePanel, EndedRestoration>();
    private static final List<String> alreadyRestoredWindows = new ArrayList<String>();
    private static final Map<String, Object> defaultWinAttributes = new HashMap<String, Object>();
    private static final List<String> currentlyRestored = new ArrayList<String>();


    private static String USER_WINDOWS_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/windowsConfiguration.xml";

    private static boolean oneTry;
    private static Document doc;

    private static boolean mustInvalidate;
    private static boolean mustSave = true;
    private static int layoutId;

    static {
        try {
            if (ScilabConstants.SCIHOME != null && ScilabConstants.SCIHOME.canRead() && ScilabConstants.SCIHOME.canWrite()) {
                USER_WINDOWS_CONFIG_FILE = ScilabConstants.SCIHOME.toString() + "/windowsConfiguration.xml";
            } else {
                USER_WINDOWS_CONFIG_FILE = DEFAULT_WINDOWS_CONFIG_FILE;
                mustSave = false;
            }

            new WindowsConfigurationManager();
            Runnable runnable = new Runnable() {
                public void run() {
                    if (mustInvalidate) {
                        File f = new File(USER_WINDOWS_CONFIG_FILE);
                        if (f.exists() && f.isFile()) {
                            f.delete();
                        }
                    }
                }
            };

            Class scilab = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.core.Scilab");
            Method registerFinalHook = scilab.getDeclaredMethod("registerFinalHook", Runnable.class);
            registerFinalHook.invoke(null, runnable);

            defaultWinAttributes.put("x", new Integer(DEFAULTX));
            defaultWinAttributes.put("y", new Integer(DEFAULTY));
            defaultWinAttributes.put("height", new Integer(DEFAULTHEIGHT));
            defaultWinAttributes.put("width", new Integer(DEFAULTWIDTH));
            defaultWinAttributes.put("state", new Integer(SwingScilabWindow.NORMAL));
            /*
              Uncomment this code for debugging focus issues

              java.awt.Toolkit.getDefaultToolkit().addAWTEventListener(new java.awt.event.AWTEventListener() {
              public void eventDispatched(java.awt.AWTEvent e) {
              System.out.println(e);
              }
              }, java.awt.AWTEvent.FOCUS_EVENT_MASK);*/
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    private WindowsConfigurationManager() {
        XConfiguration.addXConfigurationListener(this);
    }

    /**
     * Reset the layout
     */
    public static void resetLayout() {
        mustInvalidate = true;
    }

    public void configurationChanged(XConfigurationEvent e) {
        if (e.getModifiedPaths().contains(LAYOUT_PATH)) {
            Options options = getOptions();
            if (options.id != layoutId) {
                mustInvalidate = true;
                layoutId = options.id;
            }

            mustSave = options.mustSave;
        }
    }

    public static String getLayoutFilePath() {
        try {
            Document doc = XConfiguration.getXConfigurationDocument();
            if (doc != null) {
                XPath xp = ScilabXPathFactory.newInstance().newXPath();
                NodeList nodes = (NodeList) xp.compile(LAYOUT_PATH + "/layout[@id=../@id]/@path").evaluate(doc, XPathConstants.NODESET);
                if (nodes != null && nodes.getLength() > 0) {
                    return nodes.item(0).getNodeValue().replace("$SCI", System.getenv(SCI));
                }
            }
        } catch (Exception e) { }

        return WINDOWS_CONFIG_FILE;
    }

    /**
     * Create a copy of windows configuration file in the user directory
     */
    public static void createUserCopy() {
        if (isCopyNeeded() && mustSave) {
            ScilabCommonsUtils.copyFile(new File(getLayoutFilePath()), new File(USER_WINDOWS_CONFIG_FILE));
            doc = null;
        }
    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {
        if (doc == null) {
            createUserCopy();
            doc = ScilabXMLUtilities.readDocument(USER_WINDOWS_CONFIG_FILE);
            Options options = getOptions();
            if (mustSave) {
                mustSave = options.mustSave;
            }
            layoutId = options.id;
        }

        if (doc == null && !oneTry) {
            reload();
        } else if (doc == null && oneTry) {
            System.err.println("Serious problem to copy and parse the configuration file.");
            System.err.println("Please check if you have the rights to write the file: " + USER_WINDOWS_CONFIG_FILE);
            System.err.println("If the previous file exists, please check if it is a valid XML");
            System.err.println("and if yes, please report a bug: http://bugzilla.scilab.org");
        } else if (doc != null) {
            // We check that the file contains a NULLUUID (used for the console) and only one
            // No console == no Scilab !
            Element root = doc.getDocumentElement();
            List<Element> list = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "uuid", NULLUUID);
            if (list == null || list.isEmpty() || list.size() >= 2) {
                reload();
            } else {
                Element console = list.get(0);
                // Check the factory
                if (console == null || !"org.scilab.modules.core.ConsoleTabFactory".equals(console.getAttribute("factory"))) {
                    // the factory is not correct => crash so we reload the conf file
                    reload();
                }
            }
        }
    }

    /**
     * Display an error
     */
    private static void displayLoadError() {
        System.err.println("Try to reload the default configuration file.");
        ScilabModalDialog.show(null, ERROR_IN_LOADING, RESTORATION_ERROR, IconType.ERROR_ICON);
    }

    /**
     * Display a fatal error
     */
    private static void displayFatalError() {
        System.err.println("The configuration file is severely corrupted and cannot be read. It will be deleted.");
        ScilabModalDialog.show(null, FATAL_ERROR, RESTORATION_ERROR, IconType.ERROR_ICON);
    }

    /**
     * Delete configuration file
     */
    private static void deleteConfFile() {
        File f = new File(USER_WINDOWS_CONFIG_FILE);
        if (f.exists() && f.isFile()) {
            f.delete();
        }
    }

    /**
     * Reload the xml configuration file
     */
    private static void reload() {
        doc = null;
        displayLoadError();
        deleteConfFile();
        oneTry = true;
        readDocument();
    }

    /**
     * Kill scilab
     */
    private static void killScilab() {
        deleteConfFile();
        displayFatalError();
        System.exit(1);
    }

    /**
     * Write the document
     */
    private static void writeDocument() {
        if (mustSave) {
            ScilabXMLUtilities.writeDocument(doc, USER_WINDOWS_CONFIG_FILE);
        }
    }

    /**
     * @return true if a copy is needed
     */
    private static final boolean isCopyNeeded() {
        return !new File(USER_WINDOWS_CONFIG_FILE).exists();
    }

    /**
     * Register an EndedRestoration, op.finish() will be executed when the tab restoration will be finished.
     * @param tab the associated tab
     * @param ended the closing operation
     */
    public static void registerEndedRestoration(SwingScilabDockablePanel tab, EndedRestoration ended) {
        endedRestoration.put(tab, ended);
    }

    /**
     * Unregister an EndedRestoration.
     * @param tab the associated tab
     */
    public static void unregisterEndedRestoration(SwingScilabDockablePanel tab) {
        endedRestoration.remove(tab);
    }

    /**
     * Register an EndedRestoration, op.finish() will be executed when the tab restoration will be finished.
     * @param tab the associated tab
     * @param ended the closing operation
     */
    public static void registerEndedRestoration(Tab tab, EndedRestoration ended) {
        registerEndedRestoration((SwingScilabDockablePanel) tab.getAsSimpleTab(), ended);
    }

    /**
     * Create a new node with parent element
     * @param parent the parent element
     * @param nodeName the node name
     * @param attr an array containing attribute name followed by its value: "attr1", 1, "attr2", true, ...
     * @return the created element
     */
    public static Element createNode(Element parent, String nodeName, Object[] attr) {
        readDocument();
        for (int i = 0; i < attr.length; i += 2) {
            if (attr[i].equals("uuid")) {
                removeNode(parent, nodeName, (String) attr[i + 1]);
            }
        }

        return ScilabXMLUtilities.createNode(doc, parent, nodeName, attr);
    }

    /**
     * Save the window properties
     * @param window the window
     */
    public static void saveWindowProperties(SwingScilabWindow window) {
        if (ScilabConstants.isGUI()) {
            readDocument();

            Element root = doc.getDocumentElement();
            Element win = createNode(root, "Window", new Object[] {"uuid", window.getUUID(),
                                     "x", (int) window.getLastPosition().getX(),
                                     "y", (int) window.getLastPosition().getY(),
                                     "width", (int) window.getLastDimension().getWidth(),
                                     "height", (int) window.getLastDimension().getHeight(),
                                     "state", window.getExtendedState()
                                                                  });
            LayoutNode layoutNode = window.getDockingPort().exportLayout();
            LayoutNodeSerializer serializer = new LayoutNodeSerializer();
            win.appendChild(serializer.serialize(doc, layoutNode));

            for (Dockable dockable : (Set<Dockable>) window.getDockingPort().getDockables()) {
                saveTabProperties((SwingScilabDockablePanel) dockable, false);
            }

            writeDocument();
        }
    }

    /**
     * Create a window according to the uuid.
     *
     * This method can be used to create a reference windows.
     *
     * @param uuid
     *            the reference uuid
     * @param preserveUUID
     *            if true the uuid will be used on the new windows, generate a
     *            new uuid otherwise
     * @return the window
     */
    public static SwingScilabWindow createWindow(final String uuid, final boolean preserveUUID) {
        readDocument();

        final Element root = doc.getDocumentElement();
        final boolean nullUUID = uuid.equals(NULLUUID);
        final Map<String, Object> attrs = new HashMap<String, Object>();
        Element win = null;
        boolean containsX = true;

        if (!nullUUID) {
            win = getElementWithUUID(root, "Window", uuid);
            if (win == null) {
                return null;
            }

            containsX = !win.getAttribute("x").equals("");

            if (containsX) {
                attrs.put("x", int.class);
                attrs.put("y", int.class);
            }

            attrs.put("height", int.class);
            attrs.put("width", int.class);
            attrs.put("state", int.class);
            ScilabXMLUtilities.readNodeAttributes(win, attrs);
        } else {
            attrs.putAll(defaultWinAttributes);
        }

        SwingScilabWindow window = SwingScilabWindow.createWindow(true);
        window.setVisible(false);

        final String localUUID;
        if (preserveUUID) {
            localUUID = uuid;
        } else {
            localUUID = UUID.randomUUID().toString();
        }
        window.setUUID(localUUID);

        if (containsX) {
            boolean positionned = false;
            Point p = new Point(((Integer) attrs.get("x")).intValue(), ((Integer) attrs.get("y")).intValue());

            // We check that the coordinates are valid
            GraphicsDevice[] gds = GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices();
            if (gds != null) {
                for (GraphicsDevice gd : gds) {
                    Rectangle r = gd.getDefaultConfiguration().getBounds();
                    if (r.contains(p)) {
                        positionned = true;
                        window.setLocation(p.x, p.y);
                        break;
                    }
                }
            }

            if (!positionned) {
                window.setLocation(DEFAULTX, DEFAULTY);
            }
        }

        window.setSize(((Integer) attrs.get("width")).intValue(), ((Integer) attrs.get("height")).intValue());
        window.setExtendedState(((Integer) attrs.get("state")).intValue());

        return window;
    }

    public static SwingScilabWindow restoreWindow(String uuid) {
        String winuuid = UUID.randomUUID().toString();
        SwingScilabWindow win = SwingScilabWindow.createWindow(true);
        win.setUUID(winuuid);
        win.setIsRestoring(true);

        final SwingScilabDockablePanel tab = ScilabTabFactory.getInstance().getTab(uuid);
        win.addTab(tab);
        BarUpdater.forceUpdateBars(tab.getParentWindowId(), tab.getMenuBar(), tab.getToolBar(), tab.getInfoBar(), tab.getName(), tab.getWindowIcon());

        win.pack();
        win.setVisible(true);
        win.requestFocus();
        win.toFront();
        win.setIsRestoring(false);

        endedRestoration.remove(tab);

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                ActiveDockableTracker.requestDockableActivation(tab);
            }
        });

        return win;
    }

    /**
     * Restore a window with a given uuid
     *
     * @param uuid
     *            the uuid
     * @param restoreTab
     *            if true the tab is restored too
     * @return the corresponding window
     */
    public static SwingScilabWindow restoreWindow(String uuid, String defaultTabUuid, boolean restoreTab, boolean requestFocus) {
        readDocument();
        final boolean nullUUID = uuid.equals(NULLUUID);

        // create the window and preserve the uuid if not null
        final SwingScilabWindow window = createWindow(uuid, !nullUUID);
        if (window == null) {
            return null;
        }

        window.setIsRestoring(true);

        if (restoreTab) {
            if (!nullUUID) {
                final LayoutNodeSerializer serializer = new LayoutNodeSerializer();
                final Element dockingPort = getDockingPort(uuid);
                LayoutNode layoutNode = (LayoutNode) serializer.deserialize(dockingPort);
                window.getDockingPort().importLayout(layoutNode);
            } else if (defaultTabUuid != null && !defaultTabUuid.isEmpty()) {
                SwingScilabDockablePanel defaultTab = ScilabTabFactory.getInstance().getTab(defaultTabUuid);
                defaultTab.setParentWindowId(window.getId());
                DockingManager.dock(defaultTab, window.getDockingPort());
            }

            for (SwingScilabDockablePanel tab : (Set<SwingScilabDockablePanel>) window.getDockingPort().getDockables()) {
                tab.setParentWindowId(window.getId());
            }

            SwingScilabDockablePanel[] tabs = new SwingScilabDockablePanel[window.getNbDockedObjects()];
            tabs = ((Set<SwingScilabDockablePanel>) window.getDockingPort().getDockables()).toArray(tabs);

            // Be sur that the main tab will have the focus.
            // Get the elder tab and activate it
            final SwingScilabDockablePanel mainTab = ClosingOperationsManager.getElderTab(new ArrayList(Arrays.asList(tabs)));
            BarUpdater.forceUpdateBars(mainTab.getParentWindowId(), mainTab.getMenuBar(), mainTab.getToolBar(), mainTab.getInfoBar(), mainTab.getName(), mainTab.getWindowIcon());

            if (!ScilabConsole.isExistingConsole() && tabs.length == 1 && tabs[0].getPersistentId().equals(NULLUUID)) {
                // null uuid is reserved to the console and in NW mode, there is no console.
                return null;
            }

            for (SwingScilabDockablePanel tab : tabs) {
                // each tab has now a window so it can be useful for the tab to set an icon window or to center a dialog...
                EndedRestoration ended = endedRestoration.get(tab);
                if (ended != null) {
                    ended.finish();
                    endedRestoration.remove(tab);
                }
            }

            if (tabs.length == 1) {
                // we remove undock and close buttons when there is only one View in the DockingPort
                SwingScilabDockablePanel.removeActions(tabs[0]);
            } else {
                // we add undock and close buttons
                for (SwingScilabDockablePanel tab : tabs) {
                    SwingScilabDockablePanel.addActions(tab);
                }
            }

            if (requestFocus) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        final Thread t = new Thread(new Runnable() {
                            @Override
                            public void run() {
                                synchronized (currentlyRestored) {
                                    while (currentlyRestored.size() > 0) {
                                        try {
                                            currentlyRestored.wait();
                                        } catch (InterruptedException e) {
                                            e.printStackTrace();
                                        }
                                    }
                                }

                                SwingUtilities.invokeLater(new Runnable() {
                                    @Override
                                    public void run() {
                                        window.setVisible(true);
                                        window.requestFocus();
                                        window.toFront();
                                        window.setIsRestoring(false);

                                        ActiveDockableTracker.requestDockableActivation(mainTab);
                                    }
                                });
                            }
                        });
                        t.start();
                    }
                });
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        window.setIsRestoring(false);
                        window.setVisible(true);
                    }
                });
            }
        }

        return window;
    }

    private static final Element getDockingPort(final String winUUID) {
        readDocument();

        final Element root = doc.getDocumentElement();
        final Element win = getElementWithUUID(root, "Window", winUUID);
        if (win == null) {
            return null;
        }

        final NodeList children = win.getElementsByTagName(PersistenceConstants.DOCKING_PORT_NODE_ELEMENT_NAME);
        return (Element) children.item(0);
    }


    /**
     * Must be called when the restoration is finished
     * @param tab the tab
     */
    public static final void restorationFinished(SwingScilabDockablePanel tab) {
        synchronized (currentlyRestored) {
            currentlyRestored.remove(tab.getPersistentId());

            // notify after remove
            currentlyRestored.notify();
        }
    }

    /**
     * Remove a window from the already restored windows
     * @param uuid the win uuid
     */
    public static final void removeWin(String uuid) {
        alreadyRestoredWindows.remove(uuid);
    }

    /**
     * Find all the dependencies of the given tab. The returned list contains parent before its children.
     * @param uuid the tab's uuid
     * @return a list of the elements with the given uuid
     */
    public static final Set<Element> getTabDependencies(String uuid) {
        readDocument();
        Element root = doc.getDocumentElement();

        // Children
        List<Element> elements = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "depends", uuid);

        Set<Element> list = new LinkedHashSet<Element>();
        Element el = getElementWithUUID(doc.getDocumentElement(), uuid);
        if (el != null) {
            //We add the parent
            list.add(el);
            // We add the children and their own children
            for (Element e : elements) {
                list.addAll(getTabDependencies(e.getAttribute("uuid")));
            }
        }

        return list;
    }

    /**
     * Create all the tabs depending of the tab with the given uuid.
     * The creation will respect the convention: parent before children.
     * @param uuid the tab uuid
     * @return the list of all the uuids to restore
     */
    public static final Set<Element> createDescendantTabs(String uuid) {
        Set<Element> list = getTabDependencies(uuid);
        Dimension nullDims = new Dimension(0, 0);
        for (Element e : list) {
            // All the tabs created in the factory will be cached so when Flexdock will restore the docking
            // it will use the same tab as created here.
            ScilabTabFactory factory = ScilabTabFactory.getInstance();
            factory.addTabFactory(e.getAttribute("load"), e.getAttribute("factory"));
            currentlyRestored.add(e.getAttribute("uuid"));
            SwingScilabDockablePanel tab = factory.getTab(e.getAttribute("uuid"));
            if (!e.getAttribute("width").isEmpty() && !e.getAttribute("height").isEmpty()) {
                tab.setMinimumSize(nullDims);
                tab.setPreferredSize(new Dimension(Integer.parseInt(e.getAttribute("width")), Integer.parseInt(e.getAttribute("width"))));
            }
        }

        return list;
    }

    /**
     * Useful for the following case: you have 3 tabs, A, B and C, C depends of B, A and C are docked in the same window.
     * You want to restore A, the window containing A needs to restore C too (and finally B).
     * There is no direct dependency between A and C but the fact that they're in the same dockport
     * implies a dependency.
     * @param elems the elems corresponding to the tabs to restore
     * @return tabs to restore
     */
    private static final Set<Element> createAdjacentTabs(Set<Element> elems) {
        readDocument();

        Element root = doc.getDocumentElement();
        boolean jobFinished = true;
        Set<Element> toAdd = new LinkedHashSet<Element>();
        for (Element e : elems) {
            String winuuid = e.getAttribute("winuuid");
            if (!winuuid.isEmpty() && !winuuid.equals(NULLUUID)) {
                List<Element> elements = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "winuuid", winuuid);
                elements.removeAll(elems);
                jobFinished = jobFinished && elements.size() == 0;
                toAdd.addAll(elements);
            }
        }

        if (jobFinished) {
            return elems;
        }

        for (Element ee : toAdd) {
            elems.addAll(createDescendantTabs(getElderParent(ee).getAttribute("uuid")));
        }


        return createAdjacentTabs(elems);
    }

    /**
     * @param e the element
     * @return the elder parent of this element (elder for the attribute "depends")
     */
    private static final Element getElderParent(Element e) {
        readDocument();

        Element root = doc.getDocumentElement();
        String dep = e.getAttribute("depends");
        if (!dep.isEmpty()) {
            return getElderParent(ScilabXMLUtilities.getElementsWithAttributeEquals(root, "uuid", dep).get(0));
        }

        return e;
    }

    /**
     * Starts the restoration of the tab with the given uuid
     * @param uuid the tab uuid to restore
     */
    private static final void startRestoration(String uuid) {
        if (!ScilabConstants.isGUI()) {
            readDocument();
            Element root = doc.getDocumentElement();
            Element e = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "uuid", uuid).get(0);
            if (e != null) {
                String winuuid = e.getAttribute("winuuid");
                if (!winuuid.isEmpty() && !winuuid.equals(NULLUUID)) {
                    List<Element> elements = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "winuuid", winuuid);
                    for (Element ee : elements) {
                        if (ee.getNodeName().equals("Console")) {
                            restoreWindow(uuid);
                            return;
                        }
                    }
                }
            }
        }

        Set<Element> list = createDescendantTabs(uuid);
        list = createAdjacentTabs(list);

        List<String> wins = new ArrayList<String>();
        List<String> tabsWithoutWin = new ArrayList<String>();
        for (Element e : list) {
            String winuuid = e.getAttribute("winuuid");
            if (winuuid.equals(NULLUUID) || getElementWithUUID(winuuid) == null || !isDockableIdExisting(winuuid, e.getAttribute("uuid"))) {
                tabsWithoutWin.add(e.getAttribute("uuid"));
            } else if (!wins.contains(winuuid)) {
                wins.add(winuuid);
            }
        }

        boolean requestFocus = true;

        if (wins.contains(NULLUUID)) {
            wins.remove(NULLUUID);
            wins.add(NULLUUID);
        }

        for (String winuuid : wins) {
            if (!alreadyRestoredWindows.contains(winuuid)) {
                restoreWindow(winuuid, uuid, true, requestFocus);
                alreadyRestoredWindows.add(winuuid);
                if (requestFocus) {
                    requestFocus = false;
                }
            }
        }

        for (String u : tabsWithoutWin) {
            SwingScilabWindow window = restoreWindow(NULLUUID, u, true, requestFocus);
            alreadyRestoredWindows.add(window.getUUID());
            if (requestFocus) {
                requestFocus = false;
            }
        }
    }

    /**
     * Search a node (child of root) with name nodeName and with a given uuid
     * @param root the root element
     * @param nodeName the node name
     * @param uuid the uuid
     * @return the corresponding element or null if it does not exist
     */
    public static final Element getElementWithUUID(Element root, String nodeName, String uuid) {
        if (uuid == null || uuid.isEmpty()) {
            return null;
        }
        NodeList list = root.getElementsByTagName(nodeName);
        int length = getNodeListLength(list);
        for (int i = 0; i < length; i++) {
            Element elem = (Element) list.item(i);
            if (elem.getAttribute("uuid").equals(uuid)) {
                return elem;
            }
        }

        return null;
    }

    /**
     * Search a node with a given uuid
     * @param uuid the uuid
     * @return the corresponding element or null if it does not exist
     */
    public static final Element getElementWithUUID(String uuid) {
        readDocument();

        return getElementWithUUID(doc.getDocumentElement(), uuid);
    }

    /**
     * Search a node (child of root) with name nodeName and with a given uuid
     * @param root the root element
     * @param uuid the uuid
     * @return the corresponding element or null if it does not exist
     */
    public static final Element getElementWithUUID(Element root, String uuid) {
        if (uuid == null || uuid.isEmpty()) {
            return null;
        }
        List<Element> list = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "uuid", uuid);
        if (list.size() != 0) {
            return list.get(0);
        }

        return null;
    }

    /**
     * Check if there is a window which has a dockableID equal to the given uuid
     * @param winuuid the uuid of the window
     * @param uuid the uuid to test
     * @return true if a dockableId exists
     */
    public static final boolean isDockableIdExisting(String winuuid, String uuid) {
        if (winuuid == null || winuuid.isEmpty() || uuid == null || uuid.isEmpty()) {
            return false;
        }

        Element win = getElementWithUUID(winuuid);
        if (win != null) {
            List<Element> list = ScilabXMLUtilities.getElementsWithAttributeEquals(win, "dockableId", uuid);
            if (list.size() != 0) {
                return true;
            }
        }

        return false;
    }

    /**
     * Validate all the windows in checking if all the dockable nodes are ok.
     * If a split node contains an invalid uuid, then the invalid dockable is removed
     * and the split node is replaced by the valid dockable.
     */
    private static final void validateWindows() {
        // We remove all the blanks and carriage return
        try {
            XPath xp = ScilabXPathFactory.newInstance().newXPath();
            NodeList nodes = (NodeList) xp.compile("//text()").evaluate(doc, XPathConstants.NODESET);
            for (int i = 0; i < nodes.getLength(); i++) {
                nodes.item(i).getParentNode().removeChild(nodes.item(i));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        Element root = doc.getDocumentElement();
        NodeList list = root.getElementsByTagName("Window");

        int length = getNodeListLength(list);
        List<Element> elems = new ArrayList<Element>(length);
        for (int i = 0; i < length; i++) {
            elems.add((Element) list.item(i));
        }

        for (Element e : elems) {
            validateWindow(e.getAttribute("uuid"));
        }
    }

    /**
     * Validate a window element.
     * The window element in the DOM is eventually replaced by a valid one.
     * @param winuuid the window uuid
     */
    private static final void validateWindow(final String winuuid) {
        if (winuuid == null || winuuid.isEmpty()) {
            return;
        }

        Element win = getElementWithUUID(winuuid);
        if (win == null) {
            return;
        }

        Element dp = (Element) win.getFirstChild();
        if (dp == null) {
            win.getParentNode().removeChild(win);
            return;
        }

        Element e = validateDockingPortNode(winuuid, dp);
        if (e == null) {
            win.removeChild(dp);
            return;
        }

        win.removeChild(dp);
        win.appendChild(e);
    }

    /**
     * Validate a dockingport node element.
     * @param winuuid the window uuid
     * @param e the element to validate
     * @return a valid element
     */
    private static final Element validateDockingPortNode(final String winuuid, final Element e) {
        Element ee = (Element) e.getFirstChild();
        if (ee == null) {
            return null;
        }

        if (ee.getTagName().equals("DockableNode")) {
            ee = validateDockableNode(winuuid, ee);
            if (ee == null) {
                return null;
            }
            return e;
        } else {
            ee = validateSplitNode(winuuid, ee);
            if (ee == null) {
                return null;
            }
            Element eee = (Element) e.cloneNode(false);
            eee.appendChild(ee);

            return eee;
        }
    }

    /**
     * Validate a dockable node element.
     * @param winuuid the window uuid
     * @param e the element to validate
     * @return a valid element
     */
    private static final Element validateDockableNode(final String winuuid, final Element e) {
        String id = e.getAttribute("dockableId");
        Element ee = getElementWithUUID(id);
        if (ee == null || !ee.getAttribute("winuuid").equals(winuuid)) {
            return null;
        }

        return e;
    }

    /**
     * Validate a split node element.
     * @param winuuid the window uuid
     * @param e the element to validate
     * @return a valid element
     */
    private static final Element validateSplitNode(final String winuuid, final Element e) {
        NodeList set = e.getChildNodes();
        Element c1 = validateDockingPortNode(winuuid, (Element) set.item(0));
        Element c2 = validateDockingPortNode(winuuid, (Element) set.item(1));

        if (c1 != null && c2 != null) {
            Element ee = (Element) e.cloneNode(false);
            ee.appendChild(c1);
            ee.appendChild(c2);
            return ee;
        }

        if (c1 == null && c2 == null) {
            return null;
        }

        if (c1 == null) {
            return (Element) c2.getFirstChild().cloneNode(true);
        }

        return (Element) c1.getFirstChild().cloneNode(true);
    }

    /**
     * Remove a node with a given uuid
     * @param parent the parent element
     * @param nodeName the node name
     * @param uuid the uuid
     */
    private static final void removeNode(Element parent, String nodeName, String uuid) {
        if (uuid == null || uuid.isEmpty()) {
            return;
        }
        Element e = getElementWithUUID(parent, nodeName, uuid);
        if (e != null) {
            parent.removeChild(e);
        }
    }

    /**
     * Remove a node with a given uuid
     * @param nodeName the node name
     * @param uuid the uuid
     */
    public static final void removeNode(String uuid) {
        if (uuid == null || uuid.isEmpty()) {
            return;
        }
        Element e = getElementWithUUID(uuid);
        if (e != null && e.getParentNode() != null) {
            e.getParentNode().removeChild(e);
        }
    }

    /**
     * Save the tab properties
     * @param tab the tab
     * @param nullWin if true, the winuuid will be set to 0 (the tab is not docked)
     */
    public static void saveTabProperties(SwingScilabDockablePanel tab, boolean nullWin) {
        if (ScilabConstants.isGUI()) {
            readDocument();

            ScilabTabFactory factory = ScilabTabFactory.getInstance();
            String uuid = tab.getPersistentId();
            Element root = doc.getDocumentElement();

            String app = factory.getApplication(uuid);
            if (app.isEmpty()) {
                return;
            }

            String winuuid;
            if (nullWin) {
                winuuid = NULLUUID;
            } else {
                winuuid = tab.getParentWindowUUID();
            }

            Dimension dim = tab.getSize();

            createNode(root, app, new Object[] {"winuuid", winuuid,
                                                "uuid", uuid,
                                                "load", factory.getPackage(uuid),
                                                "factory", factory.getClassName(uuid),
                                                "width", (int) dim.getWidth(),
                                                "height", (int) dim.getHeight()
                                               });
            writeDocument();
        }
    }

    /**
     * Clean the document in removing the useless tags
     * and validate the different windows.
     */
    public static void clean() {
        if (ScilabConstants.isGUI()) {
            readDocument();

            validateWindows();

            Element root = doc.getDocumentElement();
            NodeList list = root.getElementsByTagName("Window");
            int len = getNodeListLength(list);
            for (int i = 0; i < len; i++) {
                if (list.item(i) instanceof Element) {
                    String uuid = ((Element) list.item(i)).getAttribute("uuid");
                    List<Element> elements = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "winuuid", uuid);
                    if (elements == null || elements.size() == 0) {
                        root.removeChild(list.item(i));
                        removeWin(uuid);
                    }
                }
            }
            writeDocument();
        }
    }

    /**
     * Make a dependency between two tabs
     * @param parentUUID the parent tab uuid
     * @param childUUID the child tab uuid
     */
    public static final void makeDependency(String parentUUID, String childUUID) {
        if (ScilabConstants.isGUI()) {
            readDocument();

            Element e = getElementWithUUID(doc.getDocumentElement(), childUUID);
            if (e != null) {
                e.setAttribute("depends", parentUUID);
            }
            writeDocument();
        }
    }

    /**
     * Remove a dependency with the parent tab
     * @param childUUID the child tab uuid
     */
    public static void removeDependency(String childUUID) {
        if (ScilabConstants.isGUI()) {
            readDocument();

            Element e = getElementWithUUID(doc.getDocumentElement(), childUUID);
            if (e != null) {
                e.removeAttribute("depends");
            }
            writeDocument();
        }
    }

    /**
     * Restore an application by its uuid
     * @param uuid the application uuid
     * @return true if the operation succeded
     */
    public static boolean restoreUUID(final String uuid) {
        readDocument();
        clean();

        Element elem = getElementWithUUID(uuid);
        if (elem == null) {
            return false;
        }

        if (SwingUtilities.isEventDispatchThread()) {
            try {
                startRestoration(uuid);
            } catch (Exception e) {
                killScilab();
            }
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        startRestoration(uuid);
                    }
                });
            } catch (Exception e) {
                killScilab();
            }
        }

        if (ScilabConstants.isGUI()) {
            writeDocument();
        }

        doc = null;

        return true;
    }

    /**
     * Get the uuids of an application
     * @param name the application anem
     * @return the corresponding uuids
     */
    public static String[] getApplicationUUIDs(String name) {
        readDocument();

        NodeList list = doc.getDocumentElement().getElementsByTagName(name);
        String[] uuids = new String[getNodeListLength(list)];
        for (int i = 0; i < uuids.length; i++) {
            if (list.item(i) instanceof Element) {
                Element elem = (Element) list.item(i);
                uuids[i] = elem.getAttribute("uuid");
            }
        }

        return uuids;
    }

    /**
     * Get the length of a NodeList (this is a workaround for a f... java bug)
     */
    private static final int getNodeListLength(NodeList list) {
        int length = 0;
        try {
            length = list.getLength();
        } catch (NullPointerException e) { }

        return length;
    }

    private static final Options getOptions() {
        return XConfiguration.get(Options.class, XConfiguration.getXConfigurationDocument(), LAYOUT_PATH)[0];
    }


    /**
     * Inner interface used to have something to execute when the restoration is finished
     */
    public interface EndedRestoration {

        /**
         * Stuff to do when the restoration is ended
         */
        public void finish();
    }

    @XConfAttribute
    private static class Options {

        public boolean mustSave;
        public int id;

        @XConfAttribute(tag = "layouts", attributes = {"id", "save-desktop"})
        private void set(int id, boolean mustSave) {
            this.id = id;
            this.mustSave = mustSave;
        }
    }
}
