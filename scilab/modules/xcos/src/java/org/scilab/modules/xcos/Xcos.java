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
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JTabbedPane;

import org.scilab.modules.action_binding.InterpreterManagement;
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
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.actions.AboutXcosAction;
import org.scilab.modules.xcos.actions.AlignBlockAction;
import org.scilab.modules.xcos.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.actions.BlockParametersAction;
import org.scilab.modules.xcos.actions.CloseAction;
import org.scilab.modules.xcos.actions.ClosePalettesAction;
import org.scilab.modules.xcos.actions.CloseViewportAction;
import org.scilab.modules.xcos.actions.CodeGenerationAction;
import org.scilab.modules.xcos.actions.ColorAction;
import org.scilab.modules.xcos.actions.CompileAction;
import org.scilab.modules.xcos.actions.DebugLevelAction;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.DumpAction;
import org.scilab.modules.xcos.actions.ExportAction;
import org.scilab.modules.xcos.actions.ExportToXMLAction;
import org.scilab.modules.xcos.actions.FitDiagramToViewAction;
import org.scilab.modules.xcos.actions.FlipAction;
import org.scilab.modules.xcos.actions.ImportFromXMLAction;
import org.scilab.modules.xcos.actions.LinkStyleAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.NewPaletteAction;
import org.scilab.modules.xcos.actions.NormalViewAction;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.actions.PrintAction;
import org.scilab.modules.xcos.actions.QuitAction;
import org.scilab.modules.xcos.actions.RecentFileAction;
import org.scilab.modules.xcos.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.actions.RotateAction;
import org.scilab.modules.xcos.actions.SaveAction;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.actions.StartAction;
import org.scilab.modules.xcos.actions.StopAction;
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
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;
import com.mxgraph.util.mxConstants;

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
	private static List<Menu> recentsMenus = new ArrayList<Menu>();
	private static List<MenuItem> startMenuItems = new ArrayList<MenuItem>();
	private static List<MenuItem> stopMenuItems = new ArrayList<MenuItem>();
	private static List<PushButton> startPushButtons = new ArrayList<PushButton>();
	private static List<PushButton> stopPushButtons = new ArrayList<PushButton>();
	private static boolean startEnabled  = true;

	/** Palette creation */
	static {
		/* load scicos libraries (macros) */
		InterpreterManagement.requestScilabExec("loadScicosLibs();");
		
		paletteThread = new Thread() {
			public void run() {
				// Add a default Java bloc in HashMap
				allBlocks.put("TEXT_f", new TextBlock("TEXT_f"));
				
				Window palWin = ScilabWindow.createWindow();
				palWin.setDims(new Size(700, 600));
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
				allpalettes.setTabLayoutPolicy(JTabbedPane.WRAP_TAB_LAYOUT); /* View all tabs at a time */
				
				((SwingScilabTab) palette.getAsSimpleTab()).setContentPane(allpalettes);

				/** Create SOURCES palette */
				String[] sourcesBlocksNames = {
				    "CLKINV_f", 
				    "CLOCK_c", 
				    "CONST_m",
				    "CURV_f", 
				    "Counter", 
				    "FROMWSB",
				    "GENSIN_f", 
				    "GENSQR_f", 
				    "INIMPL_f",
				    "IN_f", 
				    "Modulo_Count", 
				    "RAMP", 
				    "RAND_m", 
				    "READAU_f", 
				    "READC_f", 
				    "RFILE_f", 
				    "SAWTOOTH_f", 
				    "STEP_FUNCTION", 
				    "SampleCLK", 
				    "Sigbuilder", 
				    "TIME_f", 
				    "TKSCALE"
				}; 
				allpalettes.addTab(XcosMessages.SOURCES_PAL, createPalette(sourcesBlocksNames));

				palette.setVisible(true);

				/** Create CONTINUOUS palette */
				String[] continuousBlocksNames = {
				    "CLR", 
				    "CLSS", 
				    "DERIV", 
				    "INTEGRAL_f",
				    "INTEGRAL_m", 
				    "PID", 
				    "TCLSS", 
				    "TIME_DELAY", 
				    "VARIABLE_DELAY"
				};
				allpalettes.addTab(XcosMessages.CONTINUOUS_PAL, createPalette(continuousBlocksNames));

				/** Create DISCONTINUOUS palette */
				String[] discontinuousBlocksNames = {
				    "BACKLASH", 
				    "DEADBAND", 
				    "DELAYV_f",
				    "HYSTHERESIS", 
				    "RATELIMITER", 
				    "REGISTER",
				    "SATURATION"
				};
				allpalettes.addTab(XcosMessages.DISCONTINUOUS_PAL, createPalette(discontinuousBlocksNames));

				/** Create LOOKUP TABLES palette */
				String[] lookupBlocksNames = {
				    "INTRP2BLK_f", 
				    "INTRPLBLK_f",
				    "LOOKUP_f"
				};
				allpalettes.addTab(XcosMessages.LOOKUPTABLES_PAL, createPalette(lookupBlocksNames));

				/** Create SIGNAL PROCESSING palette */
				String[] signalBlocksNames = {
				    "MCLOCK_f", 
				    "MFCLCK_f", 
				    "QUANT_f", 
				    "SAMPHOLD_m"
				};
				allpalettes.addTab(XcosMessages.SIGNALPROCESSING_PAL, createPalette(signalBlocksNames));

				/** Create THRESHOLD palette */
				String[] thresholdBlocksNames = {
				    "CLINDUMMY_f",
				    "GENERAL_f",
				    "NEGTOPOS_f", 
				    "POSTONEG_f", 
				    "ZCROSS_f"
				};
				allpalettes.addTab(XcosMessages.THRESHOLD_PAL, createPalette(thresholdBlocksNames));

				/** Create MATH OPERATIONS palette */
				String[] mathsBlocksNames = {
				    "ABS_VALUE", 
				    "BIGSOM_f", 
				    "CONSTRAINT_f",
				    "COSBLK_f", 
				    "EXPBLK_m", 
				    "GAINBLK_f", 
				    "INVBLK", 
				    "LOGBLK_f",
				    "MATDIV", 
				    "MATMAGPHI", 
				    "MATZREIM", 
				    "MAXMIN", 
				    "MAX_f",
				    "MIN_f", 
				    "POWBLK_f",
				    "PRODUCT", 
				    "PROD_f", 
				    "SIGNUM", 
				    "SINBLK_f", 
				    "SQRT", 
				    "SUMMATION", 
				    "SUM_f", 
				    "TANBLK_f",
				    "TrigFun"
				};
				allpalettes.addTab(XcosMessages.MATHSOPS_PAL, createPalette(mathsBlocksNames));

				/** Create MODELICA palette */
				String[] modelicaBlocksNames = {
				    "FROMMO", 
				    "GOTOMO", 
				    "GotoTagVisibilityMO", 
				    "MBLOCK", 
				    "OUTIMPL_f"
				};
				allpalettes.addTab(XcosMessages.MODELICA_PAL, createPalette(modelicaBlocksNames));

				/** Create INTEGER palette */
				String[] integerBlocksNames = {
				    "BITCLEAR", 
				    "BITSET", 
				    "CONVERT", 
				    "DFLIPFLOP", 
				    "DLATCH", 
				    "EXTRACTBITS", 
				    "INTMUL", 
				    "JKFLIPFLOP", 
				    "LOGIC",
				    "SHIFT", 
				    "SRFLIPFLOP"
				};
				allpalettes.addTab(XcosMessages.INTEGER_PAL, createPalette(integerBlocksNames));

				/** Create MATRIX palette */
				String[] matrixBlocksNames = {
				    "CUMSUM", 
				    "EXTRACT", 
				    "EXTTRI", 
				    "MATBKSL", 
				    "MATCATH", 
				    "MATCATV", 
				    "MATDET", 
				    "MATDIAG", 
				    "MATDIV", 
				    "MATEIG",
				    "MATEXPM", 
				    "MATINV", 
				    "MATLU", 
				    "MATMAGPHI", 
				    "MATMUL", 
				    "MATPINV", 
				    "MATRESH", 
				    "MATSING", 
				    "MATSUM", 
				    "MATTRAN", 
				    "MATZCONJ", 
				    "MATZREIM", 
				    "RICC", 
				    "ROOTCOEF", 
				    "SQRT", 
				    "SUBMAT"
				};
				allpalettes.addTab(XcosMessages.MATRIX_PAL, createPalette(matrixBlocksNames));

				/** Create SINKS palette */
				String[] sinksBlocksNames = {
				    "AFFICH_m", 
				    "CANIMXY", 
				    "CANIMXY3D",
				    "CFSCOPE", 
				    "CMAT3D", 
				    "CMATVIEW", 
				    "CMSCOPE", 
				    "CSCOPE", 
				    "CSCOPXY", 
				    "CSCOPXY3D", 
				    "TOWS_c", 
				    "TRASH_f",
				    "WFILE_f",
				    "WRITEAU_f", 
				    "WRITEC_f"
				};
				allpalettes.addTab(XcosMessages.SINKS_PAL, createPalette(sinksBlocksNames));

				/** Create PORT ACTION palette */
				String[] portactionBlocksNames = {
				    "EDGE_TRIGGER",
				    "ESELECT_f", 
				    "Extract_Activation", 
				    "IFTHEL_f",
				    "IN_f",
				    "OUT_f"
				};
				allpalettes.add(XcosMessages.PORTACTION_PAL, createPalette(portactionBlocksNames));

				/** Create DISCRETE palette */
				String[] discreteBlocksNames = {
				    "DELAYV_f",
				    "DELAY_f", 
				    "DLR", 
				    "DLRADAPT_f", 
				    "DLSS", 
				    "DOLLAR_f"
				};
				allpalettes.addTab(XcosMessages.DISCRETE_PAL, createPalette(discreteBlocksNames));

				/** Create EVENTS palette */
				String[] eventsBlocksNames = {
				    "ANDBLK", 
				    "ANDLOG_f",
				    "CEVENTSCOPE",
				    "CLKFROM", 
				    "CLKGOTO", 
				    "CLKGotoTagVisibility", 
				    "CLKOUTV_f", 
				    "CLKSOMV_f", 
				    "CLOCK_c", 
				    "ESELECT_f",
				    "EVTDLY_c", 
				    "EVTGEN_f", 
				    "EVTVARDLY", 
				    "HALT_f", 
				    "IFTHEL_f", 
				    "M_freq", 
				    "SampleCLK", 
				    "freq_div"
				};
				allpalettes.addTab(XcosMessages.EVENTS_PAL, createPalette(eventsBlocksNames));

				/** Create SIGNAL ROUTING palette */
				String[] routingBlocksNames = {
				    "ANDBLK", 
				    "DEMUX",
				    "EXTRACTOR", 
				    "FROM", 
				    "GOTO", 
				    "GotoTagVisibility", 
				    "ISELECT_m", 
				    "MUX", 
				    "M_SWITCH", 
				    "NRMSOM_f", 
				    "RELAY_f", 
				    "SELECT_m", 
				    "SWITCH2_m", 
				    "SWITCH_f"
				};
				allpalettes.addTab(XcosMessages.SIGNALROUTING_PAL, createPalette(routingBlocksNames));

				/** Create COMMONLY USED BLOCKS palette */
				String[] commonBlocksNames = {
				    "ANDBLK", 
				    "BIGSOM_f",
				    "CMSCOPE", 
				    "CONST_m", 
				    "CONVERT", 
				    "CSCOPXY", 
				    "DEMUX", 
				    "DOLLAR_f", 
				    "INTEGRAL_f", 
				    "IN_f", 
				    "LOGICAL_OP", 
				    "MUX", 
				    "NRMSOM_f", 
				    "OUT_f", 
				    "PRODUCT", 
				    "RELATIONALOP", 
				    "SATURATION", 
				    "SWITCH2_m",
				    "TEXT_f"
				};
				allpalettes.addTab(XcosMessages.COMMONUSED_PAL, createPalette(commonBlocksNames));

				/** Create USER-DEFINED FUNCTIONS palette */
				String[] userdefinedBlocksNames = {
				    "CBLOCK",
				    "EXPRESSION", 
				    "MBLOCK", 
				    "PDE", 
				    "SUPER_f",
				    "TEXT_f", 
				    "c_block", 
				    "fortran_block", 
				    "generic_block3", 
				    "scifunc_block_m"
				};
				allpalettes.addTab(XcosMessages.USERDEFINEDFUNCTIONS_PAL, createPalette(userdefinedBlocksNames));

				/** Create ELECTRICAL palette */
				String[] electricalBlocksNames = {
				    "CCS", 
				    "CVS",
				    "Capacitor", 
				    "ConstantVoltage", 
				    "CurrentSensor", 
				    "Diode",
				    "Ground", 
				    "Gyrator",
				    "IdealTransformer", 
				    "Inductor",
				    "NMOS", 
				    "NPN", 
				    "OpAmp", 
				    "PMOS", 
				    "PNP", 
				    "PotentialSensor", 
				    "Resistor", 
				    "SineVoltage", 
				    "Switch", 
				    "VVsourceAC", 
				    "VariableResistor", 
				    "VoltageSensor", 
				    "VsourceAC"
				};
				allpalettes.addTab(XcosMessages.ELECTRICAL_PAL, createPalette(electricalBlocksNames));

				/** Create THERMO-HYDRAULICS palette */
				String[] thermoHydraulicsBlocksNames = {
				    "Bache", 
				    "PerteDP", 
				    "PuitsP", 
				    "SourceP",
				    "VanneReglante"
				};
				allpalettes.addTab(XcosMessages.THERMOHYDRAULICS_PAL, createPalette(thermoHydraulicsBlocksNames));

				/** Create DEMO-BLOCKS palette */
				String[] demoBlocksNames = {
				    "AUTOMAT", 
				    "BOUNCE", 
				    "BOUNCEXY", 
				    "BPLATFORM", 
				    "PDE"
				};
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
    	ConfigXcosManager.saveToRecentOpenedFiles(fileName);
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
    	String palImagesPath = System.getenv("SCI") + "/modules/xcos/images/palettes/";

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
    		
    		palette.addTemplate(blocksNames[kBlock], new ImageIcon(palImagesPath + blocksNames[kBlock] + ".jpg"), theBloc);
    	}

    	return palette;
    }
    
    public static MenuBar createMenuBar(ScilabGraph scilabGraph) {
		ConfigXcosManager.createUserCopy();	
		ArrayList<File> recentFiles = ConfigXcosManager.getAllRecentOpenedFiles();

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
		
		Menu recentsMenu = ScilabMenu.createMenu();
		recentsMenu.setText(XcosMessages.RECENT_FILES);
		for (int i = 0; i < recentFiles.size(); i++) {
			recentsMenu.add(RecentFileAction.createMenu(scilabGraph , recentFiles.get(i)));
		}
		recentsMenus.add(recentsMenu);
		fileMenu.add(recentsMenu);
		
		
		//fileMenu.add(SaveAsInterfaceFunctionAction.createMenu(scilabGraph));
		//fileMenu.addSeparator();
		fileMenu.add(PrintAction.createMenu(scilabGraph));
		fileMenu.addSeparator();
		fileMenu.add(CloseAction.createMenu(scilabGraph));
		fileMenu.addSeparator();
		fileMenu.add(QuitAction.createMenu(scilabGraph));
		
		
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
//		Menu superblockMask = ScilabMenu.createMenu();
//		superblockMask.setText(XcosMessages.SUPERBLOCK_MASK);
//		superblockMask.add(SuperblockMaskCreateAction.createMenu(scilabGraph));
//		superblockMask.add(SuperblockMaskRemoveAction.createMenu(scilabGraph));
//		superblockMask.add(SuperblockMaskCustomizeAction.createMenu(scilabGraph));
//		superblockMask.add(SuperblockMaskSaveBlockGUIAction.createMenu(scilabGraph));
//		edit.add(superblockMask);
		
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
		//view.add(ViewGetinfosAction.createMenu(scilabGraph));
		view.add(ViewDetailsAction.createMenu(scilabGraph));

		/** Simulation menu */
		Menu simulate = ScilabMenu.createMenu();
		simulate.setText(XcosMessages.SIMULATION);
		simulate.setMnemonic('S');
		menuBar.add(simulate);
		
		MenuItem startMenu =  StartAction.createMenu(scilabGraph);
		startMenuItems.add(startMenu);
		startMenu.setEnabled(startEnabled);
		MenuItem stopMenu =  StopAction.createMenu(scilabGraph);
		stopMenuItems.add(stopMenu);
		stopMenu.setEnabled(startEnabled);
		
		
		simulate.add(SetupAction.createMenu(scilabGraph));
		simulate.add(DebugLevelAction.createMenu(scilabGraph));
		simulate.add(SetContextAction.createMenu(scilabGraph));
		simulate.add(CompileAction.createMenu(scilabGraph));
		simulate.add(startMenu);
		simulate.add(stopMenu);
	
		/** Format menu */
		Menu format = ScilabMenu.createMenu();
		format.setText(XcosMessages.FORMAT);
		format.setMnemonic('O');
		menuBar.add(format);
		
		format.add(RotateAction.createMenu(scilabGraph));
		format.add(FlipAction.createMenu(scilabGraph));
		format.add(ShowHideShadowAction.createMenu(scilabGraph));
		
		format.addSeparator();
		Menu alignMenu = ScilabMenu.createMenu();
		alignMenu.setText(XcosMessages.ALIGN_BLOCKS);
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_LEFT, mxConstants.ALIGN_LEFT));
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_RIGHT, mxConstants.ALIGN_RIGHT));
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_CENTER, mxConstants.ALIGN_CENTER));
		alignMenu.addSeparator();
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_TOP, mxConstants.ALIGN_TOP));
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_BOTTOM, mxConstants.ALIGN_BOTTOM));
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_MIDDLE, mxConstants.ALIGN_MIDDLE));
		format.add(alignMenu);
		format.addSeparator();
		
		format.add(ColorAction.createMenu(scilabGraph, XcosMessages.BORDER_COLOR, mxConstants.STYLE_STROKECOLOR));
		format.add(ColorAction.createMenu(scilabGraph, XcosMessages.FILL_COLOR, mxConstants.STYLE_FILLCOLOR));
		format.addSeparator();
		
		Menu linkStyle = ScilabMenu.createMenu();
		linkStyle.setText(XcosMessages.LINK_STYLE);
		linkStyle.add(LinkStyleAction.createMenu(scilabGraph, XcosMessages.LINK_STYLE_STRAIGHT, mxConstants.SHAPE_CONNECTOR));
		linkStyle.add(LinkStyleAction.createMenu(scilabGraph, XcosMessages.LINK_STYLE_HORIZONTAL, mxConstants.ELBOW_HORIZONTAL));
		linkStyle.add(LinkStyleAction.createMenu(scilabGraph, XcosMessages.LINK_STYLE_VERTICAL, mxConstants.ELBOW_VERTICAL));
		format.add(linkStyle);
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
		help.setMnemonic('?');
		menuBar.add(help);
		
		help.add(XcosDocumentationAction.createMenu(scilabGraph));
		help.add(BlockDocumentationAction.createMenu(scilabGraph));
		help.addSeparator();
		help.add(XcosDemonstrationsAction.createMenu(scilabGraph));
		help.addSeparator();
		help.add(AboutXcosAction.createMenu(scilabGraph));
		
		/** Debug menu */
		// DEBUG / RELEASE option
		if(false) {
		    Menu debug = ScilabMenu.createMenu();
		    debug.setText("DEBUG");
		    menuBar.add(debug);
		    debug.add(DumpAction.dumpMenu(scilabGraph));
		    debug.add(ViewInScicosAction.viewInScicosMenu(scilabGraph));
		    debug.add(ExportToXMLAction.createMenu(scilabGraph));
		    debug.add(ImportFromXMLAction.createMenu(scilabGraph));
		}
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
    	PushButton startPushButton = StartAction.createButton(scilabGraph);
    	PushButton stopPushButton = StopAction.createButton(scilabGraph);
    	startPushButton.setEnabled(startEnabled);
    	stopPushButton.setEnabled(!startEnabled);
    	startPushButtons.add(startPushButton);
    	stopPushButtons.add(stopPushButton);
    	
    	toolBar.add(startPushButton);
    	toolBar.add(stopPushButton);

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

	// Get the palettes position
	if (!paletteLoaded && palette != null && palette.getParentWindow() != null) { // If at Xcos startup
		Position palPosition = palette.getParentWindow().getPosition();
		Size palSize = palette.getParentWindow().getDims();
		Position mainPosition = new Position(palPosition.getX() + palSize.getWidth(), palPosition.getY());
		main.setPosition(mainPosition);
	}
	
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

    
	public static List<MenuItem> getStartMenuItems() {
		return startMenuItems;
	}
	
	public static List<MenuItem> getStopMenuItems() {
		return stopMenuItems;
	}
	
	public static List<PushButton> getStopPushButtons() {
		return stopPushButtons;
	}
	
	public static List<PushButton> getStartPushButtons() {
		return startPushButtons;
	}
		
	public static void setStartEnabled(boolean status){
		for (int i = 0 ; i < getStartMenuItems().size() ; i++ ){
			getStartMenuItems().get(i).setEnabled(status);
			getStartPushButtons().get(i).setEnabled(status);
			startEnabled = status;
			
			getStopMenuItems().get(i).setEnabled(!status);
			getStopPushButtons().get(i).setEnabled(!status);
		}
		
	}   
	    
	/**
	 * Get recent file menu
	 * @return the menu List
	 */
	public static List<Menu> getRecentsMenu() {
		return  recentsMenus;
	}

	
	/**
	 * Update menu displaying recent opened files
	 */
	public static void updateRecentOpenedFilesMenu(ScilabGraph scilabGraph) {
		ArrayList<File> recentFiles = ConfigXcosManager.getAllRecentOpenedFiles();

		for (int j = 0 ; j < getRecentsMenu().size() ; j++) {
			((SwingScilabMenu) getRecentsMenu().get(j).getAsSimpleMenu()).removeAll();

			for (int i = 0; i < recentFiles.size(); i++) {
				getRecentsMenu().get(j).add(RecentFileAction.createMenu(scilabGraph , recentFiles.get(i)));
			}
		}
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
