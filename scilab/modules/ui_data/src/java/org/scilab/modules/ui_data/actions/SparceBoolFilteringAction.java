package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

public class SparceBoolFilteringAction extends FilteringAction {
	/** Name of the action */
	public static final String NAME = UiDataMessages.SPARCE_BOOLEAN;
	
	private static final Integer CLASS_NUMBER = 6;

	/**
	 * Constructor
	 * 
	 */
	public SparceBoolFilteringAction() {
		super();
	}
	/**
	 * Create checkbox menu for the graph menu bar
	 * 
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu() {
		CheckBoxMenuItem menu = createCheckBoxMenu( new SparceBoolFilteringAction());
		return menu;
	}

}
