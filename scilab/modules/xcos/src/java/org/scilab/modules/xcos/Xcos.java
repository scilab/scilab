/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.GroupAction;
import org.scilab.modules.graph.actions.PasteAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.UnGroupAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.actions.DumpAction;
import org.scilab.modules.xcos.actions.ViewInScicosAction;
import org.scilab.modules.xcos.block.clock.ClockBlock;
import org.scilab.modules.xcos.block.generic.GenericBlock;
import org.scilab.modules.xcos.block.scope.ScopeBlock;
import org.scilab.modules.xcos.block.sinusoid.SinusoidBlock;
import org.scilab.modules.xcos.palette.XcosPalette;

import com.mxgraph.swing.mxGraphOutline;

public class Xcos extends SwingScilabTab implements Tab {

    private XcosDiagram diagram = null;    
    /**
     * @param args
     */
    public static void main(String[] args) {
	EventQueue.invokeLater(new Runnable() {
	    public void run() {
		CreateAndShowGui();
	    }
	});
    }

    public static void xcos() {
	CreateAndShowGui();
    }

    public static void createViewPort(ScilabGraph xcosDiagramm) {
	Window outline = ScilabWindow.createWindow();
	Tab outlineTab = ScilabTab.createTab("ViewPort");
	outlineTab.setCallback(null);
	
	// Creates the graph outline component
	mxGraphOutline graphOutline = new mxGraphOutline(xcosDiagramm.getAsComponent());
	
	((SwingScilabTab) outlineTab.getAsSimpleTab()).setContentPane(graphOutline);
	outline.addTab(outlineTab);
	outline.setVisible(true);
    }

    public static void createPalette() {
	Window palette = ScilabWindow.createWindow();

	XcosPalette sources = new XcosPalette();

	palette.addTab(sources);
	palette.setVisible(true);



	//	mxCell tableTemplate = new mxCell("New Table", new mxGeometry(0, 0, 200, 280), null);
	//	tableTemplate.getGeometry().setAlternateBounds(new mxRectangle(0, 0, 140, 25));
	//	mxCell port = new mxCell("port", new mxGeometry(0,0,20,20), null);
	//	port.setParent(tableTemplate);
	//	port.setVertex(true);
	//	tableTemplate.setVertex(true);

	ClockBlock clockTemplate = new ClockBlock();
	GenericBlock genericTemplate = new GenericBlock();
	ScopeBlock scopeTemplate = new ScopeBlock();
	SinusoidBlock sinusoidTemplate = new SinusoidBlock();

	sources.addTemplate("Clock", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/CLOCK_c_blk.gif"),clockTemplate);
	sources.addTemplate("Generic", null ,genericTemplate);
	sources.addTemplate("Scope", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/CSCOPE_blk.gif"),scopeTemplate);
	sources.addTemplate("Sinusoid", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/GENSIN_f_blk.gif"),sinusoidTemplate);
    }

    public static MenuBar createMenuBar(ScilabGraph scilabGraph) {
	/*
	 * MENU BAR
	 */
	// FILE
	MenuBar menuBar = ScilabMenuBar.createMenuBar();
	Menu file = ScilabMenu.createMenu();
	file.setText("File");
	MenuItem newItem = ScilabMenuItem.createMenuItem();
	newItem.setText("New");
	JMenuItem quit = new JMenuItem("Quit");
	quit.addActionListener(new ActionListener() {
	    public void actionPerformed(ActionEvent arg0) { System.exit(0); }
	});
	file.add(newItem);
	file.add(DumpAction.dumpMenu(scilabGraph));
	file.add(ViewInScicosAction.viewInScicosMenu(scilabGraph));
	//file.add(quit);
	menuBar.add(file);

	// EDIT
	Menu edit = ScilabMenu.createMenu();
	edit.setText("Edit");
	edit.add(UndoAction.undoMenu(scilabGraph));
	edit.add(RedoAction.redoMenu(scilabGraph));
	edit.addSeparator();
	edit.add(CutAction.cutMenu(scilabGraph));
	edit.add(CopyAction.copyMenu(scilabGraph));
	edit.add(PasteAction.pasteMenu(scilabGraph));
	menuBar.add(edit);

	// VIEW
	Menu view = ScilabMenu.createMenu();
	view.setText("View");
	view.add(ZoomInAction.zoominMenu(scilabGraph));
	view.add(ZoomOutAction.zoomoutMenu(scilabGraph));
	menuBar.add(view);

	// SHAPE
	Menu shape = ScilabMenu.createMenu();
	shape.setText("Shape");
	shape.add(GroupAction.groupMenu(scilabGraph));
	shape.add(UnGroupAction.ungroupMenu(scilabGraph));
	menuBar.add(shape);

	return menuBar;
    }

    public static ToolBar createToolBar(ScilabGraph scilabGraph) {
	/*
	 * TOOL BAR
	 */
	ToolBar toolBar = ScilabToolBar.createToolBar();

	// UNDO / REDO
	toolBar.add(UndoAction.undoButton(scilabGraph));
	toolBar.add(RedoAction.redoButton(scilabGraph));
	toolBar.addSeparator();

	// CUT / COPY / PASTE
	toolBar.add(CutAction.cutButton(scilabGraph));
	toolBar.add(CopyAction.copyButton(scilabGraph));
	toolBar.add(PasteAction.pasteButton(scilabGraph));
	toolBar.addSeparator();

	// GROUP / UNGROUP
	toolBar.add(GroupAction.groupButton(scilabGraph));
	toolBar.add(UnGroupAction.ungroupButton(scilabGraph));
	toolBar.addSeparator();

	// ZOOMIN / ZOOMOUT
	toolBar.add(ZoomInAction.zoominButton(scilabGraph));
	toolBar.add(ZoomOutAction.zoomoutButton(scilabGraph));
	toolBar.addSeparator();

	return toolBar;
    }

    public Xcos(XcosDiagram diagram) {
	super("Xcos");
	this.diagram = diagram;
	// TODO : Must check if Diagramm has been modified etc etc etc ...
	this.setCallback(null);
	this.setContentPane(new JScrollPane(diagram.getAsComponent()));
    }
    
    public static void CreateAndShowGui() {
	Window main = ScilabWindow.createWindow();
	main.setTitle("Xcos");

	
	XcosDiagram xcosDiagramm = new XcosDiagram();
	Tab tab = new Xcos(xcosDiagramm);
	main.setVisible(true);
	main.addTab(tab);
	/*
	 * MENU BAR
	 */
	MenuBar menuBar = createMenuBar(xcosDiagramm);

	/*
	 * TOOL BAR
	 */
	ToolBar toolBar = createToolBar(xcosDiagramm);

	/*
	 * VIEW PORT
	 */
	createViewPort(xcosDiagramm);

	createPalette();

	tab.addMenuBar(menuBar);
	tab.addToolBar(toolBar);
    }

    public SimpleTab getAsSimpleTab() {
	return this;
    }

    public Window getParentWindow() {
	return null;
    }

    public void addInfoBar(TextBox infoBarToAdd) {
	// TODO Auto-generated method stub
	
    }

    public void addMenuBar(MenuBar menuBarToAdd) {
	((SwingScilabTab) this).setMenuBar(menuBarToAdd);
    }

    public void addToolBar(ToolBar toolBarToAdd) {
	((SwingScilabTab) this).setToolBar(toolBarToAdd);
    }

}
