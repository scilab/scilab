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
import com.mxgraph.util.mxEventSource.mxIEventListener;

public class XcosPalette extends JScrollPane implements ComponentListener {
    private static final long serialVersionUID = 5693635134906513755L;

    private JPanel panel = null;
    private String name;

    protected BlockPalette selectedEntry;

    protected mxEventSource eventSource = new mxEventSource(this);

    protected Color gradientColor = Color.LIGHT_GRAY;

    public XcosPalette(String name) {
	super(new JPanel());
	panel = (JPanel) getViewport().getComponent(0);
	this.name = name;
	setBackground(Color.WHITE);

	panel.setBackground(Color.WHITE);
	panel.setLayout(new FlowLayout(FlowLayout.LEADING, XcosConstants.PALETTE_HMARGIN, XcosConstants.PALETTE_VMARGIN));
	panel.setPreferredSize(new Dimension((XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN), 0));

	getVerticalScrollBar().setBlockIncrement(XcosConstants.PALETTE_BLOCK_HEIGHT + XcosConstants.PALETTE_VMARGIN);
	getVerticalScrollBar().setUnitIncrement(XcosConstants.PALETTE_BLOCK_HEIGHT + XcosConstants.PALETTE_VMARGIN);

	//getHorizontalScrollBar().setVisible(false);
	getHorizontalScrollBar().setBlockIncrement(XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN);
	getHorizontalScrollBar().setUnitIncrement(XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN);

	addComponentListener(this);
	// Clears the current selection when the background is clicked
	addMouseListener(new MouseListener()
	{

	    /*
	     * (non-Javadoc)
	     * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
	     */
	    public void mousePressed(MouseEvent e) {
		clearSelection();
	    }

	    /*
	     * (non-Javadoc)
	     * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
	     */
	    public void mouseClicked(MouseEvent e) {
	    }

	    /*
	     * (non-Javadoc)
	     * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
	     */
	    public void mouseEntered(MouseEvent e) {
	    }

	    /*
	     * (non-Javadoc)
	     * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
	     */
	    public void mouseExited(MouseEvent e) {
	    }

	    /*
	     * (non-Javadoc)
	     * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
	     */
	    public void mouseReleased(MouseEvent e) {
	    }

	});

	// Shows a nice icon for drag and drop but doesn't import anything
	setTransferHandler(new TransferHandler() {
	    private static final long serialVersionUID = 1L;

	    public boolean canImport(JComponent comp, DataFlavor[] flavors) {
		return true;
	    }
	});
    }

    /**
     * 
     */
    public void setGradientColor(Color c) {
	gradientColor = c;
    }

    /**
     * 
     */
    public Color getGradientColor() {
	return gradientColor;
    }

    /**
     * 
     */
    public void paintComponent(Graphics g) {
	if (gradientColor == null) {
	    super.paintComponent(g);
	} else {
	    Rectangle rect = getVisibleRect();

	    if (g.getClipBounds() != null) {
		rect = rect.intersection(g.getClipBounds());
	    }

	    Graphics2D g2 = (Graphics2D) g;

	    g2.setPaint(new GradientPaint(0, 0, getBackground(), getWidth(), 0,
		    gradientColor));
	    g2.fill(rect);
	}
    }

    /**
     * 
     */
    public void clearSelection() {
	setSelectionEntry(null, null);
    }

    /**
     * 
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

	eventSource.fireEvent(mxEvent.SELECT, new mxEventObject(new Object[] {
		selectedEntry, t, last }));
    }



    /**
     * 
     * @param name
     * @param icon
     * @param style
     * @param width
     * @param height
     * @param value
     */
    public void addTemplate(final String name, ImageIcon icon) {

	final BlockPalette entry = new BlockPalette(icon);

	entry.setPalette(this);
	entry.setToolTipText(name);
	entry.setText(name);

	// Install the handler for dragging nodes into a graph
	DragGestureListener dragGestureListener = new DragGestureListener()
	{
	    /**
	     * 
	     */
	    public void dragGestureRecognized(DragGestureEvent e) {
		e.startDrag(null, mxConstants.EMPTY_IMAGE, new Point(), entry.getTransferable(), null);
	    }

	};

	DragSource dragSource = new DragSource();
	dragSource.createDefaultDragGestureRecognizer(entry,
		DnDConstants.ACTION_COPY, dragGestureListener);

	panel.add(entry);
    }

    /**
     * @param eventName
     * @param listener
     * @see com.mxgraph.util.mxEventSource#addListener(java.lang.String, com.mxgraph.util.mxEventSource.mxIEventListener)
     */
    public void addListener(String eventName, mxIEventListener listener) {
	eventSource.addListener(eventName, listener);
    }

    /**
     * @return
     * @see com.mxgraph.util.mxEventSource#isEventsEnabled()
     */
    public boolean isEventsEnabled() {
	return eventSource.isEventsEnabled();
    }

    /**
     * @param listener
     * @see com.mxgraph.util.mxEventSource#removeListener(com.mxgraph.util.mxEventSource.mxIEventListener)
     */
    public void removeListener(mxIEventListener listener) {
	eventSource.removeListener(listener);
    }

    /**
     * @param eventName
     * @param listener
     * @see com.mxgraph.util.mxEventSource#removeListener(java.lang.String, com.mxgraph.util.mxEventSource.mxIEventListener)
     */
    public void removeListener(mxIEventListener listener, String eventName) {
	eventSource.removeListener(listener, eventName);
    }

    /**
     * @param eventsEnabled
     * @see com.mxgraph.util.mxEventSource#setEventsEnabled(boolean)
     */
    public void setEventsEnabled(boolean eventsEnabled) {
	eventSource.setEventsEnabled(eventsEnabled);
    }

    public void componentHidden(ComponentEvent arg0) {
    }

    public void componentMoved(ComponentEvent arg0) {
    }

    public void componentResized(ComponentEvent arg0) {
	if (arg0.getSource() instanceof XcosPalette) {
	    XcosPalette palette = ((XcosPalette) arg0.getSource());
	    int panelWidth = (int) palette.getSize().getWidth() - 3;

	    //take care if VerticalScrollBar is visible to compute visible area
	    if (getVerticalScrollBar().isVisible()) {
		panelWidth -=  getVerticalScrollBar().getWidth();
	    }

	    int numberOfCols = panelWidth / (XcosConstants.PALETTE_BLOCK_WIDTH + XcosConstants.PALETTE_HMARGIN);
	    double numberOfRows = (double) panel.getComponentCount() / (double) numberOfCols;
	    int preferedHeight = (int) ((XcosConstants.PALETTE_BLOCK_HEIGHT + XcosConstants.PALETTE_VMARGIN) * Math.ceil(numberOfRows));

	    panel.setPreferredSize(new Dimension(panelWidth, preferedHeight));
	}
    }

    public void componentShown(ComponentEvent arg0) {
    }

    public String toString() {
	return this.name;
    }

    public BlockPalette getSelectedEntry() {
	return selectedEntry;
    }

    public void setSelectedEntry(BlockPalette selectedEntry) {
	this.selectedEntry = selectedEntry;
    }

}
