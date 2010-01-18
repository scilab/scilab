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

package org.scilab.modules.xcos.palette;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.datatransfer.DataFlavor;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.TransferHandler;

import org.flexdock.plaf.common.border.ShadowBorder;
import org.scilab.modules.xcos.utils.XcosConstants;

import com.mxgraph.swing.util.mxGraphTransferable;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxEventSource;

/**
 * The grid panel implementation for the BlockPalettes (main block view).
 */
public class XcosPalette extends JScrollPane {

	/** Helper to manage the entry selection */
	public final class EntryManager {
		private BlockPalette selectedEntry;

		/** Default Constructor */
		private EntryManager() {
		}

		/**
		 * Clear the selection.
		 */
		public void clearSelection() {
			setSelectionEntry(null, null);
		}

		/**
		 * Does the block palette is selected ?
		 * 
		 * @param blockPalette
		 *            The tested block palette
		 * @return true if it is selected, false otherwise
		 */
		public boolean isSelectedEntry(BlockPalette blockPalette) {
			return blockPalette == selectedEntry;
		}

		/**
		 * Select a block (perform UI and control update)
		 * 
		 * @param entry
		 *            The selected block entry
		 * @param t
		 *            The associated transferable state
		 */
		public void setSelectionEntry(BlockPalette entry, mxGraphTransferable t) {
			BlockPalette last = selectedEntry;
			selectedEntry = entry;

			if (last != null) {
				last.setBorder(null);
				last.setOpaque(false);
			}

			if (selectedEntry != null) {
				selectedEntry.setBorder(new ShadowBorder());
				selectedEntry.setOpaque(true);
			}

			eventSource.fireEvent(new mxEventObject(mxEvent.SELECT, "entry",
					selectedEntry, "transferable", t, "previous", last));
		}

	}

	/**
	 * Contains all the controllers used by this class.
	 */
	private static final class XcosPaletteController {

		private ComponentListener componentListener = new ComponentListener() {

			/**
			 * Not used
			 * 
			 * @param arg0
			 *            Not used
			 * @see ComponentListener
			 */
			public void componentHidden(ComponentEvent arg0) {
			}

			/**
			 * Not used
			 * 
			 * @param arg0
			 *            Not used
			 * @see ComponentListener
			 */
			public void componentMoved(ComponentEvent arg0) {
			}

			/**
			 * Do the layout of the blocks representations
			 * 
			 * @param arg0
			 *            Event data
			 * @see ComponentListener
			 */
			public void componentResized(ComponentEvent arg0) {
				if (arg0.getSource() instanceof XcosPalette) {
					XcosPalette palette = ((XcosPalette) arg0.getSource());
					int panelWidth = (int) palette.getSize().getWidth()
							- BORDER_WIDTH;

					// take care if VerticalScrollBar is visible to compute
					// visible area
					if (palette.getVerticalScrollBar().isVisible()) {
						panelWidth -= palette.getVerticalScrollBar().getWidth();
					}

					int numberOfCols = panelWidth
							/ (XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN);
					double numberOfRows = (double) palette.panel
							.getComponentCount()
							/ (double) numberOfCols;
					int preferedHeight = (int) ((XcosConstants.PALETTE_BLOCK_HEIGHT + XcosConstants.PALETTE_VMARGIN) * Math
							.ceil(numberOfRows));

					palette.panel.setPreferredSize(new Dimension(panelWidth,
							preferedHeight));
				}
			}

			/**
			 * Not used
			 * 
			 * @param arg0
			 *            Not used
			 * @see ComponentListener
			 */
			public void componentShown(ComponentEvent arg0) {
			}

		};

		private MouseListener mouseListener = new MouseListener() {

			/*
			 * (non-Javadoc)
			 * 
			 * @see
			 * java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent
			 * )
			 */
			public void mouseClicked(MouseEvent e) {
			}

			/*
			 * (non-Javadoc)
			 * 
			 * @see
			 * java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent
			 * )
			 */
			public void mouseEntered(MouseEvent e) {
			}

			/*
			 * (non-Javadoc)
			 * 
			 * @see
			 * java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent
			 * )
			 */
			public void mouseExited(MouseEvent e) {
			}

			/*
			 * (non-Javadoc)
			 * 
			 * @see
			 * java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent
			 * )
			 */
			public void mousePressed(MouseEvent e) {
				if (e.getSource() instanceof XcosPalette) {
					XcosPalette palette = ((XcosPalette) e.getSource());
					palette.getEntryManager().clearSelection();
				}
			}

			/*
			 * (non-Javadoc)
			 * 
			 * @see
			 * java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent
			 * )
			 */
			public void mouseReleased(MouseEvent e) {
			}
		};

