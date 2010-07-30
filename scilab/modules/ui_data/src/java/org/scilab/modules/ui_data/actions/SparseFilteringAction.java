package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Manages SPARSE variable display
 * @author Allan SIMON
 */
public class SparseFilteringAction extends FilteringAction {

	/** Name of the action */
	public static final String NAME = UiDataMessages.SPARSE;

	private static final long serialVersionUID = 2739127354867960374L;

	/**
	 * Constructor
	 * 
	 */
	public SparseFilteringAction() {
		super();
	}
	/**
	 * Create checkbox menu for the graph menu bar
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu() {
		CheckBoxMenuItem menu = createCheckBoxMenu(new SparseFilteringAction());
		return menu;
	}
}
