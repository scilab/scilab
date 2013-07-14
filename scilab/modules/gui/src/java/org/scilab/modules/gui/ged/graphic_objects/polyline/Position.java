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
package org.scilab.modules.gui.ged.graphic_objects.polyline;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JTable;
import javax.swing.JToggleButton;
import javax.swing.table.DefaultTableModel;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.gui.ged.ContentLayout;

import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Position.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Position extends DataProperties {
    private ContentLayout layout = new ContentLayout();
    protected static JToggleButton bPosition;
    private JLabel lPosition;
    private JSeparator sPosition;
    protected static JPanel pPosition;
    private JLabel lMarkSizeUnit;
    private JComboBox cMarkSizeUnit;
    private JLabel lShiftX;
    private JLabel cShiftX;
    private JButton bShiftX;
    private JPanel pShiftX;
    private JLabel lShiftY;
    private JLabel cShiftY;
    private JButton bShiftY;
    private JPanel pShiftY;
    private JLabel lShiftZ;
    private JLabel cShiftZ;
    private JButton bShiftZ;
    private JPanel pShiftZ;
    private JDialog shiftDialog;
    private JScrollPane shiftScroll;
    private JTable shiftTable;
    private JButton refresh;
    private JButton ok;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
    */
    public Position(String objectID) {
        super(objectID);
        insertBase();
        components();
        shiftDialog();
        values(objectID);
    }

    /**
    * Insert show/hide button, title and main JPanel of group.
    */
    private void insertBase() {
	int position = 3; //third group

        bPosition = new JToggleButton();
        lPosition = new JLabel();
        sPosition = new JSeparator();
        pPosition = new JPanel();

        //Positioning JPanel Data Properties.
        layout.addHeader(this, pPosition, bPosition, lPosition,
                         sPosition, MessagesGED.position, position);
        bPosition.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pPosition.setVisible(!bPosition.isSelected());
                HidePolyline.checkAllButtons();
            }
        });
    }

    /**
    * It has all the components of the section Position.
    */
    private void components() {
        lMarkSizeUnit = new JLabel();
        cMarkSizeUnit = new JComboBox();
        lShiftX = new JLabel();
        cShiftX = new JLabel();
        bShiftX = new JButton();
        pShiftX = new JPanel();
        lShiftY = new JLabel();
        cShiftY = new JLabel();
        bShiftY = new JButton();
        pShiftY = new JPanel();
        lShiftZ = new JLabel();
        cShiftZ = new JLabel();
        bShiftZ = new JButton();
        pShiftZ = new JPanel();
        shiftDialog = new JDialog();
        shiftScroll = new JScrollPane();
        shiftTable = new JTable();
        refresh = new JButton();
        ok = new JButton();
        int ROW = 0;

        //Components of the property: Mark Size Unit.
        layout.addJLabel(pPosition, lMarkSizeUnit, MessagesGED.mark_size_unit, 0, ROW, LEFTMARGIN);
        layout.addJComboBox(pPosition, cMarkSizeUnit,
                new String[] {MessagesGED.point, MessagesGED.tabulated},
                1, ROW);
        ROW++;
        cMarkSizeUnit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentpolyline, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__,
                        cMarkSizeUnit.getSelectedIndex());
            }
        });

        //Components of the property: X Shift.
        layout.addJLabel(pPosition, lShiftX, MessagesGED.x_shift, 0, ROW, LEFTMARGIN);
        layout.addDataField(pPosition, pShiftX, bShiftX, cShiftX, 1, ROW, currentpolyline);
        bShiftX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateShiftTable(0);
                shiftDialog.setVisible(true);
            }
        });
        ROW++;

        //Components of the property: Y Shift.
        layout.addJLabel(pPosition, lShiftY, MessagesGED.y_shift, 0, ROW, LEFTMARGIN);
        layout.addDataField(pPosition, pShiftY, bShiftY, cShiftY, 1, ROW, currentpolyline);
        bShiftY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateShiftTable(1);
                shiftDialog.setVisible(true);
            }
        });
        ROW++;

        //Components of the property: Z Shift.
        layout.addJLabel(pPosition, lShiftZ, MessagesGED.z_shift, 0, ROW, LEFTMARGIN);
        layout.addDataField(pPosition, pShiftZ, bShiftZ, cShiftZ, 1, ROW, currentpolyline);
        bShiftZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateShiftTable(2);
                shiftDialog.setVisible(true);
            }
        });
    }

    /**
     * Implement Shift Editor
     */
    private void shiftDialog() {
        layout.addShiftDialog(shiftDialog, shiftScroll, shiftTable, refresh, ok);
        shiftDialog.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent evt) {
                dataTableDialogWindowClosing(evt);
            }

            private void dataTableDialogWindowClosing(WindowEvent evt) {
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
    * Loads the current properties of group: Position.
    * @param objectID Enters the identification of polyline.
    */
    private void values(String objectID) {
        if (objectID != null) {
            currentpolyline = objectID;

            //Get the current status of the property: Mark Size Unit
            cMarkSizeUnit.setSelectedIndex((Integer) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__));

            //Get the current status of the property: Shift
            updateShiftTable(0);
            updateShiftTable(1);
            updateShiftTable(2);
        }
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
        switch(axis) {
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
        switch(axis) {
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
            switch(axis) {
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
            for (int i=0; i<point.length; i++){
                data[i][0] = point[i];
                data[i][1] = shift[i];
            }
        } else {
            for (int i=0; i<point.length; i++){
                data[i][0] = point[i];
                data[i][1] = 0;
            }
        }
        return data;
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pPosition.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pPosition.setVisible(visible);
        bPosition.setSelected(!visible);
    }
}