package org.scilab.modules.graphic_objects;

/**
 * Mark class
 * @author juliachs
 */
public class Mark {
	/** Mark size unit type */
	private enum MarkSizeUnitType { POINT, TABULATED };

	/** Specifies whether marks must be drawn or not */
	private boolean mode;

	/** Mark style */
	private int style;

	/** Mark size unit */
	private MarkSizeUnitType markSizeUnit;

	/** Foreground color */
	private int foreground;

	/** Background color */
	private int background;

	/** Constructor */
	public Mark() {
		super();
		mode = false;
		style = 0;
		markSizeUnit = null;
		foreground = 0;
		background = 0;
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
	 * @return the foreground
	 */
	public int getForeground() {
		return foreground;
	}

	/**
	 * @param foreground the foreground to set
	 */
	public void setForeground(int foreground) {
		this.foreground = foreground;
	}

	/**
	 * @return the markSizeUnit
	 */
	public MarkSizeUnitType getMarkSizeUnit() {
		return markSizeUnit;
	}

	/**
	 * @param markSizeUnit the markSizeUnit to set
	 */
	public void setMarkSizeUnit(MarkSizeUnitType markSizeUnit) {
		this.markSizeUnit = markSizeUnit;
	}

	/**
	 * @return the mode
	 */
	public boolean isMode() {
		return mode;
	}

	/**
	 * @param mode the mode to set
	 */
	public void setMode(boolean mode) {
		this.mode = mode;
	}

	/**
	 * @return the style
	 */
	public int getStyle() {
		return style;
	}

	/**
	 * @param style the style to set
	 */
	public void setStyle(int style) {
		this.style = style;
	}
	
	//	 Methods: to be done
}
