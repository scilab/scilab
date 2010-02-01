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

package org.scilab.modules.xcos.palette.listener;

import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.palette.Palette;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.model.PaletteManagerModel;
import org.scilab.modules.xcos.palette.model.PaletteModel;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosComponent;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Manage the click on the category tree
 */
public class PaletteManagerMouseListener implements MouseListener {

	/** Default constructor */
	public PaletteManagerMouseListener() { }
	
	/**
	 * Manage category selection
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
	 */
	public void mouseClicked(MouseEvent e) {
		//Right click
		if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e))
			|| e.isPopupTrigger()
			|| XcosMessages.isMacOsPopupTrigger(e)) {

			final PaletteManager manager = PaletteManager.getInstance();
			final JTree paletteTree = manager.getView().getTree();
			final PaletteManagerModel model = manager.getModel();
		    final TreePath path = paletteTree.getPathForLocation(e.getX(), e.getY());
		    paletteTree.setSelectionPath(path);

		    ContextMenu menu = ScilabContextMenu.createContextMenu();

		    MenuItem addTo = ScilabMenuItem.createMenuItem();

		    addTo.setText(XcosMessages.REMOVE_USER_DEFINED);
		    addTo.setCallback(new CallBack(XcosMessages.REMOVE_USER_DEFINED) {
			private static final long serialVersionUID = 2975508442133933904L;

				public void callBack() {
					DefaultMutableTreeNode currentNode = (DefaultMutableTreeNode) (path
							.getLastPathComponent());

					// remove palette from ConfigXcosManager
					if (currentNode.getUserObject() instanceof XcosComponent) {
						XcosComponent comp = (XcosComponent) currentNode
								.getUserObject();
						if (comp.getGraph() instanceof PaletteDiagram) {
							PaletteDiagram diagram = (PaletteDiagram) comp
									.getGraph();
							String fileName = diagram.getFileName();
							ConfigXcosManager
									.removeUserDefinedPalettes(fileName);
						}
					} else {
						assert currentNode.getUserObject() instanceof Palette;
						Palette p = (Palette) currentNode.getUserObject();
						
						p.getModel().setEnable(false);
						ConfigXcosManager.saveDefaultPalettes(PaletteModel.values());
					}
					
					
					model.getTreeModel().removeNodeFromParent(currentNode);
					MutableTreeNode userDefinedNode = model
							.getUserDefinedRoot();
					if (userDefinedNode != null
							&& userDefinedNode.getChildCount() == 0) {
						model.getTreeModel().removeNodeFromParent(
								userDefinedNode);
						userDefinedNode = null;
					}
					paletteTree.setSelectionRow(0);
				}
			});
		    
			addTo.setEnabled(false);
			Object[] p = path.getPath();
			DefaultMutableTreeNode node = (DefaultMutableTreeNode) p[p.length - 1];
			if (node.isLeaf()) {
				addTo.setEnabled(true);
			}

		    menu.add(addTo);
		    menu.setVisible(true);
		    ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
			    MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);

		}
	    }

	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
	 */
	public void mouseEntered(MouseEvent e) {
	}

	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
	 */
	public void mouseExited(MouseEvent e) {	
	}

	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
	 */
	public void mousePressed(MouseEvent e) {
	}

	/**
	 * Not used
	 * @param e Not used
	 * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
	 */
	public void mouseReleased(MouseEvent e) {
	}

}
