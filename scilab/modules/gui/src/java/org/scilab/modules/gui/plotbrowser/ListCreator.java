/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
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
    private final ArrayList<Integer> uids = new ArrayList<Integer>();
    private ArrayList<Object> cbArray = new ArrayList<Object>();

    public ListCreator(Integer figureID) {
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

    private void filler(Integer figureID) {
        ObjectSearcher instanceObject = new ObjectSearcher();

        Integer[] axes = searchAxes(figureID);

        for (int i = 0; i < axes.length; i++) {

            Integer title = (Integer) GraphicController.getController()
                            .getProperty(axes[i], GraphicObjectProperties.__GO_TITLE__);

            String[] text = (String[]) GraphicController.getController()
                            .getProperty(title, GraphicObjectProperties.__GO_TEXT_STRINGS__);

            cbArray.add(new JCheckBox(text[0].isEmpty() ? MessagesPlotBrowser.axis : text[0]));
            uids.add(axes[i]);

            Integer[] line = instanceObject.search(axes[i], GraphicObjectProperties.__GO_POLYLINE_STYLE__);
            if (line != null) {
                for (int j = 0; j < line.length; j++) {
                    cbArray.add(new JCheckBox(MessagesPlotBrowser.polyline));
                    uids.add(line[j]);
                }
            }

            Integer[] grayplot = instanceObject.search(axes[i], GraphicObjectProperties.__GO_GRAYPLOT__);
            if (grayplot != null) {
                for (int j = 0; j < grayplot.length; j++) {
                    cbArray.add(new JCheckBox(MessagesPlotBrowser.surface));
                    uids.add(grayplot[j]);
                }
            }

            Integer[] fac3d = instanceObject.search(axes[i], GraphicObjectProperties.__GO_FAC3D__);
            if (fac3d != null) {
                for (int j = 0; j < fac3d.length; j++) {
                    cbArray.add(new JCheckBox(MessagesPlotBrowser.surface));
                    uids.add(fac3d[j]);
                }
            }

            Integer[] plot3d = instanceObject.search(axes[i], GraphicObjectProperties.__GO_PLOT3D__);
            if (plot3d != null) {
                for (int j = 0; j < plot3d.length; j++) {
                    cbArray.add(new JCheckBox(MessagesPlotBrowser.surface));
                    uids.add(plot3d[j]);
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
    public void setVisible(boolean status, Integer objectID) {
        GraphicController.getController()
        .setProperty(objectID, GraphicObjectProperties.__GO_VISIBLE__, status);
    }

    /**
    * Locates all axes are plotted in Figure.
    * @param figureID Enters the identification of figure.
    * @return the existing axes.
    */
    private static Integer[] searchAxes(Integer figureID) {
        return (new ObjectSearcher()).search(figureID, GraphicObjectProperties.__GO_AXES__);
    }
}
