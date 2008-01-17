
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
public final class ScilabRelief {
	
	/**
	 * Border used for FLAT relief property
	 */
	public static final String FLAT = "flat";
	public static final Border FLAT_BORDER = BorderFactory.createEmptyBorder();
	
	/**
	 * Border used for GROOVE relief property
	 */
	public static final String GROOVE = "groove";
	public static final Border GROOVE_BORDER = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);

	/**
	 * Border used for RAISED relief property
	 */
	public static final String RAISED = "raised";
	public static final Border RAISED_BORDER = BorderFactory.createRaisedBevelBorder();
	
	/**
	 * Border used for RIDGE relief property
	 */
	public static final String RIDGE = "ridge";
	public static final Border RIDGE_BORDER = BorderFactory.createEtchedBorder(EtchedBorder.RAISED);

	/**
	 * Border used for SOLID relief property
	 */
	public static final String SOLID = "solid";
	public static final Border SOLID_BORDER = BorderFactory.createLineBorder(Color.BLACK);
	
	/**
	 * Border used for SUNKEN relief property
	 */
	public static final String SUNKEN = "sunken";
	public static final Border SUNKEN_BORDER = BorderFactory.createLoweredBevelBorder();
	
	/**
	 * Constructor
	 */
	private ScilabRelief() {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Get the border corresponding to the Scilab relief type
	 * @param reliefType Scilab type for relief
	 * @return the corresponding border
	 */
	public static Border getBorderFromRelief(String reliefType) {
		Border returnBorder = SUNKEN_BORDER;
		if (reliefType.equals(FLAT)) {
			returnBorder = FLAT_BORDER;
		} else if (reliefType.equals(GROOVE)) {
			returnBorder = GROOVE_BORDER;
		} else if (reliefType.equals(RAISED)) {
			returnBorder = RAISED_BORDER;
		} else if (reliefType.equals(RIDGE)) {
			return RIDGE_BORDER;
		} else if (reliefType.equals(SOLID)) {
			returnBorder = SOLID_BORDER;
		}
		return returnBorder;
	}
}
