/*------------------------------------------------------------------------*/
/* file: TextAlignmentStrategy.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for drawing text inside a position matrix with        */
/*        different alignment                                             */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.textDrawing;


/**
 * Interface for drawing text inside a position matrix with different alignment.
 * @author Jean-Baptiste Silvy
 */
public interface TextAlignementStrategy {

	/** left aligned text */
	int LEFT_ALIGNED_INDEX = 1;
	/** centered aligned text */
	int CENTERED_ALIGNED_INDEX = 2;
	/** right aligned text */
	int RIGHT_ALIGNED_INDEX = 3;
	
	/**
	 * Draw the text matrix inside a position matrix using a specific text renderer.
	 * @param renderer renderer to use for drawing
	 * @param text matrix of strings
	 * @param positionMatrix matrix of positions.
	 */
	void drawTextContent(SciTextRenderer renderer, StringMatrixGL text, TextGrid positionMatrix);
	
	
}
