/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

import org.scilab.modules.graphic_export.ExportParams;
import org.scilab.modules.graphic_export.FileExporter;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;

/**
 * Window in which we can configure option for the selected format
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 *
 */
public class ExportOptionWindow extends JDialog implements ActionListener {

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
        optionDialog.setTitle("Option for " + exportData.getExportExtension().toUpperCase() + " format");
        optionDialog.setIconImage(new ImageIcon(ScilabSwingUtilities.findIcon("scilab")).getImage());
        //Center the frame
        optionDialog.setLocationRelativeTo(parentWindow);
    }

    /**
     * Selection between portrait or landscape option
     */
    public void landscapePortraitOption() {
        portrait = new JRadioButton("Portrait", true);
        landscape = new JRadioButton("Landscape", false);

        ButtonGroup bgroup = new ButtonGroup();
        bgroup.add(portrait);
        bgroup.add(landscape);

        confirmOption = new JButton("Confirm");
        abortOption = new JButton("Abort");


        JPanel mainPanel = new JPanel();
        BorderLayout layout = new BorderLayout();
        mainPanel.setLayout(layout);

        JPanel center = new JPanel();
        center.setLayout(new GridLayout(2, 1));
        center.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEtchedBorder(), " Orientation "));

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
        optionDialog.setVisible(true);
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
                properties.add(portrait.getText().toLowerCase());
            } else {
                properties.add(landscape.getText().toLowerCase());
            }
            exportData.setExportProperties(properties);
            optionDialog.dispose();

            String figId = exportData.getFigureId();
            String fileName = exportData.getExportName();
            int orientation = exportData.getExportProperties().elementAt(0).equalsIgnoreCase("landscape") ? ExportParams.LANDSCAPE : ExportParams.PORTRAIT;

            parentWindow.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
            String err = FileExporter.fileExport(figId, fileName, exportData.getExportExtension(), 1f, orientation);// 1f is the jpeg quality compression and it is useless here
            parentWindow.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));

            if (err.length() != 0) {
                ScilabModalDialog.show(parentTab, "An error occurred during export: " + err, "Export error", IconType.ERROR_ICON);
            }
        }

        if (evt.getSource() == abortOption) {
            optionDialog.dispose();
        }
    }
}
