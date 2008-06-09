package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.ColoredTriangle;
import org.scilab.modules.renderer.utils.geom3D.TriangleDecomposition;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
/**
 * Decompose triangle with GL2PS
 * @author Sylvestre Koumar
 *
 */
public class GL2PSShadeFacetDrawer implements ShadeFacetDrawer {
	
	/** Number of side of the polygon */
	private static final int TRIANGLE = 3;
	private static final int SQUARE = 4;
	
	/** Coordinates of the triangle or the square */
	private Vector3D a;
	private Vector3D b;
	private Vector3D c;
	private Vector3D d;
	
	private GL gl;
	private TexturedColorMap colorMaps;
	
	/**
	 * Default constructor
	 */
	public GL2PSShadeFacetDrawer() {

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
		
		if (coords.length == TRIANGLE) { 
			//Coordinates of the triangle
			a = new Vector3D(coords[0]);
			b = new Vector3D(coords[1]);
			c = new Vector3D(coords[2]);
			
			paintTriangle(a, b, c, colors[0], colors[1], colors[2]);
			
		} else if (coords.length >= SQUARE) {
			//Coordinates of the square
			//closing the square case
			a = new Vector3D(coords[0]);
			b = new Vector3D(coords[1]);
			c = new Vector3D(coords[2]);
			d = new Vector3D(coords[TRIANGLE]);
			
			//we separate the square on 2 triangle then we work on each triangle
			paintTriangle(a, b, c, colors[0], colors[1], colors[2]);
			paintTriangle(c, d, a, colors[2], colors[TRIANGLE], colors[0]);
			
		}
		
	}
	
	/**
	 * Decompose a triangle in polygons then paint them
	 * @param a coordinates of the polylines vertices
	 * @param b coordinates of the polylines vertices
	 * @param c coordinates of the polylines vertices
	 * @param color1 color of the polylines vertices
	 * @param color2 color of the polylines vertices
	 * @param color3 color of the polylines vertices
	 */
	public void paintTriangle(Vector3D a, Vector3D b, Vector3D c, double color1, double color2, double color3) {		
		
		//Calling this class will decompose triangle in colored polygons
		
		ColoredTriangle ct = new ColoredTriangle(a, b, c, color1, color2, color3);
		TriangleDecomposition td = ct.decomposeTriangle();	

		for (int i = 0; i < td.getNbPolygons(); i++) {
			int color = td.getPolygonColor(i);	

			//gl.glBegin(GL.GL_POLYGON);
			// we use gl.glBegin(GL.GL_TRIANGLES)
			// so we decompose the polygon into triangles

			double[] polyColor = colorMaps.getColor(color);
			gl.glColor3d(polyColor[0], polyColor[1], polyColor[2]);
			Vector3D[] polygon = td.getPolygon(i);
			
			// first triangle
			for (int j = 0; j < TRIANGLE; j++) {
				gl.glVertex3d(polygon[j].getX(), polygon[j].getY(), polygon[j].getZ());
			}
			
			// following triangles
			for (int j = TRIANGLE; j < polygon.length; j++) {
				gl.glVertex3d(polygon[0].getX(), polygon[0].getY(), polygon[0].getZ());
				gl.glVertex3d(polygon[j - 1].getX(), polygon[j - 1].getY(), polygon[j - 1].getZ());
				gl.glVertex3d(polygon[j].getX(), polygon[j].getY(), polygon[j].getZ());
			}

			//gl.glEnd();
			
		}
	}

	/**
	 * Paint the polygon given with the table of color
	 * @param triangleCoords coordinates of triangle & square
	 * @param triangleColors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	public void paintPolygon(Vector3D[] triangleCoords,
			double[] triangleColors, GL gl, TexturedColorMap colorMap) {

		this.gl = gl;
		this.colorMaps = colorMap;
		
		if (triangleCoords.length == TRIANGLE) { 
			//Coordinates of the triangle
			a = new Vector3D(triangleCoords[0]);
			b = new Vector3D(triangleCoords[1]);
			c = new Vector3D(triangleCoords[2]);
			
			paintTriangle(a, b, c, triangleColors[0], triangleColors[1], triangleColors[2]);
			
		} else if (triangleCoords.length >= SQUARE) {
			//Coordinates of the square
			//closing the square case
			a = new Vector3D(triangleCoords[0]);
			b = new Vector3D(triangleCoords[1]);
			c = new Vector3D(triangleCoords[2]);
			d = new Vector3D(triangleCoords[TRIANGLE]);
			
			//we separate the square on 2 triangle then we work on each triangle
			paintTriangle(a, b, c, triangleColors[0], triangleColors[1], triangleColors[2]);
			paintTriangle(c, d, a, triangleColors[2], triangleColors[TRIANGLE], triangleColors[0]);
			
		}		
	}

}
