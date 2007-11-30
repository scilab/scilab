/*------------------------------------------------------------------------*/
/* file: LeftAlignedTextGL.java                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class which draw left aligned text                              */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.textDrawing;


import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class which draw left aligned text
 * @author Jean-Baptiste Silvy
 */
public class LeftAlignedTextGL implements TextAlignementStrategy {

	/**
	 * Default constructor
	 */
	public LeftAlignedTextGL() { }
	
	/**
	 * Draw the text matrix inside a position matrix using a specific text renderer.
	 * @param renderer renderer to use for drawing
	 * @param text matrix of strings
	 * @param positionMatrix matrix of positions.
	 */
	public void drawTextContent(SciTextRenderer renderer, StringMatrixGL text, TextGrid positionMatrix) {
		renderer.begin3DRendering();
		
		for (int i = 0; i < text.getNbRow(); i++) {
			for (int j = 0; j < text.getNbCol(); j++) {
				Vector3D[] curCell = positionMatrix.getCellCoordinates(i, j);
				double xCoord = curCell[1].getX();
				double yCoord = curCell[1].getY() - (curCell[1].getY() - curCell[0].getY()) * TextGrid.EXTEND_FACTOR_Y / 2.0;
				renderer.draw3D(text.getMatrixElement(i, j), xCoord,
						        yCoord, curCell[1].getZ());
			}
		}
		
		renderer.end3DRendering();

	}

}
