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
			return ExportRenderer.IOEXCEPTION_ERROR;
		}		
		
		ExportRenderer export;		
		export = ExportRenderer.createExporter(figureIndex, fileName, fileType);
				
		exportedFig.getRenderingTarget().addGLEventListener(export);
		exportedFig.drawCanvas();
		exportedFig.getRenderingTarget().removeGLEventListener(export);

		return ExportRenderer.SUCCESS;		
	}	
}
