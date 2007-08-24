/*------------------------------------------------------------------------*/
/* file: ScilabGraphicWindow.java                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Window dedicated to graphics                                    */
/*------------------------------------------------------------------------*/


package org.scilab.modules.gui.graphicWindow;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;



public class ScilabGraphicWindow extends ScilabWindow {

	/**
	 * Constructor
	 * @param figureIndex index of the figure associated with the window
	 */
	public ScilabGraphicWindow() {
		super();
	}
	
	/**
	 * Specify the window we wants to use
	 * Real creation starts here
	 * @param figureIndex index of the figure associated with the window
	 */
	public void setFigureIndex(int figureIndex) {
		this.setTitle("Graphic window number " + figureIndex);
		Tab graphicTab = ScilabTab.createTab("") ;
		Canvas graphicCanvas = ScilabCanvas.createCanvas(figureIndex) ;
		graphicTab.addMember(graphicCanvas);
		this.addTab(graphicTab);
		this.draw();
		
		// link the tab and canvas with theie figure
		DrawableFigureGL associatedFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		associatedFigure.setRendererProperties(new ScilabRendererProperties(graphicTab, graphicCanvas));
		
		//graphicCanvas.display();
	}

	/**
	 * Creates a Scilab window object
	 * @param figureIndex index of the figure associated with the window
	 * @return the created window
	 */
	public static Window createWindow(int figureIndex) {
		return new ScilabGraphicWindow();
	}
	
}
