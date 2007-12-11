/*------------------------------------------------------------------------*/
/* file: XTicksDrawerGL.java                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks for the one axis                            */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.textDrawing.CenteredTextDrawerGL;
import org.scilab.modules.renderer.textDrawing.TextAlignementStrategy;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class drawing ticks for the one axis
 * @author Jean-Baptiste Silvy
 */
public abstract class TicksDrawerGL extends DrawableObjectGL {

	/** Size in pixel of ticks */
	public static final double TICKS_PIXEL_LENGTH = 0.05;
	/** Sie in pixel of subticks */
	public static final double SUBTICKS_PIXEL_LENGTH = 0.03;
	
	private double[] ticksPositions;
	private String[] ticksLabels;
	private double[] subticksPositions;
	
	private double xMin;
	private double xMax;
	private double yMin;
	private double yMax;
	private double zMin;
	private double zMax;
	
	private int lineStyle;
	private float thickness;
	private int lineColor;
	
	private CenteredTextDrawerGL textDrawer;
	
	/**
	 * Default constructor.
	 */
	public TicksDrawerGL() {
		super();
		textDrawer = new CenteredTextDrawerGL();
		textDrawer.setRotationAngle(0.0);
		textDrawer.setFont(1, 2.0);
		textDrawer.setTextAlignement(TextAlignementStrategy.CENTERED_ALIGNED_INDEX);
		textDrawer.setFilledBoxSize(0, 0);
		textDrawer.setFontColor(1);
		lineStyle = 0;
		thickness = 0.0f;
		xMin = 0.0;
		xMax = 0.0;
		yMin = 0.0;
		yMax = 0.0;
		zMin = 0.0;
		zMax = 0.0;
		lineColor = 0;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		textDrawer.initializeDrawing(parentFigureIndex);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
		textDrawer.endDrawing();
	}
	
	/**
	 * @return Nummber of ticks to draw
	 */
	protected int getNbTicks() {
		return ticksPositions.length;
	}
	
	/**
	 * @param tickIndex index of the tick
	 * @return positions of ticks on its axis.
	 */
	protected double getTickPosition(int tickIndex) {
		return ticksPositions[tickIndex];
	}
	
	/**
	 * @param tickIndex index of the tick
	 * @return label to draw in front of the tick
	 */
	protected String getTickLabel(int tickIndex) {
		return ticksLabels[tickIndex];
	}
	
	/**
	 * @return number of subticks to draw
	 */
	protected int getNbSubticks() {
		return subticksPositions.length;
	}
	
	/**
	 * @param subtickIndex index of the subtick
	 * @return position of the subtick to draw on its axis.
	 */
	protected double getSubtickPosition(int subtickIndex) {
		return subticksPositions[subtickIndex];
	}
	
	/**
	 * @return get the drawer for ticks labels
	 */
	protected CenteredTextDrawerGL getTextDrawer() {
		return textDrawer;
	}
	
	/**
	 * @return minimium bound on X axis
	 */
	protected double getXmin() {
		return xMin;
	}
	
	/**
	 * @return maximum bound on X axis
	 */
	protected double getXmax() {
		return xMax;
	}
	
	/**
	 * @return minimium bound on Y axis
	 */
	protected double getYmin() {
		return yMin;
	}
	
	/**
	 * @return maximum bound on Y axis
	 */
	protected double getYmax() {
		return yMax;
	}
	
	/**
	 * @return minimium bound on Z axis
	 */
	protected double getZmin() {
		return zMin;
	}
	
	/**
	 * @return maximum bound on Z axis
	 */
	protected double getZmax() {
		return zMax;
	}
	
