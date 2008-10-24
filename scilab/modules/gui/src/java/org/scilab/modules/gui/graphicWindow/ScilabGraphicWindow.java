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
 * Utilities to create a graphic window
 */
public final class ScilabGraphicWindow {

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
	 * Create a new window with id figureIndex.
	 * The created window contains an empty tab.
	 * @param figureIndex index of the figure to create
	 * @return id of the window
	 */
	public static int newWindow(int figureIndex) {
		Window newWindow = ScilabWindow.createWindow();
		
		newWindow.setTitle(FIGURE_TITLE + figureIndex);
		/* MENUBAR */
		MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE, figureIndex);
		/* TOOLBAR */
		ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE, figureIndex);
		
		TextBox infoBar = ScilabTextBox.createTextBox();
		
		Tab graphicTab = ScilabTab.createTab(FIGURE_TITLE + figureIndex);
		/* Destroy the graphic figure when the tab is closed */
		graphicTab.setCallback(ScilabCloseCallBack.create(figureIndex, getClosingWindowCommand(figureIndex)));
		graphicTab.addMenuBar(menuBar);
		graphicTab.addToolBar(toolBar);
		graphicTab.addInfoBar(infoBar);
		newWindow.addTab(graphicTab);
		
		// link the tab and canvas with their figure
		DrawableFigureGL associatedFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		//associatedFigure.setRendererProperties(new ScilabRendererProperties(graphicTab, graphicCanvas));
		associatedFigure.setRendererProperties(new ScilabRendererProperties(graphicTab, null));
		// don't draw now, figure will show itself when all its parameters will be set
		
		return 0;
	}
	
	/**
	 * @param figureIndex of the figure to close
	 * @return Scilab command used to close a window.
	 */
	private static String getClosingWindowCommand(int figureIndex) {
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
