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

import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.ui_data.utils.UiDataMessages;


/**
 * 
 * Class ScilabVariableBrowser
 * Implements a ScilabWindow containing Variable Browser (JTable) 
 *
 */
public final class ScilabVariableBrowser extends ScilabWindow implements VariableBrowser {

	private static VariableBrowser instance;

	private static SimpleVariableBrowser browserTab;  

	/**
	 * Constructor
	 * @param columnNames the columns title.
	 */
	private ScilabVariableBrowser(String[] columnNames) {
		super();
		browserTab = new SwingScilabVariableBrowser(columnNames);
		browserTab.setCallback(ScilabCallBack
				.createCallback("org.scilab.modules.ui_data.BrowseVar.closeVariableBrowser", ScilabCallBack.JAVA_OUT_OF_XCLICK_AND_XGETMOUSE));

		TextBox infobar = ScilabTextBox.createTextBox();
		browserTab.addInfoBar(infobar);
		addTab(browserTab);
	}

	/**
	 * Retrieve Singleton
	 * @param columnNames : columns title
	 * @param data : data from scilab (type, name, size, ...)
	 * @return the Variable Browser
	 */
	public static VariableBrowser getVariableBrowser(String[] columnNames, Object[][] data) {
		VariableBrowser variableBrowser = getVariableBrowser(columnNames);
		variableBrowser.setData(data);
		return variableBrowser;
	}

	/**
	 * Get the variable browser singleton
	 * @return the Variable Browser
	 */
	public static VariableBrowser getVariableBrowser() {
		return instance;
	}

	/**
	 * Get the variable browser singleton with specified columns title.
	 * @param columnNames : the columns title
	 * @return the Variable Browser
	 */
	public static VariableBrowser getVariableBrowser(String[] columnNames) {
		if (instance == null) {
			instance = new ScilabVariableBrowser(columnNames);
			instance.setVisible(true);
		}
		browserTab.setName(UiDataMessages.VARIABLE_BROWSER);
		return instance;
	}

	/**
	 * Close Variable Browser
	 */
	public void close() {
		ScilabWindow editvarWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(browserTab.getParentWindowId());
		editvarWindow.removeTab(browserTab);
		browserTab.setVisible(false);
		browserTab.close();
		instance = null;
	}

	/**
	 * Set columns title.
	 * @param columnNames : columns Title
	 */
	public void setColumnNames(String[] columnNames) {
		browserTab.setColumnNames(columnNames);

	}

	/**
	 * Set data displayed in JTable
	 * @param data : data to be displayed in JTable
	 */
	public void setData(Object[][] data) {
		browserTab.setData(data);
	}
	
	
	public void updateRowFiltering() {
		browserTab.updateRowFiltering();
	}
	
	/**
	 * {@inheritDoc}
	 */
	public void setVisible(boolean status) {
		super.setVisible(status);
		browserTab.setVisible(status);
	}

}