	/**
	 * Set axes box to know were to draw items.
	 * @param xMin minimun bounds on X axis.
	 * @param xMax maximum bounds on X axis.
	 * @param yMin minimun bounds on Y axis.
	 * @param yMax maximum bounds on Y axis.
	 * @param zMin minimun bounds on Z axis.
	 * @param zMax maximum bounds on Z axis.
	 */
	public void setAxesBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
		this.xMin = xMin;
		this.xMax = xMax;
		this.yMin = yMin;
		this.yMax = yMax;
		this.zMin = zMin;
		this.zMax = zMax;
	}
	
	/**
	 * Set a new line style for the line.
	 * @param lineStyle index of the line Style
	 */
	public void setLineStyle(int lineStyle) {
		this.lineStyle = lineStyle;
	}
	
	/**
	 * @return index of line style to use.
	 */
	public int getLineStyle() {
		return lineStyle;
	}
	
	/**
	 * Set the thickness
	 * @param thickness thickness of the line in pixels
	 */
	public void setThickness(float thickness) {
		this.thickness = thickness;
	}
	
	/**
	 * @return thickness of the lines to draw in pixels
	 */
	public float getThickness() {
		return thickness;
	}
	
	/**
	 * Set the color of font to use
	 * @param colorIndex index of the color in the colormap
	 */
	public void setFontColor(int colorIndex) {
		textDrawer.setFontColor(colorIndex);
	}
	
	/**
	 * Specify a new font draw the text object.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 */
	public void setFont(int fontTypeIndex, double fontSize) {
		textDrawer.setFont(fontTypeIndex, fontSize);
	}
	
	/**
	 * Set a color for the axes to draw.
	 * @param colorIndex color index in the colormap
	 */
	public void setLineColor(int colorIndex) {
		this.lineColor = colorIndex;
	}
	
	/**
	 * @return array of size 3 containing the 3 color channels
	 */
	public double[] getLineColor() {
		return getColorMap().getColor(lineColor);
	}
	
	/**
	 * Specify all constant parameters in a single function
	 * @param lineStyle index of the line Style
	 * @param lineWidth thickness of the line in pixels
	 * @param lineColor color index in the colormap of the line color
	 * @param fontType ndex of the font in the font array.
	 * @param fontSize font size to use.
	 * @param fontColor index of the color in the colormap for the font
	 */
	public void setAxisParameters(int lineStyle, float lineWidth, int lineColor,
								  int fontType, double fontSize, int fontColor) {
		setLineStyle(lineStyle);
		setThickness(lineWidth);
		setLineColor(lineColor);
		setFont(fontType, fontSize);
		setFontColor(fontColor);
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawTicks();
		endDrawing();
	}
	
	/**
	 * Check if the ticks label does not concealed each other at the specified positions.
	 * @param ticksPositions initial position of the ticks
	 * @param ticksLabels labels to display in front of the ticks
	 * @return true if the ticks can be displayed as if or false if the number of ticks needs to be reduced.
	 */
	public boolean checkTicks(double[] ticksPositions, String[] ticksLabels) {
		// TODO actually check
		return true;
	}
	
	/**
	 * Draw a set of ticks
	 * @param ticksPositions positions of each ticks on their axis.
	 * @param ticksLabels labels to draw in front of ticks.
	 *                    must have the same size with ticks positions.
	 * @param subticksPositions positions of sub ticks on their axis.
	 */
	public void drawTicks(double[] ticksPositions, String[] ticksLabels, double[] subticksPositions) {
		this.ticksPositions = ticksPositions;
		this.ticksLabels = ticksLabels;
		this.subticksPositions = subticksPositions;
		
		drawTicks();
	}
	
	/**
	 * Draw ticks segment and the axis segment
	 * @param ticksPositions starting edge of ticks lines.
	 * @param ticksDirection direction of ticks
	 * @param axisSegementStart one end of the axis segment
	 * @param axisSegmentEnd the other end
	 */
	protected void drawTicksLines(Vector3D[] ticksPositions, Vector3D ticksDirection,
								  Vector3D axisSegementStart, Vector3D axisSegmentEnd) {
		GL gl = getGL();
		
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		gl.glLineWidth(getThickness());
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		
		gl.glBegin(GL.GL_LINES);
		// draw axis segment
		gl.glVertex3d(axisSegementStart.getX(), axisSegementStart.getY(), axisSegementStart.getZ());
		gl.glVertex3d(axisSegmentEnd.getX(), axisSegmentEnd.getY(), axisSegmentEnd.getZ());
		
		// draw ticks
		for (int i = 0; i < ticksPositions.length; i++) {
			gl.glVertex3d(ticksPositions[i].getX(), ticksPositions[i].getY(), ticksPositions[i].getZ());
			Vector3D lineEnd = ticksPositions[i].add(ticksDirection);
			gl.glVertex3d(lineEnd.getX(), lineEnd.getY(), lineEnd.getZ());
		}
		gl.glEnd();
		
		GLTools.endDashMode(gl);
	}
	
	/**
	 * Draw labels in front of ticks
	 * @param ticksPosition position of ticks along the axis
	 * @param ticksDirection direction of ticks
	 */
	protected void drawLabels(Vector3D[] ticksPosition, Vector3D ticksDirection) {
		int nbLabels = getNbTicks();
		GL gl = getGL();
		
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		for (int i = 0; i < nbLabels; i++) {
			Vector3D ticksPosPix = transform.getCanvasCoordinates(gl, ticksPosition[i]);
			Vector3D ticksEndPix = transform.getCanvasCoordinates(gl, ticksPosition[i].add(ticksDirection));
			Vector3D ticksDirPix = ticksEndPix.substract(ticksPosPix);
			
			// set label text
			String[] curLabel = {getTickLabel(i)};
			getTextDrawer().setTextContent(curLabel, 1, 1);
			
			// get text bounding box
			Vector3D[] pixBoundingBox = getTextDrawer().getBoundingRectangle2D();
			double boxWidth = Math.abs(pixBoundingBox[0].getX() - pixBoundingBox[2].getX()) / 2.0;
			double boxHeight = Math.abs(pixBoundingBox[0].getY() - pixBoundingBox[2].getY()) / 2.0;
			
			Vector3D textCenter = ticksPosPix.add(ticksDirPix.scalarMult(2.0));
			
			// move text in order to put its box in front of ticks
			// the aim is to put ticks segment and text center aligned
			if (ticksDirPix.getX() > Math.abs(ticksDirPix.getY())) {
				textCenter = textCenter.add(new Vector3D(boxWidth, 0.0, 0.0));
			} else if (ticksDirPix.getX() < -Math.abs(ticksDirPix.getY())) {
				textCenter = textCenter.add(new Vector3D(-boxWidth, 0.0, 0.0));
			} else if (ticksDirPix.getY() > Math.abs(ticksDirPix.getX())) {
				textCenter = textCenter.add(new Vector3D(0.0, boxHeight, 0.0));
			} else {
				textCenter = textCenter.add(new Vector3D(0.0, -boxHeight, 0.0));
			}
			
			textCenter = transform.retrieveSceneCoordinates(gl, textCenter);
			
			getTextDrawer().setCenterPosition(textCenter.getX(), textCenter.getY(), textCenter.getZ());
			
			getTextDrawer().drawTextContent();
		}
	}
	
	/**
	 * Draw ticks from the recorded data.
	 */
	public abstract void drawTicks();

}
