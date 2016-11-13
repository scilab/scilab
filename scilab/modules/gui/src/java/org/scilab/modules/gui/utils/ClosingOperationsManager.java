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

import java.awt.Component;
import java.awt.GraphicsEnvironment;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.UUID;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JCheckBox;
import javax.swing.SwingUtilities;

import org.flexdock.docking.DockingConstants;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.commons.xml.XConfiguration;

/**
 * Class to handle the different closing operations.
 *
 * @author Calixte DENIZET
 */

@SuppressWarnings(value = { "serial" })
public class ClosingOperationsManager {

    private static final String EXIT_CONFIRM = Messages.gettext("Are you sure you want to close %s ?");
    private static final String EXIT_CONFIRM_AND = Messages.gettext("Are you sure you want to close %s and %s ?");
    private static final String DONT_SHOW = Messages.gettext("Do not show this message again");
    private static final String CONFIRMATION_PATH = "//general/confirmation-dialogs/body/tools/tool[@id='console-exit']";
    private static final String EXIT = Messages.gettext("Exit");
    private static final String NULLUUID = new UUID(0L, 0L).toString();
    private static final Map<SwingScilabDockablePanel, ClosingOperation> closingOps = new HashMap<SwingScilabDockablePanel, ClosingOperation>();
    private static final Map<SwingScilabDockablePanel, List<SwingScilabDockablePanel>> deps = new HashMap<SwingScilabDockablePanel, List<SwingScilabDockablePanel>>();

    private static final List<SwingScilabDockablePanel> dunnoList = new ArrayList<SwingScilabDockablePanel>();
    private static List<SwingScilabDockablePanel> savedList;
    private static boolean savedMustSave;

    private static SwingScilabDockablePanel root;

    static {
        deps.put(null, new ArrayList<SwingScilabDockablePanel>());
    }

    /**
     * Register a closing operation for a tab
     *
     * @param tab
     *            the associated tab
     * @param op
     *            the closing operation
     */
    public static void registerClosingOperation(SwingScilabDockablePanel tab, ClosingOperation op) {
        if (tab != null) {
            closingOps.put(tab, op);
        }
    }

    /**
     * Register a closing operation for a tab
     *
     * @param tab
     *            the associated tab
     * @param op
     *            the closing operation
     */
    public static void registerClosingOperation(Tab tab, ClosingOperation op) {
        if (tab != null) {
            registerClosingOperation((SwingScilabDockablePanel) tab.getAsSimpleTab(), op);
        }
    }

    /**
     * Unregister a closing operation for a tab
     *
     * @param tab the associated tab
     */
    public static void unregisterClosingOperation(SwingScilabDockablePanel tab) {
        if (tab != null) {
            closingOps.remove(tab);
        }
    }

    public static void checkTabForClosing(SwingScilabDockablePanel tab) {
        if (tab != null && !dunnoList.isEmpty()) {
            if (dunnoList.contains(tab)) {
                dunnoList.remove(tab);
            }
            if (dunnoList.isEmpty() && savedList != null) {
                close(savedList, null, false, savedMustSave);
                savedList = null;
                savedMustSave = false;
            }
        }
    }

    public static void removeFromDunnoList(SwingScilabDockablePanel tab) {
        if (tab != null && !dunnoList.isEmpty() && dunnoList.contains(tab)) {
            dunnoList.remove(tab);
        }
    }

    /**
     * Start a closing operation on root
     *
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperationOnRoot() {
        if (!GraphicsEnvironment.isHeadless()) {
            if (root != null) {
                // STD mode
                SwingScilabWindow win = getWindow(root);
                if (win == null) {
                    return true;
                }
                return startClosingOperation(win, true, true);
            } else if (deps.get(null).size() != 0) {
                // NW mode
                List<SwingScilabDockablePanel> list = new ArrayList<SwingScilabDockablePanel>();
                for (SwingScilabDockablePanel tab : deps.get(null)) {
                    collectTabsToClose(tab, list);
                }
                return close(list, null, true, true);
            } else {
                return true;
            }
        } else {
            return true;
        }
    }

    /**
     * Force a closing operation on root to dispose resources
     */
    public static void forceClosingOperationOnRoot() {
        if (root != null) {
            // STD mode
            SwingScilabWindow win = getWindow(root);
            if (win == null) {
                return;
            }
            startClosingOperation(win, false, false);
        } else if (deps.get(null).size() != 0) {
            // NW mode
            List<SwingScilabDockablePanel> list = new ArrayList<SwingScilabDockablePanel>();
            for (SwingScilabDockablePanel tab : deps.get(null)) {
                collectTabsToClose(tab, list);
            }
            close(list, null, false, false);
        }
    }

