package org.scilab.modules.renderer.figureDrawing;

import java.awt.image.BufferedImage;
import java.io.FileOutputStream;
import java.io.IOException;
import javax.media.opengl.GLException;
import org.scilab.modules.renderer.convertToPPM.PPMEncoder;
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
	 */
	public void exportToBitmap() {		
		try {
			o = new FileOutputStream(getFilename() + ".ppm");
			image = Screenshot.readToBufferedImage(getWidth(), getHeight());

			encoder = new PPMEncoder(image, o);
			encoder.encodeStart(image.getWidth(), image.getHeight());
			int[] pixels = image.getRGB(0, 0, image.getWidth(), image.getHeight(), null, 0, image.getWidth());
			encoder.encodePixels(0, 0, image.getWidth(), image.getHeight(),	pixels, 0, image.getWidth());
		} catch (GLException e1) {
			e1.printStackTrace();
		} catch (IOException e1) {
			e1.printStackTrace();
		}			
	}
}
