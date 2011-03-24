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
	public enum LineType { SOLID, STYLE1, STYLE2, STYLE3, STYLE4, STYLE5, STYLE6, STYLE7;

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
					return STYLE1;
				case 3:
					return STYLE2;
				case 4:
					return STYLE3;
				case 5:
					return STYLE4;
				case 6:
					return STYLE5;
				case 7:
					return STYLE6;
				case 8:
					return STYLE7;
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
                case STYLE1:
                    return (short) 0x07FF; // 5 blanks, 11 solids
                case STYLE2:
                    return (short) 0x0F0F; // 4 blanks, 4 solids, 4 blanks, 4 solids
                case STYLE3:
                    return (short) 0x1FC2; // 3 blanks, 3 solids, 3 blanks, 7 solids
                case STYLE4:
                    return (short) 0x3FC9; // 2 blanks, 8 solids, 2 blanks, 1 solid, 2 blanks, 1 solid
                case STYLE5:
                    return (short) 0x3FC6; // 3 blanks, 8 solids, 3 blanks, 2 solids
                case STYLE6:
                    return (short) 0x5555; // (1 blank, 1 solid) x 8
		case STYLE7:
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
		thickness = 0.0;
		color = 0;
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
