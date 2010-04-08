/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Sylvestre KOUMAR
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSpinner;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.RowFilter;
import javax.swing.SwingConstants;
import javax.swing.RowFilter.ComparisonType;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableRowSorter;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.ui_data.datatable.SwingTableModel;
import org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser;
import org.scilab.modules.ui_data.variablebrowser.VariableBrowser;

public class BrowseVar extends SwingScilabTab implements Tab {

    private static final long serialVersionUID = 1L;

    private JTable table;
    private TableRowSorter<SwingTableModel<String>> sorter;

    private JTextField field_name;
    private JComboBox combo_type;
    private JCheckBox check_global;
    private JCheckBox check_local;
    private JSpinner spinner_byte;

    public BrowseVar() {
	super("BrowseVar");
	//setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

	String[] columnsName = {"Name",
		"Value",
		"Type",
		"Global/Local",
		"Size",
	"Bytes used"};

	Object[][] data = {
		{"A", "[1,1;1,1]", "Double matrix", "Local", "2x2", "47"},
		{"B", "<1x5 cell>", "Cell", "Local", "1x5", "103"},
		{"C", "<2x2 cell>", "Cell", "Local", "2x2", "97"},
		{"a", "1", "Double", "Local", "1x1", "13"},
		{"b", "2", "Double", "Local", "1x1", "13"},
		{"c", "3", "Double", "Local", "1x1", "13"},
		{"tata", "hello", "Char", "Global", "1x5", "54"},
		{"titi", "nice", "Char", "Global", "1x4", "47"},
		{"toto", "to meet u", "Char", "Global", "1x9", "47"},
	};
	
	// Create a table with a sorter
	SwingTableModel<Object> model = new SwingTableModel<Object>(columnsName);
	//sorter = new TableRowSorter<SwingTableModel<String>>(model);
	model.setData(data);
	
	
	table = new JTable(model);
	//table = new JTable((Object[][]) data, (Object[]) columnsName);
	//table.setRowSorter(sorter);
	//table.se
	//table.setPreferredScrollableViewportSize(new Dimension(700, 500));
	table.setFillsViewportHeight(true);

	// Mouse selection mode
	table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

	// Display selection positions
	table.getSelectionModel().addListSelectionListener(
		new ListSelectionListener() {
		    public void valueChanged(ListSelectionEvent event) {
			int viewRow = table.getSelectedRow();
			int viewCol = table.getSelectedColumn();
			System.out.println(String.format("Selected Row: %d. " + "Selected Column: %d.", viewRow, viewCol));
		    }
		}
	);
	
	JScrollPane scrollPane = new JScrollPane(table);
	add(scrollPane);



	// Filter panel
	JPanel filter = new JPanel(new FlowLayout());

	// Name
	JLabel lab_name = new JLabel("Name:", SwingConstants.TRAILING);
	filter.add(lab_name);
	field_name = new JTextField();
	field_name.setPreferredSize(new Dimension(150, 20));
	// Apply filter on variables names
	field_name.getDocument().addDocumentListener(
		new DocumentListener() {
		    public void changedUpdate(DocumentEvent e) {
			nameFilter();
		    }
		    public void insertUpdate(DocumentEvent e) {
			nameFilter();
		    }
		    public void removeUpdate(DocumentEvent e) {
			nameFilter();
		    }
		});
	lab_name.setLabelFor(field_name);
	filter.add(field_name);

	// Type
	String[] type_list = {"All", "Double", "Cell", "Double matrix", "Char"};

	combo_type = new JComboBox(type_list);
	combo_type.setSelectedIndex(0);
	combo_type.addItemListener(
		new ItemListener() {
		    public void itemStateChanged(ItemEvent e) {
			typeFilter();
		    }
		});

	filter.add(combo_type);


	// Global &  Local
	check_global = new JCheckBox("Global");
	check_global.setSelected(true);
	check_local = new JCheckBox("Local");
	check_local.setSelected(true);

	ItemListener itemlisten = new ItemListener(){
	    public void itemStateChanged(ItemEvent e) {
		globalLocalFilter(e);
	    }
	};

	check_global.addItemListener(itemlisten);
	check_local.addItemListener(itemlisten);

	filter.add(check_local);
	filter.add(check_global);

	// Byte
	JLabel lab_byte = new JLabel("Bytes:", SwingConstants.TRAILING);
	filter.add(lab_byte);

	Vector<String> byte_table = new Vector<String>();
//	for (int i = 0; i < sorter.getModel().getRowCount(); i++) {
//	    byte_table.add((String) sorter.getModel().getValueAt(i, 5));
//	}

	// Remove duplicates in byte list
	Set set = new HashSet(byte_table);
	byte_table.clear();
	byte_table.addAll(set);
	// Sort byte list
	Collections.sort(byte_table);

	//Integer max = new Integer(byte_table.lastElement()+1);
	//SpinnerModel spin_model = new SpinnerNumberModel((Number)max, //initial value
	//	0, 			//min
//		max, 	//max
//		1); 			//step
//	spinner_byte = new JSpinner(spin_model);

//	spinner_byte.addChangeListener(new ChangeListener() {

//	    public void stateChanged(ChangeEvent e) {
//		byteFilter();
//	    }
//	});

//	filter.add(spinner_byte);



	// Add panel filter to content pane
	add(filter);

    }