    /**
     * Start a closing operation on a tab
     *
     * @param tab
     *            the tab to close
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperation(SwingScilabDockablePanel tab) {
        return close(collectTabsToClose(tab), getWindow(tab), true, true);
    }

    /**
     * Start a closing operation on a tab
     *
     * @param tab
     *            the tab to close
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperation(Tab tab) {
        return startClosingOperation((SwingScilabDockablePanel) tab.getAsSimpleTab());
    }

    /**
     * Start a closing operation on a tab
     *
     * @param tab
     *            the tab to close
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperation(SwingScilabDockablePanel tab, boolean askToExit, boolean mustSave) {
        return close(collectTabsToClose(tab), getWindow(tab), askToExit, mustSave);
    }

    /**
     * Start a closing operation on multiple tabs
     *
     * @param tabs
     *            the tabs to close
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperation(List<SwingScilabDockablePanel> tabs, boolean askToExit, boolean mustSave) {
        final SwingScilabWindow win;
        if (tabs.isEmpty()) {
            // use the null window to select the console tab.
            win = null;
        } else {
            win = getWindow(tabs.get(0));
        }
        return close(collectTabsToClose(tabs), win, askToExit, mustSave);
    }

    /**
     * Start a closing operation on a tab
     *
     * @param tab
     *            the tab to close
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperation(Tab tab, boolean askToExit, boolean mustSave) {
        return startClosingOperation((SwingScilabDockablePanel) tab.getAsSimpleTab(), askToExit, mustSave);
    }

    /**
     * Start a closing operation on a tab
     *
     * @param tab
     *            the tab to close
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperationWithoutSave(SwingScilabDockablePanel tab) {
        return close(collectTabsToClose(tab), getWindow(tab), true, false);
    }

    /**
     * Start a closing operation on a tab
     *
     * @param tab
     *            the tab to close
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperationWithoutSave(Tab tab) {
        return startClosingOperationWithoutSave((SwingScilabDockablePanel) tab.getAsSimpleTab());
    }

    /**
     * Start a closing operation on a window
     *
     * Configured to ask for close and store configuration.
     *
     * @return true if the closing operation succeeded
     * @param window
     *            the window to close
     */
    public static boolean startClosingOperation(SwingScilabWindow window) {
        return startClosingOperation(window, true, true);
    }

