package org.scilab.modules.graphic_objects;

/**
 * Camera class
 * @author juliachs
 */
public class Camera {
	/** View type */
	private enum ViewType { VIEW_2D, VIEW_3D };
	
	/** Specifies whether the projection must be performed for a 2D or a 3D scene */  
	private ViewType view;
	
	/** Specifies whether an isoview transformation must be applied or not */
	private boolean isoview;
	
	/** Specifies whether a unit cube transformation must be performed */
	private boolean cubeScaling;
	
	/** 2-element array (alpha and theta rotation angles) */
	private double [] rotationAngles;
	
	// Methods: to be done
}
