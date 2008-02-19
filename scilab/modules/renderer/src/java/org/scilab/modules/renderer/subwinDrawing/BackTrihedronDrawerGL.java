/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing only the backTrihedron of a subwin object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.subwinDrawing;

/**
 * Class drawing only the backTrihedron of a subwin object
 * @author Jean-Baptiste Silvy
 */
public class BackTrihedronDrawerGL extends SubwinBoxDrawer {

	/**
	 * Default constuctor
	 */
	public BackTrihedronDrawerGL() {
		super();
	}
	
	
	/**
	 * An other seetting for parameters since line colro is not needed
	 * @param hiddenAxisColor color of back trihedron lines
	 * @param backgroundColor color of back trihedron facets
	 * @param thickness thickness of the lines
	 */
	public void setBoxParameters(int hiddenAxisColor, int backgroundColor,
								 float thickness) {
		setHiddenAxisColor(hiddenAxisColor);
		setBackgroundColor(backgroundColor);
		setThickness(thickness);
	}
	
	/**
	 * Draw the box specified by its bounds.
	 */
	public void drawBox() {
		drawTrihedron(findConcealedCorner());
	}

}
