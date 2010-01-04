/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

import javax.swing.Timer;

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
import org.scilab.modules.xcos.actions.CloseAction;
import org.scilab.modules.xcos.actions.CloseViewportAction;
import org.scilab.modules.xcos.actions.CodeGenerationAction;
import org.scilab.modules.xcos.actions.CompileAction;
import org.scilab.modules.xcos.actions.DebugLevelAction;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.DumpAction;
import org.scilab.modules.xcos.actions.ExportAction;
import org.scilab.modules.xcos.actions.ExportToXMLAction;
import org.scilab.modules.xcos.actions.FitDiagramToViewAction;
import org.scilab.modules.xcos.actions.ImportFromXMLAction;
import org.scilab.modules.xcos.actions.LinkStyleAction;
import org.scilab.modules.xcos.actions.NewDiagramAction;
import org.scilab.modules.xcos.actions.NormalViewAction;
import org.scilab.modules.xcos.actions.OpenAction;
import org.scilab.modules.xcos.actions.PrintAction;
import org.scilab.modules.xcos.actions.QuitAction;
import org.scilab.modules.xcos.actions.RecentFileAction;
import org.scilab.modules.xcos.actions.SaveAction;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.actions.StartAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.actions.ViewDiagramBrowserAction;
import org.scilab.modules.xcos.actions.ViewGridAction;
import org.scilab.modules.xcos.actions.ViewInScicosAction;
import org.scilab.modules.xcos.actions.ViewViewportAction;
import org.scilab.modules.xcos.actions.XcosDemonstrationsAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.actions.AlignBlockAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.ColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.palette.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.mxGraphOutline;
import com.mxgraph.util.mxConstants;
import com.mxgraph.view.mxGraph;

/**
 * Xcos tab operations
 * 
 * This class implement specific operation of an Xcos Tab.
 */
public class XcosTab extends SwingScilabTab implements Tab {

    private static final long serialVersionUID = -290453474673387812L;
    /*
     * Static fields
     */
    private static List<XcosDiagram> diagrams = new Vector<XcosDiagram>();
    private static HashMap<Integer, AfficheBlock> afficheBlocks = new HashMap<Integer, AfficheBlock>();

    private static List<Menu> recentsMenus = new ArrayList<Menu>();
    private static List<MenuItem> startMenuItems = new ArrayList<MenuItem>();
    private static List<MenuItem> stopMenuItems = new ArrayList<MenuItem>();
    private static List<PushButton> startPushButtons = new ArrayList<PushButton>();
    private static List<PushButton> stopPushButtons = new ArrayList<PushButton>();

    private static boolean startEnabled = true;

    public static void closeDiagram(XcosDiagram diag) {

	// System.err.println("Xcos::closeDiagram : " + diagram);

	diag.closeChildren();
	diagrams.remove(diag);

	// for(int i = 0 ; i < diagrams.size(); i++){
	// System.err.println("diagrams[" + i + "] : " +
	// diagrams.get(i).getClass());
	// }
	if (diagrams.size() == 0) {
	    // System.err.println("close session");
	    Xcos.closeSession();
	}

    }

    /**
     * Try to focus to an already openned file
     * 
     * @param filename
     * @return True when found and focused, False otherwise
     */
    public static boolean focusOnExistingFile(String filename) {
	for (XcosDiagram diagram : diagrams) {
	    if (diagram.getSavedFile() != null) {
		if (diagram.getSavedFile().compareTo(filename) == 0) {
		    diagram.getParentTab().setCurrent();
		    return true;
		}
	    }
	}
	return false;
    }

    public static HashMap<Integer, AfficheBlock> getAfficheBlocks() {
	return afficheBlocks;
    }

    public static List<XcosDiagram> getAllDiagrams() {
	return diagrams;
    }

