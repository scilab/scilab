package org.scilab.modules.renderer.figureDrawing;

/**
 * Abstract Class allows to export screen-shots in format Bitmap (BMP, GIF, JPG, PNG and PPM)
 * @author Sylvestre Koumar
 *
 */
public abstract class ExportToFile {	
	
	/** Code-number for each bitmap format */
	public static final int BMP_EXPORT = 1;
	public static final int GIF_EXPORT = 2;
	public static final int JPG_EXPORT = 3;
	public static final int PNG_EXPORT = 4;
	public static final int PPM_EXPORT = 5;
	
	protected static final String INVALID_FILE = "File name not supported";

	
	/** Width and height of the figure  */
	private int width;
	private int height;
	/** Name of the file we're going to export */
	private String filename; 
	/** Type of the file we're going to export */
	private int filetype; 
		
	/**
	 * Default Constructor
	 * @param filename name of the exported file
	 * @param filetype type of the exported file
	 */
	protected ExportToFile(String filename, int filetype) {
		this.filename = filename;
		this.filetype = filetype;
	}

	/**
	 * Get width of the figure
	 * @return width which is an integer
	 */
	public int getWidth() {
		return width;
	}

	/**
	 * Set width of the figure
	 * @param width width in pixel of the image
	 * @param height height in pixel of the image
	 */
	public void setFileSize(int width, int height) {
		this.width = width;
		this.height = height;
	}

	/**
	 * Get height of the figure
	 * @return height which is an integer
	 */
	public int getHeight() {
		return height;
	}

	/**
	 * Get filename of the screen-shot
	 * @return filename which is a String
	 */
	public String getFilename() {
		return filename;
	}

	/**
	 * Set filename of the screen-shot
	 * @param filename must be a String
	 */
	public void setFilename(String filename) {
		this.filename = filename;
	}

	/**
	 * Get  filetype of the screen-shot
	 * @return filetype which is an integer
	 */
	public int getFiletype() {
		return filetype;
	}

	/**
	 * Set filetype of the screen-shot
	 * @param filetype must be an integer
	 */
	public void setFiletype(int filetype) {
		this.filetype = filetype;
	}
	
	/**
	 * Create a bitmap file which is the screen-shot of the figure
	 */
	public abstract void exportToBitmap();
	
	/**
	 * Choose which kind of filetype will be exported /*
	 * @param filetype type of the file
	 * @param fileName name of the file
	 * @return new ExportBitmap of
	 */
	public static ExportToFile createExporter(String fileName, int filetype) {	
		/** Select in which type the file will be exported */
		switch (filetype) {
		case BMP_EXPORT:  
		case GIF_EXPORT:
		case JPG_EXPORT:
		case PNG_EXPORT:
			return new ExportBitmap(fileName, filetype);
		case PPM_EXPORT:
			return new ExportPPM(fileName, filetype);	  
		default: System.err.println(INVALID_FILE);
		}
		return null;		
	}
}
