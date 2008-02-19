/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Interface for retreiving facets colors compising a surface 
 * (ie plot3d or fac3d) object. 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.surfaceDrawing;

/**
 * Interface for retreiving facets colors compising a surface
 * (ie plot3d or fac3d) object.
 * @author Jean-Baptiste Silvy
 */
public interface ColoredFacetDecomposer extends FacetDecomposer {

	/**
	 * @return color indices of the vertices of the facet
	 */
	int[] getCurFacetColors();
	
	
}
