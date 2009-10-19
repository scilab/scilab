package org.scilab.modules.xcos.palette;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GradientPaint;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.MouseInfo;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.datatransfer.DataFlavor;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragGestureRecognizer;
import java.awt.dnd.DragSource;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;
import javax.swing.border.Border;

import org.flexdock.plaf.common.border.ShadowBorder;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.swing.util.mxGraphTransferable;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxEventSource;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxEventSource.mxIEventListener;

public class XcosPalette extends JScrollPane {

	private static final int BLOCK_WIDTH = 150;
	private static final int BLOCK_HEIGHT = 150;
	private static final int HMARGIN = 5;
	private static final int VMARGIN = 5;
	private static final int DEFAULT_NB_COLS = 1; /* Updated dynamically at creation */
	
	private JPanel panel;

	protected JLabel selectedEntry;

	protected mxEventSource eventSource = new mxEventSource(this);

	protected Color gradientColor = Color.LIGHT_GRAY;

	public XcosPalette() {
			super(new JPanel());
			panel = (JPanel) getViewport().getComponent(0);
			setBackground(Color.WHITE);
			
			panel.setBackground(Color.WHITE);
			panel.setLayout(new FlowLayout(FlowLayout.LEADING, HMARGIN, VMARGIN));
			panel.setPreferredSize(new Dimension(DEFAULT_NB_COLS * (BLOCK_WIDTH + HMARGIN), 0));

			getVerticalScrollBar().setBlockIncrement(BLOCK_HEIGHT + VMARGIN);
			getHorizontalScrollBar().setBlockIncrement(BLOCK_WIDTH + HMARGIN);
			getVerticalScrollBar().setUnitIncrement(BLOCK_HEIGHT + VMARGIN);
			getHorizontalScrollBar().setUnitIncrement(BLOCK_WIDTH + HMARGIN);

			// Clears the current selection when the background is clicked
			addMouseListener(new MouseListener()
			{

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
				 */
				public void mousePressed(MouseEvent e)
				{
					clearSelection();
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
				 */
				public void mouseClicked(MouseEvent e)
				{
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
				 */
				public void mouseEntered(MouseEvent e)
				{
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
				 */
				public void mouseExited(MouseEvent e)
				{
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
				 */
				public void mouseReleased(MouseEvent e)
				{
				}

			});

			// Shows a nice icon for drag and drop but doesn't import anything
			setTransferHandler(new TransferHandler()
			{
				public boolean canImport(JComponent comp, DataFlavor[] flavors)
				{
					return true;
				}
			});
		}

		/**
		 * 
		 */
		public void setGradientColor(Color c)
		{
			gradientColor = c;
		}

		/**
		 * 
		 */
		public Color getGradientColor()
		{
			return gradientColor;
		}

		/**
		 * 
		 */
		public void paintComponent(Graphics g)
		{
			if (gradientColor == null)
			{
				super.paintComponent(g);
			}
			else
			{
				Rectangle rect = getVisibleRect();

				if (g.getClipBounds() != null)
				{
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
		public void clearSelection()
		{
			setSelectionEntry(null, null);
		}

		/**
		 * 
		 */
		public void setSelectionEntry(JLabel entry, mxGraphTransferable t)
		{
			JLabel last = selectedEntry;
			selectedEntry = entry;

			if (last != null)
			{
				last.setBorder(null);
				last.setOpaque(false);
			}

			if (selectedEntry != null)
			{
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
		public void addEdgeTemplate(final String name, ImageIcon icon,
				String style, int width, int height, Object value)
		{
			mxGeometry geometry = new mxGeometry(0, 0, width, height);
			geometry.setTerminalPoint(new mxPoint(0, height), true);
			geometry.setTerminalPoint(new mxPoint(width, 0), false);
			geometry.setRelative(true);

			mxCell cell = new mxCell(value, geometry, style);
			cell.setEdge(true);

			addTemplate(name, icon, cell);
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
		public void addTemplate(final String name, ImageIcon icon, String style,
				int width, int height, Object value)
		{
			mxCell cell = new mxCell(value, new mxGeometry(0, 0, width, height),
					style);
			cell.setVertex(true);

			addTemplate(name, icon, cell);
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
		public void addTemplate(final String name, ImageIcon icon, mxCell cell)
		{
			mxRectangle bounds = (mxGeometry) cell.getGeometry().clone();
			final mxGraphTransferable t = new mxGraphTransferable(
					new Object[] { cell }, bounds);
			
			final BasicBlock cloneMe = (BasicBlock) cell;
			
			// Scales the image if it's too large for the library
			if (icon != null)
			{
				if (icon.getIconWidth() > 128) {
					icon.setImage(icon.getImage().getScaledInstance(128, icon.getIconHeight() * 128 / icon.getIconWidth(),0));
				}
				if (icon.getIconHeight() > 128) {
					icon.setImage(icon.getImage().getScaledInstance(icon.getIconWidth() * 128 / icon.getIconHeight(), 128,0));
				}
			}

			final JLabel entry = new JLabel(icon)
			{
				/**
				 * 
				 */
				public void paint2(Graphics g)
				{
					boolean opaque = isOpaque();
					Color bg = getBackground();
					Border br = getBorder();

					if (selectedEntry == this)
					{
						setBackground(XcosPalette.this.getBackground().brighter());
						setBorder(new ShadowBorder());
						setOpaque(true);
					}

					super.paint(g);

					setBorder(br);
					setBackground(bg);
					setOpaque(opaque);
				}
			};

			entry.setPreferredSize(new Dimension(BLOCK_WIDTH, BLOCK_HEIGHT));
			entry.setBackground(XcosPalette.this.getBackground().brighter());
			entry.setFont(new Font(entry.getFont().getFamily(), 0, 14));

			entry.setVerticalTextPosition(JLabel.BOTTOM);
			entry.setHorizontalTextPosition(JLabel.CENTER);
			entry.setIconTextGap(0);

			entry.setToolTipText(name);
			entry.setText(name);

			entry.addMouseListener(new MouseListener()
			{

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
				 */
				public void mousePressed(MouseEvent e)
				{
					setSelectionEntry(entry, t);
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
				 */
				public void mouseClicked(MouseEvent e) {
					if (e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e)) {
						
						ContextMenu menu = ScilabContextMenu.createContextMenu();
						
						final ArrayList<XcosDiagram> allDiagrams = Xcos.getDiagrams();
						
						if (allDiagrams.size() == 0) {
							// No diagram opened: should never happen if Xcos opens an empty diagram when it is launched
							MenuItem addTo = ScilabMenuItem.createMenuItem();
							
							addTo.setText(XcosMessages.ADDTO_NEW_DIAGRAM);
							addTo.setCallback(new CallBack(name) {
								private static final long serialVersionUID = 1185879440137756636L;

								public void callBack() {
									Xcos.createEmptyDiagram().addCell(cloneMe.createClone());
								}
							});
							
							menu.add(addTo);
							
						} else if (allDiagrams.size() == 1) {
							// A single diagram opened: add to this diagram
							MenuItem addTo = ScilabMenuItem.createMenuItem();

							addTo.setText(XcosMessages.ADDTO + " " + allDiagrams.get(0).getParentTab().getName());
							final XcosDiagram theDiagram = allDiagrams.get(0);
							addTo.setCallback(new CallBack(name) {
								private static final long serialVersionUID = 1185879440137756636L;

								public void callBack() {
									theDiagram.addCell(cloneMe.createClone());
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
								diagram.setCallback(new CallBack(name) {
									private static final long serialVersionUID = -3138430622029406470L;

									public void callBack() {
										theDiagram.addCell(cloneMe.createClone());
									}
								});
								addTo.add(diagram);
							}

							menu.add(addTo);
						}
						
						
						menu.getAsSimpleContextMenu().addSeparator();
						
						MenuItem help = ScilabMenuItem.createMenuItem();
						help.setText("Block help");
						help.setCallback(new CallBack(name) {
							private static final long serialVersionUID = -8720228686621887887L;

							public void callBack() {
								InterpreterManagement.requestScilabExec("help " + name);
							}
						});
						menu.add(help);
					    
						menu.setVisible(true);

					    ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
					    		MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
					}
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
				 */
				public void mouseEntered(MouseEvent e)
				{
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
				 */
				public void mouseExited(MouseEvent e)
				{
				}

				/*
				 * (non-Javadoc)
				 * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
				 */
				public void mouseReleased(MouseEvent e)
				{
				}

			});

			// Install the handler for dragging nodes into a graph
			DragGestureListener dragGestureListener = new DragGestureListener()
			{
				/**
				 * 
				 */
				public void dragGestureRecognized(DragGestureEvent e)
				{
					e
					.startDrag(null, mxConstants.EMPTY_IMAGE, new Point(),
							t, null);
				}

			};

			DragSource dragSource = new DragSource();
			DragGestureRecognizer dgr = dragSource
			.createDefaultDragGestureRecognizer(entry,
					DnDConstants.ACTION_COPY, dragGestureListener);

			panel.add(entry);

			int panelWidth = (int) panel.getPreferredSize().getWidth();
			int numberOfCols = panelWidth / (BLOCK_WIDTH + HMARGIN);
			panel.setPreferredSize(new Dimension(panelWidth, (BLOCK_WIDTH + HMARGIN) * (panel.getComponentCount() / numberOfCols + 1)));
		}

		/**
		 * @param eventName
		 * @param listener
		 * @see com.mxgraph.util.mxEventSource#addListener(java.lang.String, com.mxgraph.util.mxEventSource.mxIEventListener)
		 */
		public void addListener(String eventName, mxIEventListener listener)
		{
			eventSource.addListener(eventName, listener);
		}

		/**
		 * @return
		 * @see com.mxgraph.util.mxEventSource#isEventsEnabled()
		 */
		public boolean isEventsEnabled()
		{
			return eventSource.isEventsEnabled();
		}

		/**
		 * @param listener
		 * @see com.mxgraph.util.mxEventSource#removeListener(com.mxgraph.util.mxEventSource.mxIEventListener)
		 */
		public void removeListener(mxIEventListener listener)
		{
			eventSource.removeListener(listener);
		}

		/**
		 * @param eventName
		 * @param listener
		 * @see com.mxgraph.util.mxEventSource#removeListener(java.lang.String, com.mxgraph.util.mxEventSource.mxIEventListener)
		 */
		public void removeListener(mxIEventListener listener, String eventName)
		{
			eventSource.removeListener(listener, eventName);
		}

		/**
		 * @param eventsEnabled
		 * @see com.mxgraph.util.mxEventSource#setEventsEnabled(boolean)
		 */
		public void setEventsEnabled(boolean eventsEnabled)
		{
			eventSource.setEventsEnabled(eventsEnabled);
		}
	}
