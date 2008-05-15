package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Interface which allows us to choose between JOGL or GL2PS 
 * @author Sylvestre Koumar
 *
 */
public interface ShadeFacetDrawer {
	
	/**
	 * Paint the polygon given with the table of color
	 * @param coords coordinates of triangle & square
	 * @param color table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	void paintPolygon(Vector3D[] coords, int[] color, GL gl, TexturedColorMap colorMap);

	/**
	 * Paint the polygon given with the table of color
	 * @param triangleCoords coordinates of triangle & square
	 * @param triangleColors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	void paintPolygon(Vector3D[] triangleCoords, double[] triangleColors,
			GL gl, TexturedColorMap colorMap);

}
