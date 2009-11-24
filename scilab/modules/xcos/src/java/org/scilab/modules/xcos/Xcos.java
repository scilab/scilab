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
import java.util.Vector;

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
import org.scilab.modules.xcos.actions.ViewGridAction;
import org.scilab.modules.xcos.actions.ViewInScicosAction;
import org.scilab.modules.xcos.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.actions.ViewViewportAction;
import org.scilab.modules.xcos.actions.XcosDemonstrationsAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphOutline;
import com.mxgraph.util.mxConstants;

public class Xcos extends SwingScilabTab implements Tab {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static List<XcosDiagram> diagrams = new Vector<XcosDiagram>();
	private static HashMap<Integer, AfficheBlock> afficheBlocks = new HashMap<Integer, AfficheBlock>();

	private static List<Menu> recentsMenus = new ArrayList<Menu>();
	private static List<MenuItem> startMenuItems = new ArrayList<MenuItem>();
	private static List<MenuItem> stopMenuItems = new ArrayList<MenuItem>();
	private static List<PushButton> startPushButtons = new ArrayList<PushButton>();
	private static List<PushButton> stopPushButtons = new ArrayList<PushButton>();
	
	private static PushButton undoPushButton ;
	private static PushButton redoPushButton ;
	private static boolean startEnabled  = true;

