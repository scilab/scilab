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
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.MenuBarBuilder;
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
	private static final String MENUSXMLFILE = System.getenv("SCI") + "/modules/gui/etc/graphics_menus.xml";;
	private static final String FILE_NOT_FOUND = "Could not find file: ";
	private static final String CANNOT_CREATE_MENUBAR = "Cannot create Figure MenuBar.\nCheck if file graphics_menus.xml is available and valid.";
	
	/**
	 * Constructor
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
		this.setTitle(FIGURE_TITLE + figureIndex);
		try {
			this.addMenuBar(MenuBarBuilder.buildMenuBar(MENUSXMLFILE));
		} catch (SAXException e) {
			System.err.println(CANNOT_CREATE_MENUBAR);
			System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
		} catch (IOException e) {
			System.err.println(CANNOT_CREATE_MENUBAR);
			System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
		} catch (ParserConfigurationException e) {
			System.err.println(CANNOT_CREATE_MENUBAR);
			System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
		}
		Tab graphicTab = ScilabTab.createTab(FIGURE_TITLE + figureIndex);
		Canvas graphicCanvas = ScilabCanvas.createCanvas(figureIndex);
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
