/*------------------------------------------------------------------------*/
/* file: FileExporter.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Static class used to create file export of graphic figures      */
/*------------------------------------------------------------------------*/


package org.scilab.modules.graphic_export;


import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * Static class used to create file export of graphic figures
 * @author Jean-Baptiste Silvy
 */
public class FileExporter {

	/**
	 * Defualt constuctor
	 */
	protected FileExporter() {
		
	}
	
	/**
	 * Export a figure into a file
	 * @param figureIndex index of tghe figure to export
	 * @param fileName name of the file to create
	 * @param fileType kind of the file
	 * @return 0 if everything worked fine, a non null integer if an exception occured
	 *         depending on the kind of error
	 */
	public static int fileExport(int figureIndex, String fileName, int fileType) {
		DrawableFigureGL exportedFig = FigureMapper.getCorrespondingFigure(figureIndex);
		
		if (exportedFig == null) {
			// figure no longer exists
			return ExportToFile.IOEXCEPTION_ERROR;
		}
		
		
		ExportRenderer export = new ExportRenderer(figureIndex);
		export.setRenderedFile(fileName, fileType);
		
		// add a glevent linstener dedicated at exporting files
		exportedFig.getRenderingTarget().addGLEventListener(export);
		
		// will draw the scene with the standard Gl eventlistener and then export it
		// with the added one
		exportedFig.drawCanvas();
		
		// remove the exporter
		exportedFig.getRenderingTarget().removeGLEventListener(export);
		return ExportToFile.SUCCESS;
	}
	
}
