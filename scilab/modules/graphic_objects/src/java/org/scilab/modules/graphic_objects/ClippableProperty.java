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

	// Methods: to be done
}
