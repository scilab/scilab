/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Colormap which can create a textre of itself 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.utils;

import java.awt.image.BufferedImage;

import com.sun.opengl.util.texture.Texture;
import com.sun.opengl.util.texture.TextureCoords;
import com.sun.opengl.util.texture.TextureData;
import com.sun.opengl.util.texture.TextureIO;
import javax.media.opengl.GL;

/**
 * Colormap which can create a textre of itself
 * @author Jean-Baptiste Silvy
 */
public class TexturedColorMap extends ColorMap {

	private static final double IMAGE_SCALING = 255.0;
	private static final int RED_SHIFT = 16;
	private static final int GREEN_SHIFT = 8;
	private static final int BLUE_SHIFT = 0;
	
	/** Move a little the indice in order to get the right color in the colormap */
	private static final double COLOR_OFFSET = 0.5;
	
	private static final int WHITE_COLOR = 0xFFFFFF;
	private static final int BLACK_COLOR = 0x000000;
	
	private Texture colorMapTexture;
	private BufferedImage textureImage;
	
	/** To know if the colormap has changed and if we need to recreate a new colormap */
	private boolean hasChanged;
	
	/** Texture coordinates might not always be in the interval [0,1] */
	private double textureLeftCoord;
	private double textureRightCoord;
	
	/**
	 * Default constructor
	 */
	public TexturedColorMap() {
		super();
		colorMapTexture = null;
		textureImage = null;
		hasChanged = true;
	}
	
	/**
	 * static factory
	 * @return new instance of textured colormap
	 */
	public static TexturedColorMap create() {
		return new TexturedColorMap();
	}
	
	/**
	 * Set new data to the colorMap
	 * @param newData nbColor x 3 matrix containign the 3 channels
	 *                the matrix is stored rowwise like in Scilab
	 */
	@Override
	public void setData(double[] newData) {
		super.setData(newData);
		hasChanged = true;
	}
	
	/**
	 * Specify a new color for the colorMaps
	 * @param index index of the color to modify
	 * @param color array of size 3 containing the 3 channels
	 */
	public void setColor(int index, double[] color) {
		super.setColor(index, color);
		hasChanged = true;
	}
	
	/**
	 * Get the texture corresponding to the colormap
	 * @return 1D texture containing the colormap 
	 */
	public Texture getTexture() {
		
		if (colorMapTexture == null) {
			colorMapTexture = createTexture();
			updateTextureCoordinates();
			hasChanged = false;
		} else if (hasChanged) {
			
			updateColorMapTexture();
			updateTextureCoordinates();
			hasChanged = false;
		}
		
		return colorMapTexture;
	}
	
	/**
	 * Update the color map with a new image
	 */
	protected void updateColorMapTexture() {
		// need to recreate a new texture data
		colorMapTexture.updateImage(createTextureData());
		
		// also need to reset parameters
		setTextureParameters(colorMapTexture);
	}
	
	/**
	 * Update left most and right most texture coordinates
	 */
	protected void updateTextureCoordinates() {
		TextureCoords coords = colorMapTexture.getImageTexCoords();
		textureLeftCoord = coords.left();
		textureRightCoord = coords.right();
	}
	
	/**
	 * Set the default paramters for textures in Scilab.
	 * @param texture texture to update
	 */
	protected void setTextureParameters(Texture texture) {
		texture.setTexParameteri(GL.GL_TEXTURE_MAG_FILTER, GL.GL_NEAREST);
		texture.setTexParameteri(GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST);
		//texture.setTexParameteri(GL.GL_TEXTURE_WRAP_S, GL.GL_REPEAT);
		//texture.setTexParameteri(GL.GL_TEXTURE_WRAP_T, GL.GL_REPEAT);
	}
	
	/**
	 * Create a new texture of the colormap
	 * @return 1D texture containing the colormap
	 */
	private Texture createTexture() {
		Texture res = TextureIO.newTexture(getTextureData());
		setTextureParameters(res);
		return res;
	}
	
	/**
	 * @return The buffered image used to draw the texture
	 */
	private BufferedImage getImage() {
		if (textureImage == null || hasChanged) {
			// create a new image
			int colorMapSize = getSize();
			textureImage = new BufferedImage(colorMapSize + 2, 1, BufferedImage.TYPE_INT_RGB);
			
			// fill the image
			
			// white and black colors are put in front like in Scilab
			textureImage.setRGB(0, 0, WHITE_COLOR); // white
			textureImage.setRGB(1, 0, BLACK_COLOR); // black
			
			// remaining colors
			for (int i = 0; i < colorMapSize; i++) {	
				textureImage.setRGB(i + 2, 0, toRGBcolor(getRedChannel(i), getGreenChannel(i), getBlueChannel(i)));
			}
			
		}
		return textureImage;
	}
	
	/**
	 * Convert a color ginving its three channels between 0 and 1 to an integer value.
	 * The resulting int second byte is red channel, third byte is green and last is blue.
	 * @param red red channel
	 * @param green green chanel
	 * @param blue blue chanel
	 * @return int containg the three chanels
	 */
	private int toRGBcolor(double red, double green, double blue) {
		int rgbColor = 0x000000;
		rgbColor |= ((int) (red   * IMAGE_SCALING)) << RED_SHIFT;
		rgbColor |= ((int) (green * IMAGE_SCALING)) << GREEN_SHIFT;
		rgbColor |= ((int) (blue  * IMAGE_SCALING)) << BLUE_SHIFT;
		return rgbColor;
	}
	
	/**
	 * Create a new texture when colormap has been modified.
	 * @return a new texture data.
	 */
	private TextureData createTextureData() {
		return getTextureData();
	}
	
	/**
	 * Create a new data to be used by the texture.
	 * @return new texture data corresponding to the colormap.
	 */
	private TextureData getTextureData() {
		return TextureIO.newTextureData(getImage(), false);
	}
	
	/**
	 * Deallocate resources used by the texture
	 */
	public void clearTexture() {
		if (colorMapTexture != null) {
			colorMapTexture.dispose();
			colorMapTexture = null;
		}
	}
	
	/**
	 * Apply a texture on the facet
	 * @param gl current Gl pipeline
	 * @param colorIndex color index to apply
	 */
	public void applyTexCoord(GL gl, double colorIndex) {
		// apply color for GL2PS
		// it won't be used with JOGL driver.
		// We just get texture color with JOGL
		gl.glColor3dv(getColor((int) colorIndex), 0);
		
		// compute texture coordinate to use between 0 and 1.
		double clampValue;
		
		// use texture
		if (colorIndex > getSize() - COLOR_OFFSET) {
			// the index is whitin the two last colors (black and white)
			// However in the texture, they are stored in the two first colors
			clampValue = (getSize() + 1 - colorIndex + COLOR_OFFSET) / (getSize() + 2);
		} else {
			// color offset is here to put the index in the middle of the color
			// ie each color in the texture is defined between i / n and (i+1)/n
			// so put it to (i+0.5)/n
			clampValue = (colorIndex + 2 + COLOR_OFFSET) / (getSize() + 2);
		}
		
		// Use texture coordinates bounds to apply texture
		gl.glTexCoord1d((textureRightCoord - textureLeftCoord) * clampValue + textureLeftCoord);
		
	}
	
}
