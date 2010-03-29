package org.scilab.modules.graphic_objects;

import java.util.ArrayList;

/**
 * TicksProperty class
 * @author juliachs
 */
public class TicksProperty {
	/** Specifies whether ticks are automatically computed or not */
	private boolean auto;

	/** Ticks locations along their axis */
	private double [] locations;

	/** Ticks labels */
	private ArrayList <FormattedText> labels;

	/** Number of subticks between two main ticks */
	private int subticks;

	/** Constructor */
	public TicksProperty() {
		auto = false;
		locations = null;
		labels = null;
		subticks = 0;
	}

	/**
	 * @return the auto
	 */
	public boolean isAuto() {
		return auto;
	}

	/**
	 * @param auto the auto to set
	 */
	public void setAuto(boolean auto) {
		this.auto = auto;
	}

	/**
	 * @return the labels
	 */
	public ArrayList<FormattedText> getLabels() {
		return labels;
	}

	/**
	 * @param labels the labels to set
	 */
	public void setLabels(ArrayList<FormattedText> labels) {
		this.labels = labels;
	}

	/**
	 * @return the locations
	 */
	public double[] getLocations() {
		return locations;
	}

	/**
	 * @param locations the locations to set
	 */
	public void setLocations(double[] locations) {
		this.locations = locations;
	}

	/**
	 * @return the subticks
	 */
	public int getSubticks() {
		return subticks;
	}

	/**
	 * @param subticks the subticks to set
	 */
	public void setSubticks(int subticks) {
		this.subticks = subticks;
	}

	//	 Methods: to be done
}