    private void nameFilter() {
	RowFilter<SwingTableModel<String>, Object> rf = null;
	// If current expression doesn't parse, don't update
	try {
	    rf = RowFilter.regexFilter(field_name.getText(), 0);
	} catch (java.util.regex.PatternSyntaxException e) {
	    return;
	}
	sorter.setRowFilter(rf);
    }

    private void typeFilter() {
	RowFilter<SwingTableModel<String>, Object> rf = null;
	// If current expression doesn't parse, don't update
	try {
	    if (combo_type.getSelectedItem().toString().equals("All")) {
		rf = RowFilter.regexFilter("", 2);
	    } else {
		rf = RowFilter.regexFilter(combo_type.getSelectedItem().toString(), 2);
	    }
	} catch (java.util.regex.PatternSyntaxException e) {
	    return;
	}
	sorter.setRowFilter(rf);
    }

    private void globalLocalFilter(ItemEvent e) {
	RowFilter<SwingTableModel<String>, Object> rf = null;
	RowFilter<SwingTableModel<String>, Object> rfg = null;
	RowFilter<SwingTableModel<String>, Object> rfl = null;
	// If current expression doesn't parse, don't update

	Object source = e.getItemSelectable();


	if (source == check_local) {
	    try {
		rfg = RowFilter.regexFilter(check_global.getText(), 3);
		sorter.setRowFilter(rfg);
	    } catch (java.util.regex.PatternSyntaxException ex) {
		return;
	    }
	} else if (source == check_global) {
	    try {
		rfl = RowFilter.regexFilter(check_local.getText(), 3);
		sorter.setRowFilter(rfl);
	    } catch (java.util.regex.PatternSyntaxException ex) {
		return;
	    }
	}

	if (e.getStateChange() == ItemEvent.SELECTED) {
	    try {
		rf = RowFilter.regexFilter("", 3);
		sorter.setRowFilter(rf);
	    } catch (java.util.regex.PatternSyntaxException ex) {
		return;
	    }
	}

    }

    private void byteFilter() {
	RowFilter<SwingTableModel<String>, Object> rf = null;
	// If current expression doesn't parse, don't update
	try {
	    rf = RowFilter.numberFilter(ComparisonType.BEFORE, Integer.parseInt(spinner_byte.getValue().toString()));
	} catch (java.util.regex.PatternSyntaxException e) {
	    return;
	}
	sorter.setRowFilter(rf);
    }


    private static void createAndShowGUI() {
//	SwingScilabWindow frame = new SwingScilabWindow();
//	frame.setTitle("BrowseVar");
//
//	Tab demo = new BrowseVar();
//	demo.setVisible(true);
//	frame.addTab(demo);
//
//
//	frame.setVisible(true);
	VariableBrowser browsevar = ScilabVariableBrowser.getVariableBrowser();
	browsevar.setVisible(true);
	
    }

    public static void main(String[] args) {
	javax.swing.SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		createAndShowGUI();
	    }
	});
    }

    @Override
    public SimpleTab getAsSimpleTab() {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public Window getParentWindow() {
	// TODO Auto-generated method stub
	return null;
    }

    @Override
    public void addInfoBar(TextBox infoBarToAdd) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void addMenuBar(MenuBar menuBarToAdd) {
	// TODO Auto-generated method stub
	
    }

    @Override
    public void addToolBar(ToolBar toolBarToAdd) {
	// TODO Auto-generated method stub
	
    }
    
    public static void openVariableBrowser() {
	VariableBrowser browsevar = ScilabVariableBrowser.getVariableBrowser();
	browsevar.setVisible(true);
    }
    
    public static void closeVariableBrowser() {
	ScilabVariableBrowser.getVariableBrowser().close();
    }
}