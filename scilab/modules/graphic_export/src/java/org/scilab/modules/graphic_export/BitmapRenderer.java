/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : GLEventListener used to export figures into files
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;

/**
 * GLEventListener used to export figures into files
 * @author Jean-Baptiste Silvy
 */
public class BitmapRenderer  extends ExportRenderer {

	private static String fileName;

        private float jpegCompressionQuality;

	/**
	 * Default constructor
	 * @param fileName name of the figure to render
	 * @param fileType type of the figure to render
	 * @param fileOrientation orientation of the figure to render
	 */
	public BitmapRenderer(String fileName, int fileType, float jpegCompressionQuality, int fileOrientation) {
		super(fileName, fileType, fileOrientation);
		this.jpegCompressionQuality = jpegCompressionQuality;
	}
	
	/**
	 * getter
	 * @return fileName name of the file
	 */
	public static String getFileName() {
		return fileName;
	}

	/**
	 * setter
	 * @param fileName name of the file
	 */
	public void setFileName(String fileName) {
		this.fileName = fileName;
	}

	/**
	 * Set the name and type of the file to render
	 * @param fileName name of the file to render
	 * @param fileType type of the file to render
	 */
	public void setRenderedFile(String fileName, int fileType) {
		this.fileName = fileName;
		super.setFileType(fileType);
	}

	/** Called by the drawable to initiate OpenGL rendering by the client.
	 * After all GLEventListeners have been notified of a display event, the 
	 * drawable will swap its buffers if necessary.
	 * @param gLDrawable The GLDrawable object.
	 */    
	public void display(GLAutoDrawable gLDrawable) {
		// normaly it is not needed to render the scene again
		// it should be already in the front buffer
		//super.display(gLDrawable);		
		
		// export it to a file
		GL gl = gLDrawable.getGL();
		
		// use the lastly modified buffer
		// first saved the current one
		int[] currentBuffer = {GL.GL_BACK};
		gl.glGetIntegerv(GL.GL_READ_BUFFER, currentBuffer, 0);
		
		// set the dispayed one
		gl.glReadBuffer(GL.GL_FRONT);
		
		
		ExportToFile export = ExportToFile.createExporter(super.getFileName(), super.getFileType(), jpegCompressionQuality);
		export.setFileSize(gLDrawable.getWidth(), gLDrawable.getHeight());
		export.exportToBitmap();
		
		setErrorNumber(export.exportToBitmap());
		
		// back to default value
		gl.glReadBuffer(currentBuffer[0]);  

		// one shot renderer
		gLDrawable.removeGLEventListener(this);
	}

	/** Called when the display mode has been changed.  <B>!! CURRENTLY UNIMPLEMENTED IN JOGL !!</B>
	 * @param gLDrawable The GLDrawable object.
	 * @param modeChanged Indicates if the video mode has changed.
	 * @param deviceChanged Indicates if the video device has changed.
	 */
	public void displayChanged(GLAutoDrawable gLDrawable, boolean modeChanged, boolean deviceChanged) { }

	/** Called by the drawable immediately after the OpenGL context is 
	 * initialized for the first time. Can be used to perform one-time OpenGL 
	 * initialization such as setup of lights and display lists.
	 * @param gLDrawable The GLDrawable object.
	 */
	public void init(GLAutoDrawable gLDrawable) {
		// nothing needs to be initialized, we just export
	}

	/** Called by the drawable during the first repaint after the component has 
	 * been resized. The client can update the viewport and view volume of the 
	 * window appropriately, for example by a call to 
	 * GL.glViewport(int, int, int, int); note that for convenience the component
	 * has already called GL.glViewport(int, int, int, int)(x, y, width, height)
	 * when this method is called, so the client may not have to do anything in
	 * this method.
	 * @param gLDrawable The GLDrawable object.
	 * @param x The X Coordinate of the viewport rectangle.
	 * @param y The Y coordinate of the viewport rectanble.
	 * @param width The new width of the window.
	 * @param height The new height of the window.
	 */
	public void reshape(GLAutoDrawable gLDrawable, int x, int y, int width, int height) { 
		
	}	
}
