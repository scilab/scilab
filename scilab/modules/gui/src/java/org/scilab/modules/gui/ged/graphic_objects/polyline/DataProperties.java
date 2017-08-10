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
package org.scilab.modules.gui.ged.graphic_objects.polyline;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.gui.ged.graphic_objects.properties.ClippableContouredObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.GraphicObject;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends SimpleSection {
    private JPanel sectionPanel;
    private static DataProperties instance;
    private Integer currentpolyline;
    private ClippableContouredObject clippableContouredObject = new ClippableContouredObject();
    private GraphicObject graphicObject = new GraphicObject();

    private JLabel lData = new JLabel();
    private JPanel pData = new JPanel();
    private JButton bData = new JButton();
    private JLabel cData = new JLabel();
    private JDialog dataTableDialog = new JDialog();
    private JScrollPane dataScroll = new JScrollPane();
    private JTable dataTable = new JTable();
    private JButton append = new JButton();
    private JButton delete = new JButton();
    private JButton refresh = new JButton();
    private JButton ok = new JButton();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public DataProperties(Integer objectID) {
        super(MessagesGED.data_properties, "polyline");
        instance = this;
        currentpolyline = objectID;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static DataProperties getInstance() {
        return instance;
    }

    /**
    * Add all the properties in this section.
    * @param objectID uid
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 0; //to inner components
        int column = 1; //first column

        //Components of the property: Clip State.
        clippableContouredObject.clipState(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Clip Box.
        clippableContouredObject.clipBox(sectionPanel, row, column, leftmargin, objectID);
        row += 2;

        //Components of the property: Data.
        dataDialog();
        addJLabel(sectionPanel, lData, MessagesGED.data, 1, row, leftmargin);
        addDataField(sectionPanel, pData, bData, cData, 2, row++);
        bData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateDataTable();
                dataTableDialog.setVisible(true);
            }
        });

        //Components of the property: Tag.
        graphicObject.tag(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: User Data.
        graphicObject.userData(sectionPanel, row++, column, leftmargin);
    }

    /**
     * Implement Data Editor.
     */
    private void dataDialog() {
        addDataDialog(dataTableDialog, dataScroll, dataTable, append, delete, refresh, ok, currentpolyline);
        dataTableDialog.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent evt) {
                updateDataTable();
            }
        });
        dataTable.getModel().addTableModelListener(new TableModelListener() {
            @Override
            public void tableChanged(TableModelEvent evt) {
                dataTableEvent(evt);
            }
        });
        refresh.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateDataTable();
            }
        });
        ok.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateDataTable();
                dataTableDialog.dispose();
            }
        });
    }

    /**
    * Get all data from a polyline.
    * @return data
    */
    public final Object[][] getData() {
        double[] dataX = (double[]) PolylineData.getDataX(currentpolyline);
        double[] dataY = (double[]) PolylineData.getDataY(currentpolyline);
        double[] dataZ = (double[]) PolylineData.getDataZ(currentpolyline);
        Object[][] data = new Object[dataX.length][3];
        for (int i = 0; i < dataX.length; i++) {
            data[i][0] = dataX[i];
            data[i][1] = dataY[i];
            data[i][2] = dataZ[i];
        }
        return data;
    }

    /**
    * Checks if the polyline has 3 dimensions.
    * @param data polyline
    * @return is3D
    */
    private boolean is3D(Object[][] data) {
        boolean is3D = false;
        for (int i = 0; i < data.length; i++) {
            if ((Double) data[i][2] != 0.0) {
                is3D = true;
            }
        }
        return is3D;
    }

    /**
    * Updates the data table.
    */
    private void updateDataTable() {
        DefaultTableModel tableModel = (DefaultTableModel) dataTable.getModel();
        Object[][] data = getData();
        if (data.length != 0) {
            if (is3D(data)) {
                tableModel.setDataVector(data, new String [] {"X", "Y", "Z"});
                cData.setText(data.length + "x3");
            } else {
                tableModel.setDataVector(data, new String [] {"X", "Y"});
                cData.setText(data.length + "x2");
            }
        }
    }

    /**
    * Assigns the table changes.
    * @param evt TableModelEvent.
    */
    private void dataTableEvent(TableModelEvent evt) {
        if (dataTable.getSelectedRow() != -1) {
            if (dataTable.getRowCount() <= dataTable.getSelectedRow()) {
                return;
            }
            Object xValue = dataTable.getValueAt(dataTable.getSelectedRow(), 0);
            Object yValue = dataTable.getValueAt(dataTable.getSelectedRow(), 1);
            if (xValue == null) {
                xValue = 0.0;
            } else if (yValue == null) {
                yValue = 0.0;
            }
            PolylineData.setPointValue(currentpolyline, dataTable.getSelectedRow(),
                                       (Double) xValue, (Double) yValue, 0.0);
        }
    }
}