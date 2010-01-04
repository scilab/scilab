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

import java.awt.Component;
import java.awt.Dimension;
import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;

import javax.swing.ImageIcon;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.palette.actions.ClosePalettesAction;
import org.scilab.modules.xcos.palette.actions.LoadAsPalAction;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosComponent;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Manage all the Block Palettes.
 */
public final class XcosPaletteManager {

    private static Thread paletteThread;
    private static boolean paletteLoadStarted;
    private static Tab palettes;
    private static JTree paletteTree = null;
    private static DefaultMutableTreeNode rootNode = null; 
    private static DefaultMutableTreeNode userDefinedNode = null; 
    private static DefaultTreeModel paletteTreeModel = null;
	/**
     * Instantiate all the known names (default configuration)
     */
    private static final PaletteStringDescriptor[] allPalettesStringDescriptor = {
	    /** COMMONLY USED BLOCKS palette */
	    new PaletteStringDescriptor(XcosMessages.COMMONUSED_PAL,
		    new String[] { "ANDBLK", "BIGSOM_f", "CMSCOPE", "CONST_m",
			    "CONVERT", "CSCOPXY", "DEMUX", "DOLLAR_f",
			    "INTEGRAL_f", "IN_f", "LOGICAL_OP", "MUX",
			    "NRMSOM_f", "OUT_f", "PRODUCT", "RELATIONALOP",
			    "SATURATION", "SWITCH2_m", "TEXT_f" }),

	    /** CONTINUOUS palette */
	    new PaletteStringDescriptor(XcosMessages.CONTINUOUS_PAL,
		    new String[] { "CLINDUMMY_f", "CLR", "CLSS", "DERIV",
			    "INTEGRAL_f", "INTEGRAL_m", "PID", "TCLSS",
			    "TIME_DELAY", "VARIABLE_DELAY", "PDE" }),

	    /** DISCONTINUOUS palette */
	    new PaletteStringDescriptor(XcosMessages.DISCONTINUOUS_PAL,
		    new String[] { "BACKLASH", "DEADBAND", "DELAYV_f",
			    "HYSTHERESIS", "RATELIMITER", "QUANT_f",
			    "SATURATION" }),

	    /** DISCRETE palette */
	    new PaletteStringDescriptor(XcosMessages.DISCRETE_PAL,
		    new String[] { "AUTOMAT", "DELAYV_f", "DELAY_f", "DLR",
			    "DLRADAPT_f", "DLSS", "DOLLAR_f", "SAMPHOLD_m",
			    "TCLSS" }),

	    /** LOOKUP TABLES palette */
	    new PaletteStringDescriptor(XcosMessages.LOOKUPTABLES_PAL,
		    new String[] { "INTRP2BLK_f", "INTRPLBLK_f", "LOOKUP_f" }),

	    /** EVENTS palette */
	    new PaletteStringDescriptor(XcosMessages.EVENTS_PAL, new String[] {
		    "ANDBLK", "ANDLOG_f", "CEVENTSCOPE", "CLKFROM", "CLKGOTO",
		    "CLKGotoTagVisibility", "CLKOUTV_f", "CLKSOMV_f",
		    "CLOCK_c", "EDGE_TRIGGER", "ENDBLK", "END_c", "ESELECT_f",
		    "EVTDLY_c", "EVTGEN_f", "EVTVARDLY", "Extract_Activation",
		    "HALT_f", "IFTHEL_f", "M_freq", "MCLOCK_f", "MFCLCK_f",
		    "REGISTER", "SampleCLK", "freq_div" }),

	    /** MATH OPERATIONS palette */
	    new PaletteStringDescriptor(XcosMessages.MATHSOPS_PAL,
		    new String[] { "ABS_VALUE", "BIGSOM_f", "COSBLK_f",
			    "EXPBLK_m", "GAINBLK_f", "INVBLK", "LOGBLK_f",
			    "MATMAGPHI", "MATZREIM", "MAXMIN", "MAX_f",
			    "MIN_f", "POWBLK_f", "PRODUCT", "PROD_f", "SIGNUM",
			    "SINBLK_f", "SQRT", "SUMMATION", "SUM_f",
			    "TANBLK_f", "TrigFun" }),

	    /** MATRIX palette */
	    new PaletteStringDescriptor(XcosMessages.MATRIX_PAL, new String[] {
		    "CUMSUM", "EXTRACT", "EXTTRI", "MATBKSL", "MATCATH",
		    "MATCATV", "MATDET", "MATDIAG", "MATDIV", "MATEIG",
		    "MATEXPM", "MATINV", "MATLU", "MATMAGPHI", "MATMUL",
		    "MATPINV", "MATRESH", "MATSING", "MATSUM", "MATTRAN",
		    "MATZCONJ", "MATZREIM", "RICC", "ROOTCOEF", "SQRT",
		    "SUBMAT" }),

	    /** ELECTRICAL palette */
	    new PaletteStringDescriptor(XcosMessages.ELECTRICAL_PAL,
		    new String[] { "CCS", "CVS", "Capacitor",
			    "ConstantVoltage", "CurrentSensor", "Diode",
			    "Ground", "Gyrator", "IdealTransformer",
			    "Inductor", "NMOS", "NPN", "OpAmp", "PMOS", "PNP",
			    "PotentialSensor", "Resistor", "SineVoltage",
			    "Switch", "VVsourceAC", "VariableResistor",
			    "VoltageSensor", "VsourceAC" }),

	    /** INTEGER palette */
	    new PaletteStringDescriptor(XcosMessages.INTEGER_PAL, new String[] {
		    "BITCLEAR", "BITSET", "CONVERT", "DFLIPFLOP", "DLATCH",
		    "EXTRACTBITS", "INTMUL", "JKFLIPFLOP", "LOGIC", "SHIFT",
		    "SRFLIPFLOP" }),

	    /** PORT ACTION palette */
	    new PaletteStringDescriptor(XcosMessages.PORTACTION_PAL,
		    new String[] { "CLKINV_f", "CLKOUTV_f", "IN_f", "INIMPL_f",
			    "OUTIMPL_f", "OUT_f", "SUPER_f" }),

	    /** THRESHOLD palette */
	    new PaletteStringDescriptor(XcosMessages.THRESHOLD_PAL,
		    new String[] { "GENERAL_f", "NEGTOPOS_f", "POSTONEG_f",
			    "ZCROSS_f" }),

	    /** Signal Routing palette */
	    new PaletteStringDescriptor(XcosMessages.SIGNALROUTING_PAL,
		    new String[] { "DEMUX", "EXTRACTOR", "FROM", "FROMMO",
			    "GOTO", "GOTOMO", "GotoTagVisibility",
			    "GotoTagVisibilityMO", "ISELECT_m", "MUX",
			    "M_SWITCH", "NRMSOM_f", "RELAY_f", "SELECT_m",
			    "SWITCH2_m", "SWITCH_f" }),

	    /** SIGNAL PROCESSING palette */
	    new PaletteStringDescriptor(XcosMessages.SIGNALPROCESSING_PAL,
		    new String[] { "QUANT_f", "SAMPHOLD_m" }),

	    /** IMPLICIT palette */
	    new PaletteStringDescriptor(XcosMessages.IMPLICIT_PAL,
		    new String[] { "CONSTRAINT_f", "DIFF_f" }),

	    /** ANNOTATIONS palette */
	    new PaletteStringDescriptor(XcosMessages.ANNOTATIONS_PAL,
		    new String[] { "TEXT_f" }),

	    /** SINKS palette */
	    new PaletteStringDescriptor(XcosMessages.SINKS_PAL, new String[] {
		    "AFFICH_m", "CANIMXY", "CANIMXY3D", "CFSCOPE", "CLKOUTV_f",
		    "CMAT3D", "CMATVIEW", "CMSCOPE", "CSCOPE", "CSCOPXY",
		    "CSCOPXY3D", "ENDBLK", "END_c", "HALT_f", "OUTIMPL_f",
		    "OUT_f", "TOWS_c", "TRASH_f", "WFILE_f", "WRITEAU_f",
		    "WRITEC_f" }),

	    /** SOURCES palette */
	    new PaletteStringDescriptor(XcosMessages.SOURCES_PAL, new String[] {
		    "CLKINV_f", "CLOCK_c", "CONST_m", "CURV_f", "Counter",
		    "FROMWSB", "GENSIN_f", "GENSQR_f", "INIMPL_f", "IN_f",
		    "Modulo_Count", "RAMP", "RAND_m", "READAU_f", "READC_f",
		    "RFILE_f", "SAWTOOTH_f", "STEP_FUNCTION", "SampleCLK",
		    "Sigbuilder", "TIME_f", "TKSCALE" }),

	    /** THERMO-HYDRAULICS palette */
	    new PaletteStringDescriptor(XcosMessages.THERMOHYDRAULICS_PAL,
		    new String[] { "Bache", "PerteDP", "PuitsP", "SourceP",
			    "VanneReglante" }),

	    /** DEMO-BLOCKS palette */
	    new PaletteStringDescriptor(XcosMessages.DEMOBLOCKS_PAL,
		    new String[] { "BOUNCE", "BOUNCEXY", "BPLATFORM" }),

	    /** USER-DEFINED FUNCTIONS palette */
	    new PaletteStringDescriptor(XcosMessages.USERDEFINEDFUNCTIONS_PAL,
		    new String[] { "CBLOCK", "DEBUG_SCICOS", "EXPRESSION",
			    "MBLOCK", "PDE", "SUPER_f", "c_block",
			    "fortran_block", "generic_block3",
			    "scifunc_block_m" }) };

