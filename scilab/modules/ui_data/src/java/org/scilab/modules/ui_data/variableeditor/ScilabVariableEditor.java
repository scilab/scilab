/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.variableeditor;

import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.localization.Messages;

/**
 * 
 * Class ScilabVariableEditor
 * Implements a ScilabWindow containing Variable Editor (JTable) 
 *
 */
public class ScilabVariableEditor extends ScilabWindow implements VariableEditor {

	private static final String MENUBARXMLFILE = System.getenv("SCI") + "/modules/ui_data/etc/variableeditor_menubar.xml";

	private static VariableEditor instance;
	private static String variableName;

	private static SimpleVariableEditor editorTab;

	/**
	 * Constructor
	 * @param data the jtable data.
	 * @param variableName the name in Scilab of the variable being edited.
	 */
	private ScilabVariableEditor(Object[][] data, String variableName) {
		super();
		ScilabVariableEditor.variableName = variableName;
		editorTab = new SwingScilabVariableEditor(data);
		editorTab.setCallback(ScilabCallBack
				.createCallback("org.scilab.modules.ui_data.EditVar.closeVariableEditor",
						ScilabCallBack.JAVA_OUT_OF_XCLICK_AND_XGETMOUSE));
		MenuBar menubar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
		editorTab.addMenuBar(menubar);

		TextBox infobar = ScilabTextBox.createTextBox();
		editorTab.addInfoBar(infobar);
		addTab(editorTab);
	}

	/**
	 * Close Variable Editor
	 */
	public void close() {
		ScilabWindow editvarWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(editorTab.getParentWindowId());
		editvarWindow.removeTab(editorTab);
		editorTab.setVisible(false);
		editorTab.close();
		instance = null;
	}

	/**
	 * Set data displayed in JTable
	 * @param data : data to be displayed in JTable
	 */
	public void setData(Object[][] data) {
		editorTab.setData(data);
	}

	/**
	 * {@inheritDoc}
	 */
	public void setValueAt(Object value, int row, int col) {
		editorTab.setValueAt(value, row, col);

	}

	/**
	 * Get the variable editor singleton
	 * @return the Variable Editor
	 */
	public static VariableEditor getVariableEditor() {
		return instance;
	}

	/**
	 * Get the variable editor singleton with specified data.
	 * @param data : the data to fill the editor with
	 * @param variableName : the scilab name of the variable being edited.
	 * @return the Variable Editor
	 */
	public static VariableEditor getVariableEditor(Object[][] data, String variableName) {
		if (instance == null) {
			instance = new ScilabVariableEditor(data, variableName);
			instance.setVisible(true);
		} else {
			instance.setVariableName(variableName);
			instance.setData(data);
		}
		editorTab.setName(Messages.gettext("Variable Editor") + " - " + variableName);
		return instance;
	}

	/**
	 * {@inheritDoc}
	 */
	public String getVariablename() {
		return ScilabVariableEditor.variableName;
	}

	/**
	 * {@inheritDoc}
	 */
	public void setVariableName(String variableName) {
		ScilabVariableEditor.variableName = variableName;
	}
	
	/**
	 * {@inheritDoc}
	 */
	public void setVisible(boolean status) {
		super.setVisible(status);
		editorTab.setVisible(status);
	}
}
