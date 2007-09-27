/*------------------------------------------------------------------------*/
/* file: LeftAlignedTextGL.java                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class which draw left aligned text                              */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.textDrawing;


import org.scilab.modules.renderer.utils.geom3D.Vector3D;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * Class which draw left aligned text
 * @author Jean-Baptiste Silvy
 */
public class LeftAlignedTextGL implements TextAlignementStrategy {

	/**
	 * Default constructor
	 */
	public LeftAlignedTextGL() {
		
	}
	
	/**
	 * Draw the text matrix inside a position matrix using a specific text renderer.
	 * @param renderer renderer to use for drawing
	 * @param text matrix of strings
	 * @param positionMatrix matrix of positions.
	 */
	public void drawTextContent(TextRenderer renderer, StringMatrixGL text, TextGrid positionMatrix) {
		renderer.begin3DRendering();
		renderer.setColor(0.0f, 0.0f, 0.0f, 0.0f);
		for (int i = 0; i < text.getNbRow(); i++) {
			for (int j = 0; j < text.getNbCol(); j++) {
				Vector3D[] curCell = positionMatrix.getCellCoordinates(i, j);
				renderer.draw3D(text.getMatrixElement(i, j), curCell[0].getXf(),
						        curCell[0].getYf(), curCell[0].getZf(), 1.0f);
			}
		}
		renderer.end3DRendering();

	}

}
