/*------------------------------------------------------------------------*/
/* file: TextContentDrawerGL.java                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Abstract class containing for drawing text content of a text    */
/*        object                                                          */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.textDrawing;

import java.awt.Font;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.AutoDrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * Abstract class containing for drawing text content of a text object
 * @author Jean-Baptiste Silvy
 */
public abstract class TextContentDrawerGL extends AutoDrawableObjectGL {
	
	private static final int DEFAULT_FONT_SIZE = 1;
	
	private StringMatrixGL textMatrix;
	private TextAlignementStrategy textDrawer;
	private int fontColorIndex;
	private Font fontType;
	private TextRenderer renderer;
	
	/**
	 * Default constructor
	 */
	public TextContentDrawerGL() {
		super();
		textDrawer = null;
		fontColorIndex = 0;
		fontType = null;
		renderer = null;
	}
	
	/**
	 * Set the with which kind of alignment (left, centered, right) the text will be drawn.
	 * @param alignmentIndex kind of alignement
	 */
	public void setTextAlignement(int alignmentIndex) {
		if (alignmentIndex == 0) {
			textDrawer = new LeftAlignedTextGL();
		}
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
	 * Specify the kind of font to display strings
	 * @param fontTypeIndex index of the font in the font array.
	 */
	public void setFontType(int fontTypeIndex) {
		// TODO select font from index
		fontType = new Font("Serif", Font.PLAIN, DEFAULT_FONT_SIZE);
	}
	
	/**
	 * @return Font to use to display strings.
	 */
	public Font getFont() {
		return fontType;
	}
	
	/**
	 * Specify a new fotn size to draw the fonts.
	 * @param fontSize font size to use.
	 */
	public void setFontSize(double fontSize) {
		fontType = fontType.deriveFont((float) fontSize);
	}
	
	/**
	 * Set many text parameters in one function.
	 * @param textAlignement kind of alignement.
	 * @param color index of the color in the colormap.
	 * @param fontType index of the font in the font array.
	 * @param fontSize font size to use.
	 */
	public void setTextParameters(int textAlignement, int color, int fontType, double fontSize) {
		setTextAlignement(textAlignement);
		setFontColor(color);
		setFontType(fontType);
		setFontSize(fontSize);
	}
	
	/**
	 * Specify the text that will be drawn.
	 * @param text string matrix to draw.
	 * @param nbRow text number of row
	 * @param nbCol text number of columns
	 */
	public void setTextContent(String text, int nbRow, int nbCol) {
		// check if the matrix need to be created or resized.
		if (textMatrix == null) {
			textMatrix = new StringMatrixGL();
		}
		String[] texto = {text};
		
		textMatrix.setData(texto, nbRow, nbCol);

	}
	
	/**
	 * Get the text matrix which will be displayed
	 * @return matrix of string
	 */
	protected StringMatrixGL getTextMatrix() {
		return textMatrix;
	}
	
	/**
	 * @return text drawer used to draw the text.
	 */
	protected TextRenderer getRenderer() {
		return renderer;
	}
	
	/**
	 * Draw a text on the screen.
	 * @param centerX X coordinate of the center point of the text.
	 * @param centerY Y coordinate of the center point of the text.
	 * @param centerZ Z coordinate of the center point of the text.
	 */
	public void drawTextContent(double centerX, double centerY, double centerZ) {
		
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		
		Vector3D textPos = new Vector3D(centerX, centerY, centerZ);
		textPos = transform.getCanvasCoordinates(gl, textPos);
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl);
		transform.update(gl);
		
		textPos = transform.retrieveSceneCoordinates(gl, textPos);
		
		renderer = new TextRenderer(fontType);
		TextGrid stringPos = getStringsPositions();
		stringPos.translate(textPos);
		textDrawer.drawTextContent(renderer, textMatrix,
								   stringPos);
		renderer.dispose();
		renderer = null;
		
		GLTools.endPixelCoordinates(gl);
	}
	
	/**
	 * Compute the 4 corners of the bounding rectangle of the text.
	 * @return array of size 12 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY, cornerZ}.
	 */
	public abstract double[] getBoundingRectangle();
	
	/**
	 * Get the bounding box of the text in pixels on the screen.
	 * @return array of size 8 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY}.
	 */
	public abstract int[] getScreenBoundingBox();
	
	/**
	 * Compute the matrix containing the positions of all texts.
	 * @return matrix of positions
	 */
	protected abstract TextGrid getStringsPositions();
	
}
