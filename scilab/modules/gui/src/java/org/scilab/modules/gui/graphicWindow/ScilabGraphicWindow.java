/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Window dedicated to graphics  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.graphicWindow;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.events.callback.ScilabCloseCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * Scilab graphic Window class
 */
public final class ScilabGraphicWindow extends ScilabWindow {

	private static final String FIGURE_TITLE = "Graphic window number ";
	
	private static final String SCIDIR = System.getenv("SCI");
	
	private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_menubar.xml";
	private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_toolbar.xml";
	
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
		MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE, figureIndex);
		/* TOOLBAR */
		ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE, figureIndex);
		
		TextBox infoBar = ScilabTextBox.createTextBox();
		
		Tab graphicTab = ScilabTab.createTab(FIGURE_TITLE + figureIndex);
		/* Destroy the graphic figure when the tab is closed */
		graphicTab.setCallback(ScilabCloseCallBack.create(figureIndex, getClosingWindowCommand(figureIndex)));
		Canvas graphicCanvas = ScilabCanvas.createCanvas(figureIndex);
		graphicTab.addMenuBar(menuBar);
		graphicTab.addToolBar(toolBar);
		graphicTab.addInfoBar(infoBar);
		graphicTab.addMember(graphicCanvas);
		this.addTab(graphicTab);
		
		// link the tab and canvas with their figure
		DrawableFigureGL associatedFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		associatedFigure.setRendererProperties(new ScilabRendererProperties(graphicTab, graphicCanvas));
		
		// don't draw now, figure will show itself when all its parameters will be set
	}

	/**
	 * Creates a Scilab window object
	 * @param figureIndex index of the figure associated with the window
	 * @return the created window
	 */
	public static Window createWindow(int figureIndex) {
		return new ScilabGraphicWindow();
	}
	
	/**
	 * @param figureIndex of the figure to close
	 * @return Scilab command used to close a window.
	 */
	private String getClosingWindowCommand(int figureIndex) {
		// // check if figure is already closed
		// if (get_figure_handle(fid) <> []) then
		//   if (get(get_figure_handle(fid), 'event_handler_enable') == 'on') then
		//     // execute closing call back
		//     execstr(get(get_figure_handle(fid), 'event_handler') + '(fid, -1, -1, -1000)');
		//   end
		//   // destory the figure
		//   delete(get_figure_handle(fid));
		// end
		return "if (get_figure_handle(" + figureIndex + ") <> []) then"
		+      "  if (get(get_figure_handle(" + figureIndex + "), 'event_handler_enable') == 'on') then"
		+      "    execstr(get(get_figure_handle(" + figureIndex + "), 'event_handler')+'(" + figureIndex + ", -1, -1, -1000)');"
		+      "  end;"
		+      "  delete(get_figure_handle(" + figureIndex + "));"
		+      "end;";
	}
	
}
