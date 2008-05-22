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

import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;


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
	 * @param gl current GL pipeline
	 * @param renderer renderer to use for drawing
	 * @param text matrix of strings
	 * @param positionMatrix matrix of positions.
	 */
	void drawTextContent(GL gl, SciTextRenderer renderer, StringMatrixGL text, TextGrid positionMatrix);
	
	
}
