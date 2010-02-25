package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * Legend class
 * @author juliachs
 */
public class Legend extends ClippableTextObject {
	/** Legend location */
	private enum LegendLocation { IN_UPPER_RIGHT, IN_UPPER_LEFT, IN_LOWER_RIGHT, IN_LOWER_LEFT,
		OUT_UPPER_RIGHT, OUT_UPPER_LEFT, OUT_LOWER_RIGHT, OUT_LOWER_LEFT,
		UPPER_CAPTION, LOWER_CAPTION, BY_COORDINATES };
	
	/** List of the polylines referred to */
	private ArrayList <Polyline> links;

	/** Legend location */
	private LegendLocation legendLocation;

	/** 2D position relative to the parent axes bounds */
	private double [] position;
	
	//	 Methods: to be done
}
