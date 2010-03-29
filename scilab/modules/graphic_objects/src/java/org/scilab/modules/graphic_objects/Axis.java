package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * Axis class
 * @author juliachs
 */
public class Axis extends ClippableContouredObject {
	/** Ticks direction */
	private enum TicksDirection { TOP, BOTTOM, LEFT, RIGHT };
	
	/** Ticks direction */
	private TicksDirection ticksDirection;

	/** Ticks x-coordinate position vector */
	private double [] xTicksCoords; 

	/** Ticks y-coordinate position vector */
	private double [] yTicksCoords;
	
	/** Ticks color */
	private int ticksColor;

	/** Specifies whether the axis segment is drawn */
	private boolean ticksSegment;

	/** Ticks labels list */
	private ArrayList <String> ticksLabels;

	/** Label format */
	private String formatn;

	/** Font */
	private Font font;

	/** Constructor */
	public Axis() {
		super();
		ticksDirection = TicksDirection.TOP;
		xTicksCoords = null;
		yTicksCoords = null;
		ticksColor = 0;
		ticksSegment = false;
		ticksLabels = null;
		formatn = null;
		font = null;
	}
	
	/**
	 * @return the font
	 */
	public Font getFont() {
		return font;
	}

	/**
	 * @param font the font to set
	 */
	public void setFont(Font font) {
		this.font = font;
	}

	/**
	 * @return the formatn
	 */
	public String getFormatn() {
		return formatn;
	}

	/**
	 * @param formatn the formatn to set
	 */
	public void setFormatn(String formatn) {
		this.formatn = formatn;
	}

	/**
	 * @return the ticksColor
	 */
	public int getTicksColor() {
		return ticksColor;
	}

	/**
	 * @param ticksColor the ticksColor to set
	 */
	public void setTicksColor(int ticksColor) {
		this.ticksColor = ticksColor;
	}

	/**
	 * @return the ticksDirection
	 */
	public TicksDirection getTicksDirection() {
		return ticksDirection;
	}

	/**
	 * @param ticksDirection the ticksDirection to set
	 */
	public void setTicksDirection(TicksDirection ticksDirection) {
		this.ticksDirection = ticksDirection;
	}

	/**
	 * @return the ticksLabels
	 */
	public ArrayList<String> getTicksLabels() {
		return ticksLabels;
	}

	/**
	 * @param ticksLabels the ticksLabels to set
	 */
	public void setTicksLabels(ArrayList<String> ticksLabels) {
		this.ticksLabels = ticksLabels;
	}

	/**
	 * @return the ticksSegment
	 */
	public boolean isTicksSegment() {
		return ticksSegment;
	}

	/**
	 * @param ticksSegment the ticksSegment to set
	 */
	public void setTicksSegment(boolean ticksSegment) {
		this.ticksSegment = ticksSegment;
	}

	/**
	 * @return the xTicksCoords
	 */
	public double[] getXTicksCoords() {
		return xTicksCoords;
	}

	/**
	 * @param ticksCoords the xTicksCoords to set
	 */
	public void setXTicksCoords(double[] ticksCoords) {
		xTicksCoords = ticksCoords;
	}

	/**
	 * @return the yTicksCoords
	 */
	public double[] getYTicksCoords() {
		return yTicksCoords;
	}

	/**
	 * @param ticksCoords the yTicksCoords to set
	 */
	public void setYTicksCoords(double[] ticksCoords) {
		yTicksCoords = ticksCoords;
	}
	
	//	 Methods: to be done
}