    /**
     * Represent a palette configuration values
     */
    private static class PaletteStringDescriptor {
	public String Name;
	public String[] Components;

	public PaletteStringDescriptor(String name, String[] components) {
	    Name = name;
	    Components = components;
	}
    }

    /**
     * Represent the instanciation of the configurations values
     */
    private static class PaletteDescriptor {
	public String Name;
	public PaletteData[] Components;

	public PaletteDescriptor(String name, PaletteData[] components) {
	    Name = name;
	    Components = components;
	}

	public String toString() {
	    return Name;
	}
    }

    /**
     * Represent any block data
     */
    private static class PaletteData {
	public String Name;
	public ImageIcon Icon;

	/**
	 * Any PaletteBlock data (arguments of the
	 * org.scilab.modules.xcos.palette.XcosPalette#addTemplate)
	 * 
	 * @param name
	 *            The name of the block
	 * @param icon
	 *            The icon of the block
	 * @param block
	 *            Extracted from PATH/BlockName.hf5
	 */
	public PaletteData(String name, ImageIcon icon) {
	    Name = name;
	    Icon = icon;
	}
    }

    /** Palette creation */
    static {
	paletteThread = new Thread() {
	    public void run() {

		final JSplitPane allpalettes = new JSplitPane(
			JSplitPane.HORIZONTAL_SPLIT);
		final XcosPalette rootPalette = new XcosPalette(
			XcosMessages.PALETTES);
		rootNode = new DefaultMutableTreeNode(
			rootPalette);
		paletteTreeModel = new DefaultTreeModel(rootNode);
		paletteTree = new JTree(paletteTreeModel);
		
		allpalettes.setRightComponent(rootPalette);
		allpalettes.setLeftComponent(new JScrollPane(paletteTree));

		paletteTree.getSelectionModel().setSelectionMode(
			TreeSelectionModel.SINGLE_TREE_SELECTION);
		
		paletteTree.addMouseListener(new MouseListener() {
		    public void mouseReleased(MouseEvent arg0) {
		    }

		    public void mousePressed(MouseEvent arg0) {
		    }

		    public void mouseExited(MouseEvent arg0) {
		    }

		    public void mouseEntered(MouseEvent arg0) {
		    }

		    public void mouseClicked(MouseEvent arg0) {
			
			//Right click
			if ((arg0.getClickCount() == 1 && SwingUtilities.isRightMouseButton(arg0))
				|| arg0.isPopupTrigger()
				|| XcosMessages.isMacOsPopupTrigger(arg0)) {

			    final TreePath path = XcosPaletteManager.paletteTree.getPathForLocation(arg0.getX(), arg0.getY());
			    paletteTree.setSelectionPath(path);

			    ContextMenu menu = ScilabContextMenu.createContextMenu();

			    MenuItem addTo = ScilabMenuItem.createMenuItem();

			    addTo.setText(XcosMessages.REMOVE_USER_DEFINED);
			    addTo.setCallback(new CallBack(XcosMessages.REMOVE_USER_DEFINED) {
				private static final long serialVersionUID = 2975508442133933904L;

				public void callBack() {
				    DefaultMutableTreeNode currentNode = (DefaultMutableTreeNode)(path.getLastPathComponent());
				    
				    //remove palette from ConfigXcosManager
				    if(currentNode.getUserObject() instanceof XcosComponent) {
					XcosComponent comp = (XcosComponent)currentNode.getUserObject();
					if(comp.getGraph() instanceof PaletteDiagram) {
					    PaletteDiagram diagram = (PaletteDiagram)comp.getGraph();
					    String fileName = diagram.getFileName();
					    ConfigXcosManager.removeUserDefinedPalettes(fileName);
					}
				    }
				    
				    paletteTreeModel.removeNodeFromParent(currentNode);
				    if(userDefinedNode != null && userDefinedNode.getChildCount() == 0) {
					paletteTreeModel.removeNodeFromParent(userDefinedNode);
					userDefinedNode = null;
				    }
				    paletteTree.setSelectionRow(0);
				}
			    });
			    
				addTo.setEnabled(false);
				if(((DefaultMutableTreeNode)path.getPath()[1]).getUserObject() instanceof String) {
				if(path.getPath().length > 2) {
				    if(((DefaultMutableTreeNode)path.getPath()[2]).getUserObject() instanceof XcosComponent) {
					addTo.setEnabled(true);
				    }
				}
			    }

			    menu.add(addTo);
			    menu.setVisible(true);
			    ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
				    MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);

			}
		    }
		});
		
		paletteTree.addTreeSelectionListener(new TreeSelectionListener() {

			    public void valueChanged(TreeSelectionEvent tree) {
				DefaultMutableTreeNode node = (DefaultMutableTreeNode) paletteTree.getLastSelectedPathComponent();

				if (node == null || node.getUserObject() instanceof String) {
				    // Nothing is selected.
				    //allpalettes.setRightComponent(null);
				    return;
				}
				
				//hide previous palette
				Component oldComp = allpalettes.getRightComponent();
				if(oldComp instanceof XcosComponent) {
				    XcosComponent comp = (XcosComponent)oldComp;
				    comp.setVisible(false);
				}

				Dimension size = allpalettes.getLeftComponent().getSize();
				allpalettes.getLeftComponent().setPreferredSize(size);
				JScrollPane nodeInfo = (JScrollPane) node.getUserObject();
				
				
				//update
				allpalettes.setRightComponent(nodeInfo);
				
				//show next palette
				if(nodeInfo instanceof XcosComponent) {
				    XcosComponent comp = (XcosComponent)nodeInfo;
				    comp.setVisible(true);
				}
			    }

			});
		
		// Add a default Java bloc in HashMap
		((SwingScilabTab) palettes.getAsSimpleTab()).setContentPane(allpalettes);

		allpalettes.setVisible(false);
		palettes.setVisible(true);

		for (PaletteStringDescriptor paletteStringDescriptor : allPalettesStringDescriptor) {
		    /* Doesn't perform UI update */
		    PaletteDescriptor currentDescriptor = new PaletteDescriptor(
			    paletteStringDescriptor.Name,createPaletteData(paletteStringDescriptor.Components));

		    /* Perform UI update */
		    XcosPalette xcosPalette = new XcosPalette(
			    paletteStringDescriptor.Name);
		    for (PaletteData iter : currentDescriptor.Components) {
			xcosPalette.addTemplate(iter.Name, iter.Icon);
		    }
		    rootNode.add(new DefaultMutableTreeNode(xcosPalette));
		}

		//load user defined palette
		ArrayList<String> files = ConfigXcosManager.getUserDefinedPalettes();
		for(String file : files) {
		    loadUserPalette(file);
		}

		paletteLoadStarted = true;
		palettes.getAsSimpleTab().getInfoBar().setText(XcosMessages.EMPTY_INFO);

		/* UI Layout specific operations */
		paletteTree.expandRow(0);
		allpalettes.getLeftComponent().setMinimumSize(paletteTree.getPreferredSize());
		paletteTree.setSelectionRow(1);
		paletteTree.setRootVisible(false);
		allpalettes.setContinuousLayout(true);
		allpalettes.setVisible(true);
	    }
	};
    }

    private XcosPaletteManager() {
    }

     public static Tab loadPalette() {
	synchronized (paletteThread) {
	    paletteThread.notifyAll();
	}
	if (paletteLoadStarted == false) {
	    createPaletteWindow();
	    palettes.getAsSimpleTab().getInfoBar().setText(XcosMessages.LOADING_PALETTES);
	    paletteThread.start();
	}
	return palettes;
    }

    private static PaletteData[] createPaletteData(String[] blocksNames) {

	PaletteData[] xcosPalette = new PaletteData[blocksNames.length];

	final String palImagesPath = System.getenv("SCI") + "/modules/xcos/images/palettes/";

	// BasicBlock theBloc = null;

	for (int kBlock = 0; kBlock < blocksNames.length; kBlock++) {
	    xcosPalette[kBlock] = new PaletteData(blocksNames[kBlock],new ImageIcon(palImagesPath + blocksNames[kBlock] + ".jpg"));

	}

	return xcosPalette;
    }

    public static Tab getPalettes() {
	return palettes;
    }

    private static void createPaletteWindow() {
	paletteLoadStarted = false;
	// Add a default Java bloc in HashMap
	// Xcos.getAllBlocks().put("TEXT_f", new TextBlock("TEXT_f"));

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

	palMenu.add(LoadAsPalAction.createMenu(null));
	palMenu.addSeparator();
	palMenu.add(ClosePalettesAction.createMenu(null));

	/** Create the toolbar */
	ToolBar palToolbar = ScilabToolBar.createToolBar();
	palettes.addToolBar(palToolbar);

	palToolbar.add(LoadAsPalAction.createButton(null));

	/** Create the infobar */
	palettes.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());

	palWin.addTab(palettes);
    }

    public static boolean isVisible() {
	if (palettes != null && palettes.isVisible() == true) {
	    return true;
	}
	return false;
    }

    public static void loadUserPalette(String newPal) {
	palettes.getAsSimpleTab().getInfoBar().setText(XcosMessages.LOADING_USER_DEFINE);
	PaletteDiagram paletteDiagram = new PaletteDiagram();
	paletteDiagram.installListeners();
	if(paletteDiagram.openDiagramAsPal(newPal) == true) {
	    if(userDefinedNode == null) {
		userDefinedNode = new DefaultMutableTreeNode(XcosMessages.USER_DEFINED);
		paletteTreeModel.insertNodeInto(userDefinedNode, rootNode, rootNode.getChildCount());
	    }
	    
	    DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(paletteDiagram.getAsComponent());
	    paletteTreeModel.insertNodeInto(newNode, userDefinedNode, userDefinedNode.getChildCount());
	    paletteDiagram.getAsComponent().setVisible(false);
	    paletteTree.expandRow(rootNode.getIndex(userDefinedNode));
	    paletteTree.setSelectionPath(new TreePath(newNode.getPath()));
	}
	palettes.getAsSimpleTab().getInfoBar().setText(XcosMessages.EMPTY_INFO);
    }
}
