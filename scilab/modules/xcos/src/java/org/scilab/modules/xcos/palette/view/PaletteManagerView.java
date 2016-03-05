/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.palette.view;

import java.util.List;
import java.util.ListIterator;

import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.PaletteSearchManager;
import org.scilab.modules.xcos.palette.actions.ClosePalettesAction;
import org.scilab.modules.xcos.palette.actions.LoadAsPalAction;
import org.scilab.modules.xcos.palette.actions.NavigationAction;
import org.scilab.modules.xcos.palette.actions.SearchAction;
import org.scilab.modules.xcos.palette.actions.ZoomAction;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the default view for the palette
 */
@SuppressWarnings(value = { "serial" })
public class PaletteManagerView extends SwingScilabDockablePanel implements SimpleTab {
    public static final String DEFAULT_WIN_UUID = "xcos-palette-default-window";
    public static final String DEFAULT_TAB_UUID = PaletteManager.MODEL_CLASS_PACKAGE;

    private final PaletteManager controller;
    private PaletteManagerPanel panel;
    private PaletteSearchManager searchMgr;

    /**
     * Default constructor
     *
     * @param controller
     *            the associated controller
     */
    private PaletteManagerView(final PaletteManager controller) {
        super(XcosMessages.PALETTE_BROWSER + " - " + Xcos.TRADENAME,
              DEFAULT_TAB_UUID);

        this.searchMgr = new PaletteSearchManager();
        this.controller = controller;
        setWindowIcon(Xcos.ICON.getImage());

        initComponents();
    }

    private static class ClosingOperation
        implements
        org.scilab.modules.gui.utils.ClosingOperationsManager.ClosingOperation {

        public ClosingOperation() {
        }

        @Override
        public int canClose() {
            return 1;
        }

        @Override
        public void destroy() {
            PaletteManager.getInstance().firePropertyChange("visible", true, false);
        }

        @Override
        public String askForClosing(List<SwingScilabDockablePanel> list) {
            return null;
        }

        @Override
        public void updateDependencies(List<SwingScilabDockablePanel> list,
                                       ListIterator<SwingScilabDockablePanel> it) {
        }
    }

    /*
     * Static API for Tabs
     */

    public static PaletteManagerView get() {
        return (PaletteManagerView) ScilabTabFactory.getInstance()
               .getFromCache(DEFAULT_TAB_UUID);
    }

    public static void restore(Void v) {
        restore(v, true);
    }

    public static void restore(Void v, final boolean visible) {
        PaletteManagerView tab = new PaletteManagerView(
            PaletteManager.getInstance());
        if (visible) {
            tab.createDefaultWindow().setVisible(true);
        }
        PaletteManager.getInstance().firePropertyChange("visible", false, true);

        ClosingOperationsManager.addDependencyWithRoot((SwingScilabDockablePanel) tab);
        ClosingOperationsManager.registerClosingOperation((SwingScilabDockablePanel) tab,
                new ClosingOperation());
        ScilabTabFactory.getInstance().addToCache(tab);
    }

    /**
     * @return the controller
     */
    protected PaletteManager getController() {
        return controller;
    }

    /**
     * @return the panel
     */
    public PaletteManagerPanel getPanel() {
        return panel;
    }

    /**
     * @param panel
     *            the panel to set
     */
    public void setPanel(final PaletteManagerPanel panel) {
        this.panel = panel;
    }

    /** Instantiate and setup all the components */
    private void initComponents() {
        /* Create the content pane */
        panel = new PaletteManagerPanel(getController());
        setContentPane(panel);

        /* Create the menu bar */
        final MenuBar menuBar = ScilabMenuBar.createMenuBar();

        // Palettes
        final Menu palettes = ScilabMenu.createMenu();
        palettes.setText(XcosMessages.PALETTES);
        palettes.setMnemonic('P');
        menuBar.add(palettes);

        palettes.add(LoadAsPalAction.createMenu(null));
        palettes.addSeparator();
        palettes.add(ClosePalettesAction.createMenu(null));

        // View
        final Menu view = ScilabMenu.createMenu();
        view.setText(XcosMessages.VIEW);
        view.setMnemonic('V');
        menuBar.add(view);

        view.add(ZoomAction.createMenuZoomIn());
        view.addSeparator();
        view.add(ZoomAction.createMenuZoomOut());
        view.addSeparator();
        view.add(NavigationAction.createMenuPrev());
        view.addSeparator();
        view.add(NavigationAction.createMenuNext());

        setMenuBar(menuBar);

        /* Create the toolbar */
        final ToolBar toolbar = ScilabToolBar.createToolBar();
        SwingScilabToolBar stb = (SwingScilabToolBar) toolbar.getAsSimpleToolBar();

        stb.add(LoadAsPalAction.createButton(null));

        stb.addSeparator();

        stb.add(NavigationAction.createButtonPrev());
        stb.add(NavigationAction.createButtonNext());

        stb.addSeparator();

        stb.add(ZoomAction.createButtonZoomIn());
        stb.add(ZoomAction.createButtonZoomOut());
        ZoomAction.registerKeyAction(getPanel().getActionMap(), getPanel().getInputMap());
        ZoomAction.registerKeyAction(getTree().getActionMap(), getTree().getInputMap());

        stb.addSeparator();

        stb.add(SearchAction.createSearchBar());

        setToolBar(toolbar);

        /* Create the infobar */
        setInfoBar(ScilabTextBox.createTextBox());

        setCallback(new ClosePalettesAction(null));
        getTree().revalidate();
        getPanel().performStartUpLayout();
    }

    /** @return the category tree */
    public JTree getTree() {
        return (JTree) ((JScrollPane) panel.getLeftComponent()).getViewport()
               .getView();
    }

    /**
     * Update the selected path on the tree
     */
    public static void updateTree() {
        if (get() == null) {
            restore(null);
        }
        final JTree t = get().getTree();
        final TreePath p = t.getSelectionPath();
        boolean isExpanded = t.isExpanded(p);

        if (p == null) {
            updateWholeTree();
        } else {
            ((DefaultTreeModel) t.getModel()).reload((TreeNode) p
                    .getLastPathComponent());
            t.setSelectionPath(p);
        }

        // if the tree was expanded, ensures that it will remain expanded
        if (isExpanded) {
            t.expandPath(p);
        }
    }

    /**
     * Update the whole tree
     */
    public static void updateWholeTree() {
        if (get() == null) {
            restore(null);
        }
        final JTree t = get().getTree();

        final TreePath selectedPath = t.getSelectionPath();
        ((DefaultTreeModel) t.getModel()).reload();
        t.setSelectionPath(selectedPath);
    }

    /**
     * @param info
     *            the information to write on the infobar
     */
    public void setInfo(final String info) {
        getInfoBar().setText(info);
    }

    private SwingScilabWindow createDefaultWindow() {
        final SwingScilabWindow win;

        final SwingScilabWindow configuration = WindowsConfigurationManager.createWindow(
                DEFAULT_WIN_UUID, false);
        if (configuration != null) {
            win = configuration;
        } else {
            win = SwingScilabWindow.createWindow(true);
        }

        win.addTab(this);
        return win;
    }

    /**
     * Get the PalettesearchManager instance
     * @return PalettesearchManager instance
     */
    public PaletteSearchManager getSearchManager() {
        return searchMgr;
    }

}
