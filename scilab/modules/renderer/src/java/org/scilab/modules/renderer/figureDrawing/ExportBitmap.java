package org.scilab.modules.renderer.figureDrawing;

import java.io.File;
import java.io.IOException;

import javax.media.opengl.GLException;

import com.sun.opengl.util.Screenshot;

/**
 * Class which allows to export screen-shots in format Bitmap (BMP, GIF, JPG, PNG and PPM)
 * @author Sylvestre Koumar
 *
 */
public class ExportBitmap extends ExportToFile {	
		
	/** File which contains the screen-shot */
	private File file;
	
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
	 */
	public void exportToBitmap() {				
		
		switch (getFiletype()) {
		case BMP_EXPORT:  file = new File(getFilename() + ".bmp");
		break;
		case GIF_EXPORT:  file = new File(getFilename() + ".gif");
		break;
		case JPG_EXPORT:  file = new File(getFilename() + ".jpg");
		break;
		case PNG_EXPORT:  file = new File(getFilename() + ".png");
		break;					  
		default: System.err.println(INVALID_FILE);
		return;
		}

		try {
			Screenshot.writeToFile(file, getWidth(), getHeight());
		} catch (GLException ex1) {
			System.err.println("Error: GLException");
			ex1.printStackTrace();

		} catch (IOException ex2) {
			System.err.println(ExportToFile.INVALID_FILE);
			ex2.printStackTrace();
		}			
	}
}

