/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing ticks for the one axis  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.subwinDrawing;

import java.awt.Font;
import java.awt.geom.Rectangle2D;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.textDrawing.SciTextRenderer;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.FontManager;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class drawing ticks for the one axis
 * @author Jean-Baptiste Silvy
 */
public abstract class TicksDrawerGL extends BoxTrimmingObjectGL {

	/** Size in pixel of ticks */
	public static final double TICKS_PIXEL_LENGTH = 0.02;
	/** Size of subticks compared to ticks */
	public static final double SUBTICKS_FACTOR = 0.6;
	
	/** Maximum accpetable value for dot product between axis direction and ticks direction */
	private static final double MAX_COS = 0.99;
	
	/** Distance from labels to axis relative to ticks length */
	private static final double LABEL_TO_AXIS_DISTANCE = 1.5;
	
	/** Exponent size compared to label size */
	private static final float EXPONENT_SIZE = 0.5f;
	
	private double[] ticksPositions;
	private String[] ticksLabels;
	private String[] labelsExponents;
	private double[] subticksPositions;
	
	private int lineStyle;
	private float thickness;
	private int lineColor;
	
	/** Specify wether the axis line (the long one) should be drawn */
	private boolean drawAxisLine;
	
	private Font labelFont;
	private int labelColor;
	
	/**
	 * Default constructor.
	 */
	public TicksDrawerGL() {
		super();
		lineStyle = 0;
		thickness = 0.0f;
		lineColor = 0;
		/* draw the line by default */
		drawAxisLine = true;
		labelColor = -1;
		labelFont = null;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
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
		labelColor = colorIndex;
	}
	
