/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Abstract class containing for drawing text content of a text object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.textDrawing;

import java.awt.Font;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;
import org.scilab.modules.renderer.utils.textRendering.FontManager;
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;



/**
 * Abstract class containing for drawing text content of a text object
 * @author Jean-Baptiste Silvy
 */
public abstract class TextContentDrawerGL extends DrawableObjectGL implements TextRenderingPipeline {
	
	/** Offset used to center text in a pixel */
	private static final double PIXEL_CENTERING_OFFSET = 0.5;
	
	private StringMatrixGL textMatrix;
	private TextAlignementStrategy textDrawer;
	private int fontColorIndex;
	private Font fontType;
	/** Rotation angle in radian */
	private double rotationAngle;
	private Vector3D textCenter;
	private Vector3D textCenterPix;
	
	private boolean useFractionalMetrics;
	
	private boolean drawBoxBackground;
	private boolean drawBoxLine;
	private int lineColor;
	private int backgroundColor;
	
	/**
	 * Default constructor
	 */
	public TextContentDrawerGL() {
		super();
		textDrawer = null;
		fontColorIndex = 0;
		fontType = null;
		rotationAngle = 0.0;
		textCenter = new Vector3D();
		useFractionalMetrics = true;
		drawBoxBackground = false;
		drawBoxLine = false;
		lineColor = 0;
		backgroundColor = 0;
	}
	
	/**
	 * Set the with which kind of alignment (left, centered, right) the text will be drawn.
	 * @param alignmentIndex kind of alignement
	 */
	public void setTextAlignement(int alignmentIndex) {
		switch(alignmentIndex) {
		case TextAlignementStrategy.LEFT_ALIGNED_INDEX:
			textDrawer = new LeftAlignedTextGL();
			break;
		case TextAlignementStrategy.CENTERED_ALIGNED_INDEX:
			textDrawer = new CenteredAlignedTextGL();
			break;
		case TextAlignementStrategy.RIGHT_ALIGNED_INDEX:
			textDrawer = new RightAlignedTextGL();
			break;
		default:
			textDrawer = new LeftAlignedTextGL();	
			break;
		}
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		
		initializeDrawing(parentFigureIndex);
		showTextContent3D();
		endDrawing();
	}
	
	/**
	 * Set the color of font to use
	 * @param colorIndex index of the color in the colormap
	 */
	public void setFontColor(int colorIndex) {
		fontColorIndex = colorIndex;
	}
	
	/**
	 * @return three channels (RGB) of the color to use.
	 */
	public double[] getFontColor() {
		return getColorMap().getColor(fontColorIndex);
	}
	
	/**
	 * @return text center in pixel
	 */
	protected Vector3D getTextCenterPix() {
		return textCenterPix;
	}
	
	/**
	 * @return Font to use to display strings.
	 */
	public Font getFont() {
		return fontType;
	}
	
	/**
	 * Specify a new angle for the text.
	 * @param angle angle in radian
	 */
	public void setRotationAngle(double angle) {
		rotationAngle = -angle;
	}
	
	/**
	 * @return rotation angle of the text.
	 */
	public double getRotationAngle() {
		return rotationAngle;
	}
	
	/**
	 * Specify a new font draw the text object.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 */
	public void setFont(int fontTypeIndex, double fontSize) {
		fontType = FontManager.getSciFontManager().getFontFromIndex(fontTypeIndex, fontSize);
	}
	
	/**
	 * Specify a new font draw the text object.
	 * @param newFont new font to use.
	 */
	public void setFont(Font newFont) {
		fontType = newFont;
	}
	
	/**
	 * @param useFractionalMetrics specify wheter to use fractional metrics or not
	 */
	public void setUseFractionalMetrics(boolean useFractionalMetrics) {
		this.useFractionalMetrics = useFractionalMetrics;
	}
	
