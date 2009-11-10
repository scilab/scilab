/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import java.util.HashMap;

import javax.swing.ImageIcon;
import javax.swing.JTabbedPane;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.actions.ClosePalettesAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.utils.XcosMessages;

public class XcosPaletteManager {

    private static Thread paletteThread;
    private static boolean paletteLoadStarted;
    private static Tab palettes;
    private static HashMap<String, BasicBlock> allBlocks = new HashMap<String, BasicBlock>();


    /** Palette creation */
    static {
	paletteThread = new Thread() {
	    public void run() {

		JTabbedPane allpalettes = new JTabbedPane();
		allpalettes.setTabPlacement(JTabbedPane.BOTTOM);
		allpalettes.setTabLayoutPolicy(JTabbedPane.WRAP_TAB_LAYOUT); /* View all tabs at a time */
		// Add a default Java bloc in HashMap
		 allBlocks.put("TEXT_f", new TextBlock("TEXT_f"));
		((SwingScilabTab) palettes.getAsSimpleTab()).setContentPane(allpalettes);

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

		palettes.setVisible(true);

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
		    this.notifyAll();
		} 

		paletteLoadStarted = true;
	    }
	};
    }

    private XcosPaletteManager() {
    }

    public static Tab loadPalette() {

	if(paletteLoadStarted == false) {
	    createPaletteWindow();
	    paletteThread.start();
	}
	return palettes;
    }

    private static XcosPalette createPalette(String[] blocksNames) {

	String blocksPath = System.getenv("SCI") + "/modules/scicos_blocks/blocks/";
	String palImagesPath = System.getenv("SCI") + "/modules/xcos/images/palettes/";

	XcosPalette xcosPalette = new XcosPalette();

	BasicBlock theBloc = null;
	for (int kBlock = 0; kBlock < blocksNames.length; kBlock++) {

//	    Search the bloc in global hashmap
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

	    xcosPalette.addTemplate(blocksNames[kBlock], new ImageIcon(palImagesPath + blocksNames[kBlock] + ".jpg"), theBloc);
	}

	return xcosPalette;
    }

    public static Tab getPalettes() {
        return palettes;
    }
    
    private static void createPaletteWindow() {
	paletteLoadStarted = false;
	// Add a default Java bloc in HashMap
	//Xcos.getAllBlocks().put("TEXT_f", new TextBlock("TEXT_f"));

	Window palWin = ScilabWindow.createWindow();
	palWin.setDims(new Size(700, 600));
	palWin.setVisible(true);
	palettes = ScilabTab.createTab(XcosMessages.PALETTE_BROWSER);

	palettes.setCallback(new ClosePalettesAction(null));

	/** Create the menubar */
	MenuBar palMenuBar = ScilabMenuBar.createMenuBar();
	palettes.addMenuBar(palMenuBar);

	Menu palMenu = ScilabMenu.createMenu();
	palMenu.setText(XcosMessages.PALETTES);
	palMenu.setMnemonic('P');
	palMenuBar.add(palMenu);

	palMenu.add(ClosePalettesAction.createMenu(null));

	/** Create the toolbar */
	ToolBar palToolbar = ScilabToolBar.createToolBar();
	palettes.addToolBar(palToolbar);
	palToolbar.add(NewDiagramAction.createButton(null));
	palToolbar.add(OpenAction.createButton(null));

	/** Create the infobar */
	palettes.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());

	palWin.addTab(palettes);
    }
    
    public static boolean isVisible() {
	if(palettes != null && palettes.isVisible() == true) {
	    return true;
	}
	return false;
    }
}