	/** Palette creation */
	static {
	    /* load scicos libraries (macros) */
	    InterpreterManagement.requestScilabExec("loadScicosLibs();");
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
		xcos();
	    }
	});
    }

    public static void xcos() {
    	XcosPaletteManager.loadPalette();
    	ViewPaletteBrowserAction.setPalettesVisible(true);
    	createEmptyDiagram();
    }
    
    public static void xcos(String fileName) {
	ConfigXcosManager.saveToRecentOpenedFiles(fileName);
	if (Xcos.focusOnExistingFile(fileName) == false) {
	    XcosDiagram diagram = createEmptyDiagram();
	    diagram.openDiagramFromFile(fileName);
	}
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

    public static MenuBar createMenuBar(ScilabGraph scilabGraph) {
		ConfigXcosManager.createUserCopy();	
		ArrayList<File> recentFiles = ConfigXcosManager.getAllRecentOpenedFiles();

    	MenuBar menuBar = ScilabMenuBar.createMenuBar();

    	/** FILE MENU */
		Menu fileMenu = ScilabMenu.createMenu();
		fileMenu.setText(XcosMessages.FILE);
		fileMenu.setMnemonic('F');
		
		//Menu newMenu = ScilabMenu.createMenu();
		//newMenu.setText(XcosMessages.NEW);
		//newMenu.add(NewDiagramAction.createMenu(scilabGraph));
		//newMenu.add(NewPaletteAction.createMenu(scilabGraph));
		//fileMenu.add(newMenu);
		fileMenu.add(NewDiagramAction.createMenu(scilabGraph));
		
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
		
		if(XcosPaletteManager.isVisible() == false) {
		    ViewPaletteBrowserAction.setPalettesVisible(true);
		}

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
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_CENTER, mxConstants.ALIGN_CENTER));
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_RIGHT, mxConstants.ALIGN_RIGHT));
		alignMenu.addSeparator();
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_TOP, mxConstants.ALIGN_TOP));
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_MIDDLE, mxConstants.ALIGN_MIDDLE));
		alignMenu.add(AlignBlockAction.createMenu(scilabGraph, XcosMessages.ALIGN_BOTTOM, mxConstants.ALIGN_BOTTOM));
		format.add(alignMenu);
		format.addSeparator();
		
		format.add(ColorAction.createMenu(scilabGraph, XcosMessages.BORDER_COLOR, mxConstants.STYLE_STROKECOLOR));
		format.add(ColorAction.createMenu(scilabGraph, XcosMessages.FILL_COLOR, mxConstants.STYLE_FILLCOLOR));
		format.addSeparator();
		
		Menu linkStyle = ScilabMenu.createMenu();
		linkStyle.setText(XcosMessages.LINK_STYLE);
		linkStyle.add(LinkStyleAction.createMenu(scilabGraph, XcosMessages.LINK_STYLE_HORIZONTAL, mxConstants.ELBOW_HORIZONTAL));
		linkStyle.add(LinkStyleAction.createMenu(scilabGraph, XcosMessages.LINK_STYLE_STRAIGHT, mxConstants.SHAPE_CONNECTOR));
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
    	// we disable undo/redo buttons at the beginning because it's irrelevant to undo/redo
    	// when nothing has been done
    	undoPushButton = UndoAction.undoButton(scilabGraph);
    	redoPushButton = RedoAction.redoButton(scilabGraph);
    	undoPushButton.setEnabled(false);
    	redoPushButton.setEnabled(false);
    	toolBar.add(undoPushButton);
    	toolBar.add(redoPushButton);
	
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
    	
    	toolBar.add(XcosDemonstrationsAction.createButton(scilabGraph));
    	toolBar.add(XcosDocumentationAction.createButton(scilabGraph));

    	return toolBar;
    }

    public Xcos(XcosDiagram diagram) {
	super(XcosMessages.XCOS);
	
	this.setCallback(new CloseAction(diagram));
	this.setContentPane(diagram.getAsComponent());
	
    }
    
    public static void closeDiagram(XcosDiagram diagram) {
	
//	System.err.println("Xcos::closeDiagram : " + diagram);
	
	diagram.closeChildren();
	diagrams.remove(diagram);

	
//	for(int i = 0 ; i < diagrams.size(); i++){
//	    System.err.println("diagrams[" + i + "] : " + diagrams.get(i).getClass());
//	}
    	if (diagrams.size() == 0) {
//    	    System.err.println("close session");
    	    closeSession();
    	}
    	
    }
    
    
    
    public static XcosDiagram createEmptyDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	showDiagram(xcosDiagramm);
	return xcosDiagramm;
    }

    public static XcosDiagram createHiddenDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	return xcosDiagramm;
    }

    public static void showDiagram(XcosDiagram xcosDiagram) {
	Window main = ScilabWindow.createWindow();
	main.setTitle(XcosMessages.XCOS);

	// Get the palettes position
	if (XcosPaletteManager.isVisible()) { // If at Xcos startup
		Position palPosition = XcosPaletteManager.getPalettes().getParentWindow().getPosition();
		Size palSize = XcosPaletteManager.getPalettes().getParentWindow().getDims();
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
	xcosDiagram.setOpened(true);
	xcosDiagram.setVisible(true);
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
		for (int i = 0; i < getStartMenuItems().size(); i++ ){
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

		for (int j = 0; j < getRecentsMenu().size(); j++) {
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
    	return XcosPaletteManager.getPalettes();
    }
    
    public static List<XcosDiagram> getDiagrams() {
    	return diagrams;
    }    
    
    /**
     * Try to focus to an already openned file
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
    
    /**
     * Look in each diagram to find the block corresponding to the given uid
     * and display a warning message.
     * 
     * @param UID - A String as UID.
     * @param message - The message to display.
     */
    public static void warnCellByUID(String UID, String message) {
	// Try to find a block with given index (UID)
	List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
	for (int i = 0; i < allDiagrams.size(); ++i) {
	    allDiagrams.get(i).warnCellByUID(UID, message);
	}
    }
    
    public void setEnabledUndo (boolean status){
    	undoPushButton.setEnabled(status);
    }

    public void setEnabledRedo (boolean status){
    	redoPushButton.setEnabled(status);
    }
    
    /**
     * This method is called when the user exits from Scilab
     */
    public static void closeXcosFromScilab() {
	//call close on all diagrams
	while(Xcos.getDiagrams().size() > 0) {
	    diagrams.get(0).closeDiagram(true);
	}
    }

    /**
     * This function convert a Xcos diagram to Scilab variable
     */
    public static int xcosDiagramToHDF5(String xcosFile, String h5File, boolean forceOverwrite) {
	XcosDiagram diagram = createHiddenDiagram();
	diagram.openDiagramFromFile(xcosFile);
	File temp = new File(h5File);
	if(temp.exists()) {
	    if(forceOverwrite == false) {
		return 1;
	    } else {
		temp.delete();
	    }
	}
	
	diagram.dumpToHdf5File(temp.getAbsolutePath());
	diagram.closeDiagram();
	return 0;
    }
}
