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
	public enum TicksPropertyProperty { AUTO, LOCATIONS, LABELS,
		FONT_SIZE, FONT_STYLE, FONT_COLOR, SUBTICKS };

	/** Default number of ticks */
	private static final int DEFAULT_NUMBER_OF_TICKS = 11;

	/** Specifies whether ticks are automatically computed or not */
	private boolean auto;

	/** Ticks locations along their axis */
	private double[] locations;

	/** Ticks labels */
	private ArrayList <FormattedText> labels;

	/** Number of ticks */
	private int number;

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

		number = DEFAULT_NUMBER_OF_TICKS;
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
		String[] labelsStrings = new String[number];

		for (int i = 0; i < number; i++) {
			labelsStrings[i] = new String(labels.get(i).getText());
		}

		return labelsStrings;
	}

	/**
	 * Sets the ticks labels strings
	 * Requires the corresponding ticks locations to have previously been set
	 * @param labels the labels to set
	 */
	public void setLabelsStrings(String[] labels) {
		if (labels.length != number) {
			return;
		}

		for (int i = 0; i < number; i++) {
			this.labels.get(i).setText(labels[i]);
		}
	}

	/**
	 * @return the number of ticks
	 */
	public Integer getNumber() {
		return number;
	}

	/**
	 * @return the locations
	 */
	public Double[] getLocations() {
		Double[] retLocations = new Double[number];
		for (int i = 0; i < number; i++) {
			retLocations[i] = locations[i];
		}

		return retLocations;
	}

	/**
	 * Sets the ticks locations
	 * Also sets the current number of ticks to the size of the locations array
	 * @param locations the locations to set
	 */
	public void setLocations(Double[] locations) {
		if (locations.length > DEFAULT_NUMBER_OF_TICKS) {
			return;
		}

		for (int i = 0; i < locations.length; i++) {
			this.locations[i] = locations[i];
		}

		number = locations.length;
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

	/**
	 * Supposes all ticks labels have the same font style.
	 * To be corrected.
	 * @return the ticks labels font style
	 */
	public Integer getFontStyle() {
		return labels.get(0).getFont().getStyle();
	}

	/**
	 * Supposes all ticks labels have the same font style.
	 * To be corrected.
	 * @param fontStyle the ticks labels font style to set
	 */
	public void setFontStyle(Integer fontStyle) {
		for (int i = 0; i < labels.size(); i++) {
			labels.get(i).getFont().setStyle(fontStyle);
		}
	}

	/**
	 * Supposes all ticks labels have the same font size.
	 * To be corrected.
	 * @return the ticks labels font size
	 */
	public Double getFontSize() {
		return labels.get(0).getFont().getSize();
	}

	/**
	 * Supposes all ticks labels have the same font size.
	 * To be corrected.
	 * @param fontSize the ticks labels font size to set
	 */
	public void setFontSize(Double fontSize) {
		for (int i = 0; i < labels.size(); i++) {
			labels.get(i).getFont().setSize(fontSize);
		}
	}

	/**
	 * Supposes all ticks labels have the same font color.
	 * To be corrected.
	 * @return the ticks labels font color
	 */
	public Integer getFontColor() {
		return labels.get(0).getFont().getColor();
	}

	/**
	 * Supposes all ticks labels have the same font color.
	 * To be corrected.
	 * @param fontColor the ticks labels font color to set
	 */
	public void setFontColor(Integer fontColor) {
		for (int i = 0; i < labels.size(); i++) {
			labels.get(i).getFont().setColor(fontColor);
		}
	}

	/**
	 * Supposes all ticks labels have the same font fractional.
	 * To be corrected.
	 * @return the ticks labels font fractional
	 */
	public Boolean getFontFractional() {
		return labels.get(0).getFont().getFractional();
	}

	/**
	 * Supposes all ticks labels have the same font fractional.
	 * To be corrected.
	 * @param fontFractional the ticks labels font fractional to set
	 */
	public void setFontFractional(Boolean fontFractional) {
		for (int i = 0; i < labels.size(); i++) {
			labels.get(i).getFont().setFractional(fontFractional);
		}
	}

}
