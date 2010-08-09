/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.imagerenderer;

import org.scilab.modules.gui.text.Text;

/**
 * Interface for ImageRenderer associated to objects in Scilab GUIs
 * @author Han DONG
 */
public interface ImageRenderer extends Text {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleImageRenderer getAsSimpleImageRenderer();
 
	/**
	 * Rotates an image
	 * @param indices the double value for the angle to rotate
	 */
 	void setRotate(double[] indices);

	/**
	 * Shears an image
	 * @param indices the double array with x, y values to shear
	 */
 	void setShear(double[] indices);

	/**
	 * Scaless an image
	 * @param indices the double array with x, y values to scale
	 */
 	void setScale(double[] indices);
}
