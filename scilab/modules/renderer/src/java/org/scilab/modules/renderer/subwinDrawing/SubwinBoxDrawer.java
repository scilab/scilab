/*------------------------------------------------------------------------*/
/* file: SubwinBoxDrawer.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class for drawing different kind of axes box             */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Parent class for drawing different kind of axes box
 * @author Jean-Baptiste Silvy
 */
public abstract class SubwinBoxDrawer extends DrawableObjectGL {

	/** Number of corners of the box */
	protected static final int BOX_NB_CORNERS = 8;
	
	/** For each coner of the box get th eindexof its neighbours */
	protected static final int[][] BOX_CORNERS_NEIGHBOURS = {{1, 4, 2},
															 {0, 3, 5},
															 {0, 3, 6},
															 {1, 2, 7},
															 {0, 5, 6},
															 {1, 4, 7},
															 {4, 2, 7},
															 {3, 5, 6}};
	
	private static final double UNMERGE_FACTOR = 0.01;
	
	private int hiddenAxisColor;
	private int backgroundColor;
	private int lineColor;
	private int lineStyle;
	private float lineThickness;
	
	private Vector3D[] boxCorners;
	private Vector3D[] smallBoxCorners;
	
	/**
	 * Default constructor
	 */
	public SubwinBoxDrawer() {
		super();
		hiddenAxisColor = 0;
		backgroundColor = 0;
		lineColor = 0;
		lineStyle = 0;
		lineThickness = 0.0f;
		boxCorners = null;
		smallBoxCorners = new Vector3D[BOX_NB_CORNERS];
	}
	
	/**
	 * Display the object using already given parameters.
	 * Display lists are not usable since hidden axis might change at every redraw.
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawBox();
		endDrawing();
	}
	
	/**
	 * Set color of the back trihedron lines
	 * @param colorIndex colormap index of the color.
	 */
	public void setHiddenAxisColor(int colorIndex) {
		this.hiddenAxisColor = colorIndex;
	}
	
	/**
	 * @return Array of size 3 containing the 3 channels of the hidden axis color.
	 */
	public double[] getHiddenAxisColor() {
		return getColorMap().getColor(hiddenAxisColor);
	}
	
	/**
	 * Set the background color of the subwindow (facets color of the back trihedron).
	 * @param colorIndex colormap index of the color.
	 */
	public void setBackgroundColor(int colorIndex) {
		this.backgroundColor = colorIndex;
	}
	
	/**
	 * @return Array of size 3 containing the 3 channels of the background color.
	 */
	public double[] getBackgroundColor() {
		return getColorMap().getColor(backgroundColor);
	}
	
	/**
	 * Set color of the subwin box lines, excluding the back trihedron.
	 * @param colorIndex colormap index of the color.
	 */
	public void setLineColor(int colorIndex) {
		this.lineColor = colorIndex;
	}
	
	/**
	 * @return Array of size 3 containing the 3 channels of the line color.
	 */
	public double[] getLineColor() {
		return getColorMap().getColor(lineColor);
	}
	
	/**
	 * Set the kind of line (pattern) to use for drawing the box.
	 * @param lineStyle index of the line style
	 */
	public void setLineStyle(int lineStyle) {
		this.lineStyle = lineStyle;
	}
	
	/**
	 * @return index of line style
	 */
	public int getLineStyle() {
		return lineStyle;
	}
	
	/**
	 * Set the tickness of the lines composing the box.
	 * @param thickness thickness in pixels
	 */
	public void setThickness(float thickness) {
		this.lineThickness = thickness;
	}
	
	/**
	 * @return thickness of box lines
	 */
	public float getThickness() {
		return lineThickness;
	}
	
	/**
	 * Set all parameters with a single function
	 * @param hiddenAxisColor color of back trihedron lines
	 * @param backgroundColor color of back trihedron facets
	 * @param lineColor color of the other lines of the box
	 * @param lineStyle pattern to use for lines
	 * @param thickness thickness of the lines
	 */
	public void setBoxParameters(int hiddenAxisColor, int backgroundColor, int lineColor,
								 int lineStyle, float thickness) {
		setHiddenAxisColor(hiddenAxisColor);
		setBackgroundColor(backgroundColor);
		setLineColor(lineColor);
		setLineStyle(lineStyle);
		setThickness(thickness);
	}
	
	/**
	 * Draw the box specified by its bounds.
	 * @param xMin minimun bounds on X axis.
	 * @param xMax maximum bounds on X axis.
	 * @param yMin minimun bounds on Y axis.
	 * @param yMax maximum bounds on Y axis.
	 * @param zMin minimun bounds on Z axis.
	 * @param zMax maximum bounds on Z axis.
	 */
	public void drawBox(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
		Vector3D[] newBoxCorners = {new Vector3D(xMin, yMin, zMin),
					   				new Vector3D(xMin, yMin, zMax),
					   				new Vector3D(xMin, yMax, zMin),
									new Vector3D(xMin, yMax, zMax),
									new Vector3D(xMax, yMin, zMin),
									new Vector3D(xMax, yMin, zMax),
									new Vector3D(xMax, yMax, zMin),
									new Vector3D(xMax, yMax, zMax)};
		boxCorners = newBoxCorners;
		
		// create a slighly smaller box to avoid merging between lines and background.
		Vector3D boxCenter = findBoxCenter();
		for (int i = 0; i < BOX_NB_CORNERS; i++) {
			smallBoxCorners[i] = boxCorners[i].add(boxCenter.substract(boxCorners[i]).scalarMult(UNMERGE_FACTOR));
		}
		
		drawBox();
	}
	
