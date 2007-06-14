/*------------------------------------------------------------------------*/
/* file: MarkDrawingStrategy.java                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing one kind of mark                   */
/*------------------------------------------------------------------------*/


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
	public static enum MarkStyle { DOT,
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
								   PENTAGRAM };
			
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
	 * Default constructor
	 */
	public MarkDrawingStrategy() { }
	
	/**
	 * Create a new instance of markdrawing strategy given its markStyle
	 * @param markStyleIndex index of the kinf of mark
	 * @return new instance of MarkDrawingStrategy
	 */
	public static MarkDrawingStrategy create(int markStyleIndex) {
		if (markStyleIndex < 0 || markStyleIndex >= MARK_STYLE.length) {
			return null;
		}
		
		switch(getMarkStyle(markStyleIndex)) {
		case DOT:
			return new DotMarkDrawer();
		default:
			return null;
		}
	}
	
	/**
	 * @param markStyleIndex index of mark style
	 * @return kind of mark
	 */
	private static MarkStyle getMarkStyle(int markStyleIndex) {
		return MARK_STYLE[markStyleIndex];
	}
	
	/**
	 * Draw a dot
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public abstract void drawMark(GL gl, double[] backColor, double[] frontColor);
	
}
