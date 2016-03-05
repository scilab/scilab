/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
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
package org.scilab.modules.gui.plotbrowser;

import java.awt.CardLayout;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

import org.scilab.modules.gui.plotbrowser.actions.CloseAction;
import org.scilab.modules.gui.plotbrowser.actions.RefreshAction;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;

/**
 * Swing implementation of Plot Browser.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class SwingPlotBrowser extends SwingScilabDockablePanel {
    private JScrollPane desktop;
    public static JPanel pReceive;
    private MenuBar menuBar;
    private Menu fileMenu;
    public static final String PLOTBROWSERUUID = "4m249547-6a71-4998-r8c-00o367s47932-B";

    /**
    * Constructor
    *
    * @param objectID Enters the identification of object.
    */
    public SwingPlotBrowser(Integer objectID) {
        super(MessagesPlotBrowser.plot_browser, PLOTBROWSERUUID);

        setAssociatedXMLIDForHelp("plotbrowser");

        buildMenuBar();
        addMenuBar(menuBar);

        ToolBar toolBar = ScilabToolBar.createToolBar();
        //toolBar.add(RefreshAction.createButton(MessagesPlotBrowser.refresh));
        toolBar.addSeparator();

        guiComponents();
        new SwapFigure(objectID);

        setContentPane(desktop);
        WindowsConfigurationManager.restorationFinished(this);
        addToolBar(toolBar);
    }

    /**
    * It has all the components of the plot browser panel.
    */
    private void guiComponents() {
        desktop = new JScrollPane();
        pReceive = new JPanel();

        desktop.setBorder(null);
        desktop.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        desktop.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        desktop.setAlignmentX(0.0F);
        desktop.setAlignmentY(0.0F);

        pReceive.setAlignmentX(0.0F);
        pReceive.setAlignmentY(0.0F);
        pReceive.setLayout(new CardLayout());

        desktop.setViewportView(pReceive);
    }

    /**
    * Creates the menuBar.
    */
    public void buildMenuBar() {
        menuBar = ScilabMenuBar.createMenuBar();

        fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(MessagesPlotBrowser.file);
        fileMenu.setMnemonic('F');
        fileMenu.add(CloseAction.createMenu());

        menuBar.add(fileMenu);
    }

    /**
     * {@inheritDoc}
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }
}