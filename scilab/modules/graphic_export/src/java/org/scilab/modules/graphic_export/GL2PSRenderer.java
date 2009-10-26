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

import java.io.File;
import java.io.IOException;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;

import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.arcDrawing.FastArcRendererFactory;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.figureDrawing.SciRenderer;
import org.scilab.modules.renderer.polylineDrawing.GL2PSShadeFacetDrawer;


/**
 * 
 * @author Sylvestre Koumar
 *
 */
public class GL2PSRenderer extends ExportRenderer {
	
	/** BufferSize WIDTH*/
	public static final int BUFFER_WIDTH = 2048;
	/** BufferSize HEIGHT*/
	public static final int BUFFER_HEIGHT = 2048;
	
	private SciRenderer sciRend;
	private int figureIndex;
	private int format;

	
	/**
	 * GL2PSRenderer
	 * @param figureIndex index of the figure
	 * @param fileName file name
	 * @param fileType file type
	 * @param fileOrientation file orientation
	 */
	public GL2PSRenderer(int figureIndex, String fileName, int fileType, int fileOrientation) {
		super(fileName, fileType, fileOrientation);
		this.figureIndex = figureIndex;
		setGL2PSFile(fileName, fileType);		
	}
	
	/**
	 * setGL2PSFile 
	 * @param fileName fileName
	 * @param fileType fileName
	 * @return ExportRenderer.INVALID_FILE if the file is not valid
	 */
	public int setGL2PSFile(String fileName, int fileType) {
				
		switch (getFileType()) {
		case ExportRenderer.EPS_EXPORT:  setFileName(ExportRenderer.getFileName() + ".eps");
									   format = GL2PS.GL2PS_EPS;
		break;
		case ExportRenderer.PDF_EXPORT:  setFileName(ExportRenderer.getFileName() + ".pdf");
									   format = GL2PS.GL2PS_PDF;
		break;
		case ExportRenderer.SVG_EXPORT:  setFileName(ExportRenderer.getFileName() + ".svg");
									   format = GL2PS.GL2PS_SVG;
		break;				  
		case ExportRenderer.PS_EXPORT:  setFileName(ExportRenderer.getFileName() + ".ps");
		   							   format = GL2PS.GL2PS_PS;
		break;	
		default: return ExportRenderer.INVALID_FILE;
		}
		return ExportRenderer.SUCCESS;			
	}
	
