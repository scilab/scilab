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
	
	private Texture colorMapTexture;
	private BufferedImage textureImage;
	
	/** To konw if the colormap has changed and if we need to recreate a new colormap */
	private boolean hasChanged;
	
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
			hasChanged = false;
		} else if (hasChanged) {
			// need to recreate a new texture data
			colorMapTexture.updateImage(createTextureData());
			hasChanged = false;
		}
		
		return colorMapTexture;
	}
	
	/**
	 * Create a new texture of the colormap
	 * @return 1D texture containing the colormap
	 */
	private Texture createTexture() {
		Texture res = TextureIO.newTexture(getTextureData());
		res.setTexParameteri(GL.GL_TEXTURE_MAG_FILTER, GL.GL_NEAREST);
		res.setTexParameteri(GL.GL_TEXTURE_MIN_FILTER, GL.GL_NEAREST);
		return res;
	}
	
	/**
	 * @return The buffered image used to draw the texture
	 */
	private BufferedImage getImage() {
		if (textureImage == null) {
			// create a new image
			int colorMapSize = getSize();
			textureImage = new BufferedImage(colorMapSize, 1, BufferedImage.TYPE_INT_RGB);
			
			// fill the image
			for (int i = 0; i < colorMapSize; i++) {	
				textureImage.setRGB(i, 0, toRGBcolor(getRedChannel(i), getGreenChannel(i), getBlueChannel(i)));
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
		textureImage = null;
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
	 * Desalocate ressources used by the texture
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
		// use texture
		// color offset is here to put the index in the missdle of the color
		gl.glTexCoord1d((colorIndex - COLOR_OFFSET) / getSize());
	}
	
}
