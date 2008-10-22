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
import java.io.FileOutputStream;
import java.io.IOException;
import javax.media.opengl.GLException;
import org.scilab.modules.graphic_export.convertToPPM.PPMEncoder;
import com.sun.opengl.util.Screenshot;

/**
 * Class which allows to export screen-shots in format PPM
 * @author Sylvestre Koumar
 *
 */
public class ExportPPM  extends ExportToFile {	

	/** Encode to PPM File */
	private PPMEncoder encoder;
	/** Create a BufferedImage */
	private BufferedImage image;
	/** Create a FileOutputStream */
	private FileOutputStream o;

	/**
	 * Default Constructor
	 * @param filename name of the exported file
	 * @param filetype type of the exported file
	 */
	public ExportPPM(String filename, int filetype) {
		super(filename, filetype);
	}	
	
	/**
	 * Create a PPM file which is the screen-shot of the figure
	 * @return a int which is a type of error
	 */
	public int exportToBitmap() {		
		try {			
			/** FileOutputStream contain's the figure */
			o = new FileOutputStream(getFilename() + ".ppm");
			image = Screenshot.readToBufferedImage(getWidth(), getHeight());

			/** Will encode image into PPM format */
			encoder = new PPMEncoder(image, o);
			encoder.encodeStart(image.getWidth(), image.getHeight());
			int[] pixels = image.getRGB(0, 0, image.getWidth(), image.getHeight(), null, 0, image.getWidth());
			encoder.encodePixels(0, 0, image.getWidth(), image.getHeight(),	pixels, 0, image.getWidth());
		} catch (GLException e1) {
			return ExportRenderer.UNKNOWN_GLEXCEPTION_ERROR;
		} catch (IOException e1) {
			return ExportRenderer.IOEXCEPTION_ERROR;
		}
		return ExportRenderer.SUCCESS;			
	}
}
