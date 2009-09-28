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
	private int preview_widht;
	private int preview_height;
	private Printable componentToPrint;
	private JComboBox scales_comboBox;
	private PreviewContainer preview_container;


	public XpadPrintPreviewWindow(Printable printableComponent, Xpad editor) {
		this(printableComponent, "Print Preview", editor);
	}

	public XpadPrintPreviewWindow(Printable printableComponent, String title, final Xpad editor) {

		// Preview JDialog
		setModal(true); // set focus priority on the JDialog
		setTitle(title);
		setSize(640, 940);

		// Component which we want the preview
		componentToPrint = printableComponent;

		// Tool bar of the preview window 
		JToolBar tool_bar = new JToolBar();
		JButton button_print = new JButton("Print");

		// Launch the print action
		ActionListener action_listener = new ActionListener() { 
			public void actionPerformed(ActionEvent e) { 
				PrintAction.printXpadDocument(editor);
			}
		};
		button_print.addActionListener(action_listener);
		button_print.setAlignmentY(0.5f);
		button_print.setMargin(new Insets(4,6,4,6));
		tool_bar.add(button_print);

		// Close the preview window
		button_print = new JButton("Close");
		action_listener = new ActionListener() { 
			public void actionPerformed(ActionEvent e) { 
				dispose();
			}
		};
		button_print.addActionListener(action_listener);
		button_print.setAlignmentY(0.5f);
		button_print.setMargin(new Insets(2,6,2,6));
		tool_bar.add(button_print);

		// Scale of the preview
		String[] scales = {"10 %", "25 %", "50 %", "100 %", "150 %", "200 %"};
		scales_comboBox = new JComboBox(scales);
		// Default scale is 100% (index 3 of scales)
		scales_comboBox.setSelectedItem(scales[3]);
		action_listener = new ActionListener() { 
			public void actionPerformed(ActionEvent e) { 
				Thread runner = new Thread() {
					public void run() {
						String selected_scale = scales_comboBox.getSelectedItem().
						toString();

						if (selected_scale.endsWith("%")) {
							selected_scale = selected_scale.substring(0, selected_scale.length()-1);
						}
						selected_scale = selected_scale.trim();

						int scale = 0;
						try { 
							scale = Integer.parseInt(selected_scale); 
						} catch (NumberFormatException e) {
							e.printStackTrace();
							return; 
						}

						int w = (int)(preview_widht * scale/100);
						int h = (int)(preview_height * scale/100);

						Component[] comps = preview_container.getComponents();
						for (int i = 0; i < comps.length; i++) {
							if (!(comps[i] instanceof ElementPreview)) {
								continue;
							}
							ElementPreview elements_to_preview = (ElementPreview)comps[i];
							elements_to_preview.setScaledSize(w, h);
						}

						preview_container.doLayout();
						preview_container.getParent().getParent().validate();
					}
				};
				runner.start();
			}
		};

		scales_comboBox.addActionListener(action_listener);
		scales_comboBox.setMaximumSize(scales_comboBox.getPreferredSize());
		scales_comboBox.setEditable(true);
		tool_bar.addSeparator();
		tool_bar.add(scales_comboBox);
		getContentPane().add(tool_bar, BorderLayout.NORTH);

		preview_container = new PreviewContainer();

		// Get page setup, if user has define a setup
		// else get the default setup page
		PageFormat page_format = PageSetupAction.getPageFormat();
		if (page_format == null) {
			PrinterJob printer_job = PrinterJob.getPrinterJob();
			page_format = printer_job.defaultPage();
			if (page_format.getHeight() == 0 || page_format.getWidth() == 0) {
				return;
			}
		}

		preview_widht = (int)(page_format.getWidth());
		preview_height = (int)(page_format.getHeight());

		// Default scale is 100%
		int scale = 100;
		int w = (int)(preview_widht * scale/100);
		int h = (int)(preview_height * scale/100);

		int page_index = 0;
		// Creating the preview image
		try {
			while (true) {
				BufferedImage img = new BufferedImage(preview_widht, preview_height, BufferedImage.TYPE_INT_RGB);
				Graphics g = img.getGraphics();
				g.setColor(Color.white);
				g.fillRect(0, 0, preview_widht, preview_height);
				if (printableComponent.print(g, page_format, page_index) != Printable.PAGE_EXISTS) {
					break;
				}
				ElementPreview element_to_preview = new ElementPreview(w, h, img);
				preview_container.add(element_to_preview);
				page_index++;
			}
		} catch (PrinterException e) {
			e.printStackTrace();
		}

		JScrollPane scroll_pane = new JScrollPane(preview_container);
		getContentPane().add(scroll_pane, BorderLayout.CENTER);

		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		setVisible(true);
	}

	private class PreviewContainer extends JPanel {
		// Gap beetwen previews, a preview is an image of the JTextpane
		protected int LEFT_RIGHT_GAP = 16;
		protected int UP_DOWN_GAP = 10;

		public Dimension getPreferredSize() {
			int number_of_components = getComponentCount();
			if (number_of_components == 0) {
				return new Dimension(LEFT_RIGHT_GAP, UP_DOWN_GAP);
			}

			Component comp = getComponent(0);
			Dimension component_dimension = comp.getPreferredSize();
			int w = component_dimension.width;
			int h = component_dimension.height;

			Dimension parent_component_dimension = getParent().getSize();
			int columns = Math.max((parent_component_dimension.width - LEFT_RIGHT_GAP) / (w + LEFT_RIGHT_GAP), 1);
			int rows = number_of_components / columns;
			if (rows * columns < number_of_components) {
				rows++;
			}

			int container_width = columns * (w + LEFT_RIGHT_GAP) + LEFT_RIGHT_GAP;
			int container_height = rows * (h + UP_DOWN_GAP) + UP_DOWN_GAP;
			Insets insets = getInsets();
			return new Dimension(container_width + insets.left + insets.right, container_height + insets.top + insets.bottom);
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

			int number_of_components = getComponentCount();
			if (number_of_components == 0) {
				return;
			}
			Component comp = getComponent(0);
			Dimension component_dimension = comp.getPreferredSize();
			int w = component_dimension.width;
			int h = component_dimension.height;

			Dimension parent_component_dimension = getParent().getSize();
			int columns = Math.max((parent_component_dimension.width - LEFT_RIGHT_GAP) / (w + LEFT_RIGHT_GAP), 1);
			int rows = number_of_components / columns;
			if (rows * columns < number_of_components) {
				rows++;
			}

			int max = 0;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					if (max >= number_of_components) {
						return;
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
		protected int element_width;
		protected int element_height;
		protected Image element_image;
		protected Image print_of_component;

		public ElementPreview(int w, int h, Image img) {
			element_width = w;
			element_height = h;
			print_of_component= img;
			element_image = print_of_component.getScaledInstance(element_width, element_height, Image.SCALE_SMOOTH);
			element_image.flush();
			setBackground(Color.white);
			setBorder(new MatteBorder(1, 1, 2, 2, Color.black));
		}

		public void setScaledSize(int w, int h) {
			element_width = w;
			element_height = h;
			element_image = print_of_component.getScaledInstance(element_width, element_height, Image.SCALE_SMOOTH);
			repaint();
		}

		public Dimension getPreferredSize() {
			Insets insets = getInsets();
			return new Dimension((element_width + insets.left + insets.right), (element_height + insets.top + insets.bottom));
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
			g.drawImage(element_image, 0, 0, this);
			paintBorder(g);
		}
	}
}
