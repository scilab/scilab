package org.scilab.modules.graphic_objects;

/**
 * Axis property class
 * @author juliachs
 */
public class AxisProperty {
	/**
	 * Axis location type
	 * BOTTOM, TOP, MIDDLE, ORIGIN are allowed for an x-axis,
	 * LEFT, RIGHT, MIDDLE or ORIGIN are allowed for a y-axis
	 */
	private static enum AxisLocation { BOTTOM, TOP, MIDDLE, ORIGIN, LEFT, RIGHT };
	
	/** Specifies whether the axis is visible or not */
	private boolean visible;
	
	/** Specifies whether the axis is reversed or not */
	private boolean reverse;
	
	/** Grid color */
	private int gridColor;

	/** Axis label */
	private Label label;

	/** Axis location */
	private AxisLocation axisLocation;

	/** Ticks property */
	private TicksProperty ticks;

	/** Specifies whether logarithmic coordinates are used or not */
	private boolean logFlag;

	/** Constructor */
	public AxisProperty() {
		visible = false;
		reverse = false;
		gridColor = 0;
		label = null;
		axisLocation = null;
		ticks = null;
		logFlag = false;
	}

	/**
	 * @return the axisLocation
	 */
	public AxisLocation getAxisLocation() {
		return axisLocation;
	}

	/**
	 * @param axisLocation the axisLocation to set
	 */
	public void setAxisLocation(AxisLocation axisLocation) {
		this.axisLocation = axisLocation;
	}

	/**
	 * @return the gridColor
	 */
	public int getGridColor() {
		return gridColor;
	}

	/**
	 * @param gridColor the gridColor to set
	 */
	public void setGridColor(int gridColor) {
		this.gridColor = gridColor;
	}

	/**
	 * @return the label
	 */
	public Label getLabel() {
		return label;
	}

	/**
	 * @param label the label to set
	 */
	public void setLabel(Label label) {
		this.label = label;
	}

	/**
	 * @return the logFlag
	 */
	public boolean isLogFlag() {
		return logFlag;
	}

	/**
	 * @param logFlag the logFlag to set
	 */
	public void setLogFlag(boolean logFlag) {
		this.logFlag = logFlag;
	}

	/**
	 * @return the reverse
	 */
	public boolean isReverse() {
		return reverse;
	}

	/**
	 * @param reverse the reverse to set
	 */
	public void setReverse(boolean reverse) {
		this.reverse = reverse;
	}

	/**
	 * @return the ticks
	 */
	public TicksProperty getTicks() {
		return ticks;
	}

	/**
	 * @param ticks the ticks to set
	 */
	public void setTicks(TicksProperty ticks) {
		this.ticks = ticks;
	}

	/**
	 * @return the visible
	 */
	public boolean isVisible() {
		return visible;
	}

	/**
	 * @param visible the visible to set
	 */
	public void setVisible(boolean visible) {
		this.visible = visible;
	}
	
	//	 Methods: to be done
}