	/**
	 * display
	 * @param gLDrawable gLDrawable
	 */
	public void display(GLAutoDrawable gLDrawable) {
		
		GL2PS gl2ps = new GL2PS();
		

		int buffsize = BUFFER_WIDTH * BUFFER_HEIGHT;			
		sciRend = new SciRenderer(this.figureIndex);
		
		setErrorNumber(ExportRenderer.SUCCESS);
				
		//Check if we have the permission to export
		File filePermission = new File(ExportRenderer.getFileName());
		if (checkWritePermission(filePermission) == ExportRenderer.SUCCESS) {
			
			DrawableFigureGL exportedFigure = FigureMapper.getCorrespondingFigure(figureIndex);
			
			//int[] viewport = exportedFigure.getViewport();
			//int[] viewport = {0, 0, 780, 600};
			
			int exportOrientation;
			if (ExportRenderer.getFileOrientation() == 1) {
				exportOrientation = GL2PS.GL2PS_LANDSCAPE;
			} else {
				exportOrientation = 0;
			}						
			
			/* Modified by Calixte to enable blending (for alpha channel) when exporting LaTeX or MathML
			   labels (which are pixmap).
			   I'm obliged to put the enabling here because, the property g2lps->blending is set in
			   gl2psBeginPage.
			   Perhaps, it would be a good idea to modify gl2ps.c to enable blending by default (it's the case for SVG export)...
			 */
			GL gl = gLDrawable.getGL();
			gl.glEnable(gl.GL_BLEND);
			/* End */

			int gl2psBeginPageStatut = gl2ps.gl2psBeginPage(exportedFigure.getTitle(), "Scilab", null, format, 
					GL2PS.GL2PS_SIMPLE_SORT, GL2PS.GL2PS_USE_CURRENT_VIEWPORT | GL2PS.GL2PS_BEST_ROOT
					| GL2PS.GL2PS_SIMPLE_LINE_OFFSET | GL2PS.GL2PS_DRAW_BACKGROUND | exportOrientation,
					GL.GL_RGBA, 0, null, null, null, null, 
					0, 0, 0, buffsize, ExportRenderer.getFileName());	

			if (gl2psBeginPageStatut != GL2PS.GL2PS_SUCCESS) {
				//Get the GL2PS error and convert it into an export error
				setExportErrorFromGL2PS(gl2psBeginPageStatut);
				return;
			}

			GL2PSGL newGL = new GL2PSGL(gl, gl2ps);
			gLDrawable.setGL(newGL);

			
			exportedFigure.setTextRendererFactory(new PSTextRendererFactory());
			exportedFigure.setArcRendererFactory(new FastArcRendererFactory());
			exportedFigure.setShadeFacetDrawer(new GL2PSShadeFacetDrawer());
			exportedFigure.disableAntialiasing();
			
			
			sciRend.init(gLDrawable);
			sciRend.display(gLDrawable);
			
			
			int gl2psEndPageStatut = gl2ps.gl2psEndPage();
			
			gLDrawable.setGL(gl);
			exportedFigure.setDefaultArcRendererFactory();
			exportedFigure.setDefaultTextRenderer();
			exportedFigure.setDefaultShadeFacetDrawer();
			exportedFigure.enableAntialiasing();

			if (gl2psEndPageStatut != GL2PS.GL2PS_SUCCESS && gl2psEndPageStatut != GL2PS.GL2PS_NO_FEEDBACK) {
				// No feedback just mean that the feedback remains empty ie that the figure is empty
				// Get the GL2PS error and convert it into an export error
				setExportErrorFromGL2PS(gl2psEndPageStatut);
				return;
			}		
			
			sciRend.init(gLDrawable);
			sciRend.display(gLDrawable);	

		} else {
			setErrorNumber(ExportRenderer.INVALID_FILE);
		}		
	}


	/**
	 * GLEventListener method
	 * @param arg0 GLAutoDrawable
	 * @param arg1 boolean
	 * @param arg2 boolean
	 */
	public void displayChanged(GLAutoDrawable arg0, boolean arg1, boolean arg2) {
		
	}

	/**
	 * GLEventListener method
	 * @param arg0 GLAutoDrawable
	 */
	public void init(GLAutoDrawable arg0) {
		
	}

	/**
	 * GLEventListener method
	 * @param arg0 GLAutoDrawable
	 * @param arg1 int
	 * @param arg2 int
	 * @param arg3 int
	 * @param arg4 int
	 */
	public void reshape(GLAutoDrawable arg0, int arg1, int arg2, int arg3,
			int arg4) {
	}
	
	/**
	 * Convert GL2PS error into Export error
	 * @param gl2psError GL2PS Error
	 */
	private void setExportErrorFromGL2PS(int gl2psError) {
		if (gl2psError == GL2PS.GL2PS_NO_FEEDBACK) {
			setErrorNumber(UNKNOWN_GLEXCEPTION_ERROR);			
		}
		if (gl2psError == GL2PS.GL2PS_OVERFLOW) {
			setErrorNumber(GL2PS_OVERFLOW);			
		}
		if (gl2psError == GL2PS.GL2PS_UNINITIALIZED) {
			setErrorNumber(GL2PS_UNINITIALIZED);			
		}
		if (gl2psError == GL2PS.GL2PS_ERROR) {
			setErrorNumber(GL2PS_ERROR);			
		}
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
	
}
