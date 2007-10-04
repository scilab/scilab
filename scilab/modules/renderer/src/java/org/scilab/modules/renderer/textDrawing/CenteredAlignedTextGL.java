/*------------------------------------------------------------------------*/
/* file: CenteredAlignedTextGL.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class which draw centered text                                  */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.textDrawing;


import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class which draw centered text.
 * @author Jean-Baptiste Silvy
 */
public class CenteredAlignedTextGL implements TextAlignementStrategy {

	/**
	 * Default constructor
	 */
	public CenteredAlignedTextGL() {
		
	}
	
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
				// put the middle of the string in the middle of the cell
				double xCoord =   curCell[1].getX()
				               + ((curCell[2].getX() - curCell[1].getX()) - text.getStringWidth(i, j)) / 2.0;
				double yCoord = curCell[1].getY() - (curCell[1].getY() - curCell[0].getY()) * TextGrid.EXTEND_FACTOR_Y / 2.0;
				renderer.draw3D(text.getMatrixElement(i, j), xCoord,
						        yCoord, curCell[1].getZ());
			}
		}
		
		renderer.end3DRendering();

	}

}
