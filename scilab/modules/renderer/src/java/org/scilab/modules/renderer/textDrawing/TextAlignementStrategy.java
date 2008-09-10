/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Interface for drawing text inside a position matrix with different alignment
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.textDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;


/**
 * Interface for drawing text inside a position matrix with different alignment.
 * @author Jean-Baptiste Silvy
 */
public abstract class TextAlignementStrategy {

	/** left aligned text */
	public static final int LEFT_ALIGNED_INDEX = 1;
	/** centered aligned text */
	public static final int CENTERED_ALIGNED_INDEX = 2;
	/** right aligned text */
	public static final int RIGHT_ALIGNED_INDEX = 3;
	
	/** Offset used to center text in a pixel */
	private static final double DISPLACEMENT_OFFSET = 1.0;
	
	/**
	 * Draw the text matrix inside a position matrix using a specific text renderer.
	 * @param gl current GL pipeline
	 * @param renderer renderer to use for drawing
	 * @param text matrix of strings
	 * @param positionMatrix matrix of positions.
	 * @param angle angle of the text to draw
	 */
	public abstract void drawTextContent(GL gl, SciTextRenderer renderer, StringMatrixGL text, TextGrid positionMatrix, double angle);
	
	
	/**
	 * Get the width of the bounding box
	 * @param textBox 4 corners of the bounding box
	 * @return width of the bounding box
	 */
	protected double getBoxWidth(Vector3D[] textBox) {
		return (textBox[2].getX() - textBox[0].getX());
	}
	
	/**
	 * Get the height of the bounding box
	 * @param textBox 4 corners of the bounding box
	 * @return width of the bounding box
	 */
	protected double getBoxHeight(Vector3D[] textBox) {
		return (textBox[0].getY() - textBox[1].getY());
	}
	
	/**
	 * @param xCoord value to center
	 * @return centered value
	 */
	protected double centerX(double xCoord) {
		return Math.round(xCoord) + DISPLACEMENT_OFFSET;
	}
	
	/**
	 * The Y coordinate is the same for all texts
	 * @param textBox 4 corners of the text cell
	 * @return Y coocdinate of the text to draw
	 */
	protected double getYCoordinate(Vector3D[] textBox) {
		return Math.round(textBox[1].getY() + getBoxHeight(textBox) * TextGrid.EXTEND_FACTOR_Y / 2.0) + DISPLACEMENT_OFFSET;
	}
	
}
