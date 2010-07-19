package org.scilab.modules.ui_data.actions;

import java.awt.event.ActionEvent;

import org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser;

public abstract class FilteringAction extends DefaultAction {

	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;
	
	private static final Integer CLASS_NUMBER = 0;

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		ScilabVariableBrowser.getVariableBrowser().updateRowFiltering();
	}

}
