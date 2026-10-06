/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Set;
import java.util.TreeSet;
import java.util.HashMap;

import javax.imageio.ImageIO;
import javax.swing.JFileChooser;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.FileMask;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;

/**
 * Diagram export management
 */
public final class ExportAllAction extends DefaultAction {
    private static final long serialVersionUID = 1L;
    /** Name of the action */
    public static final String NAME = XcosMessages.EXPORT_ALL;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit()
            .getMenuShortcutKeyMaskEx();

    private static final String HTML = "html";
    private static final String VML = "vml";
    private static final String SVG = "svg";

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
        final XcosDiagram graph = ((XcosDiagram) getGraph(null)).getRootDiagram();

        // Adds a filter for each supported image format
        Collection<String> imageFormats = Arrays.asList(ImageIO.getWriterFileSuffixes());

        // The mask ordered collection
        Set<String> mask = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

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

        int selection = fc.showSaveDialog(getGraph(null).getAsComponent());
        if (selection == JFileChooser.APPROVE_OPTION) {
            File dir = fc.getSelectedFile();

            /* getting the format */
            String format = ((FileMask) fc.getFileFilter()).getExtensionFromFilter();

            /* update states from the format */
            Color backgroundColor = ExportAction.askBackground((XcosDiagram) getGraph(null), format);

            /*
             * Instantiate all the sub-diagrams
             */
            JavaController controller = new JavaController();
            ArrayList<XcosDiagram> diagrams = new ArrayList<>();

            ArrayList<Long> stash = new ArrayList<>();
            allocateDiagrams(controller, diagrams, stash, graph.getUID(), Kind.DIAGRAM);
            while (!stash.isEmpty()) {
                final long uid = stash.remove(stash.size() - 1);
                allocateDiagrams(controller, diagrams, stash, uid, Kind.BLOCK);
            }

            /*
             * Create unique filenames
             */
            HashMap<File, Integer> filesSet = new HashMap<>(diagrams.size());
            makeUniqueFile(filesSet, 0, dir, graph.getTitle(), format);
            for (int i = 0; i < diagrams.size(); i++) {
                final XcosDiagram d = diagrams.get(i);
                makeUniqueFile(filesSet, i + 1, dir, d.getTitle(), format);
            }
            File[] files = new File[diagrams.size() + 1];
            filesSet.forEach((f, i) -> { if (i >= 0) files[i] = f;});

            /*
             * Export
             */
            try {
                ExportAction.export(graph, files[0], format, backgroundColor);

                for (int i = 0; i < diagrams.size(); i++) {
                    ExportAction.export(diagrams.get(i), files[i + 1], format, backgroundColor);
                }
            } catch (IOException e1) {
                e1.printStackTrace();
            }

        }
    }

    private static void makeUniqueFile(HashMap<File, Integer> filesSet, int fileIndex, File dir, String name, String ext) {
        final StringBuilder str = new StringBuilder(name).append(".").append(ext);

        File f = new File(dir, str.toString());
        Integer index = filesSet.get(f);

        // rename the existing file with a 0 prefix
        if (index != null && index >= 0) {
            // flag the original and rename it
            filesSet.put(f, -1);

            str.delete(name.length(), str.length());
            str.append("_").append(0).append(".").append(ext);
            f = new File(dir, str.toString());

            filesSet.put(f, index);
        }
        for (int i = 1; index != null; i++) {
            str.delete(name.length(), str.length());
            str.append("_").append(i).append(".").append(ext);

            f = new File(dir, str.toString());
            index = filesSet.get(f);
        }

        filesSet.put(f, fileIndex);
    }

    private void allocateDiagrams(JavaController controller, ArrayList<XcosDiagram> diagrams, ArrayList<Long> stash,
                                  final long uid, final Kind kind) {
        final VectorOfScicosID children = new VectorOfScicosID();
        controller.getObjectProperty(uid, kind, ObjectProperties.CHILDREN, children);

        final int len = children.size();
        for (int i = 0; i < len ; i++) {
            String[] interfaceFunction = new String[1];
            long currentUID = children.get(i);
            if (controller.getKind(currentUID) == Kind.BLOCK) {
                controller.getObjectProperty(children.get(i), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);
                if (!"SUPER_f".equals(interfaceFunction[0])) {
                    continue;
                }


                if (diagrams.stream().noneMatch(d -> d.getUID() == currentUID)) {
                    String[] strUID = new String[1];
                    controller.getObjectProperty(currentUID, Kind.BLOCK, ObjectProperties.UID, strUID);

                    final XcosDiagram child = XcosDiagram.create(controller, currentUID, Kind.BLOCK, strUID[0]);
                    XcosCellFactory.insertChildren(controller, child);
                    diagrams.add(child);
                    stash.add(currentUID);
                }
            }

        }
    }
}
