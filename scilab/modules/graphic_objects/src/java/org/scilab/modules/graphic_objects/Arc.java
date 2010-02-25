package org.scilab.modules.graphic_objects;

/**
 * Arc class
 * @author juliachs
 */
public class Arc extends ClippableContouredObject {
	/** Arc drawing method */
	private enum ArcDrawingMethod { NURBS, LINES };

	/** Bounding box upper-left point coordinates */
	private double [] upperLeftPoint;

	/** Bounding box width */
	private double width;

	/** Bounding box height */
	private double height;

	/** Start angle */
	private double startAngle;

	/** End angle */
	private double endAngle;
	
	/** Specifies which drawing method to use */
	ArcDrawingMethod arcDrawingMethod;

	//	 Methods: to be done
}
