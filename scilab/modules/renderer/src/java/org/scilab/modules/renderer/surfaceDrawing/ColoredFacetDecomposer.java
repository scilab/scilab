/*------------------------------------------------------------------------*/
/* file: ColoredFacetDecomposer.java                                      */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for retreiving facets colors compising a surface      */
/*        (ie plot3d or fac3d) object.                                    */
/*------------------------------------------------------------------------*/


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
