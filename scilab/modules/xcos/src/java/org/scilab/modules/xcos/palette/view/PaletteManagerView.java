/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.actions.ClosePalettesAction;
import org.scilab.modules.xcos.palette.actions.LoadAsPalAction;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the default view for the palette
 */
public class PaletteManagerView extends ScilabTab {
	private static final Size WIN_SIZE = new Size(700, 600);
	
	private PaletteManager controller;
	private PaletteManagerPanel panel;

	/**
	 * Default constructor
	 * @param controller the associated controller
	 */
	public PaletteManagerView(PaletteManager controller) {
		super(XcosMessages.PALETTE_BROWSER);
		this.controller = controller;
		initComponents();
	}

	/**
	 * @return the controller
	 */
	protected PaletteManager getController() {
		return controller;
	}
	
	/**
	 * @return the panel
	 */
	public PaletteManagerPanel getPanel() {
		return panel;
	}
	
	/**
	 * @param panel the panel to set
	 */
	public void setPanel(PaletteManagerPanel panel) {
		this.panel = panel;
	}
	
	/** Instantiate and setup all the components */
	private void initComponents() {
		Window window = ScilabWindow.createWindow();
		window.setDims(WIN_SIZE);
		
		/* Create the menu bar */
		MenuBar menuBar = ScilabMenuBar.createMenuBar();

		Menu menu = ScilabMenu.createMenu();
		menu.setText(XcosMessages.PALETTES);
		menu.setMnemonic('P');
		menuBar.add(menu);

		menu.add(LoadAsPalAction.createMenu(null));
		menu.addSeparator();
		menu.add(ClosePalettesAction.createMenu(null));
		
		addMenuBar(menuBar);

		/* Create the toolbar */
		ToolBar toolbar = ScilabToolBar.createToolBar();
		toolbar.add(LoadAsPalAction.createButton(null));
		
		addToolBar(toolbar);

		/* Create the content pane */
		panel = new PaletteManagerPanel(getController());
		((SwingScilabTab) getAsSimpleTab()).setContentPane(panel);
		
		/* Create the infobar */
		getAsSimpleTab().setInfoBar(ScilabTextBox.createTextBox());
		
		setCallback(new ClosePalettesAction(null));
		window.addTab(this);
		window.setVisible(true);
	}
	
	/** @return the category tree */
	public JTree getTree() {
		return (JTree) ((JScrollPane) panel.getLeftComponent()).getViewport()
				.getView();
	}
	
	/**
	 * Update the selected path on the tree
	 */
	public static void updateTree() {
		final JTree t = PaletteManager.getInstance().getView().getTree();
		final TreePath p = t.getSelectionPath();
		
		((DefaultTreeModel) t.getModel()).reload((TreeNode) p.getLastPathComponent());
	}
	
	/**
	 * Update the whole tree
	 */
	public static void updateWholeTree() {
		final JTree t = PaletteManager.getInstance().getView().getTree();
		
		TreePath selectedPath = t.getSelectionPath();
		((DefaultTreeModel) t.getModel()).reload();
		t.setSelectionPath(selectedPath);
	}
	
	/** @param info the information to write on the infobar */
	public void setInfo(String info) {
		getAsSimpleTab().getInfoBar().setText(info);
	}
}
