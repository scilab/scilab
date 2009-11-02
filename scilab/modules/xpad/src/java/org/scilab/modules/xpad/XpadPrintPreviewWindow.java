package org.scilab.modules.xpad;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;
import javax.swing.border.MatteBorder;

import org.scilab.modules.xpad.actions.PageSetupAction;
import org.scilab.modules.xpad.actions.PrintAction;

public class XpadPrintPreviewWindow extends JDialog {
	private int previewWidht;
	private int previewHeight;
	private JComboBox scalesComboBox;
	private PreviewContainer previewContainer;


	public XpadPrintPreviewWindow(Printable printableComponent, Xpad editor) {
		this(printableComponent, "Print Preview", editor);
	}

	public XpadPrintPreviewWindow(Printable printableComponent, String title, final Xpad editor) {

		// Preview JDialog
		setModal(true); // set focus priority on the JDialog
		setTitle(title);
		setSize(640, 940);

		// Tool bar of the preview window 
		JToolBar toolbar = new JToolBar();
		JButton printButton = new JButton("Print");

		// Launch the print action
		ActionListener action_listener = new ActionListener() { 
			public void actionPerformed(ActionEvent e) { 
				PrintAction.printXpadDocument(editor);
			}
		};
		printButton.addActionListener(action_listener);
		printButton.setAlignmentY(0.5f);
		printButton.setMargin(new Insets(4,6,4,6));
		toolbar.add(printButton);

		// Close the preview window
		printButton = new JButton("Close");
		action_listener = new ActionListener() { 
			public void actionPerformed(ActionEvent e) { 
				dispose();
			}
		};
		printButton.addActionListener(action_listener);
		printButton.setAlignmentY(0.5f);
		printButton.setMargin(new Insets(2,6,2,6));
		toolbar.add(printButton);

		// Scale of the preview
		String[] scales = {"10 %", "25 %", "50 %", "100 %", "150 %", "200 %"};
		scalesComboBox = new JComboBox(scales);
		// Default scale is 100% (index 3 of scales)
		scalesComboBox.setSelectedItem(scales[3]);
		action_listener = new ActionListener() { 
			public void actionPerformed(ActionEvent e) { 
				Thread runner = new Thread() {
					public void run() {
						String selectedScale = scalesComboBox.getSelectedItem().
						toString();

						if (selectedScale.endsWith("%")) {
							selectedScale = selectedScale.substring(0, selectedScale.length() - 1);
						}
						selectedScale = selectedScale.trim();

						int scale = 0;
						try { 
							scale = Integer.parseInt(selectedScale); 
						} catch (NumberFormatException e) {
							e.printStackTrace();
							return; 
						}

						int w = (int) (previewWidht * scale/100);
						int h = (int) (previewHeight * scale/100);

						Component[] comps = previewContainer.getComponents();
						for (int i = 0; i < comps.length; i++) {
							if (!(comps[i] instanceof ElementPreview)) {
								continue;
							}
							ElementPreview elementsToPreview = (ElementPreview) comps[i];
							elementsToPreview.setScaledSize(w, h);
						}

						previewContainer.doLayout();
						previewContainer.getParent().getParent().validate();
					}
				};
				runner.start();
			}
		};

		scalesComboBox.addActionListener(action_listener);
		scalesComboBox.setMaximumSize(scalesComboBox.getPreferredSize());
		scalesComboBox.setEditable(true);
		toolbar.addSeparator();
		toolbar.add(scalesComboBox);
		getContentPane().add(toolbar, BorderLayout.NORTH);

		previewContainer = new PreviewContainer();

		// Get page setup, if user has define a setup
		// else get the default setup page
		PageFormat pageFormat = PageSetupAction.getPageFormat();
		if (pageFormat == null) {
			PrinterJob printerJob = PrinterJob.getPrinterJob();
			pageFormat = printerJob.defaultPage();
			if (pageFormat.getHeight() == 0 || pageFormat.getWidth() == 0) {
				return;
			}
		}

		previewWidht = (int) (pageFormat.getWidth());
		previewHeight = (int) (pageFormat.getHeight());

		// Default scale is 100%
		int scale = 100;
		int w = (int) (previewWidht * scale/100); /* @TODO 100/100 ? */
		int h = (int) (previewHeight * scale/100); /* @TODO 100/100 ? */

		int pageIndex = 0;
		// Creating the preview image
		try {
			while (true) {
				BufferedImage img = new BufferedImage(previewWidht, previewHeight, BufferedImage.TYPE_INT_RGB);
				Graphics g = img.getGraphics();
				g.setColor(Color.white);
				g.fillRect(0, 0, previewWidht, previewHeight);
				if (printableComponent.print(g, pageFormat, pageIndex) != Printable.PAGE_EXISTS) {
					break;
				}
				ElementPreview elementToPreview = new ElementPreview(w, h, img);
				previewContainer.add(elementToPreview);
				pageIndex++;
			}
		} catch (PrinterException e) {
			e.printStackTrace();
		}

		JScrollPane scrollPane = new JScrollPane(previewContainer);
		getContentPane().add(scrollPane, BorderLayout.CENTER);

		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		setVisible(true);
	}

