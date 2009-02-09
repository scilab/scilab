/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils;

import java.awt.Dimension;
import java.awt.DisplayMode;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Toolkit;

import javax.media.opengl.GL;


/**
 * Utilitary class used to retrieve data from the rendering support
 * (screen, graphic capabilities,...)
 * @author Jean-Baptiste Silvy
 */
public final class RenderingCapabilities {

	/** 
	 * Maximum width and heigth usable for an GLJPanel.
	 * Needed to be updated when OpenGL is available.
	 */
	private static int[] maxCanvasSize;
	
	/**
	 * Should not be called
	 */
	protected RenderingCapabilities() {
		maxCanvasSize = null;
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Get the amount of memory java can allocate.
	 * @return Available memory size in byte.
	 */
	public static long getAvailableJavaMemory() {
		Runtime runtime = Runtime.getRuntime();
		long possibleAllocation = runtime.maxMemory() - runtime.totalMemory();
		long freeHeapSpace = runtime.freeMemory();
		return possibleAllocation + freeHeapSpace;

	}
	
	/**
	 * Get the size of the screen in pixel.
	 * @return Array of size 2 containing width and height.
	 */
	public static int[] getScreenSize() {
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		int[] res = {screenSize.width, screenSize.height};
		return res;
	}
	
	/**
	 * Get the maximum size that a window can have.
	 * @return maximulm size that can be asigned to a window.
	 */
	public static int[] getMaxWindowSize() {
		GraphicsDevice[] screens = GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices();
		int[] res = {0, 0};
		// take the sum of each screen width and screen height.
		// It might be a bit large, but it's better than using a too small value.
		for (int i = 0; i < screens.length; i++) {
			DisplayMode dm = screens[i].getDisplayMode();
			if (dm == null) {
				// workaround for bug 3547
				dm = screens[i].getDisplayModes()[0];
			}
			res[0] += dm.getWidth();
			res[1] += dm.getHeight();
		}
		
		return res;
		
	}
	
	/**
	 * Get the maximum width and height for a canvas.
	 * By default return the maximum window size. However it advised to wait until
	 * the values can be updated using OpenGL values.
	 * @return array of size 2 containing maximum width and height
	 */
	public static int[] getMaxCanvasSize() {
		
		if (maxCanvasSize != null) {
			int[] res = {maxCanvasSize[0], maxCanvasSize[1]};
			return res;
		} else {
			// canvas not already created, don't set limits
			int[] res = {Integer.MAX_VALUE, Integer.MAX_VALUE};
			return res;
		}
	}
	
	/**
	 * Update the value of max canvas size by retrieving OpenGL values.
	 * @param gl valid pointer to a GL object.
	 */
	public static void updateMaxCanvasSize(GL gl) {
		if (maxCanvasSize == null) {
			// get the values from OpenGL
			// use the advice posted in the JOGL forum:
			// http://www.javagaming.org/index.php/topic,19156.0.html
			maxCanvasSize = new int[2];
			
			// getting max viewport size
			gl.glGetIntegerv(GL.GL_MAX_VIEWPORT_DIMS, maxCanvasSize, 0);
			
			// try to estimate maximum size for the GLJPanel
			int[] maxRenderBufferSize = {0, 0};
			
			// available only if GL_EXT_framebuffer_object extension is available
			if (gl.isExtensionAvailable("GL_EXT_framebuffer_object")) {
				
				// only one value is retrieved
				gl.glGetIntegerv(GL.GL_MAX_RENDERBUFFER_SIZE_EXT, maxRenderBufferSize, 0);
				
				maxRenderBufferSize[1] = maxRenderBufferSize[0];
				
			}
			
			if (maxRenderBufferSize[0] == 0 || maxRenderBufferSize[1] == 0) {
				// frame buffer extension might not be available or
				// getting GL_MAX_RENDERBUFFER_SIZE_EXT might fail (this happens
				// with some MESA drivers revisions).
				// In this case update with max texture size
				
				// one value is retrieved
				gl.glGetIntegerv(GL.GL_MAX_TEXTURE_SIZE, maxRenderBufferSize, 0);
				maxRenderBufferSize[1] = maxRenderBufferSize[0];
			}
			
			if (maxRenderBufferSize[0] == 0 || maxRenderBufferSize[1] == 0) {
				// still errors, finally use the screen size
				maxRenderBufferSize = getScreenSize();
			}
			
			// update values in consequences
		    maxCanvasSize[0] = Math.min(maxCanvasSize[0], maxRenderBufferSize[0]);
			maxCanvasSize[1] = Math.min(maxCanvasSize[1], maxRenderBufferSize[0]);
		}
		// otherwise values are already retrieved.
		
	}
}
