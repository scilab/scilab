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

	/** Constructor */
	public Camera() {
		view = null;
		isoview = false;
		cubeScaling = false;
		rotationAngles = null;
	}

	/**
	 * @return the cubeScaling
	 */
	public boolean isCubeScaling() {
		return cubeScaling;
	}

	/**
	 * @param cubeScaling the cubeScaling to set
	 */
	public void setCubeScaling(boolean cubeScaling) {
		this.cubeScaling = cubeScaling;
	}

	/**
	 * @return the isoview
	 */
	public boolean isIsoview() {
		return isoview;
	}

	/**
	 * @param isoview the isoview to set
	 */
	public void setIsoview(boolean isoview) {
		this.isoview = isoview;
	}

	/**
	 * @return the rotationAngles
	 */
	public double[] getRotationAngles() {
		return rotationAngles;
	}

	/**
	 * @param rotationAngles the rotationAngles to set
	 */
	public void setRotationAngles(double[] rotationAngles) {
		this.rotationAngles = rotationAngles;
	}

	/**
	 * @return the view
	 */
	public ViewType getView() {
		return view;
	}

	/**
	 * @param view the view to set
	 */
	public void setView(ViewType view) {
		this.view = view;
	}
	
	// Methods: to be done
}