		/** Default constructor */
		private XcosPaletteController() {
		}

		/**
		 * @return the componentListener
		 */
		private ComponentListener getComponentListener() {
			return componentListener;
		}

		/**
		 * @return the mouseListener
		 */
		private MouseListener getMouseListener() {
			return mouseListener;
		}
	}
	private static final int BORDER_WIDTH = 3;
	private static final XcosPaletteController CONTROLLER = new XcosPaletteController();

	private static final Color GRADIENT_COLOR = Color.LIGHT_GRAY;

	private static final long serialVersionUID = 5693635134906513755L;
	
	private EntryManager entryManager = new EntryManager();
	private mxEventSource eventSource = new mxEventSource(this);

	private String name;
	private JPanel panel;

	/**
	 * Default constructor
	 * 
	 * @param name
	 *            The palette name
	 */
	public XcosPalette(String name) {
		super(new JPanel());
		panel = (JPanel) getViewport().getComponent(0);
		this.name = name;
		initComponents();

		addComponentListener(CONTROLLER.getComponentListener());
		// Clears the current selection when the background is clicked
		addMouseListener(CONTROLLER.getMouseListener());

		// Shows a nice icon for drag and drop but doesn't import anything
		setTransferHandler(new TransferHandler() {
			private static final long serialVersionUID = 1L;

			public boolean canImport(JComponent comp, DataFlavor[] flavors) {
				return true;
			}
		});
	}

	/**
	 * Setup the graphical components
	 */
	private void initComponents() {
		setBackground(Color.WHITE);

		panel.setBackground(Color.WHITE);
		panel.setLayout(new FlowLayout(FlowLayout.LEADING,
				XcosConstants.PALETTE_HMARGIN, XcosConstants.PALETTE_VMARGIN));
		panel.setPreferredSize(new Dimension(
				(XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN),
				0));

		getVerticalScrollBar().setBlockIncrement(
				XcosConstants.PALETTE_BLOCK_HEIGHT
						+ XcosConstants.PALETTE_VMARGIN);
		getVerticalScrollBar().setUnitIncrement(
				XcosConstants.PALETTE_BLOCK_HEIGHT
						+ XcosConstants.PALETTE_VMARGIN);

		// getHorizontalScrollBar().setVisible(false);
		getHorizontalScrollBar().setBlockIncrement(
				XcosConstants.PALETTE_BLOCK_WIDTH
						+ XcosConstants.PALETTE_HMARGIN);
		getHorizontalScrollBar().setUnitIncrement(
				XcosConstants.PALETTE_BLOCK_WIDTH
						+ XcosConstants.PALETTE_HMARGIN);
	}

	/**
	 * Add a block representative data
	 * 
	 * @param name
	 *            The block name
	 * @param icon
	 *            The associated icon
	 */
	public void addTemplate(final String name, ImageIcon icon) {

		final BlockPalette entry = new BlockPalette(icon);

		entry.setPalette(this);
		entry.setToolTipText(name);
		entry.setText(name);

		// Install the handler for dragging nodes into a graph
		DragGestureListener dragGestureListener = new DragGestureListener() {
			public void dragGestureRecognized(DragGestureEvent e) {
				e.startDrag(null, mxConstants.EMPTY_IMAGE, new Point(), entry
						.getTransferable(), null);
			}

		};

		DragSource dragSource = new DragSource();
		dragSource.createDefaultDragGestureRecognizer(entry,
				DnDConstants.ACTION_COPY, dragGestureListener);

		panel.add(entry);
	}

	/** @return the entry manager */
	public EntryManager getEntryManager() {
		return entryManager;
	}

	/**
	 * Call the UI Paint method with a {@link GradientPaint} customized by the
	 * {@link #GRADIENT_COLOR}.
	 * 
	 * @param g
	 *            Global graphical context
	 */
	@Override
	public void paintComponent(Graphics g) {
		Rectangle rect = getVisibleRect();

		if (g.getClipBounds() != null) {
			rect = rect.intersection(g.getClipBounds());
		}

		Graphics2D g2 = (Graphics2D) g;

		g2.setPaint(new GradientPaint(0, 0, getBackground(), getWidth(), 0,
				GRADIENT_COLOR));
		g2.fill(rect);
	}

	/**
	 * @return the name of the palette
	 */
	@Override
	public String toString() {
		return this.name;
	}
}
