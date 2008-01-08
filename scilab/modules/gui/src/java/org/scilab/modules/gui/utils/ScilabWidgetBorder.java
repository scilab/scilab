
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.utils;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;

/**
 * Borders for Widgets in Scilab GUIs definition
 * @author Vincent COUVERT
 */
public final class ScilabWidgetBorder {
	
	/**
	 * Border used for FLAT relief property
	 */
	public static final Border FLAT = BorderFactory.createEmptyBorder();
	
	/**
	 * Border used for GROOVE relief property
	 */
	public static final Border GROOVE = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);

	/**
	 * Border used for RAISED relief property
	 */
	public static final Border RAISED = BorderFactory.createRaisedBevelBorder();
	
	/**
	 * Border used for RIDGE relief property
	 */
	public static final Border RIDGE = BorderFactory.createEtchedBorder(EtchedBorder.RAISED);

	/**
	 * Border used for SOLID relief property
	 */
	public static final Border SOLID = BorderFactory.createLineBorder(Color.BLACK);
	
	/**
	 * Border used for SUNKEN relief property
	 */
	public static final Border SUNKEN = BorderFactory.createLoweredBevelBorder();
	
	/**
	 * Constructor
	 */
	private ScilabWidgetBorder() {
		throw new UnsupportedOperationException();
	}
	
}