    /**
     * Start a closing operation on a window
     *
     * @return true if the closing operation succeeded
     * @param window
     *            the window to close
     * @param askToExit
     *            ask to exit ?
     * @param mustSave
     *            store the configuration ?
     */
    public static boolean startClosingOperation(SwingScilabWindow window, boolean askToExit, boolean mustSave) {
        // Put the closing operation in a try/catch to avoid that an exception
        // blocks the shutting down. If it is not done, the Scilab process could stay alive.
        try {
            if (window != null) {
                List<SwingScilabDockablePanel> list = new ArrayList<SwingScilabDockablePanel>();
                if (window.getDockingPort() != null) {
                    Object[] dockArray = window.getDockingPort().getDockables().toArray();
                    for (int i = 0; i < dockArray.length; i++) {
                        collectTabsToClose((SwingScilabDockablePanel) dockArray[i], list);
                    }
                    return close(list, window, askToExit, mustSave);
                } else {
                    window.close();
                    return true;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return true;
    }

    /**
     * Start a closing operation on a window
     *
     * @param window
     *            the window to close
     * @param askToExit
     *            ask to exit ?
     * @param mustSave
     *            store the configuration ?
     * @return true if the closing operation succeeded
     */
    public static boolean startClosingOperation(Window window, boolean askToExit, boolean mustSave) {
        return startClosingOperation((SwingScilabWindow) window.getAsSimpleWindow(), askToExit, mustSave);
    }

    /**
     * Add a dependency between two tabs
     *
     * @param parent
     *            the parent tab
     * @param child
     *            the child tab
     */
    public static void addDependency(SwingScilabDockablePanel parent, SwingScilabDockablePanel child) {
        if (parent != null && child != null) {
            List<SwingScilabDockablePanel> children = deps.get(parent);
            if (children == null) {
                children = new ArrayList<SwingScilabDockablePanel>();
                deps.put(parent, children);
            }
            children.add(child);
        }
    }

    /**
     * Remove the given children from its parent
     * @param child teh child to remove
     */
    public static void removeDependency(SwingScilabDockablePanel child) {
        for (SwingScilabDockablePanel tab : deps.keySet()) {
            List<SwingScilabDockablePanel> children = deps.get(tab);
            if (children != null) {
                children.remove(child);
            }
        }
    }

    /**
     * Add a dependency between two tabs
     *
     * @param parent
     *            the parent tab
     * @param child
     *            the child tab
     */
    public static void addDependency(Tab parent, Tab child) {
        if (parent != null && child != null) {
            addDependency((SwingScilabDockablePanel) parent.getAsSimpleTab(), (SwingScilabDockablePanel) child.getAsSimpleTab());
        }
    }

    /**
     * Add a dependency between two tabs
     *
     * @param parent
     *            the parent tab
     * @param child
     *            the child tab
     */
    public static void addDependency(SwingScilabDockablePanel parent, Tab child) {
        if (parent != null && child != null) {
            addDependency(parent, (SwingScilabDockablePanel) child.getAsSimpleTab());
        }
    }

    /**
     * Add a dependency between two tabs
     *
     * @param parent
     *            the parent tab
     * @param child
     *            the child tab
     */
    public static void addDependency(Tab parent, SwingScilabDockablePanel child) {
        if (parent != null && child != null) {
            addDependency((SwingScilabDockablePanel) parent.getAsSimpleTab(), child);
        }
    }

    /**
     * Add a dependency with the root tab
     *
     * @param child
     *            the child tab
     */
    public static void addDependencyWithRoot(SwingScilabDockablePanel child) {
        if (child != null) {
            addDependency(root, child);
        }
    }

    /**
     * Add a dependency with the root tab
     *
     * @param child
     *            the child tab
     */
    public static void addDependencyWithRoot(Tab child) {
        if (child != null) {
            addDependency(root, (SwingScilabDockablePanel) child.getAsSimpleTab());
        }
    }

    /**
     * Set the root element (normally the console)
     *
     * @param root
     *            the root element
     */
    public static void setRoot(SwingScilabDockablePanel tab) {
        List<SwingScilabDockablePanel> list = deps.get(root);
        deps.remove(root);
        deps.put(tab, list);
        root = tab;
    }

    /**
     * Set the root element (normally the console)
     *
     * @param root
     *            the root element
     */
    public static void setRoot(Tab tab) {
        setRoot((SwingScilabDockablePanel) tab.getAsSimpleTab());
    }

    /**
     * Return the parent tab
     *
     * @param tab
     *            the child
     * @return the parent tab
     */
    private static SwingScilabDockablePanel getParent(SwingScilabDockablePanel tab) {
        for (SwingScilabDockablePanel key : deps.keySet()) {
            List<SwingScilabDockablePanel> list = deps.get(key);
            if (list != null && list.contains(tab)) {
                return key;
            }
        }
        return null;
    }

    /**
     * Return the parent tab
     *
     * @param tab
     *            the child
     * @return the parent tab
     */
    public static SwingScilabDockablePanel getElderTab(List<SwingScilabDockablePanel> tabs) {
        if (tabs == null || tabs.size() == 0) {
            return null;
        }

        int min = Integer.MAX_VALUE;
        SwingScilabDockablePanel elder = null;
        for (SwingScilabDockablePanel tab : tabs) {
            int level = 0;
            SwingScilabDockablePanel t = getParent(tab);
            while (t != null) {
                level++;
                t = getParent(t);
            }
            if (level < min) {
                elder = tab;
                min = level;
            }
        }

        return elder;
    }

    /**
     * Close a list of tabs
     *
     * @param list
     *            the list
     * @param window
     *            the window to use to center the modal dialog
     * @param askToExit
     *            if true, then ask to exit
     * @param mustSave
     *            if true, the configuration is saved
     * @return true if the closing operation succeeded
     */
    private static final boolean close(List<SwingScilabDockablePanel> list, SwingScilabWindow window, boolean askToExit, boolean mustSave) {
        boolean ret = false;
        if (!askToExit || canClose(list, window, mustSave)) {
            ret = true;
            SwingScilabDockablePanel console = null;
            try {
                // First thing we get the console (if it is here) to be sure to
                // kill it !
                for (SwingScilabDockablePanel tab : list) {
                    if (tab.getPersistentId().equals(NULLUUID)) {
                        console = tab;
                        break;
                    }
                }

                // We remove the tabs which have a callback and no
                // ClosingOperation
                // To avoid annoying situations the tab will be undocked and
                // closed
                List<SwingScilabDockablePanel> tabsToRemove = new ArrayList<SwingScilabDockablePanel>();
                for (SwingScilabDockablePanel tab : list) {
                    if (closingOps.get(tab) == null) {
                        tab.setVisible(false);
                        tab.getActionButton("undock").getAction().actionPerformed(null);
                        Action action = ((SciClosingAction) tab.getActionButton(DockingConstants.CLOSE_ACTION).getAction()).getAction();
                        if (action == null) {
                            SwingScilabWindow win = getWindow(tab);
                            if (win != null) {
                                win.removeTabs(new SwingScilabDockablePanel[] { tab });
                            }
                        } else {
                            action.actionPerformed(null);
                        }
                        tabsToRemove.add(tab);
                    }
                }
                list.removeAll(tabsToRemove);

                // we group the tabs by win
                Map<SwingScilabWindow, List<SwingScilabDockablePanel>> map = new HashMap<SwingScilabWindow, List<SwingScilabDockablePanel>>();
                for (SwingScilabDockablePanel tab : list) {
                    SwingScilabWindow win = getWindow(tab);
                    if (win != null) {
                        if (!map.containsKey(win)) {
                            map.put(win, new ArrayList<SwingScilabDockablePanel>());
                        }
                        map.get(win).add(tab);
                    }
                }

                List<SwingScilabWindow> winsWithOneTab = new ArrayList<SwingScilabWindow>();
                List<SwingScilabWindow> windowsToClose = new ArrayList<SwingScilabWindow>();
                for (SwingScilabWindow win : map.keySet()) {
                    List<SwingScilabDockablePanel> listTabs = map.get(win);
                    int nbDockedTabs = win.getNbDockedObjects();
                    if (nbDockedTabs == listTabs.size()) {
                        // all the tabs in the window are removed so we save the
                        // win state
                        if (mustSave) {
                            WindowsConfigurationManager.saveWindowProperties(win);
                        }
                        windowsToClose.add(win);
                    } else {
                        if (nbDockedTabs - listTabs.size() == 1) {
                            winsWithOneTab.add(win);
                        }
                        // the window will stay opened
                        if (mustSave) {
                            for (SwingScilabDockablePanel tab : listTabs) {
                                WindowsConfigurationManager.saveTabProperties(tab, true);
                            }
                        }
                    }
                }

                // If a parent and a child are removed, we make a dependency
                // between them
                // The parent restoration will imply the child one
                for (SwingScilabDockablePanel tab : list) {
                    SwingScilabDockablePanel parent = getParent(tab);
                    if (list.contains(parent) || parent == null) {
                        if (parent != null) {
                            WindowsConfigurationManager.makeDependency(parent.getPersistentId(), tab.getPersistentId());
                        } else if (!tab.getPersistentId().equals(NULLUUID)) {
                            // if the parent is null, we make a dependency with
                            // the console which is the default root
                            WindowsConfigurationManager.makeDependency(NULLUUID, tab.getPersistentId());
                        }
                    } else {
                        WindowsConfigurationManager.removeDependency(tab.getPersistentId());
                    }
                }

                WindowsConfigurationManager.clean();
                // We destroy all the tabs: children before parents.
                for (SwingScilabDockablePanel tab : list) {
                    tab.setVisible(false);
                    if (!tab.getPersistentId().equals(NULLUUID)) {
                        try {
                            closingOps.get(tab).destroy();
                            if (tab != console) {
                                closingOps.remove(tab);
                            }
                        } catch (Exception e) {
                            // An error can occurred during the destroy operation
                            // We show it but it mustn't avoid the window
                            // destruction
                            e.printStackTrace();
                        }
                    }
                }

                // We remove the tabs in each window
                // The tabs are removed in one time to avoid that the
                // ActiveDockableTracker tryes to give the activation to a
                // removed tab
                for (SwingScilabWindow win : map.keySet()) {
                    win.removeTabs(map.get(win).toArray(new SwingScilabDockablePanel[0]));
                }

                // It stays one docked tab so we remove close and undock action
                for (SwingScilabWindow win : winsWithOneTab) {
                    Object[] dockArray = win.getDockingPort().getDockables().toArray();
                    SwingScilabDockablePanel.removeActions((SwingScilabDockablePanel) dockArray[0]);
                }

                // We wait until all the windows are definitely closed
                while (windowsToClose.size() != 0) {
                    List<SwingScilabWindow> toRemove = new ArrayList<SwingScilabWindow>();
                    for (SwingScilabWindow win : windowsToClose) {
                        WindowsConfigurationManager.removeWin(win.getUUID());
                        if (win.isDisplayable()) {
                            Thread.yield();
                        } else {
                            toRemove.add(win);
                        }
                    }
                    windowsToClose.removeAll(toRemove);
                }

                // We remove the tabs from the cache
                for (SwingScilabDockablePanel tab : list) {
                    ScilabTabFactory.getInstance().removeFromCache(tab.getPersistentId());
                    SwingScilabDockablePanel parent = getParent(tab);
                    List<SwingScilabDockablePanel> l = deps.get(parent);
                    if (l != null) {
                        l.remove(tab);
                    }
                    deps.remove(tab);
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (console != null) {
                    try {
                        closingOps.get(console).destroy();
                        closingOps.remove(console);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        return ret;
    }

    /**
     * Check if the tabs of the liste are closable or not
     *
     * @param list
     *            the list of tabs
     * @param window
     *            the window to use to center the modal dialog
     * @return true if all the tabs can be closed
     */
    private static final boolean canClose(List<SwingScilabDockablePanel> list,
                                          SwingScilabWindow window,
                                          boolean mustSave) {
        CheckExitConfirmation cec = XConfiguration.get(CheckExitConfirmation.class, XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH)[0];
        dunnoList.clear();
        savedList = null;
        savedMustSave = false;

        if (cec.checked) {
            String question = makeQuestion(list);
            final boolean[] checked = new boolean[1];
            final Action action = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    checked[0] = ((JCheckBox) e.getSource()).isSelected();
                }
            };

            if (question != null) {
                final Component focused = KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusOwner();
                if (ScilabModalDialog.show(window, new String[] { question }, EXIT, IconType.WARNING_ICON, ButtonType.YES_NO, DONT_SHOW, action) == AnswerOption.NO_OPTION) {
                    if (checked[0]) {
                        XConfiguration.set(XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH + "/@state", "unchecked");
                    }
                    if (focused != null) {
                        SwingUtilities.invokeLater(new Runnable() {
                            public void run() {
                                focused.requestFocus();
                            }
                        });
                    }
                    return false;
                }
            }

            if (checked[0]) {
                XConfiguration.set(XConfiguration.getXConfigurationDocument(), CONFIRMATION_PATH + "/@state", "unchecked");
            }
        }

        for (SwingScilabDockablePanel t : list) {
            ClosingOperation op = closingOps.get(t);
            if (op != null) {
                int ret = op.canClose();
                if (ret == 0) {
                    dunnoList.clear();
                    return false;
                }
                if (ret == -1) {
                    dunnoList.add(t);
                }
            }
        }

        if (dunnoList.isEmpty()) {
            return true;
        }

        for (SwingScilabDockablePanel tab : dunnoList) {
            list.remove(tab);
        }

        savedList = list;
        savedMustSave = mustSave;

        return false;
    }

    /**
     * Make the question to ask to exit
     *
     * @param list
     *            the list of the tabs to close
     * @return the question
     */
    private static final String makeQuestion(List<SwingScilabDockablePanel> list) {
        List<String> apps = new ArrayList<String>();
        List<SwingScilabDockablePanel> toBeRemoved = Collections.unmodifiableList(list);
        for (SwingScilabDockablePanel t : list) {
            ClosingOperation op = closingOps.get(t);
            if (op != null) {
                String name = op.askForClosing(toBeRemoved);
                if (name != null && !apps.contains(name)) {
                    apps.add(name);
                }
            }
        }
        switch (apps.size()) {
            case 0:
                return null;
            case 1:
                return String.format(EXIT_CONFIRM, apps.get(0));
        }

        String str = apps.remove(0);
        String last = apps.remove(apps.size() - 1);
        for (String s : apps) {
            str += ", " + s;
        }

        return String.format(EXIT_CONFIRM_AND, str, last);
    }

    /**
     * Collect the tabs and their children to close (recursive function)
     *
     * @param tab
     *            the current tab
     * @param list
     *            the list
     */
    private static final void collectTabsToClose(SwingScilabDockablePanel tab,
            List<SwingScilabDockablePanel> list) {
        List<SwingScilabDockablePanel> children = deps.get(tab);
        if (children != null) {
            for (SwingScilabDockablePanel t : children) {
                collectTabsToClose(t, list);
            }
        }
        if (!list.contains(tab)) {
            list.add(tab);
        }

        /*
         * Update the tab list in case of hidden (eg. dynamic) dependencies
         */
        final List<SwingScilabDockablePanel> ro = Collections.unmodifiableList(list);
        for (ListIterator<SwingScilabDockablePanel> it = list.listIterator(); it.hasNext();) {
            final SwingScilabDockablePanel t = it.next();

            final ClosingOperation op = closingOps.get(t);
            if (op != null) {
                op.updateDependencies(ro, it);
            }
        }
    }

    /**
     * Collect the tabs and their children to close (recursive function)
     *
     * @param tab
     *            the current tab
     * @return the list of the tabs to close
     */
    private static final List<SwingScilabDockablePanel> collectTabsToClose(
        SwingScilabDockablePanel tab) {
        final List<SwingScilabDockablePanel> list = new ArrayList<SwingScilabDockablePanel>();
        collectTabsToClose(tab, list);
        return list;
    }

    /**
     * Collect the tabs and their children to close (recursive function)
     *
     * @param tabs
     *            the current tabs
     * @return the list of the tabs to close
     */
    private static final List<SwingScilabDockablePanel> collectTabsToClose(List<SwingScilabDockablePanel> tabs) {
        final List<SwingScilabDockablePanel> list = new ArrayList<SwingScilabDockablePanel>();
        for (final SwingScilabDockablePanel tab : tabs) {
            collectTabsToClose(tab, list);
        }
        return list;
    }

    /**
     * Get the window containing the given tab
     *
     * @param tab
     *            the tab
     * @return the corresponding window
     */
    private static final SwingScilabWindow getWindow(SwingScilabDockablePanel tab) {
        return (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, tab);
    }

    /**
     * Inner interface to handle a closing operation Must be registered with the
     * static method ClosingOperationsManager.registerClosingOperation
     */
    public interface ClosingOperation {

        /**
         * @return 0 or 1 if the associated tab can be closed or not, and -1 if the answer is unknown
         */
        public int canClose();

        /**
         * Destroy the resources associated to the tab
         */
        public void destroy();

        /**
         * @return non null String if the tab requires a
         *         "Are you sure you want to close FOO ?..."
         */
        public String askForClosing(final List<SwingScilabDockablePanel> list);

        /**
         * Update the dependency list to handle specific dependency
         *
         * @param list
         *            the tab list to update
         * @param it
         *            the iterator to update
         */
        public void updateDependencies(final List<SwingScilabDockablePanel> list, final ListIterator<SwingScilabDockablePanel> it);
    }

    @XConfAttribute
    private static class CheckExitConfirmation {

        public boolean checked;

        private CheckExitConfirmation() { }

        @XConfAttribute(attributes = {"state"})
        private void set(String checked) {
            this.checked = checked.equals("checked");
        }
    }
}
