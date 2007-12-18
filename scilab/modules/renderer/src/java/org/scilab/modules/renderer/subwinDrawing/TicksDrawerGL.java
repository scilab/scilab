/*------------------------------------------------------------------------*/
/* file: XTicksDrawerGL.java                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks for the one axis                            */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.textDrawing.StandardTextDrawerGL;
import org.scilab.modules.renderer.textDrawing.TextAlignementStrategy;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class drawing ticks for the one axis
 * @author Jean-Baptiste Silvy
 */
public abstract class TicksDrawerGL extends DrawableObjectGL {

	/** Size in pixel of ticks */
	public static final double TICKS_PIXEL_LENGTH = 0.04;
	/** Size of subticks compared to ticks */
	public static final double SUBTICKS_FACTOR = 0.5;
	
	/** Maximum accpetable value for dot product between axis direction and ticks direction */
	private static final double MAX_COS = 0.99;
	
	/** Distance from labels to axis relative to ticks length */
	private static final double LABEL_TO_AXIS_DISTANCE = 2.0;
	
	private double[] ticksPositions;
	private String[] ticksLabels;
	private String[] labelsExponents;
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
	
	private StandardTextDrawerGL labelsDrawer;
	private StandardTextDrawerGL exponentDrawer;
	
	/**
	 * Default constructor.
	 */
	public TicksDrawerGL() {
		super();
		labelsDrawer = new StandardTextDrawerGL();
		labelsDrawer.setRotationAngle(0.0);
		labelsDrawer.setFont(1, 2.0);
		labelsDrawer.setTextAlignement(TextAlignementStrategy.CENTERED_ALIGNED_INDEX);
		labelsDrawer.setFontColor(1);
		exponentDrawer = new StandardTextDrawerGL();
		exponentDrawer.setRotationAngle(0.0);
		exponentDrawer.setFont(1, 1.0);
		exponentDrawer.setTextAlignement(TextAlignementStrategy.CENTERED_ALIGNED_INDEX);
		exponentDrawer.setFontColor(1);
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
		labelsDrawer.initializeDrawing(parentFigureIndex);
		exponentDrawer.initializeDrawing(parentFigureIndex);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
		labelsDrawer.endDrawing();
		exponentDrawer.endDrawing();
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
	 * @param tickIndex index of the corresponding ticks
	 * @return string to draw in top of label
	 */
	protected String getLabelExponent(int tickIndex) {
		if (labelsExponents != null) {
			return labelsExponents[tickIndex];
		} else {
			return "";
		}
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
	 * @return positions of the subticks on their axis.
	 */
	protected double[] getSubTicksPositions() {
		return subticksPositions;
	}
	
	/**
	 * @return positions of ticks on their axis.
	 */
	protected double[] getTicksPositions() {
		return ticksPositions;
	}
	
	/**
	 * @return get the drawer for ticks labels
	 */
	protected StandardTextDrawerGL getLabelsDrawer() {
		return labelsDrawer;
	}
	
	/**
	 * @return get the drawer for ticks labels exponent
	 */
	protected StandardTextDrawerGL getExponentDrawer() {
		return exponentDrawer;
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
		labelsDrawer.setFontColor(colorIndex);
		exponentDrawer.setFontColor(colorIndex);
	}
	
	/**
	 * Specify a new font draw the text object.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 */
	public void setFont(int fontTypeIndex, double fontSize) {
		labelsDrawer.setFont(fontTypeIndex, fontSize);
		exponentDrawer.setFont(fontTypeIndex, fontSize / 2.0);
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
	 * Check if the ticks labels does not concealed each other at the specified positions.
	 * @param ticksPositions initial position of the ticks
	 * @param ticksLabels labels to display in front of the ticks
	 * @return true if the ticks can be displayed as if or false if the number of ticks needs to be reduced.
	 */
	public boolean checkTicks(double[] ticksPositions, String[] ticksLabels) {
		this.ticksPositions = ticksPositions;
		this.ticksLabels = ticksLabels;
		this.labelsExponents = null;
		return checkTicks();
	}
	
	/**
	 * Check if the ticks label does not concealed each other at the specified positions.
	 * @param ticksPositions initial position of the ticks
	 * @param ticksLabels labels to display in front of the ticks
	 * @param labelsExponents exponent to draw on top of labels
	 * @return true if the ticks can be displayed as if or false if the number of ticks needs to be reduced.
	 */
	public boolean checkTicks(double[] ticksPositions, String[] ticksLabels, String[] labelsExponents) {
		this.ticksPositions = ticksPositions;
		this.ticksLabels = ticksLabels;
		this.labelsExponents = labelsExponents;
		return checkTicks();
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
		this.labelsExponents = null;
		drawTicks();
		
	}
	
	/**
	 * Temporary function to set labels exponents
	 * @param labelsExponents exponents to draw in top of labels
	 */
	public void setLabelsExponents(String[] labelsExponents) {
		this.labelsExponents = labelsExponents;
	}
	
	/**
	 * Draw a set of ticks and labels with an exponent
	 * @param ticksPositions positions of each ticks on their axis.
	 * @param ticksLabels labels to draw in front of ticks.
	 *                    must have the same size with ticks positions.
	 * @param labelsExponents exponents to draw in top of labels
	 * @param subticksPositions positions of sub ticks on their axis.
	 */
	public void drawTicks(double[] ticksPositions, String[] ticksLabels,
						  String[] labelsExponents, double[] subticksPositions) {
		this.ticksPositions = ticksPositions;
		this.ticksLabels = ticksLabels;
		this.labelsExponents = labelsExponents;
		this.subticksPositions = subticksPositions;
		
		drawTicks();
	}
	
	/**
	 * Draw ticks segment and the axis segment
	 * @param ticksPositions starting edge of ticks lines.
	 * @param subticksPositions positions of subticks
	 * @param ticksDirection direction of ticks
	 * @param axisSegementStart one end of the axis segment
	 * @param axisSegmentEnd the other end
	 */
	protected void drawTicksLines(Vector3D[] ticksPositions, Vector3D[] subticksPositions,
								  Vector3D ticksDirection,
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
			if (ticksPositions[i] == null) { continue; }
			gl.glVertex3d(ticksPositions[i].getX(), ticksPositions[i].getY(), ticksPositions[i].getZ());
			Vector3D lineEnd = ticksPositions[i].add(ticksDirection);
			gl.glVertex3d(lineEnd.getX(), lineEnd.getY(), lineEnd.getZ());
		}
		
		// draw subticks
		// same has ticks but with a new ticks length.
		Vector3D subTicksDirection = ticksDirection.scalarMult(SUBTICKS_FACTOR);
		for (int i = 0; i < subticksPositions.length; i++) {
			if (subticksPositions[i] == null) { continue; }
			gl.glVertex3d(subticksPositions[i].getX(), subticksPositions[i].getY(), subticksPositions[i].getZ());
			Vector3D lineEnd = subticksPositions[i].add(subTicksDirection);
			gl.glVertex3d(lineEnd.getX(), lineEnd.getY(), lineEnd.getZ());
		}
		
		gl.glEnd();
		
		GLTools.endDashMode(gl);
	}
	
	/**
	 * Compute label center from its ticks direction and location
	 * @param tickPosition base of the ticks on the axis segment
	 * @param tickDirection vector defining the ticks
	 * @param exponentPosition out argument giving the position to use for exponent drawing.
	 * @return center of the text to display
	 */
	private Vector3D computeLabelCenter(Vector3D tickPosition, Vector3D tickDirection, Vector3D exponentPosition) {
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		Vector3D ticksPosPix = transform.getCanvasCoordinates(gl, tickPosition);
		Vector3D ticksEndPix = transform.getCanvasCoordinates(gl, tickPosition.add(tickDirection));
		Vector3D ticksDirPix = ticksEndPix.substract(ticksPosPix);
		
		// get text bounding box
		Vector3D[] pixBoundingBox = getLabelsDrawer().getBoundingRectangle2D();
		double boxWidth = Math.abs(pixBoundingBox[0].getX() - pixBoundingBox[2].getX());
		double boxHeight = Math.abs(pixBoundingBox[0].getY() - pixBoundingBox[2].getY());
		
		Vector3D textCenter = ticksPosPix.add(ticksDirPix.scalarMult(LABEL_TO_AXIS_DISTANCE));
		
		// move text in order to put its box in front of ticks
		// the aim is to put ticks segment and text center aligned
		if (ticksDirPix.getX() > Math.abs(ticksDirPix.getY())) {
			textCenter = textCenter.add(new Vector3D(0.0, -boxHeight / 2.0, 0.0));
		} else if (ticksDirPix.getX() < -Math.abs(ticksDirPix.getY())) {
			textCenter = textCenter.add(new Vector3D(-boxWidth, -boxHeight / 2.0, 0.0));
		} else if (ticksDirPix.getY() > Math.abs(ticksDirPix.getX())) {
			textCenter = textCenter.add(new Vector3D(-boxWidth / 2.0, 0.0, 0.0));
		} else {
			textCenter = textCenter.add(new Vector3D(-boxWidth / 2.0, -boxHeight, 0.0));
		}
		
		if (exponentPosition != null) {
			Vector3D exponentPositionPix = textCenter.add(new Vector3D(boxWidth, boxHeight, 0.0));
			exponentPosition.setValues(transform.retrieveSceneCoordinates(gl, exponentPositionPix));
		}
		
		return transform.retrieveSceneCoordinates(gl, textCenter);
	}
	
	/**
	 * Draw labels in front of ticks
	 * @param ticksPosition position of ticks along the axis
	 * @param ticksDirection direction of ticks
	 */
	protected void drawLabels(Vector3D[] ticksPosition, Vector3D ticksDirection) {
		int nbLabels = getNbTicks();
		Vector3D exponentPosition = new Vector3D();
		
		for (int i = 0; i < nbLabels; i++) {
			
			if (ticksPosition[i] == null) { continue; }
			
			// set label text
			getLabelsDrawer().setTextContent(getTickLabel(i));
			
			
			Vector3D textCenter = computeLabelCenter(ticksPosition[i], ticksDirection, exponentPosition);
			
			getLabelsDrawer().setCenterPosition(textCenter.getX(), textCenter.getY(), textCenter.getZ());
			
			getLabelsDrawer().drawTextContent();
			
			if (labelsExponents != null) {
				getExponentDrawer().setTextContent(getLabelExponent(i));
				getExponentDrawer().setCenterPosition(exponentPosition.getX(), exponentPosition.getY(), exponentPosition.getZ());
				getExponentDrawer().drawTextContent();
			}
			
		}
	}
	
	/**
	 * Check if the labels which should be displayed do not concealed each other.
	 * @param ticksPosition position of ticks along the axis
	 * @param ticksDirection direction of ticks
	 * @return true if no labels concealed, false otherwise
	 */
	protected boolean checkLabels(Vector3D[] ticksPosition, Vector3D ticksDirection) {
		
		int nbLabels = getNbTicks();
		Vector3D[] curLabelBox;
		Vector3D[] nextLabelBox;
		int firstNonNullTicksIndex = 0;
		
		// find first non null ticks
		while (ticksPosition[firstNonNullTicksIndex] == null && firstNonNullTicksIndex < nbLabels) {
			firstNonNullTicksIndex++;
		}
		
		if (firstNonNullTicksIndex == nbLabels) {
			// no ticks, no conceal possible
			return true;
		}
		
		
		// get bouding box of current label
		getLabelsDrawer().setTextContent(getTickLabel(firstNonNullTicksIndex));
		Vector3D textCenter = computeLabelCenter(ticksPosition[firstNonNullTicksIndex], ticksDirection, null);
		getLabelsDrawer().setCenterPosition(textCenter.getX(), textCenter.getY(), textCenter.getZ());
		curLabelBox = getLabelsDrawer().getBoundingRectangle2D();

		
		for (int i = firstNonNullTicksIndex + 1; i < nbLabels; i++) {
			if (ticksPosition[i] == null) { continue; }
			
			// set label text
			getLabelsDrawer().setTextContent(getTickLabel(i));
			
			textCenter = computeLabelCenter(ticksPosition[i], ticksDirection, null);
			
			getLabelsDrawer().setCenterPosition(textCenter.getX(), textCenter.getY(), textCenter.getZ());
			
			nextLabelBox = getLabelsDrawer().getBoundingRectangle2D();
			
			// remove Z coordinate
			for (int j = 0; j < GeomAlgos.RECTANGLE_NB_CORNERS; j++) {
				nextLabelBox[j].setZ(0.0);
				curLabelBox[j].setZ(0.0);
			}
				
			if (GeomAlgos.areRectangleConcealing(nextLabelBox, curLabelBox)) {			
				return false;
			}
			
			curLabelBox = nextLabelBox;
		}
		return true;
	}
	
	/**
	 * Chack if the ticks direction is not too close to the axis segment. That would lead to a bad displaying.
	 * @param ticksDirection direction of ticks
	 * @param axisSegmentStart one edge of the axis segment
	 * @param axisSegmentEnd the other edge
	 * @return true if ticks direction is OK, false otherwise
	 */
	public boolean checkTicksDirection(Vector3D ticksDirection, Vector3D axisSegmentStart, Vector3D axisSegmentEnd) {
		
		
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		// compute ticks direction in pixels
		Vector3D origin = new Vector3D(0.0, 0.0, 0.0);
		Vector3D ticksDirPix = transform.getCanvasCoordinates(gl, ticksDirection);
		origin = transform.getCanvasCoordinates(gl, origin);
		ticksDirPix = ticksDirPix.substract(origin);
		ticksDirPix.normalize();
		
		// compute axis direction in pixels
		Vector3D axisStartPix = transform.getCanvasCoordinates(gl, axisSegmentStart);
		Vector3D axisEndPix = transform.getCanvasCoordinates(gl, axisSegmentEnd);
		Vector3D axisDirPix = axisEndPix.substract(axisStartPix);
		axisDirPix.normalize();
		
		if (Math.abs(axisDirPix.dotProduct(ticksDirPix)) > MAX_COS) {
			return false;
		}
		
		return true;
		
		
	}
	
	/**
	 * Compute a new vector with the same direction than ticksDirection but with the right length
	 * @param ticksDirection initial direction of ticks whose length will be modified 
	 * @return new direction with right length
	 */
	public Vector3D setTicksDirectionLength(Vector3D ticksDirection) {
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		Vector3D origin = new Vector3D(0.0, 0.0, 0.0);
		origin = transform.getCanvasCoordinates(gl, origin);
		
		Vector3D pixDir = transform.getCanvasCoordinates(gl, ticksDirection);
		// get length in pixels
		double pixelLength = pixDir.substract(origin).getNorm();
		
		// apply number of pixels 
		return ticksDirection.scalarMult(TICKS_PIXEL_LENGTH / pixelLength);
		
	}
	
	/**
	 * Draw ticks from the recorded data.
	 */
	public abstract void drawTicks();
	
	/**
	 * Check if labels can be displayed has if.
	 * @return true if ticks can be displayed or false if we need to reduc number of ticks.
	 */
	public abstract boolean checkTicks();

}
