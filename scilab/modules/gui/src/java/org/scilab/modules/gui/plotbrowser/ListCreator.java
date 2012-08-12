/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.plotbrowser;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Vector;
import javax.swing.JCheckBox;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import org.scilab.modules.gui.editor.ObjectSearcher;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class ListCreator {
    private JList list;
    private final ArrayList<String> uids = new ArrayList<String>();
    private ArrayList<Object> cbArray = new ArrayList<Object>();

    public ListCreator(String figureID) {
        structure();
        filler(figureID);
        getVisible();
    }

    private void structure() {
        list = new JList();

        list.setCellRenderer(new CheckBoxCellRenderer());
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        list.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_SPACE) {
                    int index = list.getSelectedIndex();
                    if (index != -1) {
                        JCheckBox checkbox = getCheckbox(index);
                        checkbox.setSelected(!checkbox.isSelected());
                        setVisible(!checkbox.isSelected(), uids.get(index));
                        SwingPlotBrowser.pReceive.repaint();
                    }
                }
            }
        });

        list.addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                if (e.getX() < 20) {
                    int index = list.locationToIndex(e.getPoint());
                    if (index != -1) {
                        JCheckBox checkbox = getCheckbox(index);
                        checkbox.setSelected(!checkbox.isSelected());
                        setVisible(!checkbox.isSelected(), uids.get(index));
                        SwingPlotBrowser.pReceive.repaint();
                    }
                }
            }
        });
    }

    private void filler(String figureID) {
        ObjectSearcher instanceObject = new ObjectSearcher();

        String[] axes = searchAxes(figureID);

        for (int i = 0; i < axes.length; i++) {

            cbArray.add(new JCheckBox(MessagesPlotBrowser.axis));
            uids.add(axes[i]);

            String[] line = instanceObject.search(axes[i], "Polyline");
            if (line != null) {
                for (int j = 0; j < line.length; j++) {
                    cbArray.add(new JCheckBox(MessagesPlotBrowser.polyline));
                    uids.add(line[j]);
                }
            }
        }

        list.setListData(new Vector(cbArray));

        SwingPlotBrowser.pReceive.add(new JScrollPane(list));
    }

    /**
    * Get the checkbox specific.
    * @param index Index of the list.
    */
    private JCheckBox getCheckbox(int index) {
        JCheckBox checkbox = (JCheckBox) list.getModel().getElementAt(index);
        return checkbox;
    }

    /**
    * Loads the current status of objects.
    */
    private void getVisible() {
        for (int index = 0; index < cbArray.size(); index++) {
            boolean isVisible = (Boolean) GraphicController.getController()
                                .getProperty(uids.get(index), GraphicObjectProperties.__GO_VISIBLE__);
            JCheckBox checkboxAxis = getCheckbox(index);
            checkboxAxis.setSelected(!isVisible);
        }
    }

    /**
    * Changes the visibility of an object.
    * @param status The status of the checkbox.
    * @param objectID Enters the identification of object.
    */
    public void setVisible(boolean status, String objectID) {
        GraphicController.getController()
        .setProperty(objectID, GraphicObjectProperties.__GO_VISIBLE__, status);
    }

    /**
    * Locates all axes are plotted in Figure.
    * @param figureID Enters the identification of figure.
    * @return the existing axes.
    */
    private static String[] searchAxes(String figureID) {
        return (new ObjectSearcher()).search(figureID, GraphicObjectProperties.__GO_AXES__);
    }
}
