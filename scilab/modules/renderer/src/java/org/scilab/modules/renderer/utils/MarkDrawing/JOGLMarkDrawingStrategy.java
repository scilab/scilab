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
public abstract class JOGLMarkDrawingStrategy extends MarkDrawingStrategy {

	/**
	 * Default constructor
	 */
	public JOGLMarkDrawingStrategy() { }
	
	/**
	 * Create a new instance of markdrawing strategy given its markStyle
	 * @param markStyleIndex index of the kinf of mark
	 * @return new instance of MarkDrawingStrategy
	 */
	public MarkDrawingStrategy create(int markStyleIndex) {
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
}
