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

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.textDrawing.CenteredTextDrawerGL;
import org.scilab.modules.renderer.textDrawing.StandardTextDrawerGL;
import org.scilab.modules.renderer.textDrawing.TextAlignementStrategy;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class drawing ticks for the one axis
 * @author Jean-Baptiste Silvy
 */
public class TicksDrawerGL extends DrawableObjectGL {

	/** Size of subticks compared to ticks */
	public static final double SUBTICKS_FACTOR = 0.6;
	
	/** Distance from labels to axis relative to ticks length */
	private static final double LABEL_TO_AXIS_DISTANCE = 1.5;
	
	private String[] ticksLabels;
	private String[] labelsExponents;
	
	private Vector3D[] ticksPosition;
	private Vector3D[] subticksPosition;
	
	private Vector3D axisStart;
	private Vector3D axisEnd;
	private Vector3D ticksDirection;
	
	private int lineStyle;
	private float thickness;
	private int lineColor;
	
	/** Specify wether the axis line (the long one) should be drawn */
	private boolean drawAxisLine;
	
	private CenteredTextDrawerGL labelsDrawer;
	private StandardTextDrawerGL exponentDrawer;
	
	
	
	/**
	 * Default constructor.
	 */
	public TicksDrawerGL() {
		super();
		labelsDrawer = new CenteredTextDrawerGL();
		labelsDrawer.setRotationAngle(0.0);
		labelsDrawer.setFont(1, 2.0);
		labelsDrawer.setFilledBoxSize(0, 0);
		labelsDrawer.setTextAlignement(TextAlignementStrategy.CENTERED_ALIGNED_INDEX);
		labelsDrawer.setFontColor(1);
		exponentDrawer = new StandardTextDrawerGL();
		exponentDrawer.setRotationAngle(0.0);
		exponentDrawer.setFont(1, 1.0);
		exponentDrawer.setTextAlignement(TextAlignementStrategy.CENTERED_ALIGNED_INDEX);
		exponentDrawer.setFontColor(1);
		lineStyle = 0;
		thickness = 0.0f;
		lineColor = 0;
		/* draw the line by default */
		drawAxisLine = true;
		axisStart = null;
		axisEnd = null;
		ticksDirection = null;
	}
	
	/**
	 * Show ticks
	 * @param parentFigureIndex index of parent figure
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawTicks();
		endDrawing();
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
	 * @return get the drawer for ticks labels
	 */
	protected CenteredTextDrawerGL getLabelsDrawer() {
		return labelsDrawer;
	}
	