	/**
	 * Set many text parameters in one function.
	 * @param textAlignement kind of alignement.
	 * @param color index of the color in the colormap.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 * @param rotationAngle text rotationAngle.
	 * @param useFractionalMetrics specify wheter to use fractional metrics or not
	 */
	public void setTextParameters(int textAlignement, int color, int fontTypeIndex,
                                  double fontSize, double rotationAngle, boolean useFractionalMetrics) {
		setTextAlignement(textAlignement);
		setFontColor(color);
		setFont(fontTypeIndex, fontSize);
        setRotationAngle(rotationAngle);
        setUseFractionalMetrics(useFractionalMetrics);
	}
	
	/**
	 * Set up the box drawing for this object.
	 * @param drawBoxLine if true the surrounding line of the box is drawn
	 * @param drawBoxBackground if true the background of the box is drawn
	 * @param lineColor color of the surrounding line
	 * @param backgroundColor color of the text background
	 */
	public void setBoxDrawingParameters(boolean drawBoxLine, boolean drawBoxBackground,
			                  			int lineColor, int backgroundColor) {
		this.drawBoxLine = drawBoxLine;
		this.drawBoxBackground = drawBoxBackground;
		this.backgroundColor = backgroundColor;
		this.lineColor = lineColor;
	}
	
	/**
	 * @return array of size 3 containing the 3 channels of the background color
	 */
	public double[] getBackgroundColor() {
		return getColorMap().getColor(backgroundColor);
	}
	
	/**
	 * @return array of size 3 containing the 3 channels of the line color
	 */
	public double[] getLineColor() {
		return getColorMap().getColor(lineColor);
	}
	
	/**
	 * Set the position of the text center.
	 * @param centerX center X coordinate
	 * @param centerY center Y coordinate
	 * @param centerZ center Z  coordinate
	 */
	public void setCenterPosition(double centerX, double centerY, double centerZ) {
		textCenter = new Vector3D(centerX, centerY, centerZ);
	}
	
	/**
	 * @return text center.
	 */
	public Vector3D getTextCenter() {
		return textCenter;
	}
	
	/**
	 * Get the text renderer used to draw the text object.
	 * Create one if none has already been created.
	 * Also set the right color for the renderer.
	 * @return instance of SciTextRenderer
	 */
	public SciTextRenderer getTextRenderer() {
		SciTextRenderer res = getTextRendererNoColor();
		res.setColor(getFontColor());
		return res;
	}
	
	/**
	 * Get the text renderer used to draw the text object.
	 * Create one if none has already been created.
	 * @return instance of SciTextRenderer
	 */
	protected SciTextRenderer getTextRendererNoColor() {
		return getParentFigureGL().getTextRendererCreator().createTextRenderer(getFont(), useFractionalMetrics);
	}
	
	/**
	 * Specify the text that will be drawn.
	 * @param text string matrix to draw.
	 * @param nbRow text number of row
	 * @param nbCol text number of columns
	 */
	public void setTextContent(String[] text, int nbRow, int nbCol) {
		// check if the matrix need to be created or resized.
		if (textMatrix == null) {
			textMatrix = new StringMatrixGL();
		}
		
		textMatrix.setData(text, nbRow, nbCol);

	}
	
	/**
	 * Specify a single string to display
	 * @param text string to be displayed
	 */
	public void setTextContent(String text) {
		String[] textArray = {text};
		setTextContent(textArray, 1, 1);
	}
	
	/**
	 * Get the text matrix which will be displayed
	 * @return matrix of string
	 */
	protected StringMatrixGL getTextMatrix() {
		return textMatrix;
	}
	
	/**
	 * @param textMatrix new text matrxi to use
	 */
	protected void setTextMatrix(StringMatrixGL textMatrix) {
		this.textMatrix = textMatrix;
	}
	
	/**
	 * Draw a text on the screen.
	 * @return coordinates of the 4 corners of the text in pixels
	 */
	public Vector3D[] drawTextContent3D() {
		
		GL gl = getGL();
		
		CoordinateTransformation transform = getCoordinateTransformation();

		//Put the text on the figure
	
		textCenterPix = transform.getCanvasCoordinates(gl, getTextCenter());
		
		
		
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		
		// display lists does not work with text rendering
		
		// draw the text using the new coordinates
		Vector3D[] res = drawTextContentPix();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
		
		
		
		
		for (int i = 0; i < res.length; i++) {
			res[i] = transform.retrieveSceneCoordinates(gl, res[i]);
		}
		
		return res;
		
	}
	
