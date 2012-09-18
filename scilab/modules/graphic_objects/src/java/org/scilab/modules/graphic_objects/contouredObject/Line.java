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

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Line class
 * @author Manuel JULIACHS
 */
public class Line {
	/** Line properties */
	public enum LinePropertyType { MODE, LINESTYLE, THICKNESS, COLOR };
	
	/** Line style */
	public enum LineType { SOLID, DASH, DASH_DOT, LONG_DASH_DOT, BIG_DASH_DOT, BIG_DASH_LONG_DASH, DOT, DOUBLE_DOT;

		/**
		 * Converts a scilab line style index to the corresponding line type.
		 * @param sciIndex the scilab index.
		 * @return the line type as enum.
		 */
		public static LineType fromScilabIndex(Integer sciIndex) {
			switch (sciIndex) {
				case 1:
					return SOLID;
				case 2:
					return DASH;
				case 3:
					return DASH_DOT;
				case 4:
					return LONG_DASH_DOT;
				case 5:
					return BIG_DASH_DOT;
				case 6:
					return BIG_DASH_LONG_DASH;
				case 7:
					return DOT;
				case 8:
					return DOUBLE_DOT;
				default:
					return SOLID;
			}
		}

        /**
         * Converts the line type to the corresponding scilab line style index.
         * @return  the scilab line style index corresponding to this line type.
         */
        public int asScilabIndex() {
            return ordinal() + 1;
        }

        /**
         * Converts the line type to a 16-bit pattern.
         * @return the 16-bit pattern corresponding to the line type.
         */
        public short asPattern() {
            switch (this) {
                case DASH:
                    return (short) 0x07FF; // 5 blanks, 11 solids
                case DASH_DOT:
                    return (short) 0x0F0F; // 4 blanks, 4 solids, 4 blanks, 4 solids
                case LONG_DASH_DOT:
                    return (short) 0x1FC2; // 3 blanks, 3 solids, 3 blanks, 7 solids
                case BIG_DASH_DOT:
                    return (short) 0x3FC9; // 2 blanks, 8 solids, 2 blanks, 1 solid, 2 blanks, 1 solid
                case BIG_DASH_LONG_DASH:
                    return (short) 0x3FC6; // 3 blanks, 8 solids, 3 blanks, 2 solids
                case DOT:
                    return (short) 0x5555; // (1 blank, 1 solid) x 8
                case DOUBLE_DOT:
                    return (short) 0x3333; // (2 blanks, 2 solids) x 4
                default:
                case SOLID:
                    return (short) 0xFFFF; // 16 solids, unused equivalent to no stipple
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
		thickness = 1.0;
		color = -1;
	}

	/**
         * Copy constructor
	 * @param line the Line to copy
	 */
	public Line(Line line) {
		mode = line.mode;
		lineStyle = line.lineStyle;
		thickness = line.thickness;
		color = line.color;
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
	public UpdateStatus setColor(Integer color) {
	    if (this.color == color) {
	        return UpdateStatus.NoChange;
	    }
	    this.color = color;
	    return UpdateStatus.Success;
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
	public UpdateStatus setMode(Boolean mode) {
		if(this.mode == mode) {
		    return UpdateStatus.NoChange;
		}
		this.mode = mode;
		return UpdateStatus.Success;
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
