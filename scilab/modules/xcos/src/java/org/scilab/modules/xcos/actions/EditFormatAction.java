/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2017-2018 - ESI Group - Clement DAVID
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
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;
import java.util.List;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosGraphModel;

/**
 * Customize the block representation.
 */
public final class EditFormatAction extends DefaultAction {
    private static final long serialVersionUID = 1L;
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
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMaskEx();

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
    public static void showDialog(ScilabComponent c, String name, XcosCell selectedCell, XcosDiagram graph) {
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
    private static EditFormatDialog createDialog(XcosCell cell, final XcosDiagram graph, final Frame window) {
        String working;
        Color border;
        Color fill;
        String font;
        int fontSize;
        int fontStyle;
        Color textColor;
        String name;
        String description;
        String text;
        String image = null;

        final mxGraphModel model = (mxGraphModel) graph.getModel();
        StyleMap cellStyle = new StyleMap(cell.getStyle());

        XcosCell identifier = null;
        StyleMap identifierStyle = new StyleMap("");
        
        if (cell instanceof TextBlock) {
            identifier = cell;
            identifierStyle = cellStyle;
        } else if (cell.getKind() == Kind.ANNOTATION) {
            identifier = cell;
            identifierStyle = cellStyle;
            cell = (XcosCell) cell.getParent();
            cellStyle = new StyleMap(cell.getStyle());
        } else {
            identifier = graph.getCellIdentifier(cell);
            if (identifier != null) {
                identifierStyle.putAll(identifier.getStyle());
            }
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

        JavaController controller = new JavaController();
        String[] v = { "" };

        /*
         * Name
         */
        controller.getObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.NAME, v);
        name = v[0];

        /*
         * Description
         */
        controller.getObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.DESCRIPTION, v);
        description = v[0];

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
        dialog.setValues(border, fill, font, fontSize, fontStyle, textColor, name, description, text, image);
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
     * @param oneliner
     *            the one-line description text
     * @param text
     *            the typed text
     * @param image
     *            the image URL
     */
    // CSOFF: NPathComplexity
    private static void updateFromDialog(EditFormatDialog dialog, Color borderColor, Color backgroundColor, String fontName, int fontSize, Color textColor,
                                         boolean isBold, boolean isItalic, String name, String description, String text, String image) {
        final XcosDiagram graph = dialog.getGraph();
        final mxGraphModel model = (mxGraphModel) graph.getModel();

        final XcosCell cell = dialog.getCell();
        final StyleMap cellStyle = new StyleMap(cell.getStyle());

        final XcosCell identifier;
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
            identifierStyle.remove(mxConstants.STYLE_FONTSIZE);
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

        // the name is the block identifier (model.label, SSP, Modelica)
        name = XcosCell.toValidCIdentifier(name);

        //
        // Update the cell value and cell identifier value (related annotation)
        //
        JavaController controller = new JavaController();

        graph.cellLabelChanged(cell, name, false);
        graph.fireEvent(new mxEventObject(mxEvent.LABEL_CHANGED, "cell", cell, "value", text, "parent", cell.getParent()));

        ((XcosGraphModel) (graph.getModel())).setProperty(cell, ObjectProperties.DESCRIPTION, new String[] { description });

        graph.cellLabelChanged(identifier, text, false);
        graph.fireEvent(new mxEventObject(mxEvent.LABEL_CHANGED, "cell", identifier, "value", text, "parent", cell));

        //
        // When the block is an I/O block do some propagation:
        //  * The outter port is named after the corresponding port
        //  * The inner I/O block port is also named (The I/O block keeps its number)
        if (cell instanceof ContextUpdate) {

            VectorOfInt ipar = new VectorOfInt();
            controller.getObjectProperty(((ContextUpdate) cell).getUID(), Kind.BLOCK, ObjectProperties.IPAR, ipar);
            int portNumber = ipar.size() > 0 ? ipar.get(0) : 1;

            XcosCell parent = (XcosCell) graph.getDefaultParent();
            XcosDiagram parentGraph = Xcos.findParent(controller, parent.getUID(), parent.getKind());
            if (parentGraph != null) {
                String[] jgraphxID = {""};
                controller.getObjectProperty(parent.getUID(), parent.getKind(), ObjectProperties.UID, jgraphxID);

                XcosGraphModel parentModel = (XcosGraphModel) parentGraph.getModel();
                Object superBlock = parentModel.getCell(jgraphxID[0]);
                if (superBlock instanceof SuperBlock) {
                    // reason: cell instanceof ContextUpdate checked above (line 422)
                    Class<? extends ContextUpdate> cellClass = cell.getClass().asSubclass(ContextUpdate.class);
                    List<mxICell> ports = ContextUpdate.IOBlocks.getPorts((SuperBlock) superBlock, cellClass);

                    if (ports.size() >= portNumber) {
                        mxICell port = ports.get(portNumber - 1);
                        parentGraph.cellLabelChanged(port, name, false);
                        parentGraph.fireEvent(new mxEventObject(mxEvent.LABEL_CHANGED, "cell", port, "value", text, "parent", superBlock));
                    }
                }

            }
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

        final XcosCell cell = dialog.getCell();
        final StyleMap cellStyle = new StyleMap(cell.getStyle());

        final XcosCell identifier;
        if (cell instanceof TextBlock) {
            identifier = cell;
        } else {
            identifier = graph.getCellIdentifier(cell);
            if (identifier != null) {
                model.remove(identifier);
            }
        }

        cellStyle.clear();

        dialog.setValues(DEFAULT_BORDERCOLOR, DEFAULT_FILLCOLOR, mxConstants.DEFAULT_FONTFAMILY, mxConstants.DEFAULT_FONTSIZE, 0, DEFAULT_BORDERCOLOR, "", "", "", null);

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
                URL url;
                try {
                    url = URI.create(image).toURL();
                } catch (IllegalArgumentException e) {
                    url = new File(image).toURI().toURL();
                }
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
        actionPerformed();
    }
    
    public void actionPerformed() {
        XcosDiagram graph = (XcosDiagram) getGraph(null);
        final Object selectedCell = graph.getSelectionCell();

        if (selectedCell == null) {
            return;
        }

        EditFormatAction.showDialog((ScilabComponent) graph.getAsComponent(), NAME, (XcosCell) selectedCell, graph);

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
        private static final long serialVersionUID = 1L;
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
        private javax.swing.JComboBox<String> fontNameComboBox;
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

        private javax.swing.JScrollPane jScrollPane1;
        private javax.swing.JTabbedPane mainTab;
        private javax.swing.JLabel nameLabel;
        private javax.swing.JTextField nameTextField;
        private javax.swing.JLabel descriptionLabel;
        private javax.swing.JTextField descriptionTextField;
        private javax.swing.JTextPane textArea;
        private javax.swing.JPanel textFormat;

        private javax.swing.JButton cancelButton;
        private javax.swing.JButton okButton;
        private javax.swing.JButton resetButton;
        private javax.swing.JPanel buttonPane;

        private transient XcosDiagram graph;
        private XcosCell cell;

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
        public void setValues(Color borderColor, Color backgroundColor, String fontName, int fontSize, int fontStyle, Color textColor, String name, String description, String text, String image) {
            borderColorChooser.setColor(borderColor);
            backgroundColorChooser.setColor(backgroundColor);
            textColorChooser.setColor(textColor);

            fontNameComboBox.getModel().setSelectedItem(fontName);
            fontSizeSpinner.getModel().setValue(fontSize);

            fontStyleBold.setSelected((fontStyle & mxConstants.FONT_BOLD) != 0);
            fontStyleItalic.setSelected((fontStyle & mxConstants.FONT_ITALIC) != 0);

            nameTextField.setText(name);
            descriptionTextField.setText(description);
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
        public void setCell(XcosCell selectedCell) {
            cell = selectedCell;

            // enable/disable some tabs depending on the cell type
            switch(cell.getKind())
            {
                case BLOCK:
                    mainTab.addTab(XcosMessages.TEXT_BLOCK_SETTINGS, textFormat);
                    mainTab.addTab(XcosMessages.BORDER_BLOCK_COLOR, borderColorChooser);
                    mainTab.addTab(XcosMessages.TEXT_COLOR, textColorChooser);
                    mainTab.addTab(XcosMessages.FILL_BLOCK_COLOR, backgroundPane);
                    break;
                case DIAGRAM:
                    // TODO: use it to write "description"
                    break;
                case LINK:
                    mainTab.addTab(XcosMessages.TEXT_LINK_SETTINGS, textFormat);
                    mainTab.addTab(XcosMessages.BORDER_LINK_COLOR, borderColorChooser);
                    mainTab.addTab(XcosMessages.TEXT_COLOR, textColorChooser);
                    break;
                case ANNOTATION:
                    mainTab.addTab(XcosMessages.TEXT_BLOCK_SETTINGS, textFormat);
                    mainTab.addTab(XcosMessages.BORDER_BLOCK_COLOR, borderColorChooser);
                    mainTab.addTab(XcosMessages.TEXT_COLOR, textColorChooser);
                    mainTab.addTab(XcosMessages.FILL_BLOCK_COLOR, backgroundPane);
                break;
                case PORT:
                    // nothing it editable.
                break;
            }

            pack();
        }

        /**
         * @return the currently selected cell
         */
        public XcosCell getCell() {
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
            fontSizeLabel = new javax.swing.JLabel();
            fontSizeSpinner = new javax.swing.JSpinner();
            fontNameLabel = new javax.swing.JLabel();
            fontNameComboBox = new javax.swing.JComboBox<>();
            fontStyleBold = new javax.swing.JCheckBox();
            fontStyleItalic = new javax.swing.JCheckBox();
            fontStyleLabel = new javax.swing.JLabel();
            imageFileChooserBtn = new javax.swing.JButton(XcosMessages.DOTS);
            imagePathLabel = new javax.swing.JLabel();
            imagePath = new javax.swing.JTextField(TEXT_AREA_COLUMNS);
            backgroundPane = new javax.swing.JPanel();

            nameLabel = new javax.swing.JLabel();
            nameLabel.setText(XcosMessages.NAME_LABEL);
            nameTextField = new javax.swing.JTextField();
            nameTextField.setToolTipText(XcosMessages.VAR_NAME_TOOLTIP);

            descriptionLabel = new javax.swing.JLabel();
            descriptionLabel.setText(XcosMessages.DESCRIPTION_LABEL);
            descriptionTextField = new javax.swing.JTextField();
            descriptionTextField.setToolTipText(XcosMessages.ONELINE_DESCRIPTION_TOOLTIP);

            jScrollPane1 = new javax.swing.JScrollPane();
            textArea = new javax.swing.JTextPane();
            textArea.setToolTipText(XcosMessages.MULTILINE_DESCRIPTION_TOOLTIP);

            textArea.setContentType("text/html");

            cancelButton = new javax.swing.JButton(XcosMessages.CANCEL);
            okButton = new javax.swing.JButton(XcosMessages.OK);
            okButton.setMnemonic(KeyEvent.VK_ENTER);
            resetButton = new javax.swing.JButton(XcosMessages.RESET);
            buttonPane = new javax.swing.JPanel();

            backgroundPane.setLayout(new java.awt.BorderLayout());
            textFormat.setLayout(new java.awt.BorderLayout());

            fontSizeLabel.setText(XcosMessages.FONT_SIZE);

            fontSizeSpinner.setModel(FONTSIZE_MODEL);
            fontSizeSpinner.addChangeListener(defaultChangeListener);

            fontNameLabel.setText(XcosMessages.FONT_NAME);

            fontNameComboBox.setModel(new javax.swing.DefaultComboBoxModel<>(GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames()));

            fontNameComboBox.addActionListener(defaultActionListener);

            fontStyleLabel.setText(XcosMessages.FONT_STYLE);

            fontStyleBold.setText(XcosMessages.BOLD);
            fontStyleBold.addChangeListener(defaultChangeListener);

            fontStyleItalic.setText(XcosMessages.ITALIC);
            fontStyleItalic.addChangeListener(defaultChangeListener);

            imagePathLabel.setText(XcosMessages.IMAGE_PATH);

            jScrollPane1.setViewportView(textArea);
            jScrollPane1.setBackground(Color.WHITE);

            javax.swing.GroupLayout textFormatLayout = new javax.swing.GroupLayout(textFormat);
            textFormat.setLayout(textFormatLayout);
            textFormatLayout.setHorizontalGroup(
                textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(textFormatLayout.createSequentialGroup()
                    .addContainerGap()
                    .addGroup(textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jScrollPane1)
                        .addGroup(textFormatLayout.createSequentialGroup()
                            .addGroup(textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(nameLabel)
                                .addComponent(descriptionLabel))
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                            .addGroup(textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(descriptionTextField)
                                .addComponent(nameTextField)))
                        .addGroup(textFormatLayout.createSequentialGroup()
                            .addComponent(fontStyleLabel)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(fontStyleBold)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(fontStyleItalic)
                            .addGap(0, 0, Short.MAX_VALUE))
                        .addGroup(textFormatLayout.createSequentialGroup()
                            .addComponent(fontSizeLabel)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(fontSizeSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                            .addComponent(fontNameLabel)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(fontNameComboBox, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                    .addContainerGap())
            );
            textFormatLayout.setVerticalGroup(
                textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(textFormatLayout.createSequentialGroup()
                    .addContainerGap()
                    .addGroup(textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(nameLabel)
                        .addComponent(nameTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addGroup(textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(descriptionLabel)
                        .addComponent(descriptionTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 100, Short.MAX_VALUE)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addGroup(textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(fontSizeLabel)
                        .addComponent(fontNameLabel)
                        .addComponent(fontSizeSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(fontNameComboBox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addGroup(textFormatLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(fontStyleLabel)
                        .addComponent(fontStyleBold)
                        .addComponent(fontStyleItalic))
                    .addContainerGap())
            );

            backgroundPane.add(backgroundColorChooser, java.awt.BorderLayout.CENTER);
            javax.swing.JPanel filePane = new javax.swing.JPanel();
            filePane.setBorder(BorderFactory.createEtchedBorder());
            filePane.add(imagePathLabel);
            filePane.add(imagePath);
            filePane.add(imageFileChooserBtn);
            backgroundPane.add(filePane, java.awt.BorderLayout.SOUTH);

            // tabs are added depending on the XcosCell Kind

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
                    try
                    {
                        graph.getModel().beginUpdate();

                        EditFormatAction.updateFromDialog(getDialog(),
                                borderColorChooser.getColor(),
                                backgroundColorChooser.getColor(),
                                (String) fontNameComboBox.getSelectedItem(),
                                (Integer) fontSizeSpinner.getValue(),
                                textColorChooser.getColor(),
                                fontStyleBold.isSelected(),
                                fontStyleItalic.isSelected(),
                                nameTextField.getText(),
                                descriptionTextField.getText(),
                                mxUtils.getBodyMarkup(textArea.getText(), false),
                                imagePath.getText());
                    }
                    finally
                    {
                        graph.getModel().endUpdate();
                        getDialog().dispose();
                    }
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
