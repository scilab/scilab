/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * Copyright (C) 2011 - Calixte DENIZET
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
package org.scilab.modules.gui.bridge.filechooser;

import java.awt.Component;
import java.awt.Cursor;
import java.awt.Window;
import java.io.File;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.border.TitledBorder;
import javax.swing.filechooser.FileNameExtensionFilter;

import org.scilab.modules.graphic_export.FileExporter;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.gui.bridge.filechooser.JFXScilabFileChooser;

/**
 * This is the son of the usual Scilab file chooser,
 * it have been customized for the graphic export
 * @author Sylvestre Koumar
 *
 */
@SuppressWarnings(value = { "serial" })
public class JFXScilabExportFileChooser extends JFXScilabFileChooser {

    private static final int NB_FILE_MASKS = 10;

    // Same order as the Swing chooser: the first one, PNG, is the default
    private final String[] description = {
        Messages.gettext("PNG image"),
        Messages.gettext("JPEG image"),
        Messages.gettext("Encapsulated PostScript image (EPS)"),
        Messages.gettext("PostScript image (PS)"),
        Messages.gettext("PDF image"),
        Messages.gettext("SVG image"),
        Messages.gettext("Enhanced Metafile image (EMF)"),
        Messages.gettext("GIF image"),
        Messages.gettext("Windows BMP image"),
        Messages.gettext("PPM image")
    };

    private final String[] extensions = {
        "*.png",
        "*.jpg|*.jpeg",
        "*.eps",
        "*.ps",
        "*.pdf",
        "*.svg",
        "*.emf",
        "*.gif",
        "*.bmp",
        "*.ppm"
    };

    private String exportName;
    private String extensionSelected;
    private Integer figureUID;

    /**
     * Default constructor
     * @param figureId id of the exported figure
     */

    public JFXScilabExportFileChooser(Integer figureUID) {
        this.figureUID = figureUID;
        exportCustomFileChooser(figureUID);
    }

    /**
     * We customize the file chooser for the graphic export
     * by adding format selection
     * @param figureId exported figure number
     */
    public void exportCustomFileChooser(Integer figureUID) {
        Figure figure = (Figure) GraphicController.getController().getObjectFromId(figureUID);
        String defaultName = figure.getName();
        int figureId = figure.getId();
        if (defaultName != null && !defaultName.isEmpty()) {
            defaultName = defaultName.replaceFirst("%d", Integer.toString(figureId));
        } else {
            defaultName = Messages.gettext("Untitled-export");
        }

        setTitle(Messages.gettext("Export"));
        File exportFile = new File(defaultName);
        setInitialFileName(exportFile.getName());
        setAcceptAllFileFilterUsed(false);

        this.figureUID = figureUID;

        addMask(extensions, description);

        setUiDialogType(JFXScilabFileChooser.SAVE_DIALOG);
        displayAndWait();
        String[] selection = getSelection();
        if (selection.length!=0 && selection[0] != "") {
            exportName = selection[0];
            /* Bug 3849 fix */
            ConfigManager.saveLastOpenedDirectory(new File(exportName).getParentFile().getPath());
            String extensionCombo = getExtension(exportName);
            if (extensionCombo.equals("emf") || extensionCombo.equals("eps") || extensionCombo.equals("ps") || extensionCombo.equals("pdf") || extensionCombo.equals("svg")) {
                vectorialExport(extensionCombo);
            } else {
                bitmapExport(extensionCombo);
            }
        
        }
    }

    /**
     * Return the file extension
     * @param fileName Name of the file
     * @return the extension
     */
    public String getExtension(String fileName) {
        if (fileName != null) {
            int i = fileName.lastIndexOf('.');
            if (i > 0 && i < fileName.length() - 1) {
                return fileName.substring(i + 1).toLowerCase();
            }
        }
        return null;
    }

    /**
     * Manage the export (bitmap/vectorial format) and export errors
     */
    /**
     * Export an bitmap file
     * @param userExtension extension caught by the user
     */
    public void bitmapExport(String userExtension) {
        //Cursor old = getCursor();
        //setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

        ExportData exportData = new ExportData(figureUID, this.exportName, userExtension, null);
        FileExporter.fileExport(figureUID, this.exportName, exportData.getExportExtension(), -1, 0);

        //setCursor(old);
    }

    /**
     * Export a vectorial file
     * @param userExtension extension caught by the user
     */
    public void vectorialExport(String userExtension) {
        SwingViewObject view = SwingView.getFromId(figureUID);
        SimpleTab tab;
        if (view instanceof SimpleTab)
        {
            tab = (SimpleTab) SwingView.getFromId(figureUID);
        }
        else
        {
            tab = null;
        }
        ExportData exportData = new ExportData(figureUID, this.exportName, userExtension, null);

        ExportOptionWindow exportOptionWindow = new ExportOptionWindow(exportData);
        exportOptionWindow.displayOptionWindow(tab);
        exportOptionWindow.landscapePortraitOption();
    }
}
