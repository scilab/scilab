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
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;

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
import org.scilab.modules.xcos.actions.RunAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.actions.ViewInScicosAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockReader;
import org.scilab.modules.xcos.block.clock.ClockBlock;
import org.scilab.modules.xcos.block.generic.GenericBlock;
import org.scilab.modules.xcos.block.ramp.RampBlock;
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
	RampBlock rampBlockTemplate = new RampBlock();
	BasicBlock randm = BlockReader.read( System.getenv("SCI")+ "/modules/scicos_blocks/macros/Sources/RAND_m.h5");
	
	BasicBlock constm = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Sources/CONST_m.h5");
	sources.addTemplate("CONST_M", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/CONST_m_blk.gif"), constm);
	
	BasicBlock towsc = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Sinks/TOWS_c.h5");
	sources.addTemplate("TOWS_C", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/TOWS_c_blk.gif"), towsc);
	
	
	BasicBlock summation = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Linear/SUMMATION.h5");
	sources.addTemplate("SUMMATION", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/SUMMATION_blk.gif"), summation);
	
	BasicBlock integral = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Linear/INTEGRAL_m.h5");
	sources.addTemplate("INTEGRAL_M", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/INTEGRAL_m_blk.gif"), integral);
	
	BasicBlock clr = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Linear/CLR.h5");
	sources.addTemplate("CLR", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/CLR_blk.gif"), clr);
	
	BasicBlock gain = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Linear/GAINBLK.h5");
	sources.addTemplate("GAINBLK", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/GAINBLK_blk.gif"), gain);
	
	BasicBlock deriv = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Linear/DERIV.h5");
	sources.addTemplate("DERIV", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/DERIV_blk.gif"), deriv);
	
	BasicBlock nlProduct = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/NonLinear/PRODUCT.h5");
	sources.addTemplate("PRODUCT", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/PRODUCT_blk.gif"), nlProduct);
	
	BasicBlock trigFun = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/NonLinear/TrigFun.h5");
	sources.addTemplate("TRIGFUN", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/TrigFun_blk.gif"),trigFun);
	
	BasicBlock absvalue = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/NonLinear/ABS_VALUE.h5");
	sources.addTemplate("ABS_VALUE", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/ABS_VALUE_blk.gif"),absvalue);
	
	BasicBlock logblk = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/NonLinear/LOGBLK_f.h5");
	sources.addTemplate("LOGBLK_F", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/LOGBLK_f_blk.gif"),logblk);
	
	BasicBlock expression = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Misc/EXPRESSION.h5");
	sources.addTemplate("EXPRESSION", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/EXPRESSION_blk.gif"), expression);
	
	BasicBlock mux = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Branching/MUX.h5");
	sources.addTemplate("MUX", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/MUX_blk.gif"), mux);
	
	BasicBlock demux = BlockReader.read( System.getenv("SCI")+"/modules/scicos_blocks/macros/Branching/DEMUX.h5");
	sources.addTemplate("DEMUX", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/DEMUX_blk.gif"), demux);
	
	
	sources.addTemplate("Clock", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/CLOCK_c_blk.gif"),clockTemplate);
	//sources.addTemplate("Generic", null ,genericTemplate);
	sources.addTemplate("Scope", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/CSCOPE_blk.gif"),scopeTemplate);
	sources.addTemplate("Sinusoid", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/GENSIN_f_blk.gif"),sinusoidTemplate);
	sources.addTemplate("Ramp", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/RAMP_blk.gif"), rampBlockTemplate);
	sources.addTemplate("RAND_M", new ImageIcon(System.getenv("SCI")+"/modules/scicos/help/images/RAND_m_blk.gif"), randm);
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

	// SIMULATE
	Menu simulate = ScilabMenu.createMenu();
	simulate.setText("Simulate");
	simulate.add(RunAction.runMenu(scilabGraph));
	simulate.add(StopAction.stopMenu(scilabGraph));
	menuBar.add(simulate);
	
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
