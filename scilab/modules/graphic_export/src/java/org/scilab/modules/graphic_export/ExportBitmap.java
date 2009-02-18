/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLException;

import com.sun.opengl.impl.GLContextImpl;
import com.sun.opengl.util.ImageUtil;
import com.sun.opengl.util.Screenshot;

/**
 * Class which allows to export screen-shots in format Bitmap (BMP, GIF, JPG, PNG and PPM)
 * @author Sylvestre Koumar
 *
 */
public class ExportBitmap extends ExportToFile {	

	/** File which contains the screen-shot */
	private File file;
	private BufferedImage dump;
	private String fileExtension;

	/**
	 * Default Constructor
	 * @param filename name of the exported file
	 * @param filetype type of the exported file
	 */
	public ExportBitmap(String filename, int filetype) {
		super(filename, filetype);			
	}	

	/**
	 * Create a bitmap file which is the screen-shot of the figure
	 * @return a int which is a type of error
	 */
	public int exportToBitmap() {				

		/** Select the screen-shot format */		
		switch (getFiletype()) {
		case ExportRenderer.BMP_EXPORT:  file = new File(getFilename() + ".bmp");
		break;
		case ExportRenderer.GIF_EXPORT:  file = new File(getFilename() + ".gif");
		break;
		case ExportRenderer.JPG_EXPORT:  file = new File(getFilename() + ".jpg");
		break;
		case ExportRenderer.PNG_EXPORT:  file = new File(getFilename() + ".png");
		break;					  
		default: return ExportRenderer.INVALID_FILE;
		}

		try {			
			/** Generate the screen-shot */		
			//Check if we have the permission to export			
			if (checkWritePermission(file) == ExportRenderer.SUCCESS) {
				dump = Screenshot.readToBufferedImage(getWidth(), getHeight());
				//flip the screen-shot if the dump is mirrored
				dumpFlip();
			} else {
				return ExportRenderer.INVALID_FILE;
			}
		} catch (GLException ex1) {
			return ExportRenderer.UNKNOWN_GLEXCEPTION_ERROR;
		}
		
		return ExportRenderer.SUCCESS;			
	}

	/**
	 * Check if we have the permission to export on this file
	 * @param file exported file
	 * @return permission status
	 */
	public int checkWritePermission(File file) {
		try {
			file.createNewFile();			
			if (!file.canWrite()) {
				return ExportRenderer.INVALID_FILE;
			}
			return ExportRenderer.SUCCESS;
		} catch (IOException e1) {
			return ExportRenderer.IOEXCEPTION_ERROR;
		} catch (SecurityException e2) {
			return ExportRenderer.INVALID_FILE;
		}
	}

	/**
	 * flip the screen-shot if it's mirrored
	 * @return result of dumpFlip (success or fail)
	 */
	public int dumpFlip() {
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
			ImageUtil.flipImageVertically(dump);
		}
		try {
			ImageIO.write(dump, getFileExtension(), file);
		} catch (IOException e) {
			return ExportRenderer.IOEXCEPTION_ERROR;
		}
		return ExportRenderer.SUCCESS;
	}

	/**
	 * Gives the extension of a file name
	 * @return the extension of the file
	 */
	public String getFileExtension() {		
		
		String fileName = file.getName().toLowerCase();
		fileExtension = fileName.substring(fileName.lastIndexOf('.') + 1, fileName.length());
		return fileExtension;
	}	

}

