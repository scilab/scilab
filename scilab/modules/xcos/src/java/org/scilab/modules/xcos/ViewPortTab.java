/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

import java.net.MalformedURLException;
import java.util.List;
import java.util.ListIterator;
import java.util.Objects;
import java.util.Optional;
import java.util.UUID;

import org.flexdock.docking.activation.ActiveDockableTracker;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;

public final class ViewPortTab extends SwingScilabDockablePanel {
    private static final long serialVersionUID = 1L;
    public static final String DEFAULT_WIN_UUID = "xcos-viewport-default-window";

    private ViewPortTab(XcosDiagram graph, String uuid) {
        super(XcosMessages.VIEWPORT, uuid);

        init();

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
        public EndedRestoration(XcosDiagram graph) {
        }

        @Override
        public void finish() {
        }
    }

    /*
     * Static API for Tabs
     */

    /**
     * Make the tab visible
     */
    public static void restore(final XcosDiagram graph) {
        Optional<String> uuid = resolveTabUUID(graph);
        graph.setViewPortTab(uuid.orElse(UUID.randomUUID().toString()));
        ConfigurationManager.getInstance().addToRecentTabs(graph);
        
        boolean restored = false;
        if (uuid.isPresent()) {
            restored = WindowsConfigurationManager.restoreUUID(graph.getViewPortTab());
        } 
        if (!restored) {
            create(graph);
        }
    }

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
     * Create the viewport tab for the graph
     *
     * @param graph
     *            the graph
     */
    private static void create(final XcosDiagram graph) {
        final ViewPortTab tab = allocate(graph);
        
        final SwingScilabWindow win = WindowsConfigurationManager.createWindow(DEFAULT_WIN_UUID, false);
        win.addTab(tab);
        win.setVisible(true);

        ActiveDockableTracker.requestDockableActivation(tab);
    }

    /**
     * Resolve the UUID to a pre-existing one
     */
    private static Optional<String> resolveTabUUID(XcosDiagram graph) {
        String uuid = graph.getViewPortTab();
        if (uuid != null)
            return Optional.of(uuid);

        // check if previous uuid have been stored in settings
        final String url;
        if (graph.getSavedFile() != null)
        {
            String temp;
            try {
                temp = graph.getSavedFile().toURI().toURL().toExternalForm();  
            } catch (MalformedURLException e) {
                temp = null;
            }
            url = temp;
        } else {
            url = null;
        }
        
        Optional<String> presetID = ConfigurationManager.getInstance().streamTab()
            .filter(t -> Objects.equals(url, t.getUrl()))
            .filter(t -> t.getUuid() == graph.getGraphTab())
            .map(t -> t.getViewport())
            .findFirst();
        
        return presetID;
    }

    /**
     * Create a tab from scratch
     */
    protected static ViewPortTab allocate(final XcosDiagram graph)
    {
        ViewPortTab tab = new ViewPortTab(graph, graph.getViewPortTab());
        
        // check xcos tab menu item
        final XcosTab xcosTab = XcosTab.get(graph);
        xcosTab.setViewportChecked(true);

        ClosingOperationsManager.registerClosingOperation((SwingScilabDockablePanel) tab, new ClosingOperation(graph));
        ClosingOperationsManager.addDependency((SwingScilabDockablePanel) XcosTab.get(graph), (SwingScilabDockablePanel) tab);

        WindowsConfigurationManager.registerEndedRestoration((SwingScilabDockablePanel) tab, new EndedRestoration(graph));
        WindowsConfigurationManager.makeDependency(graph.getGraphTab(), tab.getPersistentId());
    
        ScilabTabFactory.getInstance().addToCache(tab);
        return tab;
    }

    /*
     * Specific implementation
     */

    private void initComponents(XcosDiagram graph) {
        final mxGraphOutline outline = new mxGraphOutline(graph.getAsComponent());
        outline.setDrawLabels(true);

        setContentPane(outline);
    }
}
