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
								   PENTAGRAM,
								   UNDEFINED };
			
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
  public void setFrontTransparency(boolean value)
  {
    isFrontTransparent = value;
  }

  /**
   * Set back transparency methode
   * @param value is the new value of isBackTransparent
   */
  public void setBackTransparency(boolean value)
  {
    isBackTransparent = value;
  }

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
		MarkDrawingStrategy res;
		switch(getMarkStyle(markStyleIndex)) {
		case DOT:
			res = new DotMarkDrawer();
			break;
		case PLUS:
			res = new PlusMarkDrawer();
			break;
		case CROSS:
			res = new CrossMarkDrawer();
			break;
		case STAR:
			res = new StarMarkDrawer();
			break;
		case FILLED_DIAMOND:
			res = new FilledDiamondMarkDrawer();
			break;
		case DIAMOND:
			res = new DiamondMarkDrawer();
			break;
		case TRIANGLE_UP:
			res = new TriangleMarkDrawer();
			break;
		case TRIANGLE_DOWN:
			res = new TriangleDownMarkDrawer();
			break;
		case DIAMOND_PLUS:
			res = new DiamondPlusMarkDrawer();
			break;
		case CIRCLE:
			res = new CircleMarkDrawer();
			break;
		case ASTERISK:
			res = new AsteriskMarkDrawer();
			break;
		case SQUARE:
			res = new SquareMarkDrawer();
			break;
		case TRIANGLE_RIGHT:
			res = new TriangleRightMarkDrawer();
			break;
		case TRIANGLE_LEFT:
			res = new TriangleLeftMarkDrawer();
			break;
		case PENTAGRAM:
			res = new PentagramMarkDrawer();
			break;
		default:
			res = null;
			break;
		}
		return res;
	}
	
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
