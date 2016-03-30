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
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Shift.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Shift extends SimpleSection {
    private JPanel sectionPanel;
    private static Shift instance;
    private Integer currentpolyline;

    private JLabel lShiftX = new JLabel();
    private JLabel cShiftX = new JLabel();
    private JButton bShiftX = new JButton();
    private JPanel pShiftX = new JPanel();
    private JLabel lShiftY = new JLabel();
    private JLabel cShiftY = new JLabel();
    private JButton bShiftY = new JButton();
    private JPanel pShiftY = new JPanel();
    private JLabel lShiftZ = new JLabel();
    private JLabel cShiftZ = new JLabel();
    private JButton bShiftZ = new JButton();
    private JPanel pShiftZ = new JPanel();
    private JDialog shiftDialog = new JDialog();
    private JScrollPane shiftScroll = new JScrollPane();
    private JTable shiftTable = new JTable();
    private JButton refresh = new JButton();
    private JButton ok = new JButton();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public Shift(Integer objectID) {
        super(MessagesGED.shift, "polyline");
        instance = this;
        currentpolyline = objectID;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
        updateShiftTable(0);
        updateShiftTable(1);
        updateShiftTable(2);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Shift getInstance() {
        return instance;
    }

    /**
    * Add all the properties in this section.
    * @param objectID uid
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 16; //to inner components
        int column = 0; //first column

        //Components of the property: X Shift.
        shiftDialog();
        addJLabel(sectionPanel, lShiftX, MessagesGED.x_shift, column, row, leftmargin);
        addDataField(sectionPanel, pShiftX, bShiftX, cShiftX, column + 1, row);
        bShiftX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateShiftTable(0);
                shiftDialog.setVisible(true);
            }
        });
        row++;

        //Components of the property: Y Shift.
        addJLabel(sectionPanel, lShiftY, MessagesGED.y_shift, column, row, leftmargin);
        addDataField(sectionPanel, pShiftY, bShiftY, cShiftY, column + 1, row);
        bShiftY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateShiftTable(1);
                shiftDialog.setVisible(true);
            }
        });
        row++;

        //Components of the property: Z Shift.
        addJLabel(sectionPanel, lShiftZ, MessagesGED.z_shift, column, row, leftmargin);
        addDataField(sectionPanel, pShiftZ, bShiftZ, cShiftZ, column + 1, row);
        bShiftZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateShiftTable(2);
                shiftDialog.setVisible(true);
            }
        });
        row++;
    }

    /**
     * Implement Shift Editor
     */
    private void shiftDialog() {
        addShiftDialog(shiftDialog, shiftScroll, shiftTable, refresh, ok);
        shiftDialog.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent evt) {
                updateShiftTable(0);
                updateShiftTable(1);
                updateShiftTable(2);
            }
        });
        refresh.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                String axis = shiftTable.getColumnName(0);
                if ("X".equals(axis)) {
                    updateShiftTable(0);
                } else if ("Y".equals(axis)) {
                    updateShiftTable(1);
                } else if ("Z".equals(axis)) {
                    updateShiftTable(2);
                }
            }
        });
        ok.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateShiftTable(0);
                updateShiftTable(1);
                updateShiftTable(2);
                shiftDialog.dispose();
            }
        });
        shiftTable.getModel().addTableModelListener(new TableModelListener() {
            @Override
            public void tableChanged(TableModelEvent evt) {
                shiftTableEvent(evt);
            }
        });
    }

    /**
    * Assigns the table changes.
    * @param evt TableModelEvent.
    */
    private void shiftTableEvent(TableModelEvent evt) {
        if (shiftTable.getSelectedRow() != -1) {
            Object shiftValue = shiftTable.getValueAt(shiftTable.getSelectedRow(), 1);
            if (shiftValue == null) {
                shiftValue = 0.0;
            }
            //set shift value here - not implemented yet
        }
    }

    /**
    * Updates the shift table.
    * @param axis 0 to X - 1 to Y - 2 to Z
    */
    public final void updateShiftTable(int axis) {
        Object[][] data;
        DefaultTableModel tableModel = (DefaultTableModel) shiftTable.getModel();
        switch (axis) {
            case 0:
                if (PolylineData.isXShiftSet(currentpolyline) == 0) {
                    data = getShift(0, true);
                    cShiftX.setText("null");
                } else {
                    data = getShift(0, false);
                    cShiftX.setText("1x" + data.length);
                }
                tableModel.setDataVector(data, new String [] {"X", MessagesGED.x_shift});
                break;
            case 1:
                if (PolylineData.isYShiftSet(currentpolyline) == 0) {
                    data = getShift(1, true);
                    cShiftY.setText("null");
                } else {
                    data = getShift(1, false);
                    cShiftY.setText("1x" + data.length);
                }
                tableModel.setDataVector(data, new String [] {"Y", MessagesGED.y_shift});
                break;
            case 2:
                if (PolylineData.isZShiftSet(currentpolyline) == 0) {
                    data = getShift(2, true);
                    cShiftZ.setText("null");
                } else {
                    data = getShift(2, false);
                    cShiftZ.setText("1x" + data.length);
                }
                tableModel.setDataVector(data, new String [] {"Z", MessagesGED.z_shift});
                break;
        }
    }

    /**
    * Get all data from a polyline.
    * @param axis 0 to X - 1 to Y - 2 to Z
    * @param isNull use TRUE when SHIFT is null
    * @return [data][shift]
    */
    public Object[][] getShift(int axis, boolean isNull) {
        double[] point = null;
        double[] shift = null;
        switch (axis) {
            case 0:
                point = (double[]) PolylineData.getDataX(currentpolyline);
                break;
            case 1:
                point = (double[]) PolylineData.getDataY(currentpolyline);
                break;
            case 2:
                point = (double[]) PolylineData.getDataZ(currentpolyline);
                break;
        }
        Object[][] data = new Object[point.length][2];
        if (!isNull) {
            switch (axis) {
                case 0:
                    shift = (double[]) PolylineData.getShiftX(currentpolyline);
                    break;
                case 1:
                    shift = (double[]) PolylineData.getShiftY(currentpolyline);
                    break;
                case 2:
                    shift = (double[]) PolylineData.getShiftZ(currentpolyline);
                    break;
            }
            for (int i = 0; i < point.length; i++) {
                data[i][0] = point[i];
                data[i][1] = shift[i];
            }
        } else {
            for (int i = 0; i < point.length; i++) {
                data[i][0] = point[i];
                data[i][1] = 0;
            }
        }
        return data;
    }
}