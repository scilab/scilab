package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

public class PolynomialFilteringAction extends FilteringAction{
	/** Name of the action */
	public static final String NAME = UiDataMessages.POLYNOMIAL;
	
	private static final Integer CLASS_NUMBER = 2;

	/**
	 * Constructor
	 * 
	 */
	public PolynomialFilteringAction() {
		super();
	}
	/**
	 * Create checkbox menu for the graph menu bar
	 * 
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu() {
		CheckBoxMenuItem menu = createCheckBoxMenu( new PolynomialFilteringAction());
		return menu;
	}
}
