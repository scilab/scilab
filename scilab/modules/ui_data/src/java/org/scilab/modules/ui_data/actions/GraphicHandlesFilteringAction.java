package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

public class GraphicHandlesFilteringAction extends FilteringAction {
	/** Name of the action */
	public static final String NAME = UiDataMessages.GRAPHIC_HANDLES;
	
	private static final Integer CLASS_NUMBER = 9;

	/**
	 * Constructor
	 * 
	 */
	public GraphicHandlesFilteringAction() {
		super();
	}
	/**
	 * Create checkbox menu for the graph menu bar
	 * 
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu() {
		CheckBoxMenuItem menu = createCheckBoxMenu( new GraphicHandlesFilteringAction());
		return menu;
	}

}
