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

import javax.swing.ImageIcon;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.RepaintManager;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BarUpdater;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.configuration.model.PositionType;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.actions.ClosePalettesAction;
import org.scilab.modules.xcos.palette.actions.LoadAsPalAction;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the default view for the palette
 */
public class PaletteManagerView extends ScilabTab {
	private PaletteManager controller;
	private PaletteManagerPanel panel;

	/**
	 * Default constructor
	 * @param controller the associated controller
	 */
	public PaletteManagerView(PaletteManager controller) {
		super(XcosMessages.PALETTE_BROWSER);
		((SwingScilabTab) getAsSimpleTab()).setWindowIcon(new ImageIcon(System.getenv("SCI")
										+ "/modules/gui/images/icons/32x32/apps/utilities-system-monitor.png").getImage());
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
		
		final ConfigurationManager manager = ConfigurationManager.getInstance();
		final PositionType p = manager.getSettings().getWindows().getPalette();
		
		window.setDims(new Size(p.getWidth(), p.getHeight()));
		window.setPosition(new Position(p.getX(), p.getY()));
		
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
		BarUpdater.updateBars(getParentWindowId(), getMenuBar(), getToolBar(),
				getInfoBar(), getName());
		window.setVisible(true);
		
		getTree().revalidate();
		getPanel().performStartUpLayout();
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
		
		if (p == null) {
			updateWholeTree();
		} else {
			((DefaultTreeModel) t.getModel()).reload((TreeNode) p.getLastPathComponent());
			t.setSelectionPath(p);
		}
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
		
		/*
		 * Force repaint
		 */
		((SwingScilabTextBox) getAsSimpleTab().getInfoBar()
				.getAsSimpleTextBox()).repaint();
		RepaintManager.currentManager((SwingScilabTab) this.getAsSimpleTab())
				.paintDirtyRegions();
	}
	
	/**
	 * Handle the associated Tab removing and recreation 
	 * 
	 * @param newVisibleState the new status
	 * @see org.scilab.modules.gui.tab.ScilabTab#setVisible(boolean)
	 */
	@Override
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
		
		/*
		 * Recreate the window if applicable
		 */
		if (newVisibleState && getParentWindow() == null) {
			Window paletteWindow = ScilabWindow.createWindow();
			paletteWindow.setVisible(true);
			super.setVisible(true);
			paletteWindow.addTab(this);
		}
		
		if (getParentWindow() != null) {
			if (getParentWindow().getNbDockedObjects() == 1) {
				getParentWindow().setVisible(newVisibleState);
			} else {
				if (!newVisibleState) {
					DockingManager.undock((Dockable) getAsSimpleTab());
					setParentWindowId(-1);
				}
			}
		}
	}
}
