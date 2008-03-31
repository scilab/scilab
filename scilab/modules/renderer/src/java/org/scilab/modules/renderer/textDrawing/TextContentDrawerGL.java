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
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.rectangleDrawing.RectangleDrawerStrategy;
import org.scilab.modules.renderer.rectangleDrawing.RectangleFillDrawerGL;
import org.scilab.modules.renderer.rectangleDrawing.RectangleLineDrawerGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.FontManager;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;


/**
 * Abstract class containing for drawing text content of a text object
 * @author Jean-Baptiste Silvy
 */
public abstract class TextContentDrawerGL extends DrawableObjectGL implements TextRenderingPipeline {
	
	private StringMatrixGL textMatrix;
	private TextAlignementStrategy textDrawer;
	private int fontColorIndex;
	private Font fontType;
	/** Rotation angle in radian */
	private double rotationAngle;
	private Vector3D textCenter;
	private Vector3D textCenterPix;
	
	private List<RectangleDrawerStrategy> boxDrawers;
	
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
		boxDrawers = new LinkedList<RectangleDrawerStrategy>();
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
	 * Set parampeters for box drawing
	 * @param isLine tell wether the line around the text should be drawn
	 * @param isFill tell wether the box background should be drawn
	 * @param background color index of the box background
	 * @param foreground color index of the box line
	 * @param lineWidth thickness of the line
	 * @param lineStyle style of the line
	 */
	public void setBoxParameters(boolean isLine, boolean isFill, int background, int foreground,
							     float lineWidth, int lineStyle) {
		// remove pevious ones
		boxDrawers.clear();
		
		if (isLine) {
			// add line drawer
			RectangleLineDrawerGL drawer = new RectangleLineDrawerGL();
			drawer.setLineParameters(foreground, lineWidth, lineStyle);
			boxDrawers.add(drawer);
		}
		
		if (isFill) {
			// add a box drawer
			RectangleFillDrawerGL drawer = new RectangleFillDrawerGL();
			drawer.setBackColor(background);
			boxDrawers.add(drawer);
		}
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		initializeDrawing(parentFigureIndex);
		drawTextContent();
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
	 * Set many text parameters in one function.
	 * @param textAlignement kind of alignement.
	 * @param color index of the color in the colormap.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 * @param rotationAngle text rotationAngle.
	 */
	public void setTextParameters(int textAlignement, int color, int fontTypeIndex,
                                  double fontSize, double rotationAngle) {
		setTextAlignement(textAlignement);
		setFontColor(color);
		setFont(fontTypeIndex, fontSize);
        setRotationAngle(rotationAngle);
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
	 * Specify the position of the text center in pixels
	 * @param pixelPos center position
	 */
	public void setCenterPixelPos(Vector3D pixelPos) {
		textCenterPix = new Vector3D(pixelPos);
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
	 * @return instance of SciTextRenderer
	 */
	public SciTextRenderer getTextRenderer() {
		return getParentFigureGL().getTextWriter(getFont(), getFontColor());
	}
	
	/**
	 * get the textRenderer with a new font size
	 * @param newSize new isze to apply to the font
	 * @return new instance of text renderer with a new size
	 */
	public SciTextRenderer getTextRenderer(float newSize) {
		setFont(getFont().deriveFont(newSize));
		return getTextRenderer();
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
	 * Draw a text on the screen.
	 */
	public void drawTextContent() {
		//long initTime = System.nanoTime();
		GL gl = getGL();
		
		
		
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
	
		//Vector3D textCenterPix = transform.getCanvasCoordinates(gl, getTextCenter());
		textCenterPix = transform.getCanvasCoordinates(gl, getTextCenter());
		
		// switch to pixel coordinates
//		GLTools.usePixelCoordinates(gl);
//		
//		gl.glDisable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
//		
//		
//		textCenterPix = transform.retrieveSceneCoordinates(gl, textCenterPix);
//		
//		// draw the text using the new coordinates
//		drawTextContentPix(textCenterPix);
//		
//		
//		
//		GLTools.endPixelCoordinates(gl);
//		
//		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
		
		getParentFigureGL().callInPixelCoordinates(new Runnable() {
			public void run() {
				drawInPixels();
			}
		});
 //long elapsedTime = System.nanoTime() - initTime;
		
		//System.err.println("elapsedTime = " + (elapsedTime * 1.0e-6));
	}
	
	/**
	 * Draw the text using pixel coordinates
	 */
	public void drawInPixels() {
		//long initTime = System.nanoTime();
		drawInPixels(textCenterPix);
		
		//long elapsedTime = System.nanoTime() - initTime;
		
		//System.err.println("elapsedTime = " + (elapsedTime * 1.0e-6));
	}
	
	/**
	 * Draw the text in pixel coordinates
	 * @param pixelCenter position of text center
	 */
	public void drawInPixels(Vector3D pixelCenter) {
		GL gl = getGL();
		gl.glDisable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
		
		// draw the text using the new coordinates
		gl.glPushMatrix();
		drawTextContentPix(pixelCenter);
		gl.glPopMatrix();
		
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // does not work well with thext rendering
		
		// draw box
		if (!boxDrawers.isEmpty()) {
			// get bounding box 2D
			Vector3D[] boundingBox = getBoundingRectanglePix(pixelCenter);
			for (Iterator<RectangleDrawerStrategy> it = boxDrawers.iterator(); it.hasNext();) {
				RectangleDrawerStrategy rectangleDrawer = it.next();
				((DrawableObjectGL) rectangleDrawer).initializeDrawing(getParentFigureGL().getFigureIndex());
				rectangleDrawer.drawRectangle(gl, boundingBox[0], boundingBox[1], boundingBox[2], boundingBox[2 + 1]);
				((DrawableObjectGL) rectangleDrawer).endDrawing();
			}
		}
	}
	
	/**
	 * Compute the 4 corners of the bounding rectangle of the text.
	 * @return array of size 12 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY, cornerZ}.
	 */
	public double[] getBoundingRectangle() {
		Vector3D[] resVect = getBoundingRectangle3D();
		int nbCorner = resVect.length;
		int nbDim = 2 + 1;
		double[] res = new double[nbCorner * nbDim];
		for (int i = 0; i < nbCorner; i++) {
			res[nbDim * i] = resVect[i].getX();
			res[nbDim * i + 1] = resVect[i].getY();
			res[nbDim * i + 2] = resVect[i].getZ();
		}
		return res;
	}
	
	/**
	 * Get the bounding box of the text in pixels on the screen.
	 * @return array of size 8 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY}.
	 */
	public int[] getScreenBoundingBox() {
		Vector3D[] resVect = getBoundingRectangle2D();
		int nbCorner = resVect.length;
		int nbDim = 2;
		int[] res = new int[nbCorner * nbDim];
		for (int i = 0; i < nbCorner; i++) {
			res[nbDim * i] = (int) resVect[i].getX();
			res[nbDim * i + 1] = (int) resVect[i].getY();
		}
		return res;
	}
	
	/**
	 * Get the 4 corners of the text bounding box in user coordinate system.
	 * @return array of size 4 containing the corners of the bounding rectangle.
	 */
	protected Vector3D[] getBoundingRectangle3D() {
		GL gl = getGL();		
		Vector3D[] res = getBoundingRectangle2D();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		for (int i = 0; i < res.length; i++) {
			res[i] = transform.retrieveSceneCoordinates(gl, res[i]);
		}
		return res;
	}
	
	/**
	 * Compute the 4 corners of the bounding rectangle of the text in pixels coordinates.
	 * @return array of size 4 with the four corners.
	 */
	public Vector3D[] getBoundingRectanglePix() {
		return getBoundingRectanglePix(textCenterPix);
	}
	
	/**
	 * Get the screen position in pixels of the text bounding box.
	 * @return array of size 4 containing the 4 vertices.
	 */
	public Vector3D[] getBoundingRectangle2D() {
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		Vector3D textCenterPixel = transform.getCanvasCoordinates(gl, getTextCenter());
		GLTools.usePixelCoordinates(gl);
		
		Vector3D[] resPix = getBoundingRectanglePix(textCenterPixel);
		
		// retrieve canvas coordinates
		for (int i = 0; i < resPix.length; i++) {
			resPix[i] = transform.getCanvasCoordinates(gl, resPix[i]);
		}
		
		GLTools.endPixelCoordinates(gl);
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
		textDrawer.drawTextContent(renderer, text, stringPositions);
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
	 * Draw the text using pixel coordinates.
	 * @param textCenterPix center of text to draw in pixels
	 */
	public abstract void drawTextContentPix(Vector3D textCenterPix);
	
	/**
	 * Compute the 4 corners of the bounding rectangle of the text in pixels coordinates.
	 * @param textCenterPix center of the text in pixel coordinates.
	 * @return array of size 4 with the four corners.
	 */
	public abstract Vector3D[] getBoundingRectanglePix(Vector3D textCenterPix);
	
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
