/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.util.ArrayList;
import java.util.Arrays;

import javax.media.opengl.GLEventListener;
import com.sun.opengl.util.FileUtil;

/**
 * Main export class
 * @author Sylvestre Koumar
 *
 */
public abstract class ExportRenderer implements GLEventListener {

	/** Code-number for the function statue */
	public static final int SUCCESS = 0;
	public static final int UNKNOWN_GLEXCEPTION_ERROR = 1;
	public static final int IOEXCEPTION_ERROR = 2;
	public static final int INVALID_FILE = 3;
	public static final int GL2PS_ERROR = 4;
	public static final int GL2PS_OVERFLOW = 5;
	public static final int GL2PS_UNINITIALIZED = 6;
	
	/** Code-number for each bitmap format */
	public static final int BMP_EXPORT = 1;
	public static final int GIF_EXPORT = 2;
	public static final int JPG_EXPORT = 3;
	public static final int PNG_EXPORT = 4;
	public static final int PPM_EXPORT = 5;

	/** Code-number for each postscript format */
	public static final int EPS_EXPORT = 6;
	public static final int PDF_EXPORT = 7;
	public static final int SVG_EXPORT = 8;
	public static final int PS_EXPORT = 9;
	
	/** Orientation of the exported figure */
	public static final int PORTRAIT = 0;
	public static final int LANDSCAPE = 1;

	/** File name & file type */
	private static String fileName;
	private static int fileType;
	private static int fileOrientation;

	/** File extension */
	private static String fileExtension;

	/** give the type of the error */
	private static int errorNumber;

	/**
	 * Constructor
	 * @param fileName name of the file
	 * @param fileType type of the file
	 * @param fileOrientation orientation of the file
	 */
	protected ExportRenderer(String fileName, int fileType, int fileOrientation) {
		this.fileName = fileName;
		this.fileType = fileType;	
		this.fileOrientation = fileOrientation;
		this.fileExtension = "";
		removeExtension();
	}	

	/**
	 * Choose which kind of filetype will be exported 
	 * @param figureIndex type of the file
	 * @param fileName name of the file
	 * @param fileType type of the file
	 * @param fileOrientation orientation of the file
	 * @return GL2PSRenderer export a postscript screen-shot
	 */
	public static ExportRenderer createExporter(int figureIndex, String fileName, int fileType, int fileOrientation) {
		
		GL2PS gl2ps = new GL2PS();
		
		/** Select in which type the file will be exported */		
		switch (fileType) {
		case BMP_EXPORT:  
		case GIF_EXPORT:
		case JPG_EXPORT:
		case PNG_EXPORT:
		case PPM_EXPORT:
			return new BitmapRenderer(fileName, fileType, fileOrientation);
		case EPS_EXPORT:
		case PDF_EXPORT:
		case SVG_EXPORT:
		case PS_EXPORT:
			return new GL2PSRenderer(figureIndex, fileName, fileType, fileOrientation);
		default: System.err.println(ExportRenderer.INVALID_FILE);
		}
		return null;			
	}

	/**
	 * getter 
	 * @return fileName get the file name
	 */
	public static String getFileName() {
		return fileName;
	}

	/**
	 * setter 
	 * @param fileName set the file name
	 */
	public void setFileName(String fileName) {
		this.fileName = fileName;
	}

	/**
	 * getter
	 * @return fileType get the file type
	 */
	public static int getFileType() {
		return fileType;
	}

	/**
	 * setter 
	 * @param fileType set the file name
	 */
	public void setFileType(int fileType) {
		this.fileType = fileType;
	}	

	/**
	 * @return the fileOrientation
	 */
	public static int getFileOrientation() {
		return fileOrientation;
	}

	/**
	 * @param fileOrientation the fileOrientation to set
	 */
	public static void setFileOrientation(int fileOrientation) {
		ExportRenderer.fileOrientation = fileOrientation;
	}

	/**
	 * getter 
	 * @return the fileExtension
	 */
	public static String getFileExtension() {
		return fileExtension;
	}

	/**
	 * setter 
	 * @param fileExtension the fileExtension to set
	 */
	public void setFileExtension(String fileExtension) {
		this.fileExtension = fileExtension;
	}

	/**
	 * Function allowing to format the extension of the screen-shot file  
	 */
	public void removeExtension() {		
		String suffix = FileUtil.getFileSuffix(this.fileName); //get the suffix(extension) of the file name
		int pos = this.fileName.lastIndexOf('.'); // position of the dot

		if (suffix != null) {

			String[] extensionsAllowed = {"bmp", "gif", "jpg", "jpeg", "png", "ppm", "eps", "pdf", "svg", "ps"};

			Integer[] fileTypeAllowed = {ExportRenderer.BMP_EXPORT, ExportRenderer.GIF_EXPORT, 
										 ExportRenderer.JPG_EXPORT, ExportRenderer.PNG_EXPORT, 
										 ExportRenderer.PPM_EXPORT, ExportRenderer.EPS_EXPORT, 
										 ExportRenderer.PDF_EXPORT, ExportRenderer.SVG_EXPORT, 
										 ExportRenderer.PS_EXPORT};

			suffix = suffix.toLowerCase();

			/* Transform the array to vector to have access to search methods*/

			ArrayList<String> extensionsAllowedV = new ArrayList<String>(Arrays.asList(extensionsAllowed));
			ArrayList<Integer> fileTypeAllowedV = new ArrayList(Arrays.asList(fileTypeAllowed));

			if (extensionsAllowedV.contains(suffix) && fileTypeAllowedV.contains(this.fileType)){
				this.fileName = this.fileName.substring(0, pos); /* Physically removed the extension */
				this.fileExtension = suffix; /* Store the extension... we want the same as used initially (ex: jpeg) */
			} 
		}
	}

	/**
	 * get the number of the error
	 * @return errorNumber the number of the error
	 */
	public static int getErrorNumber() {
		return errorNumber;
	}

	/**
	 * set the number of the error
	 * @param errorNumber the number of the error
	 */
	public static void setErrorNumber(int errorNumber) {
		ExportRenderer.errorNumber = errorNumber;
	}
}
