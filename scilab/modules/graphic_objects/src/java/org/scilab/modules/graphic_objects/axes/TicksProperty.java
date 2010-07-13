/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.axes;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.textObject.FormattedText;

/**
 * TicksProperty class
 * @author Manuel JULIACHS
 */
public class TicksProperty {
	/** TicksProperty properties names */
	public enum TicksPropertyProperty { AUTO, LOCATIONS, LABELS, SUBTICKS };

	/** Default number of ticks */
	private static final int DEFAULT_NUMBER_OF_TICKS = 20;

	/** Specifies whether ticks are automatically computed or not */
	private boolean auto;

	/** Ticks locations along their axis */
	private double[] locations;

	/** Ticks labels */
	private ArrayList <FormattedText> labels;

	/** Number of subticks between two main ticks */
	private int subticks;

	/** Constructor */
	public TicksProperty() {
		auto = false;
		locations = new double[DEFAULT_NUMBER_OF_TICKS];
		labels = new ArrayList<FormattedText>(DEFAULT_NUMBER_OF_TICKS);

		for (int i = 0; i < DEFAULT_NUMBER_OF_TICKS; i++) {
			labels.add(new FormattedText());
		}

		subticks = 0;
	}

	/**
	 * @return the auto
	 */
	public Boolean getAuto() {
		return auto;
	}

	/**
	 * @param auto the auto to set
	 */
	public void setAuto(Boolean auto) {
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
		if (!this.labels.isEmpty()) {
			this.labels.clear();
		}

		for (int i = 0; i < labels.size(); i++) {
			this.labels.add(i, new FormattedText(labels.get(i)));
		}
	}

	/**
	 * @return the labels strings
	 */
	public String[] getLabelsStrings() {
		String[] labelsStrings = new String[labels.size()];

		for (int i = 0; i < labels.size(); i++) {
			labelsStrings[i] = new String(labels.get(i).getText());
		}

		return labelsStrings;
	}

	/**
	 * @param labels the labels to set
	 */
	public void setLabelsStrings(String[] labels) {
		for (int i = 0; i < labels.length; i++) {
			this.labels.get(i).setText(labels[i]);
		}
	}

	/**
	 * @return the number of ticks
	 */
	public Integer getNumber() {
		return locations.length;
	}

	/**
	 * @return the locations
	 */
	public Double[] getLocations() {
		Double[] retLocations = new Double[locations.length];
		for (int i = 0; i < locations.length; i++) {
			retLocations[i] = locations[i];
		}

		return retLocations;
	}

	/**
	 * @param locations the locations to set
	 */
	public void setLocations(Double[] locations) {
		for (int i = 0; i < locations.length; i++) {
			this.locations[i] = locations[i];
		}
	}

	/**
	 * @return the subticks
	 */
	public Integer getSubticks() {
		return subticks;
	}

	/**
	 * @param subticks the subticks to set
	 */
	public void setSubticks(Integer subticks) {
		this.subticks = subticks;
	}

}
