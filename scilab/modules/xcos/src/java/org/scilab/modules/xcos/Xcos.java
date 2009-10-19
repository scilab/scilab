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
import java.util.ArrayList;
import java.util.HashMap;

import javax.swing.ImageIcon;
import javax.swing.JTabbedPane;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.InvertSelectionAction;
import org.scilab.modules.graph.actions.PasteAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.SelectAllAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.actions.AboutXcosAction;
import org.scilab.modules.xcos.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.actions.BlockParametersAction;
import org.scilab.modules.xcos.actions.CloseAction;
import org.scilab.modules.xcos.actions.ClosePalettesAction;
import org.scilab.modules.xcos.actions.CloseViewportAction;
import org.scilab.modules.xcos.actions.CodeGenerationAction;
import org.scilab.modules.xcos.actions.CompileAction;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.DumpAction;
import org.scilab.modules.xcos.actions.ExportAction;
import org.scilab.modules.xcos.actions.ExportToXMLAction;
import org.scilab.modules.xcos.actions.FitDiagramToViewAction;
import org.scilab.modules.xcos.actions.FlipAction;
import org.scilab.modules.xcos.actions.ImportFromXMLAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.NewPaletteAction;
import org.scilab.modules.xcos.actions.NormalViewAction;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.actions.PrintAction;
import org.scilab.modules.xcos.actions.QuitAction;
import org.scilab.modules.xcos.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.actions.RotateAction;
import org.scilab.modules.xcos.actions.SaveAction;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.actions.SaveAsInterfaceFunctionAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.actions.StartAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.actions.SuperblockMaskCreateAction;
import org.scilab.modules.xcos.actions.SuperblockMaskCustomizeAction;
import org.scilab.modules.xcos.actions.SuperblockMaskRemoveAction;
import org.scilab.modules.xcos.actions.SuperblockMaskSaveBlockGUIAction;
import org.scilab.modules.xcos.actions.ViewBrowserAction;
import org.scilab.modules.xcos.actions.ViewDetailsAction;
import org.scilab.modules.xcos.actions.ViewDiagramBrowserAction;
import org.scilab.modules.xcos.actions.ViewGetinfosAction;
import org.scilab.modules.xcos.actions.ViewGridAction;
import org.scilab.modules.xcos.actions.ViewInScicosAction;
import org.scilab.modules.xcos.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.actions.ViewViewportAction;
import org.scilab.modules.xcos.actions.XcosDemonstrationsAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.palette.XcosPalette;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.action_binding.InterpreterManagement;

import com.mxgraph.swing.mxGraphOutline;

public class Xcos extends SwingScilabTab implements Tab {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static ArrayList<XcosDiagram> diagrams = new ArrayList<XcosDiagram>();
	private static HashMap<String, BasicBlock> allBlocks = new HashMap<String, BasicBlock>();
	private static HashMap<Integer, AfficheBlock> afficheBlocks = new HashMap<Integer, AfficheBlock>();

	private static Tab palette;
	private static Thread paletteThread;
	private static boolean paletteLoaded;

