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

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;
import org.scilab.modules.renderer.utils.textRendering.FontManager;
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;

/**
 * Class drawing ticks for the one axis
 * @author Jean-Baptiste Silvy
 */
public class TicksDrawerGL extends DrawableObjectGL {

	/** Size in pixel of ticks */
	public static final double TICKS_PIXEL_LENGTH = 0.02;
	
	/** Size of subticks compared to ticks */
	public static final double SUBTICKS_FACTOR = 0.6;
	
	/** Distance from labels to axis relative to ticks length */
	private static final double LABEL_TO_AXIS_DISTANCE = 1.25;
	
	/** Distance from labels with exponent to the axis relative to ticks length */
	private static final double LABEL_EXPONENT_TO_AXIS_DISTANCE = 1.0;
	
	/** Exponent size compared to label size */
	private static final float EXPONENT_SIZE = 0.9f;
	
	
	
	private String[] ticksLabels;
	private String[] labelsExponents;
	
	private double[] curLabelBox = new double[GeomAlgos.RECTANGLE_NB_CORNERS]; // [xmin, xmax, ymin, ymax]
	private double[] nextLabelBox = new double[GeomAlgos.RECTANGLE_NB_CORNERS]; // [xmin, xmax, ymin, ymax]
	
	private int lineStyle;
	private float thickness;
	private int lineColor;
	
	/** Specify wether the axis line (the long one) should be drawn */
	private boolean drawAxisLine;
	
	private Font labelFont;
	private int labelColor;
	
	/** keep it for speed */
	private CoordinateTransformation transform;
	
	private Vector3D axisStart;
	private Vector3D axisEnd;
	private Vector3D[] labelsPositions;
	private Vector3D[] labelsExpPositions;
	private Vector3D[] ticksStarts;
	private Vector3D[] ticksEnds;
	private Vector3D[] subticksStarts;
	private Vector3D[] subticksEnds;
	
	private double labelToAxisDist;
	
	private boolean useFractionalMetrics;
	
	/** To know if we need to return wether the labels are concealing or not */
	private boolean needTicksDecimation;
	
	/** Sepecify where the ticks are drawn in the window
	 *  Cut the viewport in 4 distincts part 
	 */
	private enum TicksPositionCase {
		TOP, LEFT, BOTTOM, RIGHT
	};
	
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
		transform = null;
		
		axisStart = null;
		axisEnd = null;
		labelsPositions = null;
		labelsExpPositions = null;
		ticksStarts = null;
		ticksEnds = null;
		labelToAxisDist = -1.0;
		useFractionalMetrics = false;
		
