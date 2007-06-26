
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.Dimension;

import com.artenum.console.ui.ConsoleTextPane;

/**
 * Scilab Console UI which contains the previous commands and their outputs
 * @author couvert
 *
 */
public class SciOutputView extends ConsoleTextPane {
	
	private static final long serialVersionUID = 1L;
	
	/**
	 * Constructor
	 */
	public SciOutputView() {
		super();
	}

	/**
	 * Gets th eminimum size of this component
	 * @return the minimum size
	 * @see javax.swing.JComponent#getMinimumSize()
	 */
	@Override
	public Dimension getMinimumSize() {
		return SMALL;
	}

}
