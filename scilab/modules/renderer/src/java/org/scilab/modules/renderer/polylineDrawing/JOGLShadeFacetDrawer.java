package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

import com.sun.opengl.util.texture.Texture;

/**
 * Decompose triangle with JOGL
 * @author Sylvestre Koumar
 *
 */
public class JOGLShadeFacetDrawer implements ShadeFacetDrawer {
	
	private GL gl;
	private TexturedColorMap colorMaps;
	
	/**
	 * Default constructor
	 */
	public JOGLShadeFacetDrawer() {

	}

	/**
	 * Paint the polygon given with the table of color
	 * @param coords coordinates of triangle & square
	 * @param colors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	public void paintPolygon(Vector3D[] coords, int[] colors, GL gl,
							 TexturedColorMap colorMap) {
		
		this.gl = gl;
		this.colorMaps = colorMap;

		
		Texture colormapTexture = colorMap.getTexture();
		colormapTexture.enable();
		colormapTexture.bind();

		gl.glColor3d(0.0, 1.0, 0.0);
		gl.glBegin(GL.GL_TRIANGLE_FAN); // works with triangles and quads
		for (int i = 0; i < coords.length; i++) {
			colorMap.applyTexCoord(gl, colors[i]);
			//gl.glVertex3d(xCoords[i], yCoords[i], zCoords[i]);
			gl.glVertex3d(coords[i].getX(), coords[i].getY(), coords[i].getZ());
		}
		gl.glEnd();
		 			

		colormapTexture.disable();
		
	}

}
