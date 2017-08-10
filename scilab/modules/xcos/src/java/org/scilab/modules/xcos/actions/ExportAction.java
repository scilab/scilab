/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.imageio.ImageWriter;
import javax.swing.BorderFactory;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;
import javax.swing.filechooser.FileFilter;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphRenderer;
import org.scilab.modules.gui.bridge.filechooser.FileMask;
import org.scilab.modules.gui.bridge.filechooser.ImagePreview;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxUtils;
import com.mxgraph.util.mxXmlUtils;

/**
 * Diagram export management
 */
@SuppressWarnings(value = { "serial" })
public final class ExportAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.EXPORT;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_E;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    private static final String HTML = "html";
    private static final String VML = "vml";
    private static final String SVG = "svg";

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Scilab Graph
     */
    public ExportAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Create export menu
     *
     * @param scilabGraph
     *            associated Scilab Graph
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, ExportAction.class);
    }

    /**
     * Action !!!
     *
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {

        XcosDiagram graph = (XcosDiagram) getGraph(null);

        // Adds a filter for each supported image format
        Collection<String> imageFormats = Arrays.asList(ImageIO.getWriterFileSuffixes());

        // The mask ordered collection
        Set<String> mask = new TreeSet<String>(String.CASE_INSENSITIVE_ORDER);

        mask.add(SVG);
        mask.add(HTML);
        mask.add(VML);
        mask.addAll(imageFormats);

        JFileChooser fc = new JFileChooser();
        fc.setDialogTitle(XcosMessages.EXPORT);
        fc.setApproveButtonText(XcosMessages.EXPORT);
        fc.setAcceptAllFileFilterUsed(true);

        for (String string : mask) {
            fc.addChoosableFileFilter(new FileMask(string, string.toUpperCase()));
        }

        ConfigurationManager.configureCurrentDirectory(fc);

        JPanel panelPreview = new JPanel();

        // Title for preview panel
        TitledBorder titlePreview;
        titlePreview = BorderFactory.createTitledBorder(Messages.gettext("Preview"));
        panelPreview.setBorder(titlePreview);

        // add preview image
        panelPreview.add(new ImagePreview(fc));

        // Main panel contains extensionTable panel & preview panel
        JPanel accessoryPanel = new JPanel();

        // accessoryPanel.add(scrollPane);
        accessoryPanel.add(panelPreview);
        accessoryPanel.setVisible(true);
        fc.setAccessory(accessoryPanel);

        int selection = fc.showSaveDialog(graph.getAsComponent());
        if (selection == JFileChooser.APPROVE_OPTION) {
            File selected = fc.getSelectedFile();

            /* getting the format */
            String format = getFormat(graph, fc, selected);
            while (format == null) {
                selection = fc.showSaveDialog(graph.getAsComponent());
                if (selection == JFileChooser.APPROVE_OPTION) {
                    selected = fc.getSelectedFile();
                    format = getFormat(graph, fc, selected);
                } else {
                    return;
                }
            }

            try {
                /* Add an extension if no one is set */
                if (FileMask.getExtension(selected) == null || FileMask.getExtension(selected).isEmpty()) {
                    selected = new File(selected.getCanonicalPath() + '.' + format);
                }

                /* Export the file */
                if (selected.exists()) {
                    final boolean overwrite = ScilabModalDialog.show(XcosTab.get(graph), XcosMessages.OVERWRITE_EXISTING_FILE, XcosMessages.XCOS,
                                              IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.YES_OPTION;

                    if (overwrite) {
                        export(graph, selected, format);
                    } else {
                        return;
                    }
                } else {
                    export(graph, selected, format);
                }
            } catch (IOException ex) {
                Logger.getLogger(ExportAction.class.getName()).severe(e.toString());
            }
        }
    }

    /**
     * Get the file format
     *
     * @param graph
     *            the current graph
     * @param fc
     *            the file chooser
     * @param selected
     *            the selected file
     * @return the format or null
     */
    private String getFormat(XcosDiagram graph, JFileChooser fc, final File selected) {
        final String format;
        final FileFilter fileFilter = fc.getFileFilter();

        // accept all file filter or any custom file filter (generated
        // by a <TAB> press)
        if (!(fileFilter instanceof FileMask)) {

            /*
             * Get the extension from the file name. Fail safely.
             */
            if (FileMask.getExtension(selected) == null || FileMask.getExtension(selected).isEmpty()) {
                format = null;
            } else {
                format = FileMask.getExtension(selected);
            }

        } else {
            /*
             * Get the format from the file mask
             */
            format = ((FileMask) fileFilter).getExtensionFromFilter();

        }
        final boolean validFormat = isValidFormat(format);

        /*
         * When the format is unspecified, popup an error dialog
         */
        if (format == null || !validFormat) {
            JOptionPane.showMessageDialog(graph.getAsComponent(), Messages.gettext("Please specify a valid file format"), Messages.gettext("Error on export"),
                                          JOptionPane.ERROR_MESSAGE);
        }

        if (validFormat) {
            return format;
        } else {
            return null;
        }
    }

    private boolean isValidFormat(String format) {
        Iterator<ImageWriter> it = ImageIO.getImageWritersBySuffix(format);
        Collection<String> externals = Arrays.asList(SVG, HTML, VML);

        return it.hasNext() || externals.contains(format.toLowerCase());
    }

    /**
     * Export the graph into the filename.
     *
     * The filename extension is used find export format.
     *
     * @param graph
     *            the current graph
     * @param filename
     *            the filename
     * @param fileFormat
     *            the format to save
     * @throws IOException
     *             when a write problem occurs.
     */
    private void export(XcosDiagram graph, File filename, String fileFormat) throws IOException {
        if (fileFormat.equalsIgnoreCase(SVG)) {
            ScilabGraphRenderer.createSvgDocument(graph, null, 1, null, null, filename.getCanonicalPath());
        } else if (fileFormat.equalsIgnoreCase(VML)) {
            Document doc = mxCellRenderer.createVmlDocument(graph, null, 1, null, null);
            if (doc != null) {
                mxUtils.writeFile(mxXmlUtils.getXml(doc.getDocumentElement()), filename.getCanonicalPath());
            }
        } else if (fileFormat.equalsIgnoreCase(HTML)) {
            Document doc = mxCellRenderer.createHtmlDocument(graph, null, 1, null, null);
            if (doc != null) {
                mxUtils.writeFile(mxXmlUtils.getXml(doc.getDocumentElement()), filename.getCanonicalPath());
            }
        } else {
            exportBufferedImage(graph, filename, fileFormat);
        }
    }

    /**
     * Use the Java image capabilities to export the diagram
     *
     * @param graph
     *            the current diagram
     * @param filename
     *            the current filename
     * @param fileFormat
     *            the file format
     * @throws IOException
     *             when an error occurs
     */
    private void exportBufferedImage(XcosDiagram graph, File filename, String fileFormat) throws IOException {
        final mxGraphComponent graphComponent = graph.getAsComponent();

        Color bg = null;

        if ((!fileFormat.equalsIgnoreCase("png"))
                || ScilabModalDialog
                .show(XcosTab.get(graph), XcosMessages.TRANSPARENT_BACKGROUND, XcosMessages.XCOS, IconType.QUESTION_ICON, ButtonType.YES_NO) != AnswerOption.YES_OPTION) {
            bg = graphComponent.getBackground();
        }

        BufferedImage image = mxCellRenderer.createBufferedImage(graph, null, 1, bg, graphComponent.isAntiAlias(), null, graphComponent.getCanvas());

        if (image != null) {
            ImageIO.write(image, fileFormat, filename);
        } else {
            JOptionPane.showMessageDialog(graphComponent, XcosMessages.NO_IMAGE_DATA);
        }
    }
}