	/** Palette creation */
	static {
		/* load scicos libraries (macros) */
		InterpreterManagement.requestScilabExec("loadScicosLibs();");
		
		paletteThread = new Thread() {
			public void run() {
				// Add a default Java bloc in HashMap
				allBlocks.put("TEXT_f", new TextBlock("TEXT_f"));
				
				Window palWin = ScilabWindow.createWindow();
				palWin.setVisible(true);
				palette = ScilabTab.createTab(XcosMessages.PALETTE_BROWSER);
				
				palette.setCallback(new ClosePalettesAction(null));
				
				/** Create the menubar */
				MenuBar palMenuBar = ScilabMenuBar.createMenuBar();
				palette.addMenuBar(palMenuBar);
				
				Menu palMenu = ScilabMenu.createMenu();
				palMenu.setText(XcosMessages.PALETTES);
				palMenu.setMnemonic('P');
				palMenuBar.add(palMenu);
				
				palMenu.add(ClosePalettesAction.createMenu(null));
				
				/** Create the toolbar */
				ToolBar palToolbar = ScilabToolBar.createToolBar();
				palette.addToolBar(palToolbar);
				palToolbar.add(NewDiagramAction.createButton(null));
				palToolbar.add(OpenAction.createButton(null));
				
				/** Create the infobar */
				palette.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());
				
				palWin.addTab(palette);

				JTabbedPane allpalettes = new JTabbedPane();
				allpalettes.setTabPlacement(JTabbedPane.BOTTOM);
				((SwingScilabTab) palette.getAsSimpleTab()).setContentPane(allpalettes);

				/** Create SOURCES palette */
				String[] sourcesBlocksNames = {"CONST_m", "GENSQR_f", "RAMP", "RAND_m", "RFILE_f", "CLKINV_f", "CURV_f", "INIMPL_f",
						"READAU_f", "SAWTOOTH_f", "STEP_FUNCTION", "CLOCK_c", "GENSIN_f", "IN_f", "READC_f", "TIME_f", "Modulo_Count", 
						"Sigbuilder", "Counter", "SampleCLK", "TKSCALE", "FROMWSB"};
				allpalettes.addTab(XcosMessages.SOURCES_PAL, createPalette(sourcesBlocksNames));

				palette.setVisible(true);

				/** Create CONTINUOUS palette */
				String[] continuousBlocksNames = {"DERIV", "INTEGRAL_m", "CLSS", "CLR", "TIME_DELAY", "TCLSS", "VARIABLE_DELAY",
						"PID", "INTEGRAL_f"};
				allpalettes.addTab(XcosMessages.CONTINUOUS_PAL, createPalette(continuousBlocksNames));

				/** Create DISCONTINUOUS palette */
				String[] discontinuousBlocksNames = {"SATURATION", "DEADBAND", "HYSTHERESIS", "BACKLASH", "RATELIMITER", "REGISTER",
				"DELAYV_f"};
				allpalettes.addTab(XcosMessages.DISCONTINUOUS_PAL, createPalette(discontinuousBlocksNames));

				/** Create LOOKUP TABLES palette */
				String[] lookupBlocksNames = {"LOOKUP_f", "INTRP2BLK_f", "INTRPLBLK_f"};
				allpalettes.addTab(XcosMessages.LOOKUPTABLES_PAL, createPalette(lookupBlocksNames));

				/** Create SIGNAL PROCESSING palette */
				String[] signalBlocksNames = {"MCLOCK_f", "QUANT_f", "MFCLCK_f", "SAMPHOLD_m"};
				allpalettes.addTab(XcosMessages.SIGNALPROCESSING_PAL, createPalette(signalBlocksNames));

				/** Create THRESHOLD palette */
				String[] thresholdBlocksNames = {"NEGTOPOS_f", "POSTONEG_f", "ZCROSS_f", "GENERAL_f","CLINDUMMY_f"};
				allpalettes.addTab(XcosMessages.THRESHOLD_PAL, createPalette(thresholdBlocksNames));

				/** Create MATH OPERATIONS palette */
				String[] mathsBlocksNames = {"MAX_f", "MIN_f", "BIGSOM_f", "POWBLK_f","INVBLK", "SINBLK_f", "COSBLK_f", "TANBLK_f",
						"MATDIV", "EXPBLK_m", "PROD_f", "MATZREIM", "MATMAGPHI", "SQRT", "GAINBLK_f", "LOGBLK_f",
						"SUMMATION", "TrigFun", "PRODUCT", "MAXMIN", "ABS_VALUE", "SIGNUM", "SUM_f", "CONSTRAINT_f"};
				allpalettes.addTab(XcosMessages.MATHSOPS_PAL, createPalette(mathsBlocksNames));

				/** Create MODELICA palette */
				String[] modelicaBlocksNames = {"MBLOCK", "FROMMO", "GOTOMO", "GotoTagVisibilityMO", "OUTIMPL_f"};
				allpalettes.addTab(XcosMessages.MODELICA_PAL, createPalette(modelicaBlocksNames));

				/** Create INTEGER palette */
				String[] integerBlocksNames = {"BITCLEAR", "BITSET", "CONVERT", "EXTRACTBITS", "INTMUL", "SHIFT", "LOGIC",
						"DLATCH", "DFLIPFLOP", "JKFLIPFLOP", "SRFLIPFLOP"};
				allpalettes.addTab(XcosMessages.INTEGER_PAL, createPalette(integerBlocksNames));

				/** Create MATRIX palette */
				String[] matrixBlocksNames = {"CUMSUM", "SQRT", "MATZREIM", "SUBMAT", "MATBKSL", "MATINV", "MATCATV", "MATSUM", 
						"RICC", "ROOTCOEF", "MATCATH", "MATLU", "MATDIV", "MATZCONJ", "MATMAGPHI", "EXTRACT", "MATEXPM", 
						"MATDET", "MATPINV", "EXTTRI", "MATMUL", "MATTRAN", "MATSING", "MATRESH", "MATDIAG", "MATEIG"};
				allpalettes.addTab(XcosMessages.MATRIX_PAL, createPalette(matrixBlocksNames));

				/** Create SINKS palette */
				String[] sinksBlocksNames = {"CFSCOPE", "CANIMXY", "CSCOPE", "CSCOPXY", "TOWS_c", "CMAT3D", "CSCOPXY3D", "CANIMXY3D",
						"CMATVIEW", "CMSCOPE", "AFFICH_m", "TRASH_f"};
				allpalettes.addTab(XcosMessages.SINKS_PAL, createPalette(sinksBlocksNames));

				/** Create PORT ACTION palette */
				String[] portactionBlocksNames = {"Extract_Activation", "IFTHEL_f", "ESELECT_f", "EDGE_TRIGGER"};
				allpalettes.add(XcosMessages.PORTACTION_PAL, createPalette(portactionBlocksNames));

				/** Create DISCRETE palette */
				String[] discreteBlocksNames = {"DLRADAPT_f", "DLR", "DLSS", "DELAY_f", "DOLLAR_f", "DELAYV_f"};
				allpalettes.addTab(XcosMessages.DISCRETE_PAL, createPalette(discreteBlocksNames));

				/** Create EVENTS palette */
				String[] eventsBlocksNames = {"CLKFROM", "CLKGOTO", "CLKGotoTagVisibility", "SampleCLK", "CLKOUTV_f", "ESELECT_f",
						"CLKSOMV_f", "CLOCK_c", "EVTGEN_f", "EVTVARDLY", "M_freq", "ANDBLK", "HALT_f", "freq_div", "ANDLOG_f",
						"EVTDLY_c", "IFTHEL_f", "CEVENTSCOPE"};
				allpalettes.addTab(XcosMessages.EVENTS_PAL, createPalette(eventsBlocksNames));

				/** Create SIGNAL ROUTING palette */
				String[] routingBlocksNames = {"ISELECT_m", "RELAY_f", "WRITEAU_f", "SELECT_m", "EXTRACTOR", "M_SWITCH", "SWITCH_f",
						"SWITCH2_m", "NRMSOM_f", "READC_f", "WRITEC_f", "GOTO", "GotoTagVisibility", "FROM", "RFILE_f", "WFILE_f",
						"ANDBLK", "MUX", "DEMUX"};
				allpalettes.addTab(XcosMessages.SIGNALROUTING_PAL, createPalette(routingBlocksNames));

				/** Create COMMONLY USED BLOCKS palette */
				String[] commonBlocksNames = {"DEMUX", "MUX", "NRMSOM_f", "OUT_f", "IN_f", "RELATIONALOP", "PRODUCT", "BIGSOM_f",
						"DOLLAR_f", "INTEGRAL_f", "CONST_m", "SATURATION", "CMSCOPE", "CSCOPXY", "ANDBLK", "LOGICAL_OP", "SWITCH2_m",
						"CONVERT", "TEXT_f"};
				allpalettes.addTab(XcosMessages.COMMONUSED_PAL, createPalette(commonBlocksNames));

				/** Create USER-DEFINED FUNCTIONS palette */
				String[] userdefinedBlocksNames = {"PDE", "fortran_block", "MBLOCK", "EXPRESSION", "scifunc_block_m", "CBLOCK",
						"generic_block3", "TEXT_f", "c_block", "SUPER_f"};
				allpalettes.addTab(XcosMessages.USERDEFINEDFUNCTIONS_PAL, createPalette(userdefinedBlocksNames));

				/** Create ELECTRICAL palette */
				String[] electricalBlocksNames = {"Capacitor", "Ground", "VVsourceAC", "ConstantVoltage", "Inductor",
						"PotentialSensor", "VariableResistor", "CurrentSensor", "Resistor", "VoltageSensor", "Diode",
						"VsourceAC", "NPN", "PNP", "SineVoltage", "Switch", "OpAmp", "PMOS", "NMOS", "CCS", "CVS",
						"IdealTransformer", "Gyrator"};
				allpalettes.addTab(XcosMessages.ELECTRICAL_PAL, createPalette(electricalBlocksNames));

				/** Create THERMO-HYDRAULICS palette */
				String[] thermoHydraulicsBlocksNames = {"Bache", "VanneReglante", "PerteDP", "PuitsP", "SourceP"};
				allpalettes.addTab(XcosMessages.THERMOHYDRAULICS_PAL, createPalette(thermoHydraulicsBlocksNames));

				/** Create DEMO-BLOCKS palette */
				String[] demoBlocksNames = {"BOUNCE", "BOUNCEXY", "BPLATFORM", "AUTOMAT", "PDE"};
				allpalettes.addTab(XcosMessages.DEMOBLOCKS_PAL, createPalette(demoBlocksNames));
				
				synchronized (this) { 
					this.notify();
				} 
				
				paletteLoaded = true;
			}
		};
		paletteThread.start();
	}

	public static HashMap<Integer, AfficheBlock> getAfficheBlocks() {
		return afficheBlocks;
	}
    
    /**
     * @param args
     */
    public static void main(String[] args) {
	EventQueue.invokeLater(new Runnable() {
	    public void run() {
		createEmptyDiagram();
	    }
	});
    }

    public static void xcos() {
    	createEmptyDiagram();
    	ViewPaletteBrowserAction.setPalettesVisible(true);
    }
    
    public static void xcos(String fileName) {
    	XcosDiagram diagram = createEmptyDiagram();
    	ViewPaletteBrowserAction.setPalettesVisible(true);
    	synchronized(paletteThread) {
    		try {
    			if(paletteLoaded == false)
    			{
    				paletteThread.wait();
    			}
    	} catch (InterruptedException e) {
    		e.printStackTrace();
    	}
    }
    diagram.openDiagramFromFile(fileName);
    }
    

    public static void createViewPort(ScilabGraph xcosDiagramm) {
	Window outline = ScilabWindow.createWindow();
	Tab outlineTab = ScilabTab.createTab(XcosMessages.VIEWPORT);
	
	outlineTab.setCallback(new CloseViewportAction(xcosDiagramm));
	
	MenuBar vpMenuBar = ScilabMenuBar.createMenuBar();
	outlineTab.addMenuBar(vpMenuBar);
	
	Menu vpMenu = ScilabMenu.createMenu();
	vpMenu.setText(XcosMessages.VIEWPORT);
	vpMenu.setMnemonic('V');
	vpMenuBar.add(vpMenu);
	
	vpMenu.add(CloseViewportAction.createMenu(xcosDiagramm));
	
	outlineTab.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());

	((XcosDiagram) xcosDiagramm).setViewPort(outlineTab);
	
	// Creates the graph outline component
	mxGraphOutline graphOutline = new mxGraphOutline(xcosDiagramm.getAsComponent());
	
	graphOutline.setDrawLabels(true);
	
	((SwingScilabTab) outlineTab.getAsSimpleTab()).setContentPane(graphOutline);
	outline.addTab(outlineTab);
	outline.setVisible(false);
	outlineTab.setVisible(false);
    }

    public static XcosPalette createPalette(String[] blocksNames) {
 
    	String blocksPath = System.getenv("SCI") + "/modules/scicos_blocks/blocks/";
    	String palImagesPath = System.getenv("SCI") + "/modules/scicos/help/images/";

    	XcosPalette palette = new XcosPalette();
 	
    	BasicBlock theBloc = null;
    	for (int kBlock = 0; kBlock < blocksNames.length; kBlock++) {
    		// Search the bloc in global hashmap
    		theBloc = allBlocks.get(blocksNames[kBlock]);
    		
    		// If not found then create it
    		if (theBloc == null) {
    			theBloc = BlockReader.readBlockFromFile(blocksPath + blocksNames[kBlock] + ".h5");
    			allBlocks.put(blocksNames[kBlock], theBloc);
    		}

    		if (theBloc.getStyle().compareTo("") == 0) {
    		    theBloc.setStyle(theBloc.getInterfaceFunctionName());
    		    theBloc.setValue(theBloc.getInterfaceFunctionName());
    		}
    		
    		palette.addTemplate(blocksNames[kBlock], new ImageIcon(palImagesPath + blocksNames[kBlock] + "_blk.gif"), theBloc);
    	}

    	return palette;
    }
    
    public static MenuBar createMenuBar(ScilabGraph scilabGraph) {

    	MenuBar menuBar = ScilabMenuBar.createMenuBar();

    	/** FILE MENU */
		Menu fileMenu = ScilabMenu.createMenu();
		fileMenu.setText(XcosMessages.FILE);
		fileMenu.setMnemonic('F');
		
		Menu newMenu = ScilabMenu.createMenu();
		newMenu.setText(XcosMessages.NEW);
		newMenu.add(NewDiagramAction.createMenu(scilabGraph));
		newMenu.add(NewPaletteAction.createMenu(scilabGraph));
		fileMenu.add(newMenu);

		fileMenu.add(OpenAction.createMenu(scilabGraph));
		fileMenu.addSeparator();
		fileMenu.add(SaveAction.createMenu(scilabGraph));
		fileMenu.add(SaveAsAction.createMenu(scilabGraph));
		fileMenu.add(ExportAction.createMenu(scilabGraph));
		fileMenu.add(SaveAsInterfaceFunctionAction.createMenu(scilabGraph));
		fileMenu.addSeparator();
		fileMenu.add(PrintAction.createMenu(scilabGraph));
		fileMenu.addSeparator();
		fileMenu.add(CloseAction.createMenu(scilabGraph));
		fileMenu.addSeparator();
		fileMenu.add(QuitAction.createMenu(scilabGraph));
		fileMenu.addSeparator();
		fileMenu.add(DumpAction.dumpMenu(scilabGraph));
		fileMenu.add(ViewInScicosAction.viewInScicosMenu(scilabGraph));
		fileMenu.add(ExportToXMLAction.createMenu(scilabGraph));
		fileMenu.add(ImportFromXMLAction.createMenu(scilabGraph));
		
		menuBar.add(fileMenu);
	
		/** Edit menu */
		Menu edit = ScilabMenu.createMenu();
		edit.setText(XcosMessages.EDIT);
		edit.setMnemonic('E');
		menuBar.add(edit);

		edit.add(UndoAction.undoMenu(scilabGraph));
		edit.add(RedoAction.redoMenu(scilabGraph));
		edit.addSeparator();
		edit.add(CutAction.cutMenu(scilabGraph));
		edit.add(CopyAction.copyMenu(scilabGraph));
		edit.add(PasteAction.pasteMenu(scilabGraph));
		edit.add(DeleteAction.createMenu(scilabGraph));
		edit.addSeparator();
		edit.add(SelectAllAction.createMenu(scilabGraph));
		edit.add(InvertSelectionAction.createMenu(scilabGraph));
		edit.addSeparator();
		edit.add(BlockParametersAction.createMenu(scilabGraph));
		edit.addSeparator();
		edit.add(RegionToSuperblockAction.createMenu(scilabGraph));
		Menu superblockMask = ScilabMenu.createMenu();
		superblockMask.setText(XcosMessages.SUPERBLOCK_MASK);
		superblockMask.add(SuperblockMaskCreateAction.createMenu(scilabGraph));
		superblockMask.add(SuperblockMaskRemoveAction.createMenu(scilabGraph));
		superblockMask.add(SuperblockMaskCustomizeAction.createMenu(scilabGraph));
		superblockMask.add(SuperblockMaskSaveBlockGUIAction.createMenu(scilabGraph));
		edit.add(superblockMask);
		
		/** View menu */
		Menu view = ScilabMenu.createMenu();
		view.setText(XcosMessages.VIEW);
		view.setMnemonic('V');
		menuBar.add(view);
		
		view.add(ZoomInAction.zoominMenu(scilabGraph));
		view.add(ZoomOutAction.zoomoutMenu(scilabGraph));
		view.add(FitDiagramToViewAction.createMenu(scilabGraph));
		view.add(NormalViewAction.createMenu(scilabGraph));
		view.addSeparator();
		view.add(ViewPaletteBrowserAction.createCheckBoxMenu(scilabGraph));
		view.add(ViewDiagramBrowserAction.createMenu(scilabGraph));
		CheckBoxMenuItem menu = ViewViewportAction.createCheckBoxMenu(scilabGraph);
		view.add(menu);
		((XcosDiagram) scilabGraph).setViewPortMenuItem(menu);
		view.add(ViewGetinfosAction.createMenu(scilabGraph));
		view.add(ViewDetailsAction.createMenu(scilabGraph));
		view.add(ViewBrowserAction.createMenu(scilabGraph));

		/** Simulation menu */
		Menu simulate = ScilabMenu.createMenu();
		simulate.setText(XcosMessages.SIMULATION);
		simulate.setMnemonic('S');
		menuBar.add(simulate);
		
		simulate.add(SetupAction.createMenu(scilabGraph));
		simulate.add(SetContextAction.createMenu(scilabGraph));
		simulate.add(CompileAction.createMenu(scilabGraph));
		simulate.add(StartAction.createMenu(scilabGraph));
		simulate.add(StopAction.createMenu(scilabGraph));
	
		/** Format menu */
		Menu format = ScilabMenu.createMenu();
		format.setText(XcosMessages.FORMAT);
		format.setMnemonic('F');
		menuBar.add(format);
		
		format.add(RotateAction.createMenu(scilabGraph));
		format.add(FlipAction.createMenu(scilabGraph));
		format.add(ShowHideShadowAction.createMenu(scilabGraph));
		
		format.addSeparator();
		
		format.add(DiagramBackgroundAction.createMenu(scilabGraph));
		CheckBoxMenuItem gridMenu = ViewGridAction.createCheckBoxMenu(scilabGraph);
		format.add(gridMenu);
		((XcosDiagram) scilabGraph).setGridMenuItem(menu);
		
		/** Tools menu */
		Menu tools = ScilabMenu.createMenu();
		tools.setText(XcosMessages.TOOLS);
		tools.setMnemonic('T');
		menuBar.add(tools);
		
		tools.add(CodeGenerationAction.createMenu(scilabGraph));
		
		/** Help menu */
		Menu help = ScilabMenu.createMenu();
		help.setText(XcosMessages.HELP);
		help.setMnemonic('H');
		menuBar.add(help);
		
		help.add(XcosDocumentationAction.createMenu(scilabGraph));
		help.add(BlockDocumentationAction.createMenu(scilabGraph));
		help.add(XcosDemonstrationsAction.createMenu(scilabGraph));
		help.add(AboutXcosAction.createMenu(scilabGraph));
		
		return menuBar;
    }

    public static ToolBar createToolBar(ScilabGraph scilabGraph) {
    	ToolBar toolBar = ScilabToolBar.createToolBar();
    	
    	toolBar.add(NewDiagramAction.createButton(scilabGraph));

    	toolBar.add(OpenAction.createButton(scilabGraph));
    	toolBar.add(SaveAction.createButton(scilabGraph));

    	toolBar.addSeparator();
    	
    	toolBar.add(PrintAction.createButton(scilabGraph));

    	toolBar.addSeparator();
    	
    	toolBar.add(DeleteAction.createButton(scilabGraph));

    	toolBar.addSeparator();
    	
    	// UNDO / REDO
    	toolBar.add(UndoAction.undoButton(scilabGraph));
    	toolBar.add(RedoAction.redoButton(scilabGraph));
	
    	toolBar.addSeparator();

    	toolBar.add(FitDiagramToViewAction.createButton(scilabGraph));
    		
    	toolBar.addSeparator();

    	// START / STOP
    	toolBar.add(StartAction.createButton(scilabGraph));
    	toolBar.add(StopAction.createButton(scilabGraph));

    	toolBar.addSeparator();
    	
    	// ZOOMIN / ZOOMOUT
    	toolBar.add(ZoomInAction.zoominButton(scilabGraph));
    	toolBar.add(ZoomOutAction.zoomoutButton(scilabGraph));
    	
    	toolBar.addSeparator();

    	return toolBar;
    }

    public Xcos(XcosDiagram diagram) {
	super(XcosMessages.XCOS);
	
	this.setCallback(new CloseAction(diagram));
	this.setContentPane(diagram.getAsComponent());
	
    }
    
    public static void closeDiagram(XcosDiagram diagramm) {
    	diagrams.remove(diagramm);
    	if (diagrams.size() == 0) {
    		closeSession();
    	}
    }
    
    
    
    public static XcosDiagram createEmptyDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	showDiagram(xcosDiagramm);
	return xcosDiagramm;
    }
    
    public static void showDiagram(XcosDiagram xcosDiagram) {
	Window main = ScilabWindow.createWindow();
	main.setTitle(XcosMessages.XCOS);

	
	diagrams.add(xcosDiagram);
	
	Tab tab = new Xcos(xcosDiagram);
	tab.setName(XcosMessages.UNTITLED);
	xcosDiagram.setParentTab(tab);
	main.setVisible(true);
	main.addTab(tab);
	/*
	 * MENU BAR
	 */
	MenuBar menuBar = createMenuBar(xcosDiagram);
	tab.addMenuBar(menuBar);

	/*
	 * TOOL BAR
	 */
	ToolBar toolBar = createToolBar(xcosDiagram);
	tab.addToolBar(toolBar);

	/*
	 * VIEW PORT
	 */
	createViewPort(xcosDiagram);
	

	/*
	 * INFO BAR
	 */
	tab.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());
    }

    public SimpleTab getAsSimpleTab() {
	return this;
    }

    public Window getParentWindow() {
	return null;
    }

    public void addInfoBar(TextBox infoBarToAdd) {
    }

    public void addMenuBar(MenuBar menuBarToAdd) {
	((SwingScilabTab) this).setMenuBar(menuBarToAdd);
    }

    public void addToolBar(ToolBar toolBarToAdd) {
	((SwingScilabTab) this).setToolBar(toolBarToAdd);
    }

    public static void closeSession() {
    	for (int i = diagrams.size() - 1; i >= 0; i--) {
    		diagrams.get(i).closeDiagram();
    	}
    	ViewPaletteBrowserAction.setPalettesVisible(false);
    }
    
    public static Tab getPalettes() {
    	return palette;
    }
    
    public static ArrayList<XcosDiagram> getDiagrams() {
    	return diagrams;
    }
}