	/**
	 * Specify a new font draw the text object.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 */
	public void setFont(int fontTypeIndex, double fontSize) {
		labelFont = FontManager.getSciFontManager().getFontFromIndex(fontTypeIndex, fontSize);
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
	 * Specify whether the axis line should be drawn or not.
	 * By default it is drawn
	 * @param drawAxisLine boolean
	 */
	public void setAxisLineDrawing(boolean drawAxisLine) {
		this.drawAxisLine = drawAxisLine;
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
	 * @return maximum distance between labels and axis.
	 */
	public double drawTicks(double[] ticksPositions, String[] ticksLabels, double[] subticksPositions) {
		this.ticksPositions = ticksPositions;
		this.ticksLabels = ticksLabels;
		this.subticksPositions = subticksPositions;
		this.labelsExponents = null;
		return drawTicks();
		
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
	 * @return maximum distance between labels and axis.
	 */
	public double drawTicks(double[] ticksPositions, String[] ticksLabels,
						    String[] labelsExponents, double[] subticksPositions) {
		this.ticksPositions = ticksPositions;
		this.ticksLabels = ticksLabels;
		this.labelsExponents = labelsExponents;
		this.subticksPositions = subticksPositions;
		
		return drawTicks();
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
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		
		gl.glBegin(GL.GL_LINES);
		
		
		// draw axis segmentif
		if (drawAxisLine) {
			gl.glVertex3d(axisSegementStart.getX(), axisSegementStart.getY(), axisSegementStart.getZ());
			gl.glVertex3d(axisSegmentEnd.getX(), axisSegmentEnd.getY(), axisSegmentEnd.getZ());
		}
		
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
	 * @param text text to draw
	 * @param renderer text renderer used to draw the text
	 * @param tickPosition base of the ticks on the axis segment
	 * @param ticksDirPix ticks direction in pixels
	 * @param exponentPosition out argument giving the position to use for exponent drawing.
	 * @param centerPosition out argument giving the postions to use for labels drawing.
	 * @return distance from the label to the axis in pixels
	 */
	private double computeLabelCenter(String text, SciTextRenderer renderer, Vector3D tickPosition, Vector3D ticksDirPix,
									  Vector3D centerPosition, Vector3D exponentPosition) {
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		Vector3D ticksPosPix = transform.getCanvasCoordinates(gl, tickPosition);

		Rectangle2D rect = renderer.getBounds(text);
		double halfBoxWidth = rect.getWidth() / 2.0;
		double halfBoxHeight = rect.getHeight() / 2.0;
		
		Vector3D textCenter = ticksPosPix.add(ticksDirPix);
		
		// move text in order to put its box in front of ticks
		// the aim is to put ticks segment and text center aligned
		if (ticksDirPix.getX() > Math.abs(ticksDirPix.getY())) {
			textCenter.setY(textCenter.getY() - halfBoxHeight);
		} else if (ticksDirPix.getX() < -Math.abs(ticksDirPix.getY())) {
			textCenter.setX(textCenter.getX() - 2.0 * halfBoxWidth);
			textCenter.setY(textCenter.getY() - halfBoxHeight);
		} else if (ticksDirPix.getY() > Math.abs(ticksDirPix.getX())) {
			textCenter.setX(textCenter.getX() - halfBoxWidth);
			textCenter.setY(textCenter.getY() + halfBoxHeight);
		} else {
			textCenter.setX(textCenter.getX() - halfBoxWidth);
			textCenter.setY(textCenter.getY() - halfBoxHeight);
		}
		
		if (exponentPosition != null) {
			exponentPosition.setValues(textCenter.getX() + 2.0 * halfBoxWidth,
									   textCenter.getY() + 2.0 * halfBoxHeight,
									   textCenter.getZ());
		}
		
		// font the farthest corner from the tick base.
		double res = textCenter.substract(ticksPosPix).getNorm();
		// we should fin the farthest corner to compute the length but adding half of the box diagonal
		// do it.
		//res += (new Vector3D(halfBoxWidth, halfBoxHeight, 0.0)).getNorm();
		res += Math.sqrt(halfBoxWidth * halfBoxWidth + halfBoxHeight * halfBoxHeight);
		
		centerPosition.setValues(textCenter);
		return res;
	}
	
	/**
	 * Draw labels in front of ticks
	 * @param ticksPosition position of ticks along the axis
	 * @param ticksDirection direction of ticks
	 * @return maximum distance of ticks from the axis in pixel
	 */
	protected double drawLabels(Vector3D[] ticksPosition, Vector3D ticksDirection) {
		int nbLabels = getNbTicks();
		Vector3D exponentPosition = new Vector3D();
		Vector3D textCenter = new Vector3D();
		double maxDist = 0.0;
		SciTextRenderer renderer = getParentFigureGL().getTextWriter(labelFont, getColorMap().getColor(labelColor));
		
		GL gl = getGL();
		gl.glDisable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
		Vector3D ticksDirPix = CoordinateTransformation.getTransformation(gl).getCanvasDirection(gl, ticksDirection);
		ticksDirPix.scalarMultSelf(LABEL_TO_AXIS_DISTANCE);
		
		GLTools.usePixelCoordinates(gl);
		
		for (int i = 0; i < nbLabels; i++) {
			
			if (ticksPosition[i] == null) { continue; }
			
			String currentLabel = getTickLabel(i);
			
			double curDist = computeLabelCenter(currentLabel, renderer, ticksPosition[i], ticksDirPix, textCenter, exponentPosition);
			
			// find the maximum distance
			if (curDist > maxDist) {
				maxDist = curDist;
			}
			
			renderer.draw3D(getTickLabel(i), textCenter.getX(), textCenter.getY(), textCenter.getZ());
			
			if (labelsExponents != null) {
				renderer.draw3D(getLabelExponent(i),
							    exponentPosition.getXf(),
							    exponentPosition.getYf(),
							    exponentPosition.getZf(),
							    EXPONENT_SIZE);
			}
			
		}
		
		renderer.end3DRendering();
		
		GLTools.endPixelCoordinates(gl);
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
		
		return maxDist;
	}
	
	/**
	 * Check if the labels which should be displayed do not concealed each other.
	 * @param ticksPosition position of ticks along the axis
	 * @param ticksDirection direction of ticks
	 * @return true if no labels concealed, false otherwise
	 */
	protected boolean checkLabels(Vector3D[] ticksPosition, Vector3D ticksDirection) {
		
		int nbLabels = getNbTicks();
		//Vector3D[] curLabelBox;
		//Vector3D[] nextLabelBox;
		double[] curLabelBox = new double[GeomAlgos.RECTANGLE_NB_CORNERS]; // [xmin, xmax, ymin, ymax]
		double[] nextLabelBox = new double[GeomAlgos.RECTANGLE_NB_CORNERS]; // [xmin, xmax, ymin, ymax]
		int firstNonNullTicksIndex = 0;
		Vector3D ticksPosPix;
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		// find first non null ticks
		while (firstNonNullTicksIndex < nbLabels && ticksPosition[firstNonNullTicksIndex] == null) {
			firstNonNullTicksIndex++;
		}
		
		if (firstNonNullTicksIndex == nbLabels) {
			// no ticks, no conceal possible
			return true;
		}
		GLTools.usePixelCoordinates(gl);

		SciTextRenderer renderer = getParentFigureGL().getTextWriter(labelFont, getColorMap().getColor(labelColor));
		ticksPosPix = transform.getCanvasCoordinates(gl, ticksPosition[firstNonNullTicksIndex]);
		
		
		// get bouding box of current label
		Rectangle2D rect = renderer.getBounds(getTickLabel(firstNonNullTicksIndex));
		curLabelBox[0] = ticksPosPix.getX();
		curLabelBox[1] = curLabelBox[0] + rect.getWidth();
		curLabelBox[2] = ticksPosPix.getY();
		curLabelBox[2 + 1] = curLabelBox[2] + rect.getHeight();
		
		for (int i = firstNonNullTicksIndex + 1; i < nbLabels; i++) {
			if (ticksPosition[i] == null) { continue; }
			
			// set label text
			ticksPosPix = transform.getCanvasCoordinates(gl, ticksPosition[i]);
			
			rect = renderer.getBounds(getTickLabel(i));
			nextLabelBox[0] = ticksPosPix.getX();
			nextLabelBox[1] = nextLabelBox[0] + rect.getWidth();
			nextLabelBox[2] = ticksPosPix.getY();
			nextLabelBox[2 + 1] = nextLabelBox[2] + rect.getHeight();

			
			if (GeomAlgos.areRectangleConcealing(nextLabelBox, curLabelBox)) {
				GLTools.endPixelCoordinates(gl);
				return false;
			}
			
			curLabelBox[0] = nextLabelBox[0];
			curLabelBox[1] = nextLabelBox[1];
			curLabelBox[2] = nextLabelBox[2];
			curLabelBox[2 + 1] = nextLabelBox[2 + 1];
		}
		GLTools.endPixelCoordinates(gl);
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
		pixDir = pixDir.substract(origin);
		double pixelLength = pixDir.getNorm();
		
		double[] viewPort = transform.getViewPort();
		
		// compute sizes wich would apply if axes where along X or Y axis
		double xSize = viewPort[2] * TICKS_PIXEL_LENGTH;
		double ySize = viewPort[CoordinateTransformation.VIEW_PORT_SIZE - 1] * TICKS_PIXEL_LENGTH;
		
		// compute angle between ticks direction and x axis in pixel coordinates
		double angle = Math.acos(Math.abs(pixDir.getNormalized().dotProduct(new Vector3D(1.0, 0.0, 0.0))));
		
		// push it between 0 and 1.
		double fact = angle * 2.0 / Math.PI;
		
		// apply number of pixels 
		return ticksDirection.scalarMult(((1.0 - fact) * xSize + fact * ySize) / pixelLength);
		
	}
	
	
	/**
	 * Find the height of the background facet
	 * @return Z coordinate of the segment to draw
	 */
	protected double findUpperZCoordinate() {
		// inverse of lower coordinate
		if (findLowerZCoordinate() == getZmin()) {
			return getZmax();
		} else {
			return getZmin();
		}
	}
	
	/**
	 * Draw ticks from the recorded data.
	 * @return maximum distance from ticks to the axis.
	 */
	public abstract double drawTicks();
	
	/**
	 * Check if labels can be displayed has if.
	 * @return true if ticks can be displayed or false if we need to reduc number of ticks.
	 */
	public abstract boolean checkTicks();

}
