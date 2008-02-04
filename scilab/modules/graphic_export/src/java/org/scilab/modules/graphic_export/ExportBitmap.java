package org.scilab.modules.graphic_export;

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
			Screenshot.writeToFile(file, getWidth(), getHeight());			
		} catch (GLException ex1) {
			return ExportRenderer.GLEXCEPTION_ERROR;
		} catch (IOException ex2) {
			return ExportRenderer.IOEXCEPTION_ERROR;			
		}
		return ExportRenderer.SUCCESS;			
	}
}