    public static void setStartEnabled(boolean status) {
	for (int i = 0; i < startMenuItems.size(); i++) {
	    startMenuItems.get(i).setEnabled(status);
	    startPushButtons.get(i).setEnabled(status);
	    startEnabled = status;

	    stopMenuItems.get(i).setEnabled(!status);
	    stopPushButtons.get(i).setEnabled(!status);
	}

    }

    public static void createTabFromDiagram(XcosDiagram xcosDiagram) {
	Window main = ScilabWindow.createWindow();
	main.setTitle(XcosMessages.XCOS);
	main.setDims(new Size(600, 500));
	
	// Get the palettes position
	if (XcosPaletteManager.isVisible()) { // If at Xcos startup
	    Position palPosition = XcosPaletteManager.getPalettes()
		    .getParentWindow().getPosition();
	    Size palSize = XcosPaletteManager.getPalettes().getParentWindow()
		    .getDims();
	    Position mainPosition = new Position(palPosition.getX()
		    + palSize.getWidth(), palPosition.getY());
	    main.setPosition(mainPosition);
	}

	diagrams.add(xcosDiagram);

	XcosTab tab = new XcosTab(xcosDiagram);
	tab.setName(XcosMessages.UNTITLED);
	xcosDiagram.setParentTab(tab);
	main.addTab(tab);
	/*
	 * MENU BAR
	 */
	MenuBar menuBar = tab.createMenuBar(xcosDiagram);
	tab.addMenuBar(menuBar);

	/*
	 * TOOL BAR
	 */
	ToolBar toolBar = tab.createToolBar(xcosDiagram);
	tab.addToolBar(toolBar);

	/*
	 * VIEW PORT
	 */
	XcosTab.createViewPort(xcosDiagram);

	/*
	 * INFO BAR
	 */
	tab.getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());
	xcosDiagram.setOpened(true);
	
