package org.scilab.modules.graphic_objects;

/**
 * ClippableProperty class
 * @author juliachs
 */
public class ClippableProperty {
	/** Indicates how clipping is performed */
	public static enum ClipStateType { OFF, CLIPGRF, ON };

	/** Clipping state */
	private ClipStateType clipState;

	/** Clip box (4- or 6-element array) */
	private double [] clipBox;
	
	/** Constructor */
	public ClippableProperty() {
		clipState = ClipStateType.OFF;
		clipBox = null;
	}

	/**
	 * @return the clipBox
	 */
	public double[] getClipBox() {
		return clipBox;
	}

	/**
	 * @param clipBox the clipBox to set
	 */
	public void setClipBox(double[] clipBox) {
		this.clipBox = clipBox;
	}

	/**
	 * @return the clipState
	 */
	public ClipStateType getClipState() {
		return clipState;
	}

	/**
	 * @param clipState the clipState to set
	 */
	public void setClipState(ClipStateType clipState) {
		this.clipState = clipState;
	}

	// Methods: to be done
}
