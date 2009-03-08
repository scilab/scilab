/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.image.BufferedImage;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;

import org.scilab.modules.graphic_export.ExportRenderer;

import com.sun.opengl.impl.GLContextImpl;
import com.sun.opengl.util.ImageUtil;
import com.sun.opengl.util.Screenshot;

/**
 * GLEventListener whose display function export the content of a glDrawable into
 * an image
 * @author Jean-Baptiste Silvy
 */
public class ImageExportRenderer implements GLEventListener {

	private BufferedImage exportedImage;
	
	/**
	 * Default constructor
	 */
	public ImageExportRenderer() {
		exportedImage = null;
	}
	
	/**
	 * Export the content of a GLAutoDrawable into an image
	 * @param canvas GLAutoDrawable to export
	 * @return new image, copy of the inside of the GLCanvas
	 */
	public BufferedImage exportToImage(GLAutoDrawable canvas) {
		// add the export renderer
		canvas.addGLEventListener(this);
		
		// force a draw of the canvas so the exportedImage will be updated
		canvas.display();
		
		// end remove the listener
		canvas.removeGLEventListener(this);
		
		BufferedImage res = exportedImage;
		
		exportedImage = null;
		
		return res;
	}
	
	/**
	 * Main function called whenever a rendering ids required
	 * @param canvas canvas in which the renreding will be done.
	 */
	public void display(GLAutoDrawable canvas) {
		
		GL gl = canvas.getGL();
		
		// use the lastly modified buffer
		canvas.getContext().makeCurrent();
		
		// Copy buffer into exported image
		exportedImage = Screenshot.readToBufferedImage(canvas.getWidth(), canvas.getHeight());
		
		//flip the screen-shot if the dump is mirrored
		dumpFlip();
		
		// back to default value
		canvas.getContext().release();
		
	}
	
	/**
	 * Unused
	 * @param canvas canvas in which the renreding will be done.
	 * @param modeChanged ???
	 * @param deviceChanged ????
	 */
	public void displayChanged(GLAutoDrawable canvas, boolean modeChanged, boolean deviceChanged) { }

	/**
	 * Called when the canvas is reinitialized
	 * @param canvas GLAutoDrawable to export
	 */
	public void init(GLAutoDrawable canvas) {
		// nothing to do everything is done in the display function
	}

	/**
	 * Called when the canvas is reshaped
	 * @param canvas canvas in which the renreding will be done.
	 * @param x x position of new viewport
	 * @param y y position of new viewport
	 * @param width width of viewport
	 * @param height heigth of viewport
	 */
	public void reshape(GLAutoDrawable canvas, int x, int y, int width, int height) {
		// nothing to do
	}
	
	/**
	 * flip the screen-shot if it's mirrored
	 * @return result of dumpFlip (success or fail)
	 */
	public void dumpFlip() {
		// check if it was the case
		boolean needFlip;
		try {
			// raises an exception if hardware acceleration is on
			needFlip = !((GLContextImpl)GLContext.getCurrent()).offscreenImageNeedsVerticalFlip();
		} catch (GLException e) {
			// hardware acceleration is on
			needFlip = false;
		}
		if (needFlip) {
			// flip it back
			ImageUtil.flipImageVertically(exportedImage);
		}
	}

}