	/**
	 * Draw the text from already computed data
	 */
	public void showTextContent3D() {
		GL gl = getGL();
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		
		// display the display list of text
		showTextContentPix();
		//displayDL();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
		
	}
	
	/**
	 * Draw the text and compute its bounding box in pixels
	 * @return array of size 12 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY, cornerZ}.
	 */
	public double[] drawTextContent() {
		return convertToArray(drawTextContent3D());
	}
	
	/**
	 * Concatenate the coordinates of each points into an array of double
	 * @param vects vectors to concatenate
	 * @return array of size nbVects * 3, containing the nummber of vectors
	 */
	protected double[] convertToArray(Vector3D[] vects) {
		
		int nbVects = vects.length;
		double[] res = new double[nbVects * Vector3D.DIMENSION];
		for (int i = 0; i < nbVects; i++) {
			res[Vector3D.DIMENSION * i] = vects[i].getX();
			res[Vector3D.DIMENSION * i + 1] = vects[i].getY();
			res[Vector3D.DIMENSION * i + 2] = vects[i].getZ();
		}
		return res;
	}
	
	/**
	 * Get the bounding box of the text in pixels on the screen.
	 * This function unlike other graphic functions operate only with Java2D
	 * and thus does not require an OpenGL to be current.
	 * @param centerPixX X coordinate of text center in pixel
	 * @param centerPixY Y coordinate of text center in pixel
	 * @param centerPixZ Z coordinate of text center in pixel
	 * @return array of size 12 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY, cornerZ}.
	 */
	public double[] getScreenBoundingBox(double centerPixX, double centerPixY, double centerPixZ) {
		this.textCenterPix = new Vector3D(centerPixX, centerPixY, centerPixZ);
		// computing text size in pixel does not need the OpenGL context to be current.
		Vector3D[] resVect = getBoundingRectanglePix();
		return convertToArray(resVect);
	}
	
	/**
	 * Get the screen position in pixels of the text bounding box.
	 * @return array of size 4 containing the 4 vertices.
	 */
	public Vector3D[] getBoundingRectangle2D() {
		GL gl = getGL();
		CoordinateTransformation transform = getCoordinateTransformation();
		textCenterPix = transform.getCanvasCoordinates(gl, getTextCenter());
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		
		Vector3D[] resPix = getBoundingRectanglePix();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
		return resPix;
	}
	
	/**
	 * Compute a matrix with the size of every of its strings/
	 * @param renderer text renderer to display strings.
	 * @param inputMatrix matrix of text
	 * @return the new matrix filled.
	 */
	public StringMatrixGL computeStringSizes(SciTextRenderer renderer, StringMatrixGL inputMatrix) {
		inputMatrix.update(renderer);
		return inputMatrix;
	}
	
	/**
	 * Draw the text at the right position.
	 * @param renderer TextRenderer used to render the text.
	 * @param text Matrix of the string to display
	 * @param stringPositions positons of the strings
	 */
	public void drawText(SciTextRenderer renderer, StringMatrixGL text, TextGrid stringPositions) {
		GL gl = getGL();
		drawBox(stringPositions);
		gl.glDisable(GL.GL_COLOR_LOGIC_OP); // does not work well with text rendering
		textDrawer.drawTextContent(gl, renderer, text, stringPositions, rotationAngle);
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // does not work well with text rendering
	}
	
	/**
	 * Show the text at the right position.
	 * @param renderer TextRenderer used to render the text.
	 * @param text Matrix of the string to display
	 * @param stringPositions positons of the strings
	 */
	public void showText(SciTextRenderer renderer, StringMatrixGL text, TextGrid stringPositions) {
		GL gl = getGL();
		showBox();
		gl.glDisable(GL.GL_COLOR_LOGIC_OP); // does not work well with text rendering
		textDrawer.drawTextContent(gl, renderer, text, stringPositions, rotationAngle);
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // does not work well with text rendering
	}
	
