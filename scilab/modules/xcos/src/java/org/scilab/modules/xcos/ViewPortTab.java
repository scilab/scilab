/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Clement DAVID
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

import java.util.List;
import java.util.ListIterator;
import java.util.UUID;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;

@SuppressWarnings(value = { "serial" })
public final class ViewPortTab extends SwingScilabDockablePanel implements SimpleTab {
    public static final String DEFAULT_WIN_UUID = "xcos-viewport-default-window";
    public static final String DEFAULT_TAB_UUID = "xcos-viewport-default-tab";

    private ViewPortTab(XcosDiagram graph, String uuid) {
        super(XcosMessages.VIEWPORT, uuid);

        graph.setViewPortTab(uuid);
        setWindowIcon(Xcos.ICON.getImage());

        initComponents(graph);
    }

    private static class ClosingOperation implements org.scilab.modules.gui.utils.ClosingOperationsManager.ClosingOperation {
        private final XcosDiagram graph;

        public ClosingOperation(final XcosDiagram graph) {
            this.graph = graph;
        }

        @Override
        public int canClose() {
            return 1;
        }

        @Override
        public void destroy() {
            graph.setViewPortTab(null);

            final XcosTab tab = XcosTab.get(graph);
            tab.setViewportChecked(false);
        }

        @Override
        public String askForClosing(List<SwingScilabDockablePanel> list) {
            return null;
        }

        @Override
        public void updateDependencies(List<SwingScilabDockablePanel> list, ListIterator<SwingScilabDockablePanel> it) {
        }

    }

    private static class EndedRestoration implements WindowsConfigurationManager.EndedRestoration {
        private final XcosDiagram graph;

        public EndedRestoration(XcosDiagram graph) {
            this.graph = graph;
        }

        @Override
        public void finish() {
            ConfigurationManager.getInstance().removeFromRecentTabs(graph.getViewPortTab());
        }
    }

    /*
     * Static API for Tabs
     */

    /**
     * Get the viewport for a graph.
     *
     * @param graph
     *            the graph
     * @return the view port
     */
    public static ViewPortTab get(XcosDiagram graph) {
        final String uuid = graph.getViewPortTab();
        return (ViewPortTab) ScilabTabFactory.getInstance().getFromCache(uuid);
    }

    /**
     * Restore or create the viewport tab for the graph
     *
     * @param graph
     *            the graph
     */
    public static void restore(XcosDiagram graph) {
        restore(graph, true);
    }

    /**
     * Restore or create the viewport tab for the graph
     *
     * @param graph
     *            the graph
     * @param visible
     *            should the tab should be visible
     */
    public static void restore(final XcosDiagram graph, final boolean visible) {
        String uuid = graph.getViewPortTab();
        if (uuid == null) {
            uuid = UUID.randomUUID().toString();
        }

        ViewPortTab tab = new ViewPortTab(graph, uuid);
        if (visible) {
            tab.createDefaultWindow().setVisible(true);
            tab.setCurrent();
        }
        ScilabTabFactory.getInstance().addToCache(tab);

        ClosingOperationsManager.registerClosingOperation((SwingScilabDockablePanel) tab, new ClosingOperation(graph));
        ClosingOperationsManager.addDependency((SwingScilabDockablePanel) XcosTab.get(graph), (SwingScilabDockablePanel) tab);

        WindowsConfigurationManager.registerEndedRestoration((SwingScilabDockablePanel) tab, new EndedRestoration(graph));
    }

    /*
     * Specific implementation
     */

    private void initComponents(XcosDiagram graph) {
        final mxGraphOutline outline = new mxGraphOutline(graph.getAsComponent());
        outline.setDrawLabels(true);

        setContentPane(outline);
    }

    private SwingScilabWindow createDefaultWindow() {
        final SwingScilabWindow win;

        final SwingScilabWindow configuration = WindowsConfigurationManager.createWindow(DEFAULT_WIN_UUID, false);
        if (configuration != null) {
            win = configuration;
        } else {
            win = SwingScilabWindow.createWindow(true);
        }

        win.addTab(this);
        return win;
    }

}
