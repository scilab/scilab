package org.scilab.modules.graphic_objects;

/**
 * Contoured object class 
 * @author juliachs
 */
public abstract class ContouredObject extends GraphicObject {
	/** Line property */
	private Line line;

	/** Specifies whether the object is filled or not */
	private boolean fillMode;

	/** Background (fill) color */
	private int background;

	/** Mark property */
	private Mark mark;

	/** Default constructor */
	public ContouredObject() {
		line = null;
		fillMode = false;
		background = 0;
		mark = null;
	}
	
	/**
	 * @return the background
	 */
	public int getBackground() {
		return background;
	}

	/**
	 * @param background the background to set
	 */
	public void setBackground(int background) {
		this.background = background;
	}

	/**
	 * @return the fillMode
	 */
	public boolean isFillMode() {
		return fillMode;
	}

	/**
	 * @param fillMode the fillMode to set
	 */
	public void setFillMode(boolean fillMode) {
		this.fillMode = fillMode;
	}

	/**
	 * @return the line
	 */
	public Line getLine() {
		return line;
	}

	/**
	 * @param line the line to set
	 */
	public void setLine(Line line) {
		this.line = line;
	}

	/**
	 * @return the mark
	 */
	public Mark getMark() {
		return mark;
	}

	/**
	 * @param mark the mark to set
	 */
	public void setMark(Mark mark) {
		this.mark = mark;
	}

	// Methods: to be done
}
