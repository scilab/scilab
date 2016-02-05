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
package org.scilab.modules.gui.ged;

import java.awt.CardLayout;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.ged.actions.CloseAction;
import org.scilab.modules.gui.ged.actions.ShowHide;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;

/**
 * Swing implementation of Scilab Graphic Editor.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class SwingInspector extends SwingScilabDockablePanel {
    private static SwingInspector instance;
    private JScrollPane desktop;
    private static JPanel pReceive;
    private MenuBar menuBar;
    private Menu fileMenu;
    public static final String INSPECTORUUID = "4m249547-6a71-4998-r8c-00o367s47932";

    private static String imagepath = System.getenv("SCI") + "/modules/gui/images/icons/";
    public static String icon_color_fill = imagepath + "16x16/actions/color-fill.png";
    public static String icon_collapse = imagepath + "16x16/actions/tree-collapse.png";
    public static String icon_expand = imagepath + "16x16/actions/media-playback-start.png";
    public static String icon_expand_all = imagepath + "32x32/actions/tree-diagramm.png";
    public static String icon_collapse_all = imagepath + "32x32/actions/tree-diagramm-delete.png";
    public static String icon_table = imagepath + "16x16/actions/table.png";

    /**
    * Constructor
    *
    * @param objectID Enters the identification of object.
    */
    public SwingInspector(Integer objectID) {
        super(MessagesGED.property_list, INSPECTORUUID);
        instance = this;

        setAssociatedXMLIDForHelp("quickged");

        buildMenuBar();
        addMenuBar(menuBar);

        ToolBar toolBar = ScilabToolBar.createToolBar();
        ((SwingScilabToolBar) toolBar.getAsSimpleToolBar()).add(ShowHide.createButton(MessagesGED.hide));
        toolBar.addSeparator();

        guiComponents();
        new SwapObject(objectID);

        setContentPane(desktop);
        WindowsConfigurationManager.restorationFinished(this);
        addToolBar(toolBar);
    }

    /**
    * It has all the components of the inspector panel.
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
     * Creates the menuBar
     */
    public void buildMenuBar() {
        menuBar = ScilabMenuBar.createMenuBar();

        fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(MessagesGED.file);
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

    /**
    * Prepare the panel to receive a new JPanel.
    * @param panel new jpanel
    * @param title object name
    */
    public static void setPanel(JPanel panel, String title) {
        //Resets the panel.
        pReceive.removeAll();
        pReceive.repaint();
        //Resets the Show/Hide button.
        ShowHide.setStatus(false);
        //Insert panel
        pReceive.add(panel, "");
        try {
            instance.setName(MessagesGED.property_list + ": " + title);
        } catch (NullPointerException e) {
            instance.setTitle(MessagesGED.property_list + ": " + title);
        }
    }
}