	/**
	 * @return index of the coner which is concealed
	 */
	private int findConcealedCorner() {
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		// get box corners in canvas coordinates
		Vector3D[] canvasCorners = transform.getCanvasCoordinates(gl, boxCorners);
		
		
		
		// find the farthest one
		double eyeDistance = canvasCorners[0].getZ();
		int farthestCornerIndex = 0;
		for (int i = 1; i < BOX_NB_CORNERS; i++) {
			if (canvasCorners[i].getZ() > eyeDistance) {
				eyeDistance = canvasCorners[i].getZ();
				farthestCornerIndex = i;
			}
		}
		return farthestCornerIndex;
		
	}
	
	/**
	 * @return center of the clipping box
	 */
	private Vector3D findBoxCenter() {
		Vector3D center = new Vector3D(0.0, 0.0, 0.0);
		
		for (int i = 0; i < BOX_NB_CORNERS; i++) {
			center = center.add(boxCorners[i]);
		}
		center.scalarMultSelf(1.0 / BOX_NB_CORNERS);
		return center;
	}
	/**
	 * Draw the back triedre of the subwin box
	 */
	protected void drawTrihedron() {
		// find farthest triedre
		int firstPointIndex = findConcealedCorner();
		int secondPointIndex = BOX_CORNERS_NEIGHBOURS[firstPointIndex][0];
		int thirdPointIndex = BOX_CORNERS_NEIGHBOURS[firstPointIndex][1];
		int fourthPointIndex = BOX_CORNERS_NEIGHBOURS[firstPointIndex][2];
		
		
		GL gl = getGL();
		
		//		 draw background
		double[] backColor = getBackgroundColor();
		gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		
		gl.glBegin(GL.GL_QUADS);
		drawFacet(gl, firstPointIndex, secondPointIndex, thirdPointIndex);
		drawFacet(gl, firstPointIndex, secondPointIndex, fourthPointIndex);
		drawFacet(gl, firstPointIndex, thirdPointIndex, fourthPointIndex);
		gl.glEnd();
		
		
		double[] hiddenColor = getHiddenAxisColor();
		gl.glColor3d(hiddenColor[0], hiddenColor[1], hiddenColor[2]);
		
		
		// the concealed line is draw wih dashes
		GLTools.beginDashMode(gl, 2, getThickness());
		
		gl.glBegin(GL.GL_LINES);
		Vector3D firstPoint = smallBoxCorners[firstPointIndex];
		Vector3D secondPoint = smallBoxCorners[secondPointIndex];
		gl.glVertex3d(firstPoint.getX(), firstPoint.getY(), firstPoint.getZ());
		gl.glVertex3d(secondPoint.getX(), secondPoint.getY(), secondPoint.getZ());
		
		Vector3D thirdPoint = smallBoxCorners[thirdPointIndex];
		gl.glVertex3d(firstPoint.getX(), firstPoint.getY(), firstPoint.getZ());
		gl.glVertex3d(thirdPoint.getX(), thirdPoint.getY(), thirdPoint.getZ());
		
		Vector3D fourthPoint = smallBoxCorners[fourthPointIndex];
		gl.glVertex3d(firstPoint.getX(), firstPoint.getY(), firstPoint.getZ());
		gl.glVertex3d(fourthPoint.getX(), fourthPoint.getY(), fourthPoint.getZ());
		
		gl.glEnd();
		
		GLTools.endDashMode(gl);
		
		
	}
	
	/**
	 * Draw a facet of the subwindow knowing only three of its corners.
	 * The drawing order will be coner1, corner2, missing corner, corner3
	 * @param gl current GL pipeline
	 * @param cornerIndex1 first corner
	 * @param cornerIndex2 second corner
	 * @param cornerIndex3 third corner
	 */
	private void drawFacet(GL gl, int cornerIndex1, int cornerIndex2, int cornerIndex3) {
		// find last corner, it is neigbour of corner 2 and 3;
		int[] corner2Neighbours = BOX_CORNERS_NEIGHBOURS[cornerIndex2];
		int[] corner3Neighbours = BOX_CORNERS_NEIGHBOURS[cornerIndex3];
		
		int commonNeighbourIndex = 0;
		// find the common corner which is not corner1
		for (int i = 0; i < corner2Neighbours.length; i++) {
			for (int j = 0; j < corner3Neighbours.length; j++) {
				if (corner2Neighbours[i] != cornerIndex1 && corner2Neighbours[i] == corner3Neighbours[j]) {
					commonNeighbourIndex = corner2Neighbours[i];
				}
			}
		}
		
		Vector3D corner1 = boxCorners[cornerIndex1];
		Vector3D corner2 = boxCorners[cornerIndex2];
		Vector3D corner3 = boxCorners[commonNeighbourIndex];
		Vector3D corner4 = boxCorners[cornerIndex3];
		
		gl.glVertex3d(corner1.getX(), corner1.getY(), corner1.getZ());
		gl.glVertex3d(corner2.getX(), corner2.getY(), corner2.getZ());
		gl.glVertex3d(corner3.getX(), corner3.getY(), corner3.getZ());
		gl.glVertex3d(corner4.getX(), corner4.getY(), corner4.getZ());
		
	}
	
	
	/**
	 * Draw the box specified by its bounds.
	 */
	protected abstract void drawBox();
	
}
