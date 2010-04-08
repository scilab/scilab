/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variablebrowser;

import java.awt.Color;

import javax.swing.ImageIcon;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.ui_data.datatable.SwingTableModel;
import org.scilab.modules.ui_data.datatable.ScilabTable;

public class SwingScilabVariableBrowser extends SwingScilabTab implements Tab {

    public SwingScilabVariableBrowser() {
	super(Messages.gettext("Variable Browser"));
	String[] columnsName = {"Type", 
		"Name",
		"Value",
		"Type",
		"Global/Local",
		"Size",
	"Bytes used"};

	String iconPath = System.getenv("SCI") + "/modules/ui_data/images/icons/";
	ImageIcon tp00 = new ImageIcon(iconPath + "tp00.gif");
	ImageIcon tp01 = new ImageIcon(iconPath + "tp01gif");
	ImageIcon tp02 = new ImageIcon(iconPath + "tp02.gif");
	ImageIcon tp04 = new ImageIcon(iconPath + "tp04.gif");
	ImageIcon tp05 = new ImageIcon(iconPath + "tp05.gif");
	ImageIcon tp06 = new ImageIcon(iconPath + "tp06.gif");
	ImageIcon tp08 = new ImageIcon(iconPath + "tp08.gif");
	ImageIcon tp10 = new ImageIcon(iconPath + "tp10.gif");
	ImageIcon tp15 = new ImageIcon(iconPath + "tp15.gif");
	ImageIcon tp16 = new ImageIcon(iconPath + "tp16.gif");
	ImageIcon tp17 = new ImageIcon(iconPath + "tp17.gif");
	
	System.out.println(tp00.toString());

	
	Object[][] data = {
		{tp00,"A", "[1,1;1,1]", "Double matrix", "Local", "2x2", "47"},
		{tp01,"B", "<1x5 cell>", "Cell", "Local", "1x5", "103"},
		{tp02,"C", "<2x2 cell>", "Cell", "Local", "2x2", "97"},
		{tp04,"a", "1", "Double", "Local", "1x1", "13"},
		{tp05,"b", "2", "Double", "Local", "1x1", "13"},
		{tp06,"c", "3", "Double", "Local", "1x1", "13"},
		{tp08,"tata", "hello", "Char", "Global", "1x5", "54"},
		{tp10,"titi", "nice", "Char", "Global", "1x4", "47"},
		{tp15,"toto", "to meet u", "Char", "Global", "1x9", "47"},
		{tp16,"toto", "to meet u", "Char", "Global", "1x9", "47"},
		{tp17,"toto", "to meet u", "Char", "Global", "1x9", "47"},
	};
	
	ScilabTable<Object> model = new SwingTableModel<Object>(columnsName);
	//sorter = new TableRowSorter<SwingTableModel<String>>(model);
	model.setData(data);
	
	
	JTable table = new JTable(model);
	//table = new JTable((Object[][]) data, (Object[]) columnsName);
	//table.setRowSorter(sorter);
	//table.se
	//table.setPreferredScrollableViewportSize(new Dimension(700, 500));
	table.setFillsViewportHeight(true);
	table.setAutoResizeMode(CENTER);
	table.setAutoCreateRowSorter(true);
	
	// Mouse selection mode
	table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	
	JScrollPane scrollPane = new JScrollPane(table);
	table.setBackground(Color.WHITE);
	setContentPane(scrollPane);
	//add(table);
	
    }

    public void addInfoBar(TextBox infoBarToAdd) {
	setInfoBar(infoBarToAdd);
    }

    public void addMenuBar(MenuBar menuBarToAdd) {
	setMenuBar(menuBarToAdd);
    }

    public void addToolBar(ToolBar toolBarToAdd) {
	setToolBar(toolBarToAdd);
    }

    @Override
    public SimpleTab getAsSimpleTab() {
	// TODO Auto-generated method stub
	return this;
    }

    @Override
    public Window getParentWindow() {
	// TODO Auto-generated method stub
	return null;
    }

  
}
