package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Manage BOOLEAN SPARSE variables display
 * @author Allan SIMON
 */
public class SparseBoolFilteringAction extends FilteringAction {
	
	/** Name of the action */
	public static final String NAME = UiDataMessages.SPARSE_BOOLEAN;

	private static final long serialVersionUID = 438747338817314109L;

	/**
	 * Constructor
	 * 
	 */
	public SparseBoolFilteringAction() {
		super();
	}
	
	/**
	 * Create checkbox menu for the graph menu bar
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu() {
		CheckBoxMenuItem menu = createCheckBoxMenu(new SparseBoolFilteringAction());
		return menu;
	}

}
