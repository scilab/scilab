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
import java.io.File;

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
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.actions.ViewInScicosAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockReader;
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
	
	graphOutline.setDrawLabels(true);
	
	((SwingScilabTab) outlineTab.getAsSimpleTab()).setContentPane(graphOutline);
	outline.addTab(outlineTab);
	outline.setVisible(true);
    }

    public static void createPalette() {

    	Window palette = ScilabWindow.createWindow();

    	/** Create SOURCES palette */
    	String[] sourcesBlocksNames = {"CONST_m", "GENSQR_f", "RAMP", "RAND_m", "RFILE_f", "CLKINV_f", "CURV_f", "INIMPL_f",
    			"READAU_f", "SAWTOOTH_f", "STEP_FUNCTION", "CLOCK_c", "GENSIN_f", "IN_f", "READC_f", "TIME_f", "Modulo_Count", 
    			"Sigbuilder", "Counter", "SampleCLK", "TKSCALE", "FROMWSB", "FROMWS_c", "OUT_f"};
    	palette.addTab(createPalette("Sources", sourcesBlocksNames));

    	palette.setVisible(true);

    	/** Create CONTINUOUS palette */
    	String[] continuousBlocksNames = {"DERIV", "INTEGRAL_m", "CLSS", "CLR", "TIME_DELAY", "TCLSS", "VARIABLE_DELAY",
    			"PID", "INTEGRAL_f"};
    	palette.addTab(createPalette("Continuous", continuousBlocksNames));

    	/** Create DISCONTINUOUS palette */
    	String[] discontinuousBlocksNames = {"SATURATION", "DEADBAND", "HYSTHERESIS", "BACKLASH", "RATELIMITER", "REGISTER",
    	"DELAYV_f"};
    	palette.addTab(createPalette("Discontinuous", discontinuousBlocksNames));

    	/** Create LOOKUP TABLES palette */
    	String[] lookupBlocksNames = {"LOOKUP_f", "INTRP2BLK_f", "INTRPLBLK_f"};
    	palette.addTab(createPalette("Lookup Tables", lookupBlocksNames));

    	/** Create SIGNAL PROCESSING palette */
    	String[] signalBlocksNames = {"MCLOCK_f", "QUANT_f", "MFCLCK_f", "SAMPHOLD_m"};
    	palette.addTab(createPalette("Signal Processing", signalBlocksNames));

    	/** Create THRESHOLD palette */
    	String[] thresholdBlocksNames = {"NEGTOPOS_f", "POSTONEG_f", "ZCROSS_f", "GENERAL_f","CLINDUMMY_f"};
    	palette.addTab(createPalette("Threshold", thresholdBlocksNames));

    	/** Create MATH OPERATIONS palette */
    	String[] mathsBlocksNames = {"MAX_f", "MIN_f", "BIGSOM_f", "POWBLK_f","INVBLK", "SINBLK_f", "COSBLK_f", "TANBLK_f",
    			"MATDIV", "EXPBLK_m", "MATDIV", "EXPBLK_m", "PROD_f", "MATZREIM", "MATMAGPHI", "SQRT", "GAINBLK_f", "LOGBLK_f",
    			"SUMMATION", "TrigFun", "PRODUCT", "MAXMIN", "ABS_VALUE", "SIGNUM", "SUM_f", "CONSTRAINT_f"};
    	palette.addTab(createPalette("Math Operations", mathsBlocksNames));

    	/** Create MODELICA palette */
    	String[] modelicaBlocksNames = {"MBLOCK", "FROMMO", "GOTOMO", "GotoTagVisibilityMO", "OUTIMPL_f"};
    	palette.addTab(createPalette("Modelica", modelicaBlocksNames));

    	/** Create INTEGER palette */
    	String[] integerBlocksNames = {"BITCLEAR", "BITSET", "CONVERT", "EXTRACTBITS", "INTMUL", "SHIFT", "LOGIC",
    			"DLATCH", "DFLIPFLOP", "JKFLIPFLOP", "SRFLIPFLOP"};
    	palette.addTab(createPalette("Integer", integerBlocksNames));
    	
    	/** Create SINKS palette */
    	String[] sinksBlocksNames = {"CFSCOPE", "CANIMXY", "CSCOPE", "CSCOPXY", "TOWS_c", "CMAT3D", "CSCOPXY3D", "CANIMXY3D",
    			"CMATVIEW", "CMSCOPE", "AFFICH_m", "TRASH_f"};
    	palette.addTab(createPalette("Sinks", sinksBlocksNames));
    	
      	/** Create PORT ACTION palette */
    	String[] portactionBlocksNames = {"Extract_Activation", "IFTHEL_f", "ESELECT_f", "EDGE_TRIGGER"};
    	palette.addTab(createPalette("Port Action", portactionBlocksNames));
    	
    	/** Create DISCRETE palette */
    	String[] discreteBlocksNames = {"DLRADAPT_f", "DLR", "DLSS", "DELAY_f", "DOLLAR_f", "DELAYV_f"};
    	palette.addTab(createPalette("Discrete", discreteBlocksNames));
    	
      	/** Create EVENTS palette */
    	String[] eventsBlocksNames = {"CLKFROM", "CLKGOTO", "GotoTagVisibility", "SampleCLK", "CLKOUTV_f", "ESELECT_f",
    			"CLKSOMV_f", "CLOCK_c", "EVTGEN_f", "EVTVARDLY", "M_freq", "ANDBLK", "HALT_f", "freq_div", "ANDLOG_f",
    			"EVTDLY_c", "IFTHEL_f", "CEVENTSCOPE"};
    	palette.addTab(createPalette("Events", eventsBlocksNames));
    	
    	/** Create SIGNAL ROUTING palette */
    	String[] routingBlocksNames = {"MUX", "DEMUX"};
    	palette.addTab(createPalette("Signal Routing", routingBlocksNames));
    	
    	/** Create COMMONLY USED BLOCKS palette */
    	String[] commonBlocksNames = {"DEMUX", "MUX", "NRMSOM_f", "OUT_f", "IN_f", "RELATIONALOP", "PRODUCT", "BIGSOM_f",
    			"DOLLAR_f", "INTEGRAL_f", "CONST_m", "SATURATION", "CMSCOPE", "CSCOPXY", "ANDBLK", "LOGICAL_OP", "SWITCH2_m",
    			"CONVERT", "TEXT_f"};
    	palette.addTab(createPalette("Commonly Used Blocks", commonBlocksNames));
    	
    	/** Create USER-DEFINED FUNCTIONS palette */
    	String[] userdefinedBlocksNames = {"PDE", "fortran_block", "MBLOCK", "EXPRESSION", "scifunc_block_m", "CBLOCK",
    			"generic_block3", "TEXT_f", "c_block", "SUPER_f"};
    	palette.addTab(createPalette("User-Defined Functions", userdefinedBlocksNames));

    }
    
    public static XcosPalette createPalette(String paletteName, String[] blocksNames) {
 
    	String blocksPath = System.getenv("SCI")+ "/modules/scicos_blocks/blocks/";
    	String imagesPath = System.getenv("SCI")+ "/modules/scicos/help/images/";

    	XcosPalette palette = new XcosPalette(paletteName);
 	
    	BasicBlock theBloc = null;
    	for (int kBlock = 0; kBlock < blocksNames.length; kBlock++) {
    		File hdf5 = new File(blocksPath + blocksNames[kBlock] + ".h5");
    		if (!hdf5.exists()) {
    			System.err.println(blocksPath + blocksNames[kBlock] + ".h5 does not exists !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    		}
    		theBloc = BlockReader.read(blocksPath + blocksNames[kBlock] + ".h5");
    		palette.addTemplate(blocksNames[kBlock], new ImageIcon(imagesPath + blocksNames[kBlock] + "_blk.gif"), theBloc);
    		
    	}

    	return palette;
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
	simulate.add(SetupAction.setupMenu(scilabGraph));
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

	public XcosDiagram getDiagram() {
		return diagram;
	}

	public void setDiagram(XcosDiagram diagram) {
		this.diagram = diagram;
	}

}