	main.setVisible(true);
    }
    
    public static void showTabFromDiagram(XcosDiagram xcosDiagram) {
	assert xcosDiagram.isOpened() == true;
	XcosTab tab = (XcosTab) xcosDiagram.getParentTab();
	
	xcosDiagram.setVisible(true);

	/*
	 * Superblock specific customizations
	 */
	if (xcosDiagram instanceof SuperBlockDiagram) {
	    tab.setSimulationActionEnabled(false);
	}

	/*
	 * Set buttons enabled
	 */
	tab.setActionsEnabled(true);

	// Disabling undo/redo at startup
	tab.setEnabledRedo(false);
	tab.setEnabledUndo(false);
    }
    
    /**
     * Update menu displaying recent opened files
     */
    public static void updateRecentOpenedFilesMenu(ScilabGraph scilabGraph) {
	ArrayList<File> recentFiles = ConfigXcosManager
		.getAllRecentOpenedFiles();

	for (int j = 0; j < recentsMenus.size(); j++) {
	    ((SwingScilabMenu) recentsMenus.get(j).getAsSimpleMenu())
		    .removeAll();

	    for (int i = 0; i < recentFiles.size(); i++) {
		recentsMenus.get(j).add(
			RecentFileAction.createMenu(scilabGraph, recentFiles
				.get(i)));
	    }
	}
    }
    
    /*
     * Internal class
     */
    private class ArrowKeysListener implements KeyListener {

	private static final double DEFAULT_PIXEL_MOVE = 5;
	private static final int DEFAULT_DELAY = 800; // milliseconds

	private double xIncrement = 0;
	private double yIncrement = 0;
	private mxGraph graph;

	private Timer repetitionTimer;
	private ActionListener doMove = new ActionListener() {
	    public void actionPerformed(ActionEvent arg0) {
		if (graph != null) {
		    Object[] cells = graph.getSelectionCells();

		    graph.getModel().beginUpdate();
		    for (Object cell : cells) {
			if (cell instanceof BasicBlock || cell instanceof BasicLink) {
			    graph.translateCell(cell, xIncrement, yIncrement);
			}
		    }
		    graph.getModel().endUpdate();
		    graph.refresh();
		}
	    }
	};

	public ArrowKeysListener() {
	    repetitionTimer = new Timer(DEFAULT_DELAY, doMove);
	    repetitionTimer.setInitialDelay(0);
	}

	public void keyPressed(KeyEvent arg0) {
	    double realMove;
	    
	    mxGraphComponent diagram = (mxGraphComponent) arg0.getSource();
	    graph = diagram.getGraph();
	    
	    if (graph.isGridEnabled()) {
		realMove = graph.getGridSize();
	    } else {
		realMove = DEFAULT_PIXEL_MOVE;
	    }

	    switch (arg0.getKeyCode()) {
	    case KeyEvent.VK_UP:
		yIncrement = -realMove;
		break;

	    case KeyEvent.VK_DOWN:
		yIncrement = realMove;
		break;

	    case KeyEvent.VK_RIGHT:
		xIncrement = realMove;
		break;

	    case KeyEvent.VK_LEFT:
		xIncrement = -realMove;
		break;

	    default:
		break;
	    }

	    if (graph.isGridEnabled() != true) {
		xIncrement *= diagram.getZoomFactor();
		yIncrement *= diagram.getZoomFactor();
	    }

	    repetitionTimer.start();
	}

	public void keyReleased(KeyEvent arg0) {
	    repetitionTimer.stop();
	    yIncrement = 0;
	    xIncrement = 0;
	    graph = null;
	}

	public void keyTyped(KeyEvent arg0) {

	}
    }

    /*
     * Instance fields
     */
    private XcosDiagram diagram;
    
    private MenuBar menuBar;
    private Menu fileMenu;
    private Menu recentsMenu;
    private Menu edit;
    private Menu view;
    private Menu simulate;
    private Menu format;
    private Menu alignMenu;
    private Menu linkStyle;
    private Menu tools;
    private Menu help;
    
    private PushButton openAction;
    private PushButton saveAction;
    private PushButton saveAsAction;
    private PushButton printAction;
    private PushButton newDiagramAction;
    private PushButton deleteAction;
    private PushButton undoAction;
    private PushButton redoAction;
    private PushButton fitDiagramToViewAction;
    private PushButton startAction;
    private PushButton stopAction;
    private PushButton zoomInAction;
    private PushButton zoomOutAction;
    private PushButton xcosDemonstrationAction;
    private PushButton xcosDocumentationAction;
    
    private boolean actionsEnabled;

    public XcosTab(XcosDiagram diagram) {
	super(XcosMessages.XCOS);

	this.diagram = diagram;
	this.actionsEnabled = false;

	this.setCallback(new CloseAction(diagram));
	this.setContentPane(diagram.getAsComponent());
	
	diagram.getAsComponent().addKeyListener(new ArrowKeysListener());
    }

    public void addInfoBar(TextBox infoBarToAdd) {
    }

    public void addMenuBar(MenuBar menuBarToAdd) {
	((SwingScilabTab) this).setMenuBar(menuBarToAdd);
    }

    public void addToolBar(ToolBar toolBarToAdd) {
	((SwingScilabTab) this).setToolBar(toolBarToAdd);
    }

    public void closeDiagram() {

	// System.err.println("Xcos::closeDiagram : " + diagram);

	diagram.closeChildren();
	diagrams.remove(diagram);

	// for(int i = 0 ; i < diagrams.size(); i++){
	// System.err.println("diagrams[" + i + "] : " +
	// diagrams.get(i).getClass());
	// }
	if (diagrams.size() == 0) {
	    // System.err.println("close session");
	    Xcos.closeSession();
	}

    }

    public MenuBar createMenuBar(ScilabGraph scilabGraph) {
	ArrayList<File> recentFiles = ConfigXcosManager
		.getAllRecentOpenedFiles();

	menuBar = ScilabMenuBar.createMenuBar();

	/** FILE MENU */
	fileMenu = ScilabMenu.createMenu();
	fileMenu.setText(XcosMessages.FILE);
	fileMenu.setMnemonic('F');

	// Menu newMenu = ScilabMenu.createMenu();
	// newMenu.setText(XcosMessages.NEW);
	// newMenu.add(NewDiagramAction.createMenu(scilabGraph));
	// newMenu.add(NewPaletteAction.createMenu(scilabGraph));
	// fileMenu.add(newMenu);
	fileMenu.add(NewDiagramAction.createMenu(scilabGraph));

	fileMenu.add(OpenAction.createMenu(scilabGraph));
	fileMenu.addSeparator();
	fileMenu.add(SaveAction.createMenu(scilabGraph));
	fileMenu.add(SaveAsAction.createMenu(scilabGraph));
	fileMenu.add(ExportAction.createMenu(scilabGraph));

	recentsMenu = ScilabMenu.createMenu();
	recentsMenu.setText(XcosMessages.RECENT_FILES);
	for (int i = 0; i < recentFiles.size(); i++) {
	    recentsMenu.add(RecentFileAction.createMenu(scilabGraph,
		    recentFiles.get(i)));
	}
	recentsMenus.add(recentsMenu);
	fileMenu.add(recentsMenu);

	// fileMenu.add(SaveAsInterfaceFunctionAction.createMenu(scilabGraph));
	// fileMenu.addSeparator();
	fileMenu.add(PrintAction.createMenu(scilabGraph));
	fileMenu.addSeparator();
	fileMenu.add(CloseAction.createMenu(scilabGraph));
	fileMenu.addSeparator();
	fileMenu.add(QuitAction.createMenu(scilabGraph));

	menuBar.add(fileMenu);

	/** Edit menu */
	edit = ScilabMenu.createMenu();
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
	// Menu superblockMask = ScilabMenu.createMenu();
	// superblockMask.setText(XcosMessages.SUPERBLOCK_MASK);
	// superblockMask.add(SuperblockMaskCreateAction.createMenu(scilabGraph));
	// superblockMask.add(SuperblockMaskRemoveAction.createMenu(scilabGraph));
	// superblockMask.add(SuperblockMaskCustomizeAction.createMenu(scilabGraph));
	// superblockMask.add(SuperblockMaskSaveBlockGUIAction.createMenu(scilabGraph));
	// edit.add(superblockMask);

	/** View menu */
	view = ScilabMenu.createMenu();
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
	CheckBoxMenuItem menu = ViewViewportAction
		.createCheckBoxMenu(scilabGraph);
	view.add(menu);
	((XcosDiagram) scilabGraph).setViewPortMenuItem(menu);
	// view.add(ViewGetinfosAction.createMenu(scilabGraph));
	view.add(ViewDetailsAction.createMenu(scilabGraph));

	/** Simulation menu */
	simulate = ScilabMenu.createMenu();
	simulate.setText(XcosMessages.SIMULATION);
	simulate.setMnemonic('S');
	menuBar.add(simulate);

	MenuItem startMenu = StartAction.createMenu(scilabGraph);
	startMenuItems.add(startMenu);
	startMenu.setEnabled(startEnabled);
	MenuItem stopMenu = StopAction.createMenu(scilabGraph);
	stopMenuItems.add(stopMenu);
	stopMenu.setEnabled(startEnabled);

	simulate.add(SetupAction.createMenu(scilabGraph));
	simulate.add(DebugLevelAction.createMenu(scilabGraph));
	simulate.add(SetContextAction.createMenu(scilabGraph));
	simulate.add(CompileAction.createMenu(scilabGraph));
	simulate.add(startMenu);
	simulate.add(stopMenu);

	/** Format menu */
	format = ScilabMenu.createMenu();
	format.setText(XcosMessages.FORMAT);
	format.setMnemonic('O');
	menuBar.add(format);

	format.add(RotateAction.createMenu(scilabGraph));
	format.add(FlipAction.createMenu(scilabGraph));
	format.add(MirrorAction.createMenu(scilabGraph));
	format.add(ShowHideShadowAction.createMenu(scilabGraph));

	format.addSeparator();
	alignMenu = ScilabMenu.createMenu();
	alignMenu.setText(XcosMessages.ALIGN_BLOCKS);
	alignMenu.add(AlignBlockAction.createMenu(scilabGraph,
		XcosMessages.ALIGN_LEFT, mxConstants.ALIGN_LEFT));
	alignMenu.add(AlignBlockAction.createMenu(scilabGraph,
		XcosMessages.ALIGN_CENTER, mxConstants.ALIGN_CENTER));
	alignMenu.add(AlignBlockAction.createMenu(scilabGraph,
		XcosMessages.ALIGN_RIGHT, mxConstants.ALIGN_RIGHT));
	alignMenu.addSeparator();
	alignMenu.add(AlignBlockAction.createMenu(scilabGraph,
		XcosMessages.ALIGN_TOP, mxConstants.ALIGN_TOP));
	alignMenu.add(AlignBlockAction.createMenu(scilabGraph,
		XcosMessages.ALIGN_MIDDLE, mxConstants.ALIGN_MIDDLE));
	alignMenu.add(AlignBlockAction.createMenu(scilabGraph,
		XcosMessages.ALIGN_BOTTOM, mxConstants.ALIGN_BOTTOM));
	format.add(alignMenu);
	format.addSeparator();

	format.add(ColorAction.createMenu(scilabGraph,
		XcosMessages.BORDER_COLOR, mxConstants.STYLE_STROKECOLOR));
	format.add(ColorAction.createMenu(scilabGraph, XcosMessages.FILL_COLOR,
		mxConstants.STYLE_FILLCOLOR));
	format.addSeparator();

	linkStyle = ScilabMenu.createMenu();
	linkStyle.setText(XcosMessages.LINK_STYLE);
	linkStyle.add(LinkStyleAction.createMenu(scilabGraph,
		XcosMessages.LINK_STYLE_HORIZONTAL,
		mxConstants.ELBOW_HORIZONTAL));
	linkStyle.add(LinkStyleAction.createMenu(scilabGraph,
		XcosMessages.LINK_STYLE_STRAIGHT, mxConstants.SHAPE_CONNECTOR));
	linkStyle.add(LinkStyleAction.createMenu(scilabGraph,
		XcosMessages.LINK_STYLE_VERTICAL, mxConstants.ELBOW_VERTICAL));
	format.add(linkStyle);
	format.addSeparator();

	format.add(DiagramBackgroundAction.createMenu(scilabGraph));
	CheckBoxMenuItem gridMenu = ViewGridAction
		.createCheckBoxMenu(scilabGraph);
	format.add(gridMenu);
	((XcosDiagram) scilabGraph).setGridMenuItem(menu);

	/** Tools menu */
	tools = ScilabMenu.createMenu();
	tools.setText(XcosMessages.TOOLS);
	tools.setMnemonic('T');
	menuBar.add(tools);

	tools.add(CodeGenerationAction.createMenu(scilabGraph));

	/** Help menu */
	help = ScilabMenu.createMenu();
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
	if (false) {
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

    public ToolBar createToolBar(ScilabGraph scilabGraph) {
	ToolBar toolBar = ScilabToolBar.createToolBar();

	newDiagramAction = NewDiagramAction.createButton(scilabGraph);
	toolBar.add(newDiagramAction);

	openAction = OpenAction.createButton(scilabGraph);
	toolBar.add(openAction);
	
	toolBar.addSeparator();
	
	saveAction = SaveAction.createButton(scilabGraph);
	toolBar.add(saveAction);
	saveAsAction = SaveAsAction.createButton(scilabGraph);
	toolBar.add(saveAsAction);

	toolBar.addSeparator();

	printAction = PrintAction.createButton(scilabGraph);
	toolBar.add(printAction);

	toolBar.addSeparator();

	deleteAction = DeleteAction.createButton(scilabGraph);
	toolBar.add(deleteAction);

	toolBar.addSeparator();

	// UNDO / REDO
	undoAction = UndoAction.undoButton(scilabGraph);
	redoAction = RedoAction.redoButton(scilabGraph);
	toolBar.add(undoAction);
	toolBar.add(redoAction);

	toolBar.addSeparator();

	fitDiagramToViewAction = FitDiagramToViewAction.createButton(scilabGraph);
	toolBar.add(fitDiagramToViewAction);

	toolBar.addSeparator();

	// START / STOP
	startAction = StartAction.createButton(scilabGraph);
	stopAction = StopAction.createButton(scilabGraph);
	startAction.setEnabled(startEnabled);
	stopAction.setEnabled(!startEnabled);
	startPushButtons.add(startAction);
	stopPushButtons.add(stopAction);

	toolBar.add(startAction);
	toolBar.add(stopAction);

	toolBar.addSeparator();

	// ZOOMIN / ZOOMOUT
	zoomInAction = ZoomInAction.zoominButton(scilabGraph);
	toolBar.add(zoomInAction);
	zoomOutAction = ZoomOutAction.zoomoutButton(scilabGraph);
	toolBar.add(zoomOutAction);

	toolBar.addSeparator();

	xcosDemonstrationAction = XcosDemonstrationsAction.createButton(scilabGraph);
	toolBar.add(xcosDemonstrationAction);
	xcosDocumentationAction = XcosDocumentationAction.createButton(scilabGraph);
	toolBar.add(xcosDocumentationAction);

	return toolBar;
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
	mxGraphOutline graphOutline = new mxGraphOutline(xcosDiagramm
		.getAsComponent());

	graphOutline.setDrawLabels(true);

	((SwingScilabTab) outlineTab.getAsSimpleTab())
		.setContentPane(graphOutline);
	outline.addTab(outlineTab);
	outline.setVisible(false);
	outlineTab.setVisible(false);
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.scilab.modules.gui.tab.Tab#getAsSimpleTab()
     */
    public SimpleTab getAsSimpleTab() {
	return this;
    }

    /*
     * (non-Javadoc)
     * 
     * @see org.scilab.modules.gui.tab.Tab#getParentWindow()
     */
    public Window getParentWindow() {
	return null;
    }

    public void setEnabledRedo(boolean status) {
	if (actionsEnabled) {
	redoAction.setEnabled(status);
	}
    }

    public void setEnabledUndo(boolean status) {
	if (actionsEnabled) {
	undoAction.setEnabled(status);
	}
    }
    
    /**
     * Enable or Disable simulation related actions
     * @param state True if it have to be enabled, flase otherwise
     */
    public void setSimulationActionEnabled(boolean state) {
	if (actionsEnabled) {
    	simulate.setEnabled(state);
    	startAction.setEnabled(state);
    	stopAction.setEnabled(state);
	}
    }
    
    /**
     * Enable or disable all actions
     * @param status True if they have to be enabled, false otherwise
     */
    public void setActionsEnabled(boolean status) {
	fileMenu.setEnabled(status);
	edit.setEnabled(status);
	view.setEnabled(status);
	simulate.setEnabled(status);
	format.setEnabled(status);
	alignMenu.setEnabled(status);
	linkStyle.setEnabled(status);
	tools.setEnabled(status);
	help.setEnabled(status);
	openAction.setEnabled(status);
	saveAction.setEnabled(status);
	saveAsAction.setEnabled(status);
	printAction.setEnabled(status);
	newDiagramAction.setEnabled(status);
	deleteAction.setEnabled(status);
	undoAction.setEnabled(status);
	redoAction.setEnabled(status);
	fitDiagramToViewAction.setEnabled(status);
	startAction.setEnabled(status);
	stopAction.setEnabled(false);
	zoomInAction.setEnabled(status);
	zoomOutAction.setEnabled(status);
	xcosDemonstrationAction.setEnabled(status);
	xcosDocumentationAction.setEnabled(status);
	
	actionsEnabled = status;
    }
}
