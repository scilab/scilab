package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.ClippableProperty.ClipStateType;

/**
 * GraphicClippableObject class 
 * @author juliachs
 */
public abstract class GraphicClippableObject extends GraphicObject {
	/** Stores the actual clipping state */
	private ClippableProperty clipProperty;
	
	/** Default constructor */
	GraphicClippableObject() {
		super();
		clipProperty = null;
	}
	
	/*
	GraphicClippableObject(ClipStateType clipState, double[] clipBox) {
		super();
		clipProperty = new ClippableProperty(clipState, clipBox);
	}
	*/

	/**
	 * @return the clipProperty
	 */
	public ClippableProperty getClipProperty() {
		return clipProperty;
	}

	/**
	 * @param clipProperty the clipProperty to set
	 */
	public void setClipProperty(ClippableProperty clipProperty) {
		this.clipProperty = clipProperty;
	}

	//	 Methods: to be done
}
