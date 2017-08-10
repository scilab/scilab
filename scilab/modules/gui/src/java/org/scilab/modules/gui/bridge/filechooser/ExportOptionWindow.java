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

import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.GridLayout;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.graphic_export.ExportParams;
import org.scilab.modules.graphic_export.FileExporter;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.localization.Messages;

/**
 * Window in which we can configure option for the selected format
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 *
 */
public class ExportOptionWindow implements ActionListener {

    private static final long serialVersionUID = 1L;

    private final ExportData exportData;
    private Window parentWindow;
    private SimpleTab parentTab;
    private JDialog optionDialog;
    private JRadioButton portrait;
    private JRadioButton landscape;
    private JButton confirmOption;
    private JButton abortOption;

    /**
     * Default constructor
     * @param exportData information about the export
     */
    public ExportOptionWindow(ExportData exportData) {
        this.exportData = exportData;
    }

    /**
     * Display the option window
     */
    public void displayOptionWindow(SimpleTab tab) {
        parentTab = tab;
        parentWindow = (Window) SwingUtilities.getAncestorOfClass(Window.class, (JComponent) tab);
        optionDialog = new JDialog(parentWindow);
        optionDialog.setTitle(String.format(Messages.gettext("Option for %s format"), exportData.getExportExtension().toUpperCase()));
        optionDialog.setIconImage(new ImageIcon(FindIconHelper.findIcon("scilab")).getImage());
    }

    /**
     * Selection between portrait or landscape option
     */
    public void landscapePortraitOption() {
        portrait = new JRadioButton(Messages.gettext("Portrait"), true);
        landscape = new JRadioButton(Messages.gettext("Landscape"), false);

        ButtonGroup bgroup = new ButtonGroup();
        bgroup.add(portrait);
        bgroup.add(landscape);

        confirmOption = new JButton(Messages.gettext("Confirm"));
        abortOption = new JButton(Messages.gettext("Cancel"));

        JPanel mainPanel = new JPanel();
        BorderLayout layout = new BorderLayout();
        mainPanel.setLayout(layout);

        JPanel center = new JPanel();
        center.setLayout(new GridLayout(2, 1));
        center.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEtchedBorder(), String.format(" %s ", Messages.gettext("Orientation"))));

        center.add(portrait);
        center.add(landscape);

        JPanel south = new JPanel();

        south.add(confirmOption);
        south.add(abortOption);

        mainPanel.add(center, BorderLayout.CENTER);
        mainPanel.add(south, BorderLayout.EAST);

        optionDialog.add(mainPanel);
        optionDialog.pack();

        portrait.addActionListener(this);
        landscape.addActionListener(this);
        confirmOption.addActionListener(this);
        abortOption.addActionListener(this);

        optionDialog.setModal(true);
        optionDialog.setResizable(false);
        optionDialog.setLocationRelativeTo(parentWindow);
        optionDialog.setVisible(true);
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
     * Action manager
     * @param evt ActionEvent
     */
    @Override
    public void actionPerformed(ActionEvent evt) {
        boolean b = portrait.isSelected();
        Vector<String> properties = new Vector<String>();

        if (evt.getSource() == confirmOption) {
            if (b) {
                properties.add("portrait");
            } else {
                properties.add("landscape");
            }
            exportData.setExportProperties(properties);
            optionDialog.dispose();

            Integer figId = exportData.getFigureId();
            String fileName = exportData.getExportName();
            if (this.getExtension(fileName) == null) {
                // Could not get the extension from the user input
                // take the one from the list
                fileName += "." + exportData.getExportExtension();
            }

            int orientation = exportData.getExportProperties().elementAt(0).equalsIgnoreCase("landscape") ? ExportParams.LANDSCAPE : ExportParams.PORTRAIT;

            Cursor old = parentWindow.getCursor();
            parentWindow.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
            String err = FileExporter.fileExport(figId, fileName, exportData.getExportExtension(), 1f, orientation);// 1f is the jpeg quality compression and it is useless here
            parentWindow.setCursor(old);

            if (err.length() != 0) {
                ScilabModalDialog.show(parentTab, String.format(Messages.gettext("An error occurred during export: %s"), err), Messages.gettext("Export error"), IconType.ERROR_ICON);
            }
        }

        if (evt.getSource() == abortOption) {
            optionDialog.dispose();
        }
    }
}
