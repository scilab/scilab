/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing one kind of mark
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.GL;

/**
 * Class specialized in drawing one kind of mark
 * @author Jean-Baptiste Silvy
 */
public abstract class MarkDrawingStrategy {

	/**
	 * Different types of marks
	 */
	public static enum MarkStyle {DOT,
		PLUS,
		CROSS,
		STAR,
		FILLED_DIAMOND,
		DIAMOND,
		TRIANGLE_UP,
		TRIANGLE_DOWN,
		DIAMOND_PLUS,
		CIRCLE,
		ASTERISK,
		SQUARE,
		TRIANGLE_RIGHT,
		TRIANGLE_LEFT,
		PENTAGRAM,
		UNDEFINED};
    
        /**
	 * Mathching between indices and mark styles. 
	 */
	private static final MarkStyle[] MARK_STYLE = {MarkStyle.DOT,
						       MarkStyle.PLUS,
						       MarkStyle.CROSS,
						       MarkStyle.STAR,
						       MarkStyle.FILLED_DIAMOND,
						       MarkStyle.DIAMOND,
						       MarkStyle.TRIANGLE_UP,
						       MarkStyle.TRIANGLE_DOWN,
						       MarkStyle.DIAMOND_PLUS,
						       MarkStyle.CIRCLE,
						       MarkStyle.ASTERISK,
						       MarkStyle.SQUARE,
						       MarkStyle.TRIANGLE_RIGHT,
						       MarkStyle.TRIANGLE_LEFT,
						       MarkStyle.PENTAGRAM };
 
        /**
	 * Transparency variables
	 */
        protected boolean isFrontTransparent = false;
        protected boolean isBackTransparent = false;

        /**
	 * Set front transparency methode
	 * @param value is the new value of isFrontTransparent
	 */
        public void setFrontTransparency(boolean value) {
	        isFrontTransparent = value;
	}

        /**
	 * Set back transparency methode
	 * @param value is the new value of isBackTransparent
	 */
        public void setBackTransparency(boolean value) {
	        isBackTransparent = value;
	}

	/**
	 * Create a new instance of markdrawing strategy given its markStyle
	 * @param markStyleIndex index of the kinf of mark
	 * @return new instance of MarkDrawingStrategy
	 */
        public abstract MarkDrawingStrategy create(int markStyleIndex);

	/**
	 * @param markStyleIndex index of mark style
	 * @return kind of mark
	 */
	public static MarkStyle getMarkStyle(int markStyleIndex) {
		if (markStyleIndex < 0 || markStyleIndex >= MARK_STYLE.length) {
			return MarkStyle.UNDEFINED;
		}
		return MARK_STYLE[markStyleIndex];
	}
	
	/**
	 * Draw a single mark.
	 * Be sure that background is drawn before the foreground if any.
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public abstract void drawMark(GL gl, double[] backColor, double[] frontColor);	
}
