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

package org.scilab.modules.xcos.block.actions;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.GraphicsEnvironment;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

import javax.swing.SpinnerModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Customize the block representation.
 */
public class EditBlockFormatAction extends DefaultAction {
	/**
	 * Name of the action
	 */
	public static final String NAME = XcosMessages.EDIT + XcosMessages.DOTS;
	/**
	 * Icon of the action
	 */
	public static final String SMALL_ICON = "";
	/**
	 * Mnemonic key for the action
	 */
	public static final int MNEMONIC_KEY = KeyEvent.VK_F2;
	/**
	 * Accelerator key for the action
	 */
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

	/**
	 * The default color used on non initialized border color.
	 */
	private static final Color DEFAULT_BORDERCOLOR = new Color(0, 0, 0, 0);
	/**
	 * The default color used on non initialized filled color.
	 */
	private static final Color DEFAULT_FILLCOLOR = new Color(255, 255, 255, 0);
	
	/**
	 * Default constructor
	 * @param scilabGraph the current graph
	 */
	public EditBlockFormatAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Menu added to the menubar
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, EditBlockFormatAction.class);
	}
	
	/**
	 * Create a new dialog for editing this cell
	 * @param c the current component 
	 * @param name the window name
	 * @param selectedCell the selected cell
	 */
	public static void showDialog(ScilabComponent c, String name, mxCell selectedCell) {
		/*
		 * Looking for the parent window
		 */
		final Frame window = javax.swing.JOptionPane.getFrameForComponent(c);
		
		/*
		 * Create and show the dialog
		 */
		EditFormatDialog dialog = createDialog(selectedCell, window);
		dialog.setName(name);
		dialog.setVisible(true);
	}

	/**
	 * Create the dialog and set the default values
	 * @param selectedCell the current selected cell
	 * @param window the current windows
	 * @return the instantiated dialog
	 */
	private static EditFormatDialog createDialog(mxCell selectedCell,
			final Frame window) {
		String working;
		Color border;
		Color fill;
		String font;
		int fontSize;
		int fontStyle;
		Color textColor;
		String text;
		
		StyleMap style = new StyleMap(selectedCell.getStyle());
		
		working = style.get(mxConstants.STYLE_STROKECOLOR);
		if (working == null) {
			border = DEFAULT_BORDERCOLOR;
		} else {
			border = mxUtils.parseColor(working);
		}
		
		working = style.get(mxConstants.STYLE_FILLCOLOR);
		if (working == null) {
			fill = DEFAULT_FILLCOLOR;
		} else {
			fill = mxUtils.parseColor(working);
		}
		
		working = style.get(mxConstants.STYLE_FONTFAMILY);
		if (working == null) {
			font = mxConstants.DEFAULT_FONTFAMILY;
		} else {
			font = working;
		}
		
		working = style.get(mxConstants.STYLE_FONTSIZE);
		if (working == null) {
			fontSize = mxConstants.DEFAULT_FONTSIZE;
		} else {
			fontSize = Integer.parseInt(working);
		}

		working = style.get(mxConstants.STYLE_FONTSTYLE);
		if (working == null) {
			fontStyle = 0;
		} else {
			fontStyle = Integer.parseInt(working);
		}
		
		working = style.get(mxConstants.STYLE_FONTCOLOR);
		if (working == null) {
			textColor = Color.BLACK;
		} else {
			textColor = mxUtils.parseColor(working);
		}
		
		Object current = selectedCell.getValue();
		if (current == null) {
			text = "";
		} else {
			text = mxUtils.getBodyMarkup(current.toString(), false);
		}
		
		EditFormatDialog dialog = new EditFormatDialog(window);
		dialog.setValues(border, fill, font, fontSize, fontStyle, textColor, text);
		dialog.setCell(selectedCell);
		return dialog;
	}
	
	/**
	 * Update the cell value from the dialog ones.
	 * 
	 * @param dialog the current dialog
	 * @param borderColor the selected border color
	 * @param backgroundColor the selected background color
	 * @param fontName the selected font name
	 * @param fontSize the selected font size
	 * @param textColor the selected color
	 * @param text the typed text
	 */
	private static void updateFromDialog(EditFormatDialog dialog, Color borderColor, Color backgroundColor, 
				String fontName, int fontSize, Color textColor, String text) {
		mxCell cell = dialog.getCell();
		
		StyleMap style = new StyleMap(cell.getStyle());
		
		if (!borderColor.equals(DEFAULT_BORDERCOLOR)) {
			style.put(mxConstants.STYLE_STROKECOLOR, mxUtils.hexString(borderColor));
		}
		
		if (!backgroundColor.equals(DEFAULT_FILLCOLOR)) {
			style.put(mxConstants.STYLE_FILLCOLOR, mxUtils.hexString(backgroundColor));
		}
		
		if (!fontName.equals(mxConstants.DEFAULT_FONTFAMILY)) {
			style.put(mxConstants.STYLE_FONTFAMILY, fontName);
		}
		
		if (!fontName.equals(mxConstants.DEFAULT_FONTFAMILY)) {
			style.put(mxConstants.STYLE_FONTFAMILY, fontName);
		}
		
		if (fontSize != mxConstants.DEFAULT_FONTSIZE) {
			style.put(mxConstants.STYLE_FONTSIZE, Integer.toString(fontSize));
		}
		
		if (!textColor.equals(DEFAULT_BORDERCOLOR)) {
			style.put(mxConstants.STYLE_FONTCOLOR, mxUtils.hexString(textColor));
		}
		
		cell.setStyle(style.toString());
		cell.setValue(text);
	}
	
	/**
	 * Open a dialog to set the parameters
	 * @param e the current event
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		XcosDiagram graph = (XcosDiagram) getGraph(null);
		Object[] selectedCells = graph.getSelectionCells();
		
		EditBlockFormatAction.showDialog((ScilabComponent) graph
				.getAsComponent(), NAME, (mxCell) selectedCells[0]);
		
		graph.getView().clear(selectedCells[0], true, true);
		graph.updateCellSize(selectedCells[0]);
		graph.refresh();
	}
	
	/**
	 * Dialog used to edit the current cell.
	 * 
	 * This class perform UI instantiation and thus doesn't pass checkstyle
	 * metrics.
	 */
	// CSOFF: ClassDataAbstractionCoupling
	// CSOFF: ClassFanOutComplexity
	private static final class EditFormatDialog extends javax.swing.JDialog  {
		private static final int TEXT_AREA_ROWS = 5;
		private static final int TEXT_AREA_COLUMNS = 20;
		
		/**
		 * The default model used to set a font size. 
		 */
		private static final SpinnerModel FONTSIZE_MODEL = new javax.swing.SpinnerNumberModel(10, 0, 100, 1);
		/**
		 * The default border size used to separate buttons
		 */
		private static final int BORDER_SIZE = 10;
		
	    private javax.swing.JColorChooser backgroundColorChooser;
	    private javax.swing.JColorChooser borderColorChooser;
	    private javax.swing.JColorChooser textColorChooser;
	    private javax.swing.JComboBox fontNameComboBox;
	    private javax.swing.JLabel fontNameLabel;
	    private javax.swing.JSpinner fontSizeSpinner;
	    private javax.swing.JLabel fontSizeLabel;
	    private javax.swing.JCheckBox fontStyleBold;
	    private javax.swing.JCheckBox fontStyleItalic;
	    private javax.swing.JLabel fontStyleLabel;
	    
	    private javax.swing.JPanel jPanel2;
	    private javax.swing.JScrollPane jScrollPane1;
	    private javax.swing.JTabbedPane mainTab;
	    private javax.swing.JTextArea textArea;
	    private javax.swing.JPanel textFormat;
	    
	    private javax.swing.JButton cancelButton;
	    private javax.swing.JButton okButton;
	    private javax.swing.JPanel buttonPane;
	    
	    private mxCell cell;
		
	    private final transient ChangeListener defaultChangeListener = new ChangeListener() {
        	/**
        	 * Update the text area font
        	 * @param e the event parameters
        	 * @see javax.swing.event.ChangeListener#stateChanged(javax.swing.event.ChangeEvent)
        	 */
			@Override
			public void stateChanged(ChangeEvent e) {
				updateFont();
			}
		};
		
		private final transient ActionListener defaultActionListener = new ActionListener() {
        	/**
        	 * Update the text area font
        	 * @param e the event parameters
        	 * @see javax.swing.event.ChangeListener#stateChanged(javax.swing.event.ChangeEvent)
        	 */
			@Override
			public void actionPerformed(ActionEvent e) {
				updateFont();
			}
		};
	    
		/**
		 * Construct the dialog
		 * 
		 * @param f the current graph frame
		 */
		public EditFormatDialog(Frame f) {
			super(f, true);
			setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			setLocationRelativeTo(f);
			
			initComponents();
		}

		/**
		 * Initialize the dialog parameters
		 * 
		 * @param borderColor the default border color
		 * @param backgroundColor the default background color
		 * @param fontName The default font name
		 * @param fontSize the default font size
		 * @param fontStyle the current font style
		 * @param textColor the current text color
		 * @param text the current text
		 */
		public void setValues(Color borderColor, Color backgroundColor, 
				String fontName, int fontSize, int fontStyle, Color textColor, 
				String text) {
			borderColorChooser.setColor(borderColor);
			backgroundColorChooser.setColor(backgroundColor);
			textColorChooser.setColor(textColor);
			
			fontNameComboBox.getModel().setSelectedItem(fontName);
			fontSizeSpinner.getModel().setValue(fontSize);
			
			fontStyleBold.setSelected((fontStyle & mxConstants.FONT_BOLD) != 0);
			fontStyleItalic.setSelected((fontStyle & mxConstants.FONT_ITALIC) != 0);
			
			textArea.setText(text);
		}
		
		/**
		 * Set the currently selected cell
		 * @param selectedCell the current cell
		 */
		public void setCell(mxCell selectedCell) {
			cell = selectedCell;
		}
		
		/** 
		 * @return the currently selected cell
		 */
		public mxCell getCell() {
			return cell;
		}

		/**
		 * @return the current dialog
		 */
		public EditFormatDialog getDialog() {
			return this;
		}
		
		/**
		 * Initialize the dialog.
		 * 
		 * This code is UI initialization and thus doesn't pass checkstyle
		 * metrics.
		 */
		// CSOFF: JavaNCSS
		// CSOFF: LineLength
		// CSOFF: MethodLength
		private void initComponents() {
			
	        mainTab = new javax.swing.JTabbedPane();
	        borderColorChooser = new javax.swing.JColorChooser();
	        backgroundColorChooser = new javax.swing.JColorChooser();
	        textColorChooser = new javax.swing.JColorChooser();
	        textFormat = new javax.swing.JPanel();
	        jPanel2 = new javax.swing.JPanel();
	        fontSizeLabel = new javax.swing.JLabel();
	        fontSizeSpinner = new javax.swing.JSpinner();
	        fontNameLabel = new javax.swing.JLabel();
	        fontNameComboBox = new javax.swing.JComboBox();
	        fontStyleBold = new javax.swing.JCheckBox();
	        fontStyleItalic = new javax.swing.JCheckBox();
	        fontStyleLabel = new javax.swing.JLabel();
	        
	        jScrollPane1 = new javax.swing.JScrollPane();
	        textArea = new javax.swing.JTextArea();
	        
	        textArea.setColumns(TEXT_AREA_COLUMNS);
	        textArea.setRows(TEXT_AREA_ROWS);
	        textArea.setLineWrap(true);
	        textArea.setWrapStyleWord(true);
	        
	        cancelButton = new javax.swing.JButton(XcosMessages.CANCEL);
	        okButton = new javax.swing.JButton(XcosMessages.OK);
	        buttonPane = new javax.swing.JPanel();

	        mainTab.addTab(XcosMessages.BORDER_COLOR, borderColorChooser);
	        mainTab.addTab(XcosMessages.FILL_COLOR, backgroundColorChooser);
	        mainTab.addTab(XcosMessages.TEXT_COLOR, textColorChooser);

	        textFormat.setLayout(new java.awt.BorderLayout());

	        fontSizeLabel.setText(XcosMessages.FONT_SIZE);

	        fontSizeSpinner.setModel(FONTSIZE_MODEL);
	        fontSizeSpinner.addChangeListener(defaultChangeListener);

	        fontNameLabel.setText(XcosMessages.FONT_NAME);

			fontNameComboBox.setModel(new javax.swing.DefaultComboBoxModel(
					GraphicsEnvironment.getLocalGraphicsEnvironment()
							.getAvailableFontFamilyNames()));
			
	        fontNameComboBox.addActionListener(defaultActionListener);
	        
	        fontStyleLabel.setText(XcosMessages.FONT_STYLE);
	        
	        fontStyleBold.setText(XcosMessages.BOLD);
	        fontStyleBold.addChangeListener(defaultChangeListener);
	        
	        fontStyleItalic.setText(XcosMessages.ITALIC);
	        fontStyleItalic.addChangeListener(defaultChangeListener);
	        
	        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
	        jPanel2.setLayout(jPanel2Layout);
	        jPanel2Layout.setHorizontalGroup(
	            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
	            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
	                .addContainerGap()
	                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
	                    .addComponent(fontNameLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
	                    .addComponent(fontSizeLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
	                    .addComponent(fontStyleLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
	                .addGap(BORDER_SIZE)
	                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
	                    .addComponent(fontNameComboBox, 0, javax.swing.GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
	                    .addComponent(fontSizeSpinner, 0, javax.swing.GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
	                    .addGroup(jPanel2Layout.createSequentialGroup()
	                    	.addComponent(fontStyleBold)
	                    	.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
	                    	.addComponent(fontStyleItalic)))
	                .addContainerGap())
	        );
	        jPanel2Layout.setVerticalGroup(
	            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
	            .addGroup(jPanel2Layout.createSequentialGroup()
	                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
	                    .addComponent(fontSizeLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
	                    .addComponent(fontSizeSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
	                .addGap(BORDER_SIZE)
	                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
	                    .addComponent(fontNameLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
	                    .addComponent(fontNameComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
	                .addGap(BORDER_SIZE)
	                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
	                    .addComponent(fontStyleLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
	                    .addComponent(fontStyleBold, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
	                    .addComponent(fontStyleItalic, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
	                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
	        );

	        textFormat.add(jPanel2, java.awt.BorderLayout.PAGE_END);

	        jScrollPane1.setViewportView(textArea);
	        jScrollPane1.setBackground(Color.WHITE);

	        textFormat.add(jScrollPane1, java.awt.BorderLayout.CENTER);

	        mainTab.addTab(XcosMessages.TEXT_SETTINGS, textFormat);

	        mainTab.addChangeListener(defaultChangeListener);
	        
			cancelButton.addActionListener(new ActionListener() {
				/**
				 * On cancel close the window
				 * 
				 * @param e
				 *            the current event parameter
				 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
				 */
				@Override
				public void actionPerformed(ActionEvent e) {
					getDialog().dispose();
				}
			});

			okButton.addActionListener(new ActionListener() {
				/**
				 * On OK, set the current parameter on the cell
				 * 
				 * @param e
				 *            the current parameters
				 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
				 */
				@Override
				public void actionPerformed(ActionEvent e) {
					EditBlockFormatAction.updateFromDialog(getDialog(),
							borderColorChooser.getColor(),
							backgroundColorChooser.getColor(),
							(String) fontNameComboBox.getSelectedItem(),
							(Integer) fontSizeSpinner.getValue(), 
							textColorChooser.getColor(),
							textArea.getText());
					getDialog().dispose();
				}
			});
			
	        getRootPane().setDefaultButton(okButton);
	        
	        buttonPane.setLayout(new javax.swing.BoxLayout(buttonPane, javax.swing.BoxLayout.LINE_AXIS));
			buttonPane.setBorder(javax.swing.BorderFactory.createEmptyBorder(
					BORDER_SIZE, BORDER_SIZE, BORDER_SIZE, BORDER_SIZE));
	        buttonPane.add(javax.swing.Box.createHorizontalGlue());
	        buttonPane.add(cancelButton);
			buttonPane.add(javax.swing.Box.createRigidArea(new Dimension(
					BORDER_SIZE, 0)));
	        buttonPane.add(okButton);

	        java.awt.Container contentPane = getContentPane();
	        contentPane.add(mainTab, java.awt.BorderLayout.CENTER);
	        contentPane.add(buttonPane, java.awt.BorderLayout.PAGE_END);
	        
	        pack();
		}
		// CSON: JavaNCSS
		// CSON: LineLength
		// CSON: MethodLength

		/**
		 * Update the text area from the font
		 */
		protected void updateFont() {
			int style = 0;
			if (fontStyleBold.isSelected()) {
				style |= Font.BOLD;
			}
			if (fontStyleItalic.isSelected()) {
				style |= Font.ITALIC;
			}
			
			Font f = new Font((String) fontNameComboBox.getSelectedItem(), style,
					(Integer) fontSizeSpinner.getValue());
			textArea.setFont(f);
			textArea.setBackground(backgroundColorChooser.getColor());
			textArea.setForeground(textColorChooser.getColor());
			
			// Repaint the parent scroll pane to force a full redraw call. 
			jScrollPane1.repaint();
		}
	}
	// CSON: ClassDataAbstractionCoupling
	// CSON: ClassFanOutComplexity
}
