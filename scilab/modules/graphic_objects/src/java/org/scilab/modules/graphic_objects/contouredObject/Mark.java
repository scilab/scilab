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

package org.scilab.modules.graphic_objects.contouredObject;

/**
 * Mark class
 * @author Manuel JULIACHS
 */
public class Mark {
	/** Mark properties */
	public enum MarkPropertyType { MODE, STYLE, MARKSIZEUNIT, FOREGROUND, BACKGROUND };

	/** Mark size unit type */
	public enum MarkSizeUnitType { POINT, TABULATED };

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
		markSizeUnit = MarkSizeUnitType.POINT;
		foreground = 0;
		background = 0;
	}

	/**
	 * @return the background
	 */
	public Integer getBackground() {
		return background;
	}

	/**
	 * @param background the background to set
	 */
	public void setBackground(Integer background) {
		this.background = background;
	}

	/**
	 * @return the foreground
	 */
	public Integer getForeground() {
		return foreground;
	}

	/**
	 * @param foreground the foreground to set
	 */
	public void setForeground(Integer foreground) {
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
	public Boolean getMode() {
		return mode;
	}

	/**
	 * @param mode the mode to set
	 */
	public void setMode(Boolean mode) {
		this.mode = mode;
	}

	/**
	 * @return the style
	 */
	public Integer getStyle() {
		return style;
	}

	/**
	 * @param style the style to set
	 */
	public void setStyle(Integer style) {
		this.style = style;
	}
	
}