		needTicksDecimation = false;
	}
	
	/**
	 * Should not be called
	 * @param parentFigureIndex index of parent figure
	 */
	public void show(int parentFigureIndex) {
		// should not be called
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		transform = getCoordinateTransformation();
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
	}
	
	/**
	 * @return Coordinate trasnformation
	 */
	public CoordinateTransformation getTransform() {
		return transform;
	}
	
	/** 
	 * @return true if labels expo,e,ts are displayes
	 */
	public boolean isDisplayingExponents() {
		return labelsExponents != null;
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
	 * @param useFractionalMetrics specify if the font should use fractional metrics or not
	 */
	public void setUseFractionalMetrics(boolean useFractionalMetrics) {
		this.useFractionalMetrics = useFractionalMetrics;
	}
	
	/**
	 * Specify all constant parameters in a single function
	 * @param lineStyle index of the line Style
	 * @param lineWidth thickness of the line in pixels
	 * @param lineColor color index in the colormap of the line color
	 * @param fontType ndex of the font in the font array.
	 * @param fontSize font size to use.
	 * @param fontColor index of the color in the colormap for the font
	 * @param useFractionalMetrics specify if the font should use fractional metrics or not
	 */
	public void setAxisParameters(int lineStyle, float lineWidth, int lineColor,
								  int fontType, double fontSize, int fontColor,
								  boolean useFractionalMetrics) {
		setLineStyle(lineStyle);
		setThickness(lineWidth);
		setLineColor(lineColor);
		setFont(fontType, fontSize);
		setFontColor(fontColor);
		setUseFractionalMetrics(useFractionalMetrics);
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
	 * Specify wether we need ticks decimation or not
	 * @param needTicksDecimation if true the ticks drawing will
	 *                            stop if some ticks are concealing.
	 */
	public void setNeedTicksDecimation(boolean needTicksDecimation) {
		this.needTicksDecimation = needTicksDecimation;
	}
	
	/**
	 * Temporary function to set labels exponents
	 * @param labelsExponents exponents to draw in top of labels
	 */
	public void setLabelsExponents(String[] labelsExponents) {
		this.labelsExponents = labelsExponents;
	}
	
	/**
	 * Draw ticks segment and the axis segment
	 * @param ticksPosPix starting edge of ticks lines.
	 * @param subticksPosPix positions of subticks
	 * @param ticksDirPix direction of ticks in pixels
	 * @param axisSegementStart one end of the axis segment
	 * @param axisSegmentEnd the other end
	 */
	protected void drawTicksLines(Vector3D[] ticksPosPix, Vector3D[] subticksPosPix,
								  Vector3D ticksDirPix,
								  Vector3D axisSegementStart, Vector3D axisSegmentEnd) {
		
		// first store the data for later reuse
		int nbTicks = ticksPosPix.length;
		ticksStarts = new Vector3D[nbTicks];
		ticksEnds = new Vector3D[nbTicks];
		int nbSubticks = subticksPosPix.length;
		subticksStarts = new Vector3D[nbSubticks];
		subticksEnds = new Vector3D[nbSubticks];
		
		axisStart = axisSegementStart;
		axisEnd = axisSegmentEnd;
		
		// store ticks
		for (int i = 0; i < nbTicks; i++) {
			if (ticksPosPix[i] == null) {
				ticksStarts[i] = null;
				ticksEnds[i] = null;
			} else {
				ticksStarts[i] = ticksPosPix[i];
				ticksEnds[i] = ticksPosPix[i].add(ticksDirPix);
			}
		}
		
		// draw subticks
		// same has ticks but with a new ticks length.
		Vector3D subTicksDirection = ticksDirPix.scalarMult(SUBTICKS_FACTOR);
		for (int i = 0; i < nbSubticks; i++) {
			if (subticksPosPix[i] == null) {
				subticksStarts[i] = null;
				subticksEnds[i] = null;
			} else {
				subticksStarts[i] = subticksPosPix[i];
				subticksEnds[i] = subticksPosPix[i].add(subTicksDirection);
			}
		}
		
		// then draw everything
		showTicksLines();
	}
	
	/**
	 * Draw ticks lines from precomputed data
	 */
	protected void showTicksLines() {
		int nbTicks = ticksStarts.length;
		int nbSubticks = subticksStarts.length;
		
		GL gl = getGL();
		
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		gl.glBegin(GL.GL_LINES);
		
		
		// draw axis segment if requested
		if (drawAxisLine) {
			gl.glVertex3d(axisStart.getX(), axisStart.getY(), axisStart.getZ());
			gl.glVertex3d(axisEnd.getX(), axisEnd.getY(), axisEnd.getZ());
		}
		
		// draw ticks
		for (int i = 0; i < nbTicks; i++) {
			if (ticksStarts[i] == null) { continue; }
			gl.glVertex3d(ticksStarts[i].getX(), ticksStarts[i].getY(), ticksStarts[i].getZ());
			gl.glVertex3d(ticksEnds[i].getX(), ticksEnds[i].getY(), ticksEnds[i].getZ());
		}
		
		// draw subticks
		for (int i = 0; i < nbSubticks; i++) {
			if (subticksStarts[i] == null) { continue; }
			gl.glVertex3d(subticksStarts[i].getX(), subticksStarts[i].getY(), subticksStarts[i].getZ());
			gl.glVertex3d(subticksEnds[i].getX(), subticksEnds[i].getY(), subticksEnds[i].getZ());
		}
		
		gl.glEnd();
		
		GLTools.endDashMode(gl);
	}
	
	/**
	 * Compute label center from its ticks direction and location
	 * @param textWidth width of the text to place
	 * @param textHeight height of the text to draw
	 * @param tickPosPix base of the ticks on the axis segment in pixel
	 * @param ticksDirPix ticks direction in pixels
	 * @param centerPosition out argument giving the postions to use for labels drawing.
	 * @param ticksSide define the direction of ticks
	 * @return distance from the label to the end of ticks in pixels
	 */
	private double computeLabelCenter(double textWidth, double textHeight, Vector3D tickPosPix, Vector3D ticksDirPix,
									  Vector3D centerPosition,
									  TicksPositionCase ticksSide) {
				
		Vector3D textCenter = tickPosPix.add(ticksDirPix);
		double res;
		
		// move text in order to put its box in front of ticks
		// the aim is to put ticks segment and text center aligned
		switch (ticksSide) {
		case RIGHT:
			res = textWidth;
			textCenter.setY(textCenter.getY() - textHeight / 2.0);
			break;
		case LEFT:
			res = textWidth;
			textCenter.setX(textCenter.getX() - textWidth);
			textCenter.setY(textCenter.getY() - textHeight / 2.0);
			break;
		case TOP:
			res = textHeight;
			textCenter.setX(textCenter.getX() - textWidth / 2.0);
			break;
		case BOTTOM:
			res = textHeight;
			textCenter.setX(textCenter.getX() - textWidth / 2.0);
			textCenter.setY(textCenter.getY() - textHeight);
			break;
		default:
			res = 0.0;
			break;
		}
		/* We need to use rounded values to avoid grabbaled text */
		centerPosition.setValues(Math.round(textCenter.getX()), Math.round(textCenter.getY()), textCenter.getZ());
		return res;
	}
	
	/**
	 * Compute the position of the text to draw from the 
	 * @param textCenter center of the label and exponent
	 * @param labelWidth width of the bottom part of the label
	 * @param labelHeight height of the bottom part of the label
	 * @return position of the exponent to draw
	 */
	private Vector3D computeExponentPosition(Vector3D textCenter, double labelWidth, double labelHeight) {
		return textCenter.add(new Vector3D(Math.round(labelWidth), Math.round(labelHeight), 0.0));
	}
	
	/**
	 * Draw labels in front of ticks
	 * @param renderer textrenderer used to draw text
	 * @param exponentRenderer renderer used to draw exponents if needed
	 * @param ticksPosPix position of ticks along the axis in pixel
	 * @param ticksDirPix direction of ticks in pixels
	 * @param bboxWidth contains width of labels bounding box
	 * @param bboxHeight contains height of labels bounding box
	 * @return maximum distance of ticks from the axis in pixel
	 */
	protected double drawLabels(SciTextRenderer renderer, SciTextRenderer exponentRenderer,
								Vector3D[] ticksPosPix, Vector3D ticksDirPix,
								double[] bboxWidth, double[] bboxHeight) {
		int nbLabels = ticksPosPix.length;
		Vector3D textCenter = new Vector3D();
		double maxDist = 0.0;
		
		labelsPositions = new Vector3D[nbLabels];
		if (isDisplayingExponents()) {
			labelsExpPositions = new Vector3D[nbLabels];
		} else {
			labelsExpPositions = null;
		}
		
		if (isDisplayingExponents()) {
			ticksDirPix.scalarMultSelf(LABEL_EXPONENT_TO_AXIS_DISTANCE);
		} else {
			ticksDirPix.scalarMultSelf(LABEL_TO_AXIS_DISTANCE);
		}
		
		// compute orientation of ticks
		TicksPositionCase ticksOrientation = computeGlobalOrientation(ticksDirPix);
		GL gl = getGL();
		gl.glDisable(GL.GL_COLOR_LOGIC_OP); // does not work well with text rendering
		
		renderer.begin3DRendering(gl);
		
		for (int i = 0; i < nbLabels; i++) {
			
			if (ticksPosPix[i] == null) { continue; }
			
			double curDist = computeLabelCenter(bboxWidth[i],
												bboxHeight[i],
												ticksPosPix[i],
												ticksDirPix,
												textCenter,
												ticksOrientation);
			
			
			// find the maximum distance
			if (curDist > maxDist) {
				maxDist = curDist;
			}
			
			labelsPositions[i] = new Vector3D(textCenter);
			renderer.draw3D(gl, getTickLabel(i),
					        labelsPositions[i].getX(), labelsPositions[i].getY(),
					        labelsPositions[i].getZ(), 0.0);
			
			
			
		}
		
		
		renderer.end3DRendering(gl);
		
		if (isDisplayingExponents()) {
			exponentRenderer.begin3DRendering(gl);
			for (int i = 0; i < nbLabels; i++) {
				if (ticksPosPix[i] == null) { continue; }
				double baseWidth = bboxWidth[i + nbLabels];
				double baseHeight = bboxHeight[i + nbLabels];
				labelsExpPositions[i] = computeExponentPosition(labelsPositions[i], baseWidth, baseHeight);
				if (labelsExpPositions[i] == null) { continue; }
				exponentRenderer.draw3D(gl, getLabelExponent(i),
										labelsExpPositions[i].getX(),
										labelsExpPositions[i].getY(),
										labelsExpPositions[i].getZ(),
										0.0);
			}
			exponentRenderer.end3DRendering(gl);
		}
		
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
		
		//labelToAxisDist =  maxDist + ticksDirPix.getNorm();
		double ticksDirPixNorm = ticksDirPix.getNorm();
		Vector3D labelDisplacament = ticksDirPix.scalarMult((ticksDirPixNorm + maxDist) / ticksDirPixNorm);
		
		// convert labelDisplacement length into user coordinates
		labelDisplacament = transform.retrieveSceneCoordinates(gl, labelDisplacament);
		labelDisplacament = labelDisplacament.substract(transform.retrieveSceneCoordinates(gl, Vector3D.ORIGIN));
		
		//showLabels(renderer);
		labelToAxisDist = labelDisplacament.getNorm();
		
		
		return labelToAxisDist;
	}
	
	/**
	 * Compute on which part of the plane the ticksDirection is
	 * @param ticksDirPix direction of the ticks in pixel coordinates
	 * @return One of the 4 plane partition
	 */
	private TicksPositionCase computeGlobalOrientation(Vector3D ticksDirPix) {
		if (ticksDirPix.getX() > Math.abs(ticksDirPix.getY())) {
			return TicksPositionCase.RIGHT;
		} else if (ticksDirPix.getX() < -Math.abs(ticksDirPix.getY())) {
			return TicksPositionCase.LEFT;
		} else if (ticksDirPix.getY() > Math.abs(ticksDirPix.getX())) {
			return TicksPositionCase.TOP;
		} else {
			return TicksPositionCase.BOTTOM;
		}
	}
	
	/**
	 * Draw labels from precomputed positions
	 * @param renderer textrenderer used to draw text
	 * @param exponentRenderer renderer used to draw labels if needed
	 */
	protected void showLabels(SciTextRenderer renderer, SciTextRenderer exponentRenderer) {
		int nbLabels = labelsPositions.length;
		
		GL gl = getGL();
		gl.glDisable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
		
		renderer.begin3DRendering(gl);
		
		for (int i = 0; i < nbLabels; i++) {
			if (labelsPositions[i] == null) { continue; }
			renderer.draw3D(gl, getTickLabel(i), labelsPositions[i].getX(), labelsPositions[i].getY(),
					        labelsPositions[i].getZ(), 0.0);
		}
		
		renderer.end3DRendering(gl);
		
		
		if (labelsExpPositions != null) {
			exponentRenderer.begin3DRendering(gl);
			for (int i = 0; i < nbLabels; i++) {
				if (labelsExpPositions[i] == null) { continue; }
				exponentRenderer.draw3D(gl, getLabelExponent(i),
								labelsExpPositions[i].getX(),
								labelsExpPositions[i].getY(),
								labelsExpPositions[i].getZ(),
								0.0);
			}
			exponentRenderer.end3DRendering(gl);
		}
		
		
		
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
	}
	
	/**
	 * Check if the labels which should be displayed do not concealed each other.
	 * @param ticksPosPix position of ticks along the axis in pixel
	 * @param ticksDirPix direction of ticks in pixel
	 * @param bboxWidth contains width of labels bounding box
	 * @param bboxHeight contains height of labels bounding box
	 * @return true if no labels concealed, false otherwise
	 */
	protected boolean checkLabels(Vector3D[] ticksPosPix, Vector3D ticksDirPix,
								  double[] bboxWidth, double[] bboxHeight) {
		
		int nbLabels = ticksPosPix.length;

		
		int firstNonNullTicksIndex = 0;
		
		// find first non null ticks
		while (firstNonNullTicksIndex < nbLabels && ticksPosPix[firstNonNullTicksIndex] == null) {
			firstNonNullTicksIndex++;
		}
		
		if (firstNonNullTicksIndex == nbLabels) {
			// no ticks, no conceal possible
			return true;
		}
		
		// get bouding box of current label
		curLabelBox[0] = ticksPosPix[firstNonNullTicksIndex].getX();
		curLabelBox[1] = curLabelBox[0] + bboxWidth[firstNonNullTicksIndex];
		curLabelBox[2] = ticksPosPix[firstNonNullTicksIndex].getY();
		curLabelBox[2 + 1] = curLabelBox[2] + bboxHeight[firstNonNullTicksIndex];
		
		for (int i = firstNonNullTicksIndex + 1; i < nbLabels; i++) {
			if (ticksPosPix[i] == null) { continue; }
			
			// set label text
			nextLabelBox[0] = ticksPosPix[i].getX();
			nextLabelBox[1] = nextLabelBox[0] + bboxWidth[i];
			nextLabelBox[2] = ticksPosPix[i].getY();
			nextLabelBox[2 + 1] = nextLabelBox[2] + bboxHeight[i];

			
			if (GeomAlgos.areRectangleConcealing(nextLabelBox, curLabelBox)) {
				return false;
			}
			
			curLabelBox[0] = nextLabelBox[0];
			curLabelBox[1] = nextLabelBox[1];
			curLabelBox[2] = nextLabelBox[2];
			curLabelBox[2 + 1] = nextLabelBox[2 + 1];
		}
		return true;
	}
	
	/**
	 * Compute the bounding box of all the labels
	 * @param renderer textrenderer used to draw text
	 * @param ticksPosPix position of ticks along the axis in pixel
	 * @param bboxWidth result, contains width of labels bounding box
	 * @param bboxHeight result, contains height of labels bounding box
	 * @param exponentRenderer renderer used to draw exponent if needed
	 */
	protected void computeBoundingBoxes(SciTextRenderer renderer, SciTextRenderer exponentRenderer,
									    Vector3D[] ticksPosPix,
										double[] bboxWidth, double[] bboxHeight) {
		int nbLabels = ticksPosPix.length;		
		for (int i = 0; i < nbLabels; i++) {
			if (ticksPosPix[i] == null) { continue; }
			
			// set label text
			Rectangle2D rect = renderer.getBounds(getTickLabel(i));
			bboxWidth[i] = rect.getWidth();
			bboxHeight[i] = rect.getHeight();
			
			if (isDisplayingExponents()) {
				
				// put the global box in the first part of the arrays
				// put the box of the base part in the second part of the array
				
				// upper part
				bboxWidth[i + nbLabels] = bboxWidth[i];
				bboxHeight[i + nbLabels] = bboxHeight[i];
				
				// global box
				bboxWidth[i] *= 1.0 + EXPONENT_SIZE;
				bboxHeight[i] *= 1.0 + EXPONENT_SIZE;
			}
		}
	}
	
	/**
	 * Draw ticks knowing ticksPositions, subticks positions
	 * @param ticksPosPix position of ticks along the axis in pixel
	 * @param subticksPositions positions of subticks
	 * @param ticksDirPix direction of ticks in pixel
	 * @param axisSegementStart one end of the axis segment
	 * @param axisSegmentEnd the other end
	 * @return negative value if some texts are concealing, distance from the label to the axis in pixels
	 *        otherwise
	 */
	public double drawTicks(Vector3D[] ticksPosPix, Vector3D[] subticksPositions,
							Vector3D ticksDirPix,
							Vector3D axisSegementStart, Vector3D axisSegmentEnd) {
		int nbTicks = ticksPosPix.length;
		double[] bboxWidth = null;
		double[] bboxHeight = null;
		
		if (isDisplayingExponents()) {
			bboxWidth = new double[2 * nbTicks];
			bboxHeight = new double[2 * nbTicks];
		} else {
			bboxWidth = new double[nbTicks];
			bboxHeight = new double[nbTicks];
		}
		
		GL gl = getGL();
		
		// get text renderer
		SciTextRenderer renderer = getTextRenderer();
		SciTextRenderer exponentRenderer = getExponentRenderer();
		
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
	
		// compute bounding boxes of text
		computeBoundingBoxes(renderer, exponentRenderer, ticksPosPix, bboxWidth, bboxHeight);
		
		// check that labels are not concealing
		if (needTicksDecimation && !checkLabels(ticksPosPix, ticksDirPix, bboxWidth, bboxHeight)) {
			GLTools.endPixelCoordinates(gl, getParentFigureGL());
			return -1.0;
		}
		
		
		
		drawTicksLines(ticksPosPix, subticksPositions, ticksDirPix, axisSegementStart, axisSegmentEnd);
		
		double res = drawLabels(renderer, exponentRenderer, ticksPosPix, ticksDirPix, bboxWidth, bboxHeight);
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
		
		return res;
		
	}
	
	/**
	 * Draw ticks knowing ticksPositions, subticks positions
	 * @param axisSegmentStart array of size 3, one end of the axis segment
	 * @param axisSegmentEnd array of size 3, the other end
	 * @param ticksDir direction of ticks
	 * @param relativeTicksPos relative position of ticks between axis start and end
	 * @param relativeSubticksPos relative position of subticks between axis start and end
	 * @return negative value if some texts are concealing, distance from the label to the axis in pixels
	 *         otherwise
	 */
	public double drawTicks(double[] axisSegmentStart, double[] axisSegmentEnd,
							double[] ticksDir, double[] relativeTicksPos,
							double[] relativeSubticksPos) {
		transform = getCoordinateTransformation();
		GL gl = getGL();
		
		// convert positions into pixels
		Vector3D ticksDirPix = new Vector3D(ticksDir);
		ticksDirPix = transform.getCanvasCoordinates(gl, ticksDirPix);
		Vector3D originPix = transform.getCanvasCoordinates(gl, Vector3D.ORIGIN);
		ticksDirPix = ticksDirPix.substract(originPix);
		
		Vector3D axisStartPix = new Vector3D(axisSegmentStart);
		axisStartPix = transform.getCanvasCoordinates(gl, axisStartPix);
		
		Vector3D axisEndPix = new Vector3D(axisSegmentEnd);
		axisEndPix = transform.getCanvasCoordinates(gl, axisEndPix);
		
		Vector3D axisSegment = axisEndPix.substract(axisStartPix);
		
		int nbTicks = relativeTicksPos.length;
		Vector3D[] ticksPosPix = new Vector3D[nbTicks];
		for (int i = 0; i < nbTicks; i++) {
			if (relativeTicksPos[i] <= 1.0 && relativeTicksPos[i] >= 0.0) {
				ticksPosPix[i] = axisStartPix.add(axisSegment.scalarMult(relativeTicksPos[i]));
			} else {
				ticksPosPix[i] = null;
			}
		}
		
		int nbSubticks = relativeSubticksPos.length;
		Vector3D[] subticksPosPix = new Vector3D[nbSubticks];
		for (int i = 0; i < nbSubticks; i++) {
			if (relativeSubticksPos[i] <= 1.0 && relativeSubticksPos[i] >= 0.0) {
				subticksPosPix[i] = axisStartPix.add(axisSegment.scalarMult(relativeSubticksPos[i]));
			} else {
				subticksPosPix[i] = null;
			}
			
		}
		
		return drawTicks(ticksPosPix, subticksPosPix, ticksDirPix, axisStartPix, axisEndPix);
	}
	
	/**
	 * Draw ticks knowing ticksPositions, subticks positions
	 * @param axisSegmentStart array of size 3, one end of the axis segment
	 * @param axisSegmentEnd array of size 3, the other end
	 * @param ticksDir direction of ticks
	 * @param relativeTicksPos relative position of ticks between axis start and end
	 * @param relativeSubticksPos relative position of subticks between axis start and end
	 * @param ticksLabels labels to draw in front of strings
	 * @return negative value if some texts are concealing, distance from the label to the axis in pixels
	 *         otherwise
	 */
	public double drawTicks(double[] axisSegmentStart, double[] axisSegmentEnd,
							double[] ticksDir, double[] relativeTicksPos,
							double[] relativeSubticksPos, String[] ticksLabels) {
		this.ticksLabels = ticksLabels;
		return drawTicks(axisSegmentStart, axisSegmentEnd, ticksDir, relativeTicksPos, relativeSubticksPos);
	}
	
	/**
	 * Draw ticks knowing ticksPositions, subticks positions
	 * @param axisSegmentStart array of size 3, one end of the axis segment
	 * @param axisSegmentEnd array of size 3, the other end
	 * @param ticksDir direction of ticks
	 * @param relativeTicksPos relative position of ticks between axis start and end
	 * @param relativeSubticksPos relative position of subticks between axis start and end
	 * @param ticksLabels labels to draw in front of strings
	 * @param labelsExponents exponents to draw on top of ticks labels
	 * @return negative value if some texts are concealing, distance from the label to the axis in pixels
	 *         otherwise
	 */
	public double drawTicks(double[] axisSegmentStart, double[] axisSegmentEnd,
							double[] ticksDir, double[] relativeTicksPos,
							double[] relativeSubticksPos, String[] ticksLabels,
							String[] labelsExponents) {
		this.ticksLabels = ticksLabels;
		this.labelsExponents = labelsExponents;
		return drawTicks(axisSegmentStart, axisSegmentEnd, ticksDir, relativeTicksPos, relativeSubticksPos);
	}
	
	
	/**
	 * Draw ticks from precomputed data
	 * @return negative value if some texts are concealing, distance from the label to the axis in pixels
	 *         otherwise
	 */
	public double showTicks() {
		GL gl = getGL();
		
		// get text renderer
		SciTextRenderer renderer = getTextRenderer();
		SciTextRenderer exponentRenderer = getExponentRenderer();
		
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		showTicksLines();
		showLabels(renderer, exponentRenderer);
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
		
		return labelToAxisDist;
	}
	
	/**
	 * @return text renderer to use to draw the ticks labels
	 */
	private SciTextRenderer getTextRenderer() {
		double[] color = getColorMap().getColor(labelColor);
		SciTextRenderer res = getParentFigureGL().getTextRendererCreator().createTextRenderer(labelFont, useFractionalMetrics); 
		res.setColor(color);
		return res;
	}
	
	/**
	 * @return text renderer used to draw ticks exponents
	 */
	private SciTextRenderer getExponentRenderer() {
		if (isDisplayingExponents()) {
			double[] color = getColorMap().getColor(labelColor);
			Font exponentFont = labelFont.deriveFont(labelFont.getSize2D() * EXPONENT_SIZE);
			SciTextRenderer res = getParentFigureGL().getTextRendererCreator().createTextRenderer(exponentFont, useFractionalMetrics);
			res.setColor(color);
			return res;
		} else {
			// no need for exponent renderer
			return null;
		}
		
	}

}
