/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Frame;
import java.awt.GraphicsEnvironment;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.io.File;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;

import javax.swing.BorderFactory;
import javax.swing.SpinnerModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;

/**
 * Customize the block representation.
 */
@SuppressWarnings(value = { "serial" })
public final class EditFormatAction extends DefaultAction {
    /**
     * Name of the action
     */
    public static final String NAME = XcosMessages.EDIT + XcosMessages.DOTS;
    /**
     * Icon of the action
     */
    public static final String SMALL_ICON = "select-by-color";
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
    private static final Color DEFAULT_BORDERCOLOR = Color.BLACK;
    /**
     * The default color used on non initialized filled color.
     */
    private static final Color DEFAULT_FILLCOLOR = Color.WHITE;

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            the current graph
     */
    public EditFormatAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Menu added to the menubar
     *
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, EditFormatAction.class);
    }

    /**
     * Create a new dialog for editing this cell
     *
     * @param c
     *            the current component
     * @param name
     *            the window name
     * @param selectedCell
     *            the selected cell
     * @param graph
     *            the current graph
     */
    public static void showDialog(ScilabComponent c, String name, mxCell selectedCell, XcosDiagram graph) {
        /*
         * Looking for the parent window
         */
        final Frame window = javax.swing.JOptionPane.getFrameForComponent(c);

        /*
         * Create and show the dialog
         */
        EditFormatDialog dialog = createDialog(selectedCell, graph, window);
        dialog.setName(name);
        dialog.setVisible(true);
    }

    /**
     * Create the dialog and set the default values
     *
     * @param cell
     *            the current selected cell
     * @param graph
     *            the current graph
     * @param window
     *            the current windows
     * @return the instantiated dialog
     */
    // CSOFF: NPathComplexity
    // CSOFF: JavaNCSS
    private static EditFormatDialog createDialog(final mxCell cell, final XcosDiagram graph, final Frame window) {
        String working;
        Color border;
        Color fill;
        String font;
        int fontSize;
        int fontStyle;
        Color textColor;
        String text;
        String image = null;

        final mxGraphModel model = (mxGraphModel) graph.getModel();
        final StyleMap cellStyle = new StyleMap(cell.getStyle());

        final mxCell identifier;
        final StyleMap identifierStyle;
        if (cell instanceof TextBlock) {
            identifier = cell;
            identifierStyle = cellStyle;
        } else {
            identifier = graph.getOrCreateCellIdentifier(cell);
            identifierStyle = new StyleMap(identifier.getStyle());
        }

        /*
         * Stroke color
         */
        working = cellStyle.get(mxConstants.STYLE_STROKECOLOR);
        if (working == null) {
            border = DEFAULT_BORDERCOLOR;
        } else {
            border = mxUtils.parseColor(working);
        }

        /*
         * Fill color
         */
        working = cellStyle.get(mxConstants.STYLE_FILLCOLOR);
        if (working == null) {
            fill = DEFAULT_FILLCOLOR;
        } else {
            fill = mxUtils.parseColor(working);
        }

        /*
         * Font
         */
        working = identifierStyle.get(mxConstants.STYLE_FONTFAMILY);
        if (working == null) {
            font = mxConstants.DEFAULT_FONTFAMILY;
        } else {
            font = working;
        }

        /*
         * Font size
         */
        working = identifierStyle.get(mxConstants.STYLE_FONTSIZE);
        if (working == null) {
            fontSize = mxConstants.DEFAULT_FONTSIZE;
        } else {
            fontSize = Integer.parseInt(working);
        }

        /*
         * Font modifier
         */
        working = identifierStyle.get(mxConstants.STYLE_FONTSTYLE);
        if (working == null) {
            fontStyle = 0;
        } else {
            fontStyle = Integer.parseInt(working);
        }

        /*
         * Font color
         */
        working = identifierStyle.get(mxConstants.STYLE_FONTCOLOR);
        if (working == null) {
            textColor = Color.BLACK;
        } else {
            textColor = mxUtils.parseColor(working);
        }

        /*
         * Image
         */
        working = cellStyle.get(mxConstants.STYLE_IMAGE);
        if (working != null) {
            image = working;
        }

        /*
         * Text
         */
        final Object current = model.getValue(identifier);
        if (current == null) {
            text = "";
        } else {
            text = mxUtils.getBodyMarkup(current.toString(), false);
        }

        EditFormatDialog dialog = new EditFormatDialog(window);
        dialog.setValues(border, fill, font, fontSize, fontStyle, textColor, text, image);
        dialog.setGraph(graph);
        dialog.setCell(cell);
        return dialog;
    }

    // CSON: JavaNCSS
    // CSON: NPathComplexity

    /**
     * Update the cell value from the dialog ones.
     *
     * @param dialog
     *            the current dialog
     * @param borderColor
     *            the selected border color
     * @param backgroundColor
     *            the selected background color
     * @param fontName
     *            the selected font name
     * @param fontSize
     *            the selected font size
     * @param isBold
     *            is the text bold ?
     * @param isItalic
     *            is the text italic ?
     * @param textColor
     *            the selected color
     * @param text
     *            the typed text
     * @param image
     *            the image URL
     */
    // CSOFF: NPathComplexity
    private static void updateFromDialog(EditFormatDialog dialog, Color borderColor, Color backgroundColor, String fontName, int fontSize, Color textColor,
                                         boolean isBold, boolean isItalic, String text, String image) {
        final XcosDiagram graph = dialog.getGraph();
        final mxGraphModel model = (mxGraphModel) graph.getModel();

        final mxCell cell = dialog.getCell();
        final StyleMap cellStyle = new StyleMap(cell.getStyle());

        final mxCell identifier;
        final StyleMap identifierStyle;
        if (cell instanceof TextBlock) {
            identifier = cell;
            identifierStyle = cellStyle;
        } else {
            identifier = graph.getOrCreateCellIdentifier(cell);
            identifierStyle = new StyleMap(identifier.getStyle());
        }

        if (!borderColor.equals(DEFAULT_BORDERCOLOR)) {
            cellStyle.put(mxConstants.STYLE_STROKECOLOR, mxUtils.hexString(borderColor));
        } else {
            cellStyle.remove(mxConstants.STYLE_STROKECOLOR);
        }

        if (!backgroundColor.equals(DEFAULT_FILLCOLOR)) {
            cellStyle.put(mxConstants.STYLE_FILLCOLOR, mxUtils.hexString(backgroundColor));
        } else {
            cellStyle.remove(mxConstants.STYLE_FILLCOLOR);
        }

        if (!fontName.equals(mxConstants.DEFAULT_FONTFAMILY)) {
            identifierStyle.put(mxConstants.STYLE_FONTFAMILY, fontName);
        } else {
            identifierStyle.remove(mxConstants.STYLE_FONTFAMILY);
        }

        applyFontStyle(isBold, isItalic, identifierStyle);

        if (fontSize != mxConstants.DEFAULT_FONTSIZE) {
            identifierStyle.put(mxConstants.STYLE_FONTSIZE, Integer.toString(fontSize));
        } else {
            identifierStyle.remove(mxConstants.DEFAULT_FONTSIZE);
        }

        if (!textColor.equals(DEFAULT_BORDERCOLOR)) {
            identifierStyle.put(mxConstants.STYLE_FONTCOLOR, mxUtils.hexString(textColor));
        } else {
            identifierStyle.remove(mxConstants.STYLE_FONTCOLOR);
        }

        applyImage(image, cellStyle);

        model.setStyle(cell, cellStyle.toString());
        if (cell != identifier) {
            model.setStyle(identifier, identifierStyle.toString());
        }

        graph.cellLabelChanged(identifier, text, false);
        graph.fireEvent(new mxEventObject(mxEvent.LABEL_CHANGED, "cell", identifier, "value", text, "parent", cell));

        /*
         * Should also update diagram title
         */
        if (cell instanceof SuperBlock) {
            graph.cellLabelChanged(cell, text, false);
        }
    }

    // CSON: NPathComplexity

    /**
     * Reset to the default values
     *
     * @param dialog
     *            the dialog to reset
     */
    private static void reset(EditFormatDialog dialog) {
        final XcosDiagram graph = dialog.getGraph();
        final mxGraphModel model = (mxGraphModel) graph.getModel();

        final mxCell cell = dialog.getCell();
        final StyleMap cellStyle = new StyleMap(cell.getStyle());

        final mxCell identifier;
        if (cell instanceof TextBlock) {
            identifier = cell;
        } else {
            identifier = graph.getCellIdentifier(cell);
            if (identifier != null) {
                model.remove(identifier);
            }
        }

        cellStyle.clear();

        dialog.setValues(DEFAULT_BORDERCOLOR, DEFAULT_FILLCOLOR, mxConstants.DEFAULT_FONTFAMILY, mxConstants.DEFAULT_FONTSIZE, 0, DEFAULT_BORDERCOLOR, "", null);

        dialog.updateFont();
    }

    /**
     * Apply image to the identifier style
     *
     * @param image
     *            the image path
     * @param cellStyle
     *            the cell style
     */
    private static void applyImage(String image, final StyleMap cellStyle) {
        if (image != null && !image.isEmpty()) {
            String path;
            try {
                URL url = new URL(image);
                path = url.toExternalForm();
            } catch (MalformedURLException e) {
                path = image;
            }

            cellStyle.put(mxConstants.STYLE_IMAGE, path);
        } else {
            cellStyle.remove(mxConstants.STYLE_IMAGE);
        }
    }

    /**
     * Apply font style to the identifier style
     *
     * @param isBold
     *            true if the font is bold
     * @param isItalic
     *            true is the font is italic
     * @param identifierStyle
     *            the identifier style
     */
    private static void applyFontStyle(boolean isBold, boolean isItalic, final StyleMap identifierStyle) {
        int fontStyle = 0;
        if (isBold) {
            fontStyle |= mxConstants.FONT_BOLD;
        }
        if (isItalic) {
            fontStyle |= mxConstants.FONT_ITALIC;
        }

        if (fontStyle != 0) {
            identifierStyle.put(mxConstants.STYLE_FONTSTYLE, Integer.toString(fontStyle));
        } else {
            identifierStyle.remove(mxConstants.STYLE_FONTSTYLE);
        }
    }

    /**
     * Open a dialog to set the parameters
     *
     * @param e
     *            the current event
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        XcosDiagram graph = (XcosDiagram) getGraph(null);
        final Object selectedCell = graph.getSelectionCell();

        if (selectedCell == null) {
            return;
        }

        EditFormatAction.showDialog((ScilabComponent) graph.getAsComponent(), NAME, (mxCell) selectedCell, graph);

        graph.getView().clear(selectedCell, true, true);
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
    private static final class EditFormatDialog extends javax.swing.JDialog {
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
        private javax.swing.JPanel backgroundPane;

        private javax.swing.JLabel imagePathLabel;
        private javax.swing.JTextField imagePath;
        private javax.swing.JButton imageFileChooserBtn;

        private javax.swing.JPanel jPanel2;
        private javax.swing.JScrollPane jScrollPane1;
        private javax.swing.JTabbedPane mainTab;
        private javax.swing.JTextArea textArea;
        private javax.swing.JPanel textFormat;

        private javax.swing.JButton cancelButton;
        private javax.swing.JButton okButton;
        private javax.swing.JButton resetButton;
        private javax.swing.JPanel buttonPane;

        private XcosDiagram graph;
        private mxCell cell;

        private final transient ChangeListener defaultChangeListener = new ChangeListener() {
            /**
             * Update the text area font
             *
             * @param e
             *            the event parameters
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
             *
             * @param e
             *            the event parameters
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
         * @param f
         *            the current graph frame
         */
        public EditFormatDialog(Frame f) {
            super(f, true);
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
            setLocationRelativeTo(f);
            ScilabSwingUtilities.closeOnEscape(this);

            initComponents();
        }

        /**
         * Initialize the dialog parameters
         *
         * @param borderColor
         *            the default border color
         * @param backgroundColor
         *            the default background color
         * @param fontName
         *            The default font name
         * @param fontSize
         *            the default font size
         * @param fontStyle
         *            the current font style
         * @param textColor
         *            the current text color
         * @param text
         *            the current text
         * @param image
         *            the current URL of the image (may be null, absolute or
         *            relative)
         */
        public void setValues(Color borderColor, Color backgroundColor, String fontName, int fontSize, int fontStyle, Color textColor, String text, String image) {
            borderColorChooser.setColor(borderColor);
            backgroundColorChooser.setColor(backgroundColor);
            textColorChooser.setColor(textColor);

            fontNameComboBox.getModel().setSelectedItem(fontName);
            fontSizeSpinner.getModel().setValue(fontSize);

            fontStyleBold.setSelected((fontStyle & mxConstants.FONT_BOLD) != 0);
            fontStyleItalic.setSelected((fontStyle & mxConstants.FONT_ITALIC) != 0);

            textArea.setText(text);
            if (image != null) {
                imagePath.setText(image);
            }
        }

        /**
         * @param graph
         *            the current graph to set
         */
        public void setGraph(XcosDiagram graph) {
            this.graph = graph;
        }

        /**
         * @return the current graph
         */
        public XcosDiagram getGraph() {
            return graph;
        }

        /**
         * Set the currently selected cell
         *
         * @param selectedCell
         *            the current cell
         */
        public void setCell(mxCell selectedCell) {
            cell = selectedCell;

            // enable/disable the fill color pane
            if (selectedCell.isVertex()) {
                mainTab.addTab(XcosMessages.FILL_COLOR, backgroundPane);
            } else {
                mainTab.remove(backgroundPane);
            }

            pack();
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
            imageFileChooserBtn = new javax.swing.JButton(XcosMessages.DOTS);
            imagePathLabel = new javax.swing.JLabel();
            imagePath = new javax.swing.JTextField(TEXT_AREA_COLUMNS);
            backgroundPane = new javax.swing.JPanel();

            jScrollPane1 = new javax.swing.JScrollPane();
            textArea = new javax.swing.JTextArea();

            textArea.setColumns(TEXT_AREA_COLUMNS);
            textArea.setRows(TEXT_AREA_ROWS);
            textArea.setLineWrap(true);
            textArea.setWrapStyleWord(true);

            cancelButton = new javax.swing.JButton(XcosMessages.CANCEL);
            okButton = new javax.swing.JButton(XcosMessages.OK);
            resetButton = new javax.swing.JButton(XcosMessages.RESET);
            buttonPane = new javax.swing.JPanel();

            backgroundPane.setLayout(new java.awt.BorderLayout());
            textFormat.setLayout(new java.awt.BorderLayout());

            fontSizeLabel.setText(XcosMessages.FONT_SIZE);

            fontSizeSpinner.setModel(FONTSIZE_MODEL);
            fontSizeSpinner.addChangeListener(defaultChangeListener);

            fontNameLabel.setText(XcosMessages.FONT_NAME);

            fontNameComboBox.setModel(new javax.swing.DefaultComboBoxModel(GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames()));

            fontNameComboBox.addActionListener(defaultActionListener);

            fontStyleLabel.setText(XcosMessages.FONT_STYLE);

            fontStyleBold.setText(XcosMessages.BOLD);
            fontStyleBold.addChangeListener(defaultChangeListener);

            fontStyleItalic.setText(XcosMessages.ITALIC);
            fontStyleItalic.addChangeListener(defaultChangeListener);

            imagePathLabel.setText(XcosMessages.IMAGE_PATH);

            javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
            jPanel2.setLayout(jPanel2Layout);
            jPanel2Layout.setHorizontalGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                             .addGroup(
                                                 javax.swing.GroupLayout.Alignment.TRAILING,
                                                 jPanel2Layout
                                                 .createSequentialGroup()
                                                 .addContainerGap()
                                                 .addGroup(
                                                         jPanel2Layout
                                                         .createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                                         .addComponent(fontNameLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE,
                                                                 Short.MAX_VALUE)
                                                         .addComponent(fontSizeLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE,
                                                                 Short.MAX_VALUE)
                                                         .addComponent(fontStyleLabel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE,
                                                                 Short.MAX_VALUE))
                                                 .addGap(BORDER_SIZE)
                                                 .addGroup(
                                                         jPanel2Layout
                                                         .createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                                         .addComponent(fontNameComboBox, 0, javax.swing.GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                                                         .addComponent(fontSizeSpinner, 0, javax.swing.GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                                                         .addGroup(
                                                                 jPanel2Layout.createSequentialGroup().addComponent(fontStyleBold)
                                                                 .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                 .addComponent(fontStyleItalic))).addContainerGap()));
            jPanel2Layout.setVerticalGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING).addGroup(
                                               jPanel2Layout
                                               .createSequentialGroup()
                                               .addGroup(
                                                   jPanel2Layout
                                                   .createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                                   .addComponent(fontSizeLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE,
                                                           javax.swing.GroupLayout.PREFERRED_SIZE)
                                                   .addComponent(fontSizeSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE,
                                                           javax.swing.GroupLayout.PREFERRED_SIZE))
                                               .addGap(BORDER_SIZE)
                                               .addGroup(
                                                   jPanel2Layout
                                                   .createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                                   .addComponent(fontNameLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE,
                                                           javax.swing.GroupLayout.PREFERRED_SIZE)
                                                   .addComponent(fontNameComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE,
                                                           javax.swing.GroupLayout.PREFERRED_SIZE))
                                               .addGap(BORDER_SIZE)
                                               .addGroup(
                                                   jPanel2Layout
                                                   .createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                                   .addComponent(fontStyleLabel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE,
                                                           javax.swing.GroupLayout.PREFERRED_SIZE)
                                                   .addComponent(fontStyleBold, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE,
                                                           javax.swing.GroupLayout.PREFERRED_SIZE)
                                                   .addComponent(fontStyleItalic, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE,
                                                           javax.swing.GroupLayout.PREFERRED_SIZE))
                                               .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)));

            textFormat.add(jPanel2, java.awt.BorderLayout.PAGE_END);

            jScrollPane1.setViewportView(textArea);
            jScrollPane1.setBackground(Color.WHITE);

            textFormat.add(jScrollPane1, java.awt.BorderLayout.CENTER);

            backgroundPane.add(backgroundColorChooser, java.awt.BorderLayout.CENTER);
            javax.swing.JPanel filePane = new javax.swing.JPanel();
            filePane.setBorder(BorderFactory.createEtchedBorder());
            filePane.add(imagePathLabel);
            filePane.add(imagePath);
            filePane.add(imageFileChooserBtn);
            backgroundPane.add(filePane, java.awt.BorderLayout.SOUTH);

            mainTab.addTab(XcosMessages.TEXT_SETTINGS, textFormat);
            mainTab.addTab(XcosMessages.BORDER_COLOR, borderColorChooser);
            mainTab.addTab(XcosMessages.TEXT_COLOR, textColorChooser);
            // backgroundColorChooser is added on the setCell method only if the
            // cell is a vertex
            // mainTab.addTab(XcosMessages.FILL_COLOR, backgroundColorChooser);

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
                    graph.getModel().beginUpdate();
                    EditFormatAction.updateFromDialog(getDialog(), borderColorChooser.getColor(), backgroundColorChooser.getColor(),
                                                      (String) fontNameComboBox.getSelectedItem(), (Integer) fontSizeSpinner.getValue(), textColorChooser.getColor(),
                                                      fontStyleBold.isSelected(), fontStyleItalic.isSelected(), textArea.getText(), imagePath.getText());
                    graph.getModel().endUpdate();
                    getDialog().dispose();
                }
            });

            resetButton.addActionListener(new ActionListener() {

                @Override
                public void actionPerformed(ActionEvent e) {
                    graph.getModel().beginUpdate();
                    EditFormatAction.reset(getDialog());
                    graph.getModel().endUpdate();
                }
            });

            imageFileChooserBtn.addActionListener(new ActionListener() {
                /**
                 * On file chooser open the file chooser with image filter.
                 *
                 * @param e
                 *            the event
                 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
                 */
                @Override
                public void actionPerformed(ActionEvent e) {
                    javax.swing.JFileChooser chooser = new javax.swing.JFileChooser();
                    javax.swing.filechooser.FileNameExtensionFilter filter = new javax.swing.filechooser.FileNameExtensionFilter("Images", "jpg", "png", "svg",
                            "gif");
                    chooser.setFileFilter(filter);

                    final String current = imagePath.getText();
                    final File savedFile = getGraph().getSavedFile();
                    if (current != null && !current.isEmpty()) {
                        try {
                            // try to handle an absolute URL
                            final URI uri = new URI(current);
                            chooser.setSelectedFile(new File(uri));
                        } catch (URISyntaxException e1) {
                            // this is a relative path
                            if (savedFile != null) {
                                final File parent = savedFile.getParentFile();
                                chooser.setSelectedFile(new File(parent, current));
                            }
                        }
                    } else {
                        if (savedFile != null) {
                            chooser.setCurrentDirectory(savedFile.getParentFile());
                        }
                    }

                    int returnVal = chooser.showOpenDialog(mainTab);
                    if (returnVal == javax.swing.JFileChooser.APPROVE_OPTION) {
                        if (savedFile != null) {
                            final String relativeChild = savedFile.getParentFile().toURI().relativize(chooser.getSelectedFile().toURI()).toASCIIString();
                            imagePath.setText(relativeChild);
                        } else {
                            final String uri = chooser.getSelectedFile().toURI().toASCIIString();
                            imagePath.setText(uri);
                        }
                    }
                }
            });

            getRootPane().setDefaultButton(okButton);

            buttonPane.setLayout(new javax.swing.BoxLayout(buttonPane, javax.swing.BoxLayout.LINE_AXIS));
            buttonPane.setBorder(javax.swing.BorderFactory.createEmptyBorder(BORDER_SIZE, BORDER_SIZE, BORDER_SIZE, BORDER_SIZE));
            buttonPane.add(javax.swing.Box.createHorizontalGlue());
            buttonPane.add(okButton);
            buttonPane.add(javax.swing.Box.createRigidArea(new Dimension(BORDER_SIZE, 0)));
            buttonPane.add(cancelButton);
            buttonPane.add(javax.swing.Box.createRigidArea(new Dimension(BORDER_SIZE, 0)));
            buttonPane.add(javax.swing.Box.createRigidArea(new Dimension(BORDER_SIZE, 0)));
            buttonPane.add(resetButton);

            java.awt.Container contentPane = getContentPane();
            contentPane.add(mainTab, java.awt.BorderLayout.CENTER);
            contentPane.add(buttonPane, java.awt.BorderLayout.PAGE_END);
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

            Font f = new Font((String) fontNameComboBox.getSelectedItem(), style, (Integer) fontSizeSpinner.getValue());
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
