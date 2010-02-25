package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * TicksProperty class
 * @author juliachs
 */
public class TicksProperty {
	/** Specifies whether ticks are automatically computed or not */
	private boolean auto;

	/** Ticks locations along their axis */
	private double [] locations;

	/** Ticks labels */
	private ArrayList <FormattedText> labels;
	
	/** Number of subticks between two main ticks */
	private int subticks;

	//	 Methods: to be done
}
