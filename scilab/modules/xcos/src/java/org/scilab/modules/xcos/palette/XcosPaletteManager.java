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
import java.util.List;

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
import org.scilab.modules.xcos.utils.PaletteComponent;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Manage all the Block Palettes.
 */
public final class XcosPaletteManager {

    private static final Size WIN_SIZE = new Size(700, 600);
	private static Thread paletteThread;
    private static boolean paletteLoadStarted;
    private static Tab palettes;
    private static JTree paletteTree;
    private static DefaultMutableTreeNode rootNode; 
    private static DefaultMutableTreeNode userDefinedNode; 
    private static DefaultTreeModel paletteTreeModel;

    /**
     * Represent the instantiation of the configurations values
     */
    private static final class PaletteDescriptor {
	private final PaletteData[] components;

	/**
	 * Default constructor
	 * @param components The palette associated data
	 */
	private PaletteDescriptor(PaletteData[] components) {
	    this.components = components;
	}
	
	/** @return the component list*/
	public PaletteData[] getComponents() {
		return components;
	}
    }

    /**
     * Represent any block data
     */
    private static final class PaletteData {
	private final String name;
	private final ImageIcon icon;

	/**
	 * Any PaletteBlock data (arguments of the
	 * org.scilab.modules.xcos.palette.XcosPalette#addTemplate)
	 * 
	 * @param name
	 *            The name of the block
	 * @param icon
	 *            The icon of the block
	 */
	public PaletteData(String name, ImageIcon icon) {
	    this.name = name;
	    this.icon = icon;
	}
	
	/** @return the block name */
	public String getName() {
		return name;
	}
	
	/** @return the icon of the block*/
	public ImageIcon getIcon() {
		return icon;
	}
	
    }

    private static final MouseListener MOUSE_LISTENER = new MouseListener() {
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
			    DefaultMutableTreeNode currentNode = (DefaultMutableTreeNode) path.getLastPathComponent();
			    
			    //remove palette from ConfigXcosManager
			    if (currentNode.getUserObject() instanceof PaletteComponent) {
				PaletteComponent comp = (PaletteComponent) currentNode.getUserObject();
				if (comp.getGraph() instanceof PaletteDiagram) {
				    PaletteDiagram diagram = (PaletteDiagram) comp.getGraph();
				    String fileName = diagram.getFileName();
				    ConfigXcosManager.removeUserDefinedPalettes(fileName);
				}
			    }
			    
			    paletteTreeModel.removeNodeFromParent(currentNode);
			    if (userDefinedNode != null && userDefinedNode.getChildCount() == 0) {
				paletteTreeModel.removeNodeFromParent(userDefinedNode);
				userDefinedNode = null;
			    }
			    paletteTree.setSelectionRow(0);
			}
		    });
		    
			addTo.setEnabled(false);
			if (((DefaultMutableTreeNode) path.getPath()[1]).getUserObject() instanceof String) {
			if (path.getPath().length > 2) {
			    if (((DefaultMutableTreeNode) path.getPath()[2]).getUserObject() instanceof PaletteComponent) {
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
	};
    
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
		
		paletteTree.addMouseListener(MOUSE_LISTENER);
		
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
				if (oldComp instanceof PaletteComponent) {
				    PaletteComponent comp = (PaletteComponent) oldComp;
				    comp.setVisible(false);
				}

				Dimension size = allpalettes.getLeftComponent().getSize();
				allpalettes.getLeftComponent().setPreferredSize(size);
				JScrollPane nodeInfo = (JScrollPane) node.getUserObject();
				
				
				//update
				allpalettes.setRightComponent(nodeInfo);
				
				//show next palette
				if (nodeInfo instanceof PaletteComponent) {
				    PaletteComponent comp = (PaletteComponent) nodeInfo;
				    comp.setVisible(true);
				}
			    }

			});
		
		// Add a default Java bloc in HashMap
		((SwingScilabTab) palettes.getAsSimpleTab()).setContentPane(allpalettes);

		allpalettes.setVisible(false);
		palettes.setVisible(true);

		for (Category cat : Category.values()) {
		    /* Doesn't perform UI update */
		    PaletteDescriptor currentDescriptor = new PaletteDescriptor(createPaletteData(cat.getBlockNames()));

		    /* Perform UI update */
		    XcosPalette xcosPalette = new XcosPalette(
			    cat.toString());
		    for (PaletteData iter : currentDescriptor.getComponents()) {
			xcosPalette.addTemplate(iter.getName(), iter.getIcon());
		    }
		    rootNode.add(new DefaultMutableTreeNode(xcosPalette));
		}

		//load user defined palette
		List<String> files = ConfigXcosManager.getUserDefinedPalettes();
		for (String file : files) {
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

    /** This class is a static singleton, thus it must not be instantiated */
    private XcosPaletteManager() { }

    /**
     * Load the entire palette
     * @return The Tab with the palette inside
     */
     public static Tab loadPalette() {
	synchronized (paletteThread) {
	    paletteThread.notifyAll();
	}
	if (!paletteLoadStarted) {
	    createPaletteWindow();
	    palettes.getAsSimpleTab().getInfoBar().setText(XcosMessages.LOADING_PALETTES);
	    paletteThread.start();
	}
	return palettes;
    }

    /**
     * Create the palette data for the associated block names
     * @param blocksNames The block which will be constructed
     * @return the new full palette data
     */
    private static PaletteData[] createPaletteData(String[] blocksNames) {

	PaletteData[] xcosPalette = new PaletteData[blocksNames.length];

	final String palImagesPath = System.getenv("SCI") + "/modules/xcos/images/palettes/";

	for (int kBlock = 0; kBlock < blocksNames.length; kBlock++) {
	    xcosPalette[kBlock] = new PaletteData(blocksNames[kBlock], new ImageIcon(palImagesPath + blocksNames[kBlock] + ".jpg"));

	}
	
	return xcosPalette;
    }

    /**
     * @return The palette Tab
     */
    public static Tab getPalettes() {
	return palettes;
    }

    /**
     * Create a palette window with the default loaded palette tree
     */
    private static void createPaletteWindow() {
	paletteLoadStarted = false;
	// Add a default Java bloc in HashMap
	// Xcos.getAllBlocks().put("TEXT_f", new TextBlock("TEXT_f"));

	Window palWin = ScilabWindow.createWindow();
	palWin.setDims(WIN_SIZE);
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

    /** @return True if the palette window is visible, false otherwise */
    public static boolean isVisible() {
	if (palettes != null && palettes.isVisible()) {
	    return true;
	}
	return false;
    }

    /**
     * Load a user palette on the palette tree.
     * @param newPal Path of the palette file
     */
    public static void loadUserPalette(String newPal) {
	palettes.getAsSimpleTab().getInfoBar().setText(XcosMessages.LOADING_USER_DEFINE);
	PaletteDiagram paletteDiagram = new PaletteDiagram();
	paletteDiagram.installListeners();
	if (paletteDiagram.openDiagramAsPal(newPal)) {
	    if (userDefinedNode == null) {
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
