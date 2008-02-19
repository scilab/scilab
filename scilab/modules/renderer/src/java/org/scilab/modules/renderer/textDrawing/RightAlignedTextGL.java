/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class which draw left aligned text
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.textDrawing;


import org.scilab.modules.renderer.utils.geom3D.Vector3D;


/**
 * Class which draw left aligned text
 * @author Jean-Baptiste Silvy
 */
public class RightAlignedTextGL implements TextAlignementStrategy {

	/**
	 * Default constructor
	 */
	public RightAlignedTextGL() {
		
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
				double xCoord = curCell[2].getX() - TextGrid.EXTEND_FACTOR_X / 2.0 - text.getStringWidth(i, j);
				double yCoord = curCell[1].getY() - (curCell[1].getY() - curCell[0].getY()) * TextGrid.EXTEND_FACTOR_Y / 2.0;
				renderer.draw3D(text.getMatrixElement(i, j), xCoord,
						        yCoord, curCell[1].getZ());
			}
		}
		
		renderer.end3DRendering();

	}

}