	private class PreviewContainer extends JPanel {
		// Gap beetwen previews, a preview is an image of the JTextpane
		protected int LEFT_RIGHT_GAP = 16;
		protected int UP_DOWN_GAP = 10;

		public Dimension getPreferredSize() {
			int numberOfComponents = getComponentCount();
			if (numberOfComponents == 0) {
				return new Dimension(LEFT_RIGHT_GAP, UP_DOWN_GAP);
			}

			Component comp = getComponent(0);
			Dimension componentDimension = comp.getPreferredSize();
			int w = componentDimension.width;
			int h = componentDimension.height;

			Dimension parentComponentDimension = getParent().getSize();
			int columns = Math.max((parentComponentDimension.width - LEFT_RIGHT_GAP) / (w + LEFT_RIGHT_GAP), 1);
			int rows = numberOfComponents / columns;
			if (rows * columns < numberOfComponents) {
				rows++;
			}

			int containerWidth = columns * (w + LEFT_RIGHT_GAP) + LEFT_RIGHT_GAP;
			int containerHeight = rows * (h + UP_DOWN_GAP) + UP_DOWN_GAP;
			Insets insets = getInsets();
			return new Dimension(containerWidth + insets.left + insets.right, containerHeight + insets.top + insets.bottom);
		}

		// Get dimensions for PreviewContainer
		public Dimension getMaximumSize() {
			return getPreferredSize();
		}

		public Dimension getMinimumSize() {
			return getPreferredSize();
		}

		public void doLayout() {
			Insets insets = getInsets();
			int x = insets.left + LEFT_RIGHT_GAP;
			int y = insets.top + UP_DOWN_GAP;

			int numberOfComponents = getComponentCount();
			if (numberOfComponents == 0) {
				return;
			}
			Component comp = getComponent(0);
			Dimension componentDimension = comp.getPreferredSize();
			int w = componentDimension.width;
			int h = componentDimension.height;

			Dimension parentComponentDimension = getParent().getSize();
			int columns = Math.max((parentComponentDimension.width - LEFT_RIGHT_GAP) / (w + LEFT_RIGHT_GAP), 1);
			int rows = numberOfComponents / columns;
			if (rows * columns < numberOfComponents) {
				rows++;
			}

			int max = 0;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					if (max >= numberOfComponents) {
						return; /* @TODO add comment */
					}

					comp = getComponent(max++);
					comp.setBounds(x, y, w, h);
					x += w + LEFT_RIGHT_GAP;
				}
				y += h + UP_DOWN_GAP;
				x = insets.left + LEFT_RIGHT_GAP;
			}
		}
	}

	private class ElementPreview extends JPanel {
		// Dimensions of element to preview
		protected int elementWidth;
		protected int elementHeight;
		protected Image elementImage;
		protected Image printOfComponent;

		public ElementPreview(int w, int h, Image img) {
			elementWidth = w;
			elementHeight = h;
			printOfComponent= img;
			elementImage = printOfComponent.getScaledInstance(elementWidth, elementHeight, Image.SCALE_SMOOTH);
			elementImage.flush();
			setBackground(Color.white);
			setBorder(new MatteBorder(1, 1, 2, 2, Color.black));
		}

		public void setScaledSize(int w, int h) {
			elementWidth = w;
			elementHeight = h;
			elementImage = printOfComponent.getScaledInstance(elementWidth, elementHeight, Image.SCALE_SMOOTH);
			repaint();
		}

		public Dimension getPreferredSize() {
			Insets insets = getInsets();
			return new Dimension((elementWidth + insets.left + insets.right), (elementHeight + insets.top + insets.bottom));
		}

		// Get dimensions for ElementPreview	
		public Dimension getMaximumSize() {
			return getPreferredSize();
		}

		public Dimension getMinimumSize() {
			return getPreferredSize();
		}

		public void paint(Graphics g) {
			g.setColor(getBackground());
			g.fillRect(0, 0, getWidth(), getHeight());
			g.drawImage(elementImage, 0, 0, this);
			paintBorder(g);
		}
	}
}
