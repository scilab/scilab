/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Color;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.ListIterator;
import java.util.Set;
import java.util.TreeSet;

import javax.imageio.ImageIO;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphRenderer;
import org.scilab.modules.gui.bridge.filechooser.FileMask;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxGraphModel.Filter;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxUtils;

/**
 * Diagram export management
 */
@SuppressWarnings(value = { "serial" })
public final class ExportAllAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.EXPORT_ALL;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit()
            .getMenuShortcutKeyMask();

    private static final String HTML = "html";
    private static final String VML = "vml";
    private static final String SVG = "svg";

    private boolean useBackground = true;

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated Scilab Graph
     */
    public ExportAllAction(ScilabGraph scilabGraph) {
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
        return createMenu(scilabGraph, ExportAllAction.class);
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

        final XcosDiagram graph = (XcosDiagram) getGraph(null);

        // Adds a filter for each supported image format
        Collection<String> imageFormats = Arrays.asList(ImageIO
                                          .getWriterFileSuffixes());

        // The mask ordered collection
        Set<String> mask = new TreeSet<String>(String.CASE_INSENSITIVE_ORDER);

        mask.add(SVG);
        mask.add(HTML);
        mask.add(VML);
        mask.addAll(imageFormats);

        JFileChooser fc = new JFileChooser();
        fc.setDialogTitle(XcosMessages.EXPORT_ALL + XcosMessages.DOTS);
        fc.setApproveButtonText(XcosMessages.EXPORT);
        fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        fc.setAcceptAllFileFilterUsed(false);
        for (String string : mask) {
            fc.addChoosableFileFilter(new FileMask(string, string.toUpperCase()));
        }

        ConfigurationManager.configureCurrentDirectory(fc);

        int selection = fc.showSaveDialog(graph.getAsComponent());
        if (selection == JFileChooser.APPROVE_OPTION) {
            File dir = fc.getSelectedFile();

            /* getting the format */
            String format = ((FileMask) fc.getFileFilter()).getExtensionFromFilter();

            /* update states from the format */
            if ((!format.equalsIgnoreCase("png"))
                    || ScilabModalDialog.show(XcosTab.get(graph),
                                              XcosMessages.TRANSPARENT_BACKGROUND, XcosMessages.XCOS,
                                              IconType.QUESTION_ICON, ButtonType.YES_NO) != AnswerOption.YES_OPTION) {
                useBackground = true;
            }

            final Filter superBlockFilter = new Filter() {
                @Override
                public boolean filter(Object cell) {
                    if (cell instanceof SuperBlock) {
                        final SuperBlock blk = (SuperBlock) cell;
                        blk.createChildDiagram();
                        return true;
                    }
                    return false;
                }
            };

            /*
             * Append all the superblocks
             */
            final ArrayList<Object> superBlocks = new ArrayList<Object>();
            final ArrayList<String> filenameDescriptors = new ArrayList<String>();
            Collection<Object> filtered = mxGraphModel.filterDescendants(graph.getModel(), superBlockFilter);
            superBlocks.addAll(filtered);

            fillFilenameDescriptors(graph.getTitle(), graph, filenameDescriptors, filtered);

            /*
             * Iterate recursively on children
             */
            for (int i = 0; i < superBlocks.size(); i++) {
                final SuperBlock blk = (SuperBlock) superBlocks.get(i);
                final SuperBlockDiagram diag = blk.getChild();

                filtered = mxGraphModel.filterDescendants(diag.getModel(), superBlockFilter);
                superBlocks.addAll(filtered);

                fillFilenameDescriptors(graph.getTitle(), graph, filenameDescriptors, filtered);
            }

            /*
             * Export
             */
            try {
                export(graph, new File(dir, graph.getTitle() + "." + format), format);

                for (int i = 0; i < superBlocks.size(); i++) {
                    export(((SuperBlock)superBlocks.get(i)).getChild(), new File(dir, filenameDescriptors.get(i) + "." + format), format);
                }
            } catch (IOException e1) {
                e1.printStackTrace();
            }

        }
    }

    private void fillFilenameDescriptors(final String rootName, final XcosDiagram graph, final ArrayList<String> filenameDescriptors, final Collection<Object> filtered) {
        filenameDescriptors.ensureCapacity(filenameDescriptors.size() + filtered.size());

        ListIterator<String> it = filenameDescriptors.listIterator();
        for (Object object : filtered) {
            final SuperBlock blk = (SuperBlock) object;

            final mxCell identifierLabel = graph.getCellIdentifier(blk);
            final String identifier;
            if (identifierLabel != null) {
                identifier = identifierLabel.getValue().toString();
            } else {
                identifier = "";
            }


            String filename = rootName;
            if (!identifier.isEmpty()) {
                filename += "_" + identifier;
            } else {
                filename += "_" + it.nextIndex();
            }
            it.add(filename);
        }
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
    private void export(XcosDiagram graph, File filename, String fileFormat)
    throws IOException {
        filename.getParentFile().mkdirs();

        if (fileFormat.equalsIgnoreCase(SVG)) {
            ScilabGraphRenderer.createSvgDocument(graph, null, 1, null, null,
                                                  filename.getCanonicalPath());
        } else if (fileFormat.equalsIgnoreCase(VML)) {
            Document doc = mxCellRenderer.createVmlDocument(graph, null, 1,
                           null, null);
            if (doc != null) {
                mxUtils.writeFile(mxUtils.getXml(doc.getDocumentElement()),
                                  filename.getCanonicalPath());
            }
        } else if (fileFormat.equalsIgnoreCase(HTML)) {
            Document doc = mxCellRenderer.createHtmlDocument(graph, null, 1,
                           null, null);
            if (doc != null) {
                mxUtils.writeFile(mxUtils.getXml(doc.getDocumentElement()),
                                  filename.getCanonicalPath());
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
    private void exportBufferedImage(XcosDiagram graph, File filename,
                                     String fileFormat) throws IOException {
        final mxGraphComponent graphComponent = graph.getAsComponent();

        Color bg = null;

        if (useBackground) {
            bg = graphComponent.getBackground();
        }

        BufferedImage image = mxCellRenderer.createBufferedImage(graph, null,
                              1, bg, graphComponent.isAntiAlias(), null,
                              graphComponent.getCanvas());

        if (image != null) {
            ImageIO.write(image, fileFormat, filename);
        } else {
            JOptionPane.showMessageDialog(graphComponent,
                                          XcosMessages.NO_IMAGE_DATA);
        }
    }
}
