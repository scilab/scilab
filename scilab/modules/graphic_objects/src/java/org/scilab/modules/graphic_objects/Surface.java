package org.scilab.modules.graphic_objects;

/**
 * Surface class
 * @author juliachs
 */
public abstract class Surface extends ClippableContouredObject {
	// TBD Data -> Data Model

	/** Specifies whether the surface is drawn or not */
	private boolean surfaceMode;

	/** Specifies the front face color (when colorFlag == 0) */
	private int colorMode;

	/** Specifies how facet colors are computed */
	private int colorFlag;
	
	//	 Methods: to be done
}
