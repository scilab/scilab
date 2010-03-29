package org.scilab.modules.graphic_objects;

/**
 * Label class
 * @author juliachs
 */
public class Label extends TextObject {
	/** Text orientation */
	private double fontAngle;
	
	 /** Specifies whether the label position is automatically computed or not */
	private boolean autoPosition;

	/** 3-element array: (x, y, z) position */
	private double [] position;

	/** Specifies whether the label orientation is automatically computed or not */
	private boolean autoRotation;

	/** Constructor */
	public Label() {
		super();
		fontAngle = 0.0;
		autoPosition = false;
		position = null;
		autoRotation = false;
	}

	/**
	 * @return the autoPosition
	 */
	public boolean isAutoPosition() {
		return autoPosition;
	}

	/**
	 * @param autoPosition the autoPosition to set
	 */
	public void setAutoPosition(boolean autoPosition) {
		this.autoPosition = autoPosition;
	}

	/**
	 * @return the autoRotation
	 */
	public boolean isAutoRotation() {
		return autoRotation;
	}

	/**
	 * @param autoRotation the autoRotation to set
	 */
	public void setAutoRotation(boolean autoRotation) {
		this.autoRotation = autoRotation;
	}

	/**
	 * @return the fontAngle
	 */
	public double getFontAngle() {
		return fontAngle;
	}

	/**
	 * @param fontAngle the fontAngle to set
	 */
	public void setFontAngle(double fontAngle) {
		this.fontAngle = fontAngle;
	}

	/**
	 * @return the position
	 */
	public double[] getPosition() {
		return position;
	}

	/**
	 * @param position the position to set
	 */
	public void setPosition(double[] position) {
		this.position = position;
	}

	//	 Methods: to be done
}
