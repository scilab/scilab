/*------------------------------------------------------------------------*/
/* file: ScilabGraphicWindow.java                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy, Vincent COUVERT                         */
/* desc : Window dedicated to graphics                                    */
/*------------------------------------------------------------------------*/


package org.scilab.modules.gui.graphicWindow;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.xml.sax.SAXException;

/**
 * Scilab graphic Window class
 */
public class ScilabGraphicWindow extends ScilabWindow {

	private static final String FIGURE_TITLE = "Graphic window number ";
	
	private static final String SCIDIR = System.getenv("SCI");
	
	private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_menubar.xml";;
	private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_toolbar.xml";;
	
	private static final String FILE_NOT_FOUND = "Could not find file: ";
	
	private static final String CANNOT_CREATE_MENUBAR = "Cannot create Figure MenuBar.\n"
								+ "Check if file graphics_menubar.xml is available and valid.";
	private static final String CANNOT_CREATE_TOOLBAR = "Cannot create Figure ToolBar.\n"
								+ "Check if file graphics_toolbar.xml is available and valid.";
	
	/**
	 * Constructor
	 */
	private ScilabGraphicWindow() {
		super();
	}
	
	/**
	 * Specify the window we wants to use
	 * Real creation starts here
	 * @param figureIndex index of the figure associated with the window
	 */
	public void setFigureIndex(int figureIndex) {
		this.setTitle(FIGURE_TITLE + figureIndex);
		/* MENUBAR */
		MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE);
		/* TOOLBAR */
		ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE);
		
		
		Tab graphicTab = ScilabTab.createTab(FIGURE_TITLE + figureIndex);
		Canvas graphicCanvas = ScilabCanvas.createCanvas(figureIndex);
		graphicTab.addMenuBar(menuBar);
		graphicTab.addToolBar(toolBar);
		graphicTab.addMember(graphicCanvas);
		this.addTab(graphicTab);
		// don't draw for now
		
		
		// link the tab and canvas with theie figure
		DrawableFigureGL associatedFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		associatedFigure.setRendererProperties(new ScilabRendererProperties(graphicTab, graphicCanvas));
		
		//graphicCanvas.display();
		this.draw();
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
