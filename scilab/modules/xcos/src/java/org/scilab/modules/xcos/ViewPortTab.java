package org.scilab.modules.xcos;

import java.util.List;
import java.util.ListIterator;
import java.util.UUID;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;

public final class ViewPortTab extends SwingScilabTab implements Tab {
    public static final String DEFAULT_WIN_UUID = "xcos-viewport-default-window";
    public static final String DEFAULT_TAB_UUID = "xcos-viewport-default-tab";

    private ViewPortTab(XcosDiagram graph, String uuid) {
        super(XcosMessages.VIEWPORT, uuid);

        graph.setViewPortTab(uuid);
        setWindowIcon(Xcos.ICON.getImage());

        initComponents(graph);
    }

    private static class ClosingOperation
            implements
            org.scilab.modules.gui.utils.ClosingOperationsManager.ClosingOperation {
        private final XcosDiagram graph;

        public ClosingOperation(final XcosDiagram graph) {
            this.graph = graph;
        }

        @Override
        public boolean canClose() {
            return true;
        }

        @Override
        public void destroy() {
            graph.setViewPortTab(null);

            final XcosTab tab = XcosTab.get(graph);
            tab.setViewportChecked(false);
        }

        @Override
        public String askForClosing(List<SwingScilabTab> list) {
            return null;
        }

        @Override
        public void updateDependencies(List<SwingScilabTab> list,
                ListIterator<SwingScilabTab> it) {
        }

    }

    private static class EndedRestoration implements
            WindowsConfigurationManager.EndedRestoration {
        private final XcosDiagram graph;

        public EndedRestoration(XcosDiagram graph) {
            this.graph = graph;
        }

        @Override
        public void finish() {
            ConfigurationManager.getInstance().removeFromRecentTabs(
                    graph.getViewPortTab());
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

        ClosingOperationsManager.registerClosingOperation((SwingScilabTab) tab,
                new ClosingOperation(graph));
        ClosingOperationsManager.addDependency(
                (SwingScilabTab) XcosTab.get(graph), (SwingScilabTab) tab);

        WindowsConfigurationManager.registerEndedRestoration(
                (SwingScilabTab) tab, new EndedRestoration(graph));
    }

    /*
     * Specific implementation
     */

    private void initComponents(XcosDiagram graph) {
        final mxGraphOutline outline = new mxGraphOutline(
                graph.getAsComponent());
        outline.setDrawLabels(true);

        setContentPane(outline);
    }

    private Window createDefaultWindow() {
        final Window win;

        final Window configuration = WindowsConfigurationManager.createWindow(
                DEFAULT_WIN_UUID, false);
        if (configuration != null) {
            win = configuration;
        } else {
            win = ScilabWindow.createWindow();
        }

        win.addTab(this);
        return win;
    }

    /*
     * Implement Tab
     */

    @Deprecated
    @Override
    public void addToolBar(ToolBar toolBarToAdd) {
        // no toolbar
    }

    @Deprecated
    @Override
    public void addMenuBar(MenuBar menuBarToAdd) {
        // no menubar
    }

    @Deprecated
    @Override
    public void addInfoBar(TextBox infoBarToAdd) {
        // no infobar
    }

    @Deprecated
    @Override
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    @Deprecated
    @Override
    public Window getParentWindow() {
        return createDefaultWindow();
    }
}
