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
import org.scilab.modules.renderer.utils.FontManager;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;


/**
 * Abstract class containing for drawing text content of a text object
 * @author Jean-Baptiste Silvy
 */
public abstract class TextContentDrawerGL extends AutoDrawableObjectGL {

	
	private StringMatrixGL textMatrix;
	private TextAlignementStrategy textDrawer;
	private int fontColorIndex;
	private Font fontType;
	private SciTextRenderer renderer;
	/** Rotation angle in radian */
	private double rotationAngle;
	private Vector3D textCenter;
	
	/**
	 * Default constructor
	 */
	public TextContentDrawerGL() {
		super();
		textDrawer = null;
		fontColorIndex = 0;
		fontType = null;
		renderer = null;
		rotationAngle = 0.0;
		textCenter = null;
	}
	
	/**
	 * Set the with which kind of alignment (left, centered, right) the text will be drawn.
	 * @param alignmentIndex kind of alignement
	 */
	public void setTextAlignement(int alignmentIndex) {
		switch(alignmentIndex) {
		case 1:
			textDrawer = new LeftAlignedTextGL();
			break;
		case 2:
			textDrawer = new CenteredAlignedTextGL();
			break;
		case 2 + 1:
			textDrawer = new RightAlignedTextGL();
			break;
		default:
			textDrawer = new LeftAlignedTextGL();	
			break;
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
	 * Specify a new fotn size to draw the fonts.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 */
	public void setFont(int fontTypeIndex, double fontSize) {
		fontType = FontManager.getSciFontManager().getFontFromIndex(fontTypeIndex, fontSize);
	}
	
	/**
	 * Set many text parameters in one function.
	 * @param textAlignement kind of alignement.
	 * @param color index of the color in the colormap.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 */
	public void setTextParameters(int textAlignement, int color, int fontTypeIndex, double fontSize) {
		setTextAlignement(textAlignement);
		setFontColor(color);
		setFont(fontTypeIndex, fontSize);
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
		String[] texto = {"Hello", "I like scilab", "So much", "and you?"};
		
		textMatrix.setData(texto, 2, 2);

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
	protected SciTextRenderer getRenderer() {
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
		
		textCenter = new Vector3D(centerX, centerY, centerZ);
		textCenter = transform.getCanvasCoordinates(gl, textCenter);
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl);
		transform.update(gl);
		
		textCenter = transform.retrieveSceneCoordinates(gl, textCenter);
//		gl.glPointSize(5.0f);
//		gl.glColor3d(1.0, 0.0, 0.0);
//		gl.glBegin(GL.GL_POINTS);
//		gl.glVertex3d(textCenter.getX(), textCenter.getY(), textCenter.getZ());
//		gl.glEnd();
		
		renderer = SciTextRenderer.create(fontType);
		double[] color = getFontColor();
		renderer.setColor(color[0], color[1], color[2]);
		TextGrid stringPos = getStringsPositions();
		// move the text base.
		gl.glTranslated(textCenter.getX(), textCenter.getY(), textCenter.getZ());
		gl.glRotated(Math.toDegrees(rotationAngle), 0.0, 0.0, 1.0);
		textDrawer.drawTextContent(renderer, textMatrix,
								   stringPos);
		renderer.dispose();
		
		
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
