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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;
import javax.swing.border.Border;

import org.flexdock.plaf.common.border.ShadowBorder;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.util.mxGraphTransferable;
import com.mxgraph.util.mxRectangle;

/**
 * Component of the XcosPalette view
 */
public class BlockPalette extends JLabel {

	private static final long serialVersionUID = 2045511112700166300L;
	
	private static final int DEFAULT_ICON_TEXT_GAP = 5;
	private static final int DEFAULT_FONT_SIZE = 12;
	private static final Dimension PREFERRED_SIZE = new Dimension(XcosConstants.PALETTE_BLOCK_WIDTH, XcosConstants.PALETTE_BLOCK_HEIGHT);
	private static final int DEFAULT_POSITION = 10;
	
	private BasicBlock block;
	private XcosPalette palette;
	private mxGraphTransferable transferable;
	
	private final MouseListener mouseListener = new MouseListener() {

		public void mousePressed(MouseEvent e) {
			getPalette().getEntryManager().setSelectionEntry(BlockPalette.this, getTransferable());
		}

		/**
		 * Select the current block
		 * @param e click parameters
		 */
		public void mouseClicked(MouseEvent e) {
			if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e))
					|| e.isPopupTrigger()
					|| XcosMessages.isMacOsPopupTrigger(e)) {

				ContextMenu menu = ScilabContextMenu.createContextMenu();

				final List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
				
				// No diagram opened: should never happen as Xcos opens an empty diagram when it is launched
				assert allDiagrams.size() != 0;
				
				if (allDiagrams.size() == 1) {
					// A single diagram opened: add to this diagram
					MenuItem addTo = ScilabMenuItem.createMenuItem();

					addTo.setText(XcosMessages.ADDTO + " " + allDiagrams.get(0).getParentTab().getName());
					final XcosDiagram theDiagram = allDiagrams.get(0);
					addTo.setCallback(new CallBack(BlockPalette.this.getText()) {
						private static final long serialVersionUID = 1185879440137756636L;

						public void callBack() {
							BasicBlock current = (BasicBlock) BlockFactory.createClone(getBlock());
							current.getGeometry().setX(DEFAULT_POSITION);
							current.getGeometry().setY(DEFAULT_POSITION);
							theDiagram.addCell(current);
						}
					});

					menu.add(addTo);

				} else {
					// The user has to choose
					Menu addTo = ScilabMenu.createMenu();

					addTo.setText(XcosMessages.ADDTO);

					for (int i = 0; i < allDiagrams.size(); i++) {
						MenuItem diagram = ScilabMenuItem.createMenuItem();
						final XcosDiagram theDiagram = allDiagrams.get(i);
						diagram.setText(allDiagrams.get(i).getParentTab().getName());
						diagram.setCallback(new CallBack(BlockPalette.this.getText()) {
							private static final long serialVersionUID = -3138430622029406470L;

							public void callBack() {
								BasicBlock current = (BasicBlock) BlockFactory.createClone(getBlock());
								current.getGeometry().setX(DEFAULT_POSITION);
								current.getGeometry().setY(DEFAULT_POSITION);
								theDiagram.addCell(current);
							}
						});
						addTo.add(diagram);
					}

					menu.add(addTo);
				}


				menu.getAsSimpleContextMenu().addSeparator();

				MenuItem help = ScilabMenuItem.createMenuItem();
				help.setText("Block help");
				help.setCallback(new CallBack(BlockPalette.this.getText()) {
					private static final long serialVersionUID = -8720228686621887887L;

					public void callBack() {
						XcosInterpreterManagement.requestScilabExec("help " + BlockPalette.this.getText());
					}
				});
				menu.add(help);

				menu.setVisible(true);

				((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
						MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
			}
		}

		public void mouseEntered(MouseEvent e) { }
		public void mouseExited(MouseEvent e) { }
		public void mouseReleased(MouseEvent e) { }
	}; 

	/**
	 * Default constructor
	 * @param icon The block icon
	 */
	public BlockPalette(ImageIcon icon) {
		super(icon);
		addMouseListener(mouseListener);
	}

	/**
	 * @return the block associated with the palette component
	 */
	public BasicBlock getBlock() {

		if (block == null) {
			if (getText().compareTo("TEXT_f") != 0) {
				String blocksPath = System.getenv("SCI") + "/modules/scicos_blocks/blocks/";

				// Search the bloc in global hashmap
				block = BlockReader.readBlockFromFile(blocksPath + getText() + ".h5");

				if (block.getStyle().compareTo("") == 0) {
					block.setStyle(block.getInterfaceFunctionName());
					block.setValue(block.getInterfaceFunctionName());
				}
			} else {
				block = BlockFactory.createBlock(BlockInterFunction.TEXT_f);
			}
		}
		return block;
	}
	
	/**
	 * Paint the component
	 * @param g the graphical context
	 */
	public void paint2(Graphics g) {
		boolean opaque = isOpaque();
		Color bg = getBackground();
		Border br = getBorder();

		if (getPalette().getEntryManager().isSelectedEntry(this)) {
			setBackground(getPalette().getBackground().brighter());
			setBorder(new ShadowBorder());
			setOpaque(true);
		}

		super.paint(g);

		setBorder(br);
		setBackground(bg);
		setOpaque(opaque);
	}

	/**
	 * @return the associated palette
	 */
	private XcosPalette getPalette() {
		return palette;
	}

	/**
	 * @param palette the palette parent
	 */
	public void setPalette(XcosPalette palette) {
		this.palette = palette;
		setPreferredSize(PREFERRED_SIZE);
		setBackground(palette.getBackground().brighter());
		setFont(new Font(getFont().getFamily(), 0, DEFAULT_FONT_SIZE));

		setVerticalTextPosition(JLabel.BOTTOM);
		setHorizontalTextPosition(JLabel.CENTER);
		setIconTextGap(DEFAULT_ICON_TEXT_GAP);
	}

	/**
	 * @return the block transferable
	 */
	public mxGraphTransferable getTransferable() {
		if (transferable == null) {
			transferable = new mxGraphTransferable(new Object[] {BlockPalette.this.getBlock()},
					(mxRectangle) BlockPalette.this.getBlock().getGeometry().clone());	    
		}
		return transferable;
	}
}
