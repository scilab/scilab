package org.scilab.modules.graphic_objects;

/**
 * ClippableTextObject
 * @author juliachs
 */
public abstract class ClippableTextObject extends TextObject {
	/** Clipping state */
	private ClippableProperty clipProperty;

	/**  Constructor */
	public ClippableTextObject() {
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