	/**
	 * Draw the box of the text
	 * @param stringPositions positons of the strings
	 */
	public void drawBox(TextGrid stringPositions) {
		if (drawBoxBackground || drawBoxLine) {
			GL gl = getGL();
			startRecordDL();
			Vector3D[] bounds = stringPositions.getExtremBounds();
			
			// ne need for polygon offset here since text is draw n vertically
			// so the depth of both is equal. Consequently we just need to
			// draw it before the line
			if (drawBoxBackground) {
				double[] color = getBackgroundColor();
				gl.glColor3d(color[0], color[1], color[2]);
				gl.glBegin(GL.GL_QUADS);
				for (int i = 0; i < bounds.length; i++) {
					// use round to center the box on the pixel
					// and so avoid flickering
					gl.glVertex3d(roundHalf(bounds[i].getX()),
								  roundHalf(bounds[i].getY()),
								  bounds[i].getZ());
				}
				gl.glEnd();
			}
			
			if (drawBoxLine) {
				double[] color = getLineColor();
				gl.glColor3d(color[0], color[1], color[2]);
				gl.glBegin(GL.GL_LINE_LOOP);
				for (int i = 0; i < bounds.length; i++) {
					// use round to center the box on the pixel
					// and so avoid flickering
					gl.glVertex3d(roundHalf(bounds[i].getX()),
							  	  roundHalf(bounds[i].getY()),
							      bounds[i].getZ());
				}	
				gl.glEnd();
			}
			
			endRecordDL();
		}
		
	}
	
	/**
	 * Show the bounding box of the text
	 */
	public void showBox() {
		if (drawBoxBackground || drawBoxLine) {
			displayDL();
		}
	}
	
	/**
	 * Compute the matrix containing the positions of all texts.
	 * @param text matrix of string with their size to draw
	 * @return matrix of positions
	 */
	public TextGrid getStringsPositions(StringMatrixGL text) {

		double cellsHeights = text.getTallestString();
		double[] heights = new double[text.getNbRow()];
		for (int i = 0; i < text.getNbRow(); i++) {
			heights[i] = cellsHeights;
		}
		
		double[] widths = text.getLongestStrings();
		
		TextGrid res = new TextGrid(text.getNbRow(), text.getNbCol(), heights, widths);
		return res;
		
	}
	
	/**
	 * @return Current font size.
	 */
	public float getFontSize() {
		return getFont().getSize2D();
	}
	
	/**
	 * @param value a double
	 * @return closest value of form i + 0.5 where i is an integer to value
	 */
	private double roundHalf(double value) {
		return Math.round(value + PIXEL_CENTERING_OFFSET) - PIXEL_CENTERING_OFFSET;
	}
	
	/**
	 * Draw the text using pixel coordinates.
	 * @return 4 corners of the rectangle bounding box.
	 */
	public abstract Vector3D[] drawTextContentPix();
	
	/**
	 * Display some from text from already precomputed positions.
	 */
	public abstract void showTextContentPix();
	
	/**
	 * Compute the 4 corners of the bounding rectangle of the text in pixels coordinates.
	 * @return array of size 4 with the four corners.
	 */
	public abstract Vector3D[] getBoundingRectanglePix();
	
	/**
	 * Get the bounding box of the text matrix centerd at the origin.
	 * @param text matrix of strings with the size of each string already computed.
	 * @return 4 corners of the bounding box.
	 */
	public abstract Vector3D[] getBoundingBox(StringMatrixGL text);
	
	/**
	 * Move the bounding box to the right position.
	 * @param bbox intial bounding box centered at the origin.
	 * @param textCenter text center
	 * @param rotationAngle rotation angle around the text center
	 * @return new bouding box turned
	 */
	public abstract Vector3D[] placeBoundingBox(Vector3D[] bbox, Vector3D textCenter, double rotationAngle);
	
	/**
	 * Put the text grid at the righ tposition
	 * @param stringPositions Initial position of strings, centered on (0,0).
	 * @param textCenterPix position of the center in pixel coordinates
	 * @param rotationAngle angle in radian.
	 * @return the new text grid ut at the right position.
	 */
	public abstract TextGrid placeTextGrid(TextGrid stringPositions, Vector3D textCenterPix, double rotationAngle);
	
	
}