	/**
	 * @return get the drawer for ticks labels exponent
	 */
	protected StandardTextDrawerGL getExponentDrawer() {
		return exponentDrawer;
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
	 * @param drawAxisSegement specify wether the axis segment should be drawn or not
	 */
	public void setAxisParameters(int lineStyle, float lineWidth, int lineColor,
								  int fontType, double fontSize, int fontColor,
								  boolean drawAxisSegement) {
		setLineStyle(lineStyle);
		setThickness(lineWidth);
		setLineColor(lineColor);
		setFont(fontType, fontSize);
		setFontColor(fontColor);
		setAxisLineDrawing(drawAxisSegement);
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
	 * Temporary function to set labels exponents
	 * @param labelsExponents exponents to draw in top of labels
	 */
	public void setLabelsExponents(String[] labelsExponents) {
		this.labelsExponents = labelsExponents;
	}
	
	/**
	 * Specify position of ticks along the axis in pixels
	 * @param ticksPosX X coordinates of ticks positions
	 * @param ticksPosY Y coordinates of ticks positions
	 * @param ticksPosZ Z coordinates of ticks positions
	 */
	public void setTicksPosition(double[] ticksPosX, double[] ticksPosY, double[] ticksPosZ) {
		int nbTicks = ticksPosX.length;
		ticksPosition = new Vector3D[nbTicks];
		for (int i = 0; i < nbTicks; i++) {
			ticksPosition[i] = new Vector3D(ticksPosX[i], ticksPosY[i], ticksPosZ[i]);
		}
		
		
	}
	
	/**
	 * Specify subticks positions alonng the axis in pixels
	 * @param subticksPosX X coordinates of subticks positions 
	 * @param subTicksPosY Y coordinates of subticks positions
	 * @param subTicksPosZ Z coordinates of subticks positions
	 */
	public void setSubticksPosition(double[] subticksPosX, double[] subTicksPosY, double[] subTicksPosZ) {
		int nbSubticks = subticksPosX.length;
		subticksPosition = new Vector3D[nbSubticks];
		for (int i = 0; i < nbSubticks; i++) {
			subticksPosition[i] = new Vector3D(subticksPosX[i], subTicksPosY[i], subTicksPosZ[i]);
		}
	}
	
	/**
	 * Specify ticks labels and exponents
	 * @param labels labels to display in front of ticks
	 * @param exponents exponent to display above labels
	 */
	public void setTicksLabels(String[] labels, String[] exponents) {
		this.ticksLabels = labels;
		this.labelsExponents = exponents;
	}
	
	/**
	 * Specify ticks labels with no exponent
	 * @param labels labels to display in front of ticks
	 */
	public void setTicksLabels(String[] labels) {
		this.ticksLabels = labels;
		this.labelsExponents = null;
	}
	
	
	/**
	 * Specify the vector defining ticks line
	 * @param ticksDirX X coordinate for ticks line
	 * @param ticksDirY Y coordinate for ticks line
	 * @param ticksDirZ Z coordinate for ticks line
	 */
	public void setTicksDirection(double ticksDirX, double ticksDirY, double ticksDirZ) {
		ticksDirection = new Vector3D(ticksDirX, ticksDirY, ticksDirZ);
	}
	
	/**
	 * Set the two bounds of the axis to draw
	 * @param axisStartX X coordinate of one of the axis bound
	 * @param axisStartY Y coordinate of one of the axis bound
	 * @param axisStartZ Z coordinate of one of the axis bound
	 * @param axisEndX X coordinate of the other bound
	 * @param axisEndY Y coordinate of the other bound
	 * @param axisEndZ Z coordinate of the other bound
	 */
	public void setAxisBounds(double axisStartX, double axisStartY, double axisStartZ,
							  double axisEndX, double axisEndY, double axisEndZ) {
		axisStart = new Vector3D(axisStartX, axisStartY, axisStartZ);
		axisEnd = new Vector3D(axisEndX, axisEndY, axisEndZ);
	}
							
	
	/**
	 * Draw ticks segment and the axis segment
	 * @param ticksPositions starting edge of ticks lines.
	 * @param subticksPositions positions of subticks
	 * @param ticksDirection direction of ticks
	 * @param axisSegmentStart one end of the axis segment
	 * @param axisSegmentEnd the other end
	 */
	protected void drawTicksLines(Vector3D[] ticksPositions, Vector3D[] subticksPositions,
								  Vector3D ticksDirection,
								  Vector3D axisSegmentStart, Vector3D axisSegmentEnd) {
		GL gl = getGL();
		
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		gl.glBegin(GL.GL_LINES);
		
		
		// draw axis segmentif
		if (drawAxisLine) {
			gl.glVertex3d(axisSegmentStart.getX(), axisSegmentStart.getY(), axisSegmentStart.getZ());
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
	 * @param tickPosition base of the ticks on the axis segment
	 * @param tickDirection vector defining the ticks
	 * @param exponentPosition out argument giving the position to use for exponent drawing.
	 * @param centerPosition out argument giving the postions to use for labels drawing.
	 * @return distance from the label to the axis in pixels
	 */
	private double computeLabelCenter(Vector3D tickPosition, Vector3D tickDirection,
									  Vector3D centerPosition, Vector3D exponentPosition) {
		
		Vector3D ticksPosPix = new Vector3D(tickPosition);
		Vector3D ticksEndPix = tickPosition.add(tickDirection);
		Vector3D ticksDirPix = ticksEndPix.substract(ticksPosPix);
		
		// get text bounding box
		getLabelsDrawer().setCenterPixelPos(centerPosition);
		Vector3D[] pixBoundingBox = getLabelsDrawer().getBoundingRectanglePix();
		double halfBoxWidth = Math.abs(pixBoundingBox[0].getX() - pixBoundingBox[2].getX()) / 2.0;
		double halfBoxHeight = Math.abs(pixBoundingBox[0].getY() - pixBoundingBox[2].getY()) / 2.0;
		
		ticksDirPix.scalarMultSelf(LABEL_TO_AXIS_DISTANCE);
		Vector3D textCenter = ticksPosPix.add(ticksDirPix);
		
		// move text in order to put its box in front of ticks
		// the aim is to put ticks segment and text center aligned
		if (ticksDirPix.getX() > Math.abs(ticksDirPix.getY())) {
			textCenter = textCenter.add(new Vector3D(halfBoxWidth, 0.0, 0.0));
		} else if (ticksDirPix.getX() < -Math.abs(ticksDirPix.getY())) {
			textCenter = textCenter.add(new Vector3D(-halfBoxWidth, 0.0, 0.0));
		} else if (ticksDirPix.getY() > Math.abs(ticksDirPix.getX())) {
			textCenter = textCenter.add(new Vector3D(0.0, halfBoxHeight, 0.0));
		} else {
			textCenter = textCenter.add(new Vector3D(0.0, -halfBoxHeight, 0.0));
		}
		
		if (exponentPosition != null) {
			exponentPosition.setValues(textCenter.add(new Vector3D(halfBoxWidth, halfBoxHeight, 0.0)));
		}
		
		// font the farthest corner from the tick base.
		double res = textCenter.substract(ticksPosPix).getNorm();
		// we should find the farthest corner to compute the length but adding half of the box diagonal
		// do it.
		res += (new Vector3D(halfBoxWidth, halfBoxHeight, 0.0)).getNorm();
		
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
		int nbLabels = ticksPosition.length;
		Vector3D exponentPosition = new Vector3D();
		Vector3D textCenter = new Vector3D();
		double maxDist = 0.0;
		
		for (int i = 0; i < nbLabels; i++) {
			
			// set label text
			getLabelsDrawer().setTextContent(getTickLabel(i));
			
			
			double curDist = computeLabelCenter(ticksPosition[i], ticksDirection, textCenter, exponentPosition);
			
			
			// find the maximum distance
			if (curDist > maxDist) {
				maxDist = curDist;
			}
			
			getLabelsDrawer().setCenterPixelPos(textCenter);
			
			getLabelsDrawer().drawInPixels();
			
			if (labelsExponents != null) {
				getExponentDrawer().setTextContent(getLabelExponent(i));
				getExponentDrawer().setCenterPixelPos(exponentPosition);
				getExponentDrawer().drawInPixels();
			}
			
		}
		return maxDist;
	}
	
	/**
	 * Check if the labels which should be displayed do not concealed each other.
	 * @param ticksPosition position of ticks along the axis
	 * @param ticksDirection direction of ticks
	 * @return true if no labels concealed, false otherwise
	 */
	protected boolean checkLabels(Vector3D[] ticksPosition, Vector3D ticksDirection) {
		
		int nbLabels = ticksPosition.length;
		Vector3D[] curLabelBox;
		Vector3D[] nextLabelBox;
		int firstNonNullTicksIndex = 0;
		Vector3D textCenter = new Vector3D();
		
		// find first non null ticks
		while (firstNonNullTicksIndex < nbLabels && ticksPosition[firstNonNullTicksIndex] == null) {
			firstNonNullTicksIndex++;
		}
		
		if (firstNonNullTicksIndex == nbLabels) {
			// no ticks, no conceal possible
			return true;
		}
		
		
		// get bouding box of current label
		getLabelsDrawer().setTextContent(getTickLabel(firstNonNullTicksIndex));
		computeLabelCenter(ticksPosition[firstNonNullTicksIndex], ticksDirection, textCenter, null);
		getLabelsDrawer().setCenterPixelPos(textCenter);
		curLabelBox = getLabelsDrawer().getBoundingRectanglePix();

		
		for (int i = firstNonNullTicksIndex + 1; i < nbLabels; i++) {
			
			// set label text
			getLabelsDrawer().setTextContent(getTickLabel(i));
			
			computeLabelCenter(ticksPosition[i], ticksDirection, textCenter, null);
			
			getLabelsDrawer().setCenterPixelPos(textCenter);
			
			nextLabelBox = getLabelsDrawer().getBoundingRectanglePix();
			
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
	 * Draw ticks from the recorded data.
	 * @return maximum distance from ticks to the axis.
	 */
	public double drawTicks() {
		drawTicksLines(ticksPosition, subticksPosition, ticksDirection, axisStart, axisEnd);
		return drawLabels(ticksPosition, ticksDirection);
	};
	
	/**
	 * Check if labels can be displayed has if.
	 * @return true if ticks can be displayed or false if we need to reduc number of ticks.
	 */
	public boolean checkTicks() {
		return checkLabels(ticksPosition, ticksDirection);
	}

}
