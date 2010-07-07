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
 * Line class
 * @author Manuel JULIACHS
 */
public class Line {
	/** Line properties */
	public enum LinePropertyType { MODE, LINESTYLE, THICKNESS, COLOR };
	
	/** Line style */
	enum LineType { SOLID, STYLE1, STYLE2, STYLE3, STYLE4, STYLE5, STYLE6;

		/**
		 * Converts an integer to the corresponding enum
		 * @param intValue the integer value
		 * @return the line type enum
		 */
		public static LineType intToEnum(Integer intValue) {
			switch (intValue) {
				case 0:
					return LineType.SOLID;
				case 1:
					return LineType.STYLE1;
				case 2:
					return LineType.STYLE2;
				case 3:
					return LineType.STYLE3;
				case 4:
					return LineType.STYLE4;
				case 5:
					return LineType.STYLE5;
				case 6:
					return LineType.STYLE6;
				default:
					return null;
			}
		}
	}
	
	/** Specifies whether the line is drawn or not */
	private boolean mode;

	/** Line style */
	private LineType lineStyle;

	/** Line thickness */
	private double thickness;

	/** Line color */
	private int color;

	/** Constructor */
	public Line() {
		mode = false;
		lineStyle = LineType.SOLID;
		thickness = 0.0;
		color = 0;
	}

	/**
	 * @return the color
	 */
	public Integer getColor() {
		return color;
	}

	/**
	 * @param color the color to set
	 */
	public void setColor(Integer color) {
		this.color = color;
	}

	/**
	 * @return the lineStyle
	 */
	public LineType getLineStyle() {
		return lineStyle;
	}

	/**
	 * @param lineStyle the lineStyle to set
	 */
	public void setLineStyle(LineType lineStyle) {
		this.lineStyle = lineStyle;
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
	 * @return the thickness
	 */
	public Double getThickness() {
		return thickness;
	}

	/**
	 * @param thickness the thickness to set
	 */
	public void setThickness(Double thickness) {
		this.thickness = thickness;
	}

}
