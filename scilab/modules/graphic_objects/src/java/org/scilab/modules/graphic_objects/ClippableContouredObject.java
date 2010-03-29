package org.scilab.modules.graphic_objects;

/**
 * ClippableContouredObject
 * @author juliachs
 */
public abstract class ClippableContouredObject extends ContouredObject {
	/** Clipping state */
	private ClippableProperty clipProperty;

	/** Constructor */
	public ClippableContouredObject() {
		super();
		clipProperty = null;
	}

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

	// Methods: to be done
}
