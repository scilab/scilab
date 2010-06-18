package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

public class SparceFilteringAction extends FilteringAction{
	/** Name of the action */
	public static final String NAME = UiDataMessages.SPARCE;
	
	private static final Integer CLASS_NUMBER = 5;

	/**
	 * Constructor
	 * 
	 */
	public SparceFilteringAction() {
		super();
	}
	/**
	 * Create checkbox menu for the graph menu bar
	 * 
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu() {
		CheckBoxMenuItem menu = createCheckBoxMenu( new SparceFilteringAction());
		return menu;
	}
}
