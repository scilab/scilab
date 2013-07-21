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
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;
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
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends Polyline implements SimpleSection {
    private String currentpolyline;
    private ContentLayout layout = new ContentLayout();
    private int LEFTMARGIN = 0;
    private int LEFTCOLUMN = 1;
    private int RIGHTCOLUMN = 2;

    private static JToggleButton bDataProperties;
    private JLabel lDataProperties;
    private JSeparator sDataProperties;
    private static JPanel pDataProperties;
    private JToggleButton bClipBox;
    private JLabel lClipBox;
    private JTextField cClipBox;
    private JPanel pClipBox;
    private JLabel lClipBoxUpper;
    private JTextField cClipBoxUpper;
    private JLabel lClipBoxPoint;
    private JTextField cClipBoxPoint;
    private JLabel lClipBoxWidth;
    private JTextField cClipBoxWidth;
    private JLabel lClipBoxHeight;
    private JTextField cClipBoxHeight;
    private JLabel lClipState;
    private JComboBox cClipState;
    private JLabel lTag;
    private JTextField cTag;
    private JLabel lData;
    private JPanel pData;
    private JButton bData;
    private JLabel cData;
    private JDialog dataTableDialog;
    private JScrollPane dataScroll;
    private JTable dataTable;
    private JButton append;
    private JButton delete;
    private JButton refresh;
    private JButton ok;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
    */
    public DataProperties(String objectID) {
        currentpolyline = objectID;
        insertBase();
        components();
        dataDialog();
        values(objectID);
    }

    /**
    * Insert show/hide button, title and main JPanel of group.
    */
    @Override
    public final void insertBase() {
        String SECTIONNAME = MessagesGED.data_properties;
        this.setName(SECTIONNAME);
        bDataProperties = new JToggleButton();
        lDataProperties = new JLabel();
        sDataProperties = new JSeparator();
        pDataProperties = new JPanel();

        //Positioning JPanel Data Properties.
        layout.addHeader(this, pDataProperties, bDataProperties, lDataProperties, sDataProperties, SECTIONNAME);
        bDataProperties.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pDataProperties.setVisible(!bDataProperties.isSelected());
                HidePolyline.checkAllButtons();
            }
        });
    }

    /**
    * It has all the components of the section Data Properties.
    */
    @Override
    public final void components() {
        bClipBox = new JToggleButton();
        lClipBox = new JLabel();
        cClipBox = new JTextField();
        pClipBox = new JPanel();
        lClipBoxUpper = new JLabel();
        cClipBoxUpper = new JTextField();
        lClipBoxPoint = new JLabel();
        cClipBoxPoint = new JTextField();
        lClipBoxWidth = new JLabel();
        cClipBoxWidth = new JTextField();
        lClipBoxHeight = new JLabel();
        cClipBoxHeight = new JTextField();
        lClipState = new JLabel();
        cClipState = new JComboBox();
        lTag = new JLabel();
        cTag = new JTextField();
        lData = new JLabel();
        pData = new JPanel();
        bData = new JButton();
        cData = new JLabel();
        dataTableDialog = new JDialog();
        dataScroll = new JScrollPane();
        dataTable = new JTable();
        append = new JButton();
        delete = new JButton();
        refresh = new JButton();
        ok = new JButton();
        int ROW = 0;

        //Components of the property: Clip State.
        layout.addJLabel(pDataProperties, lClipState, MessagesGED.clip_state, LEFTCOLUMN, ROW, LEFTMARGIN);
        layout.addJComboBox(pDataProperties, cClipState,
                new String[] {MessagesGED.off, MessagesGED.clipgrf, MessagesGED.on},
                RIGHTCOLUMN, ROW);
        cClipState.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentpolyline, GraphicObjectProperties.__GO_CLIP_STATE__,
                        cClipState.getSelectedIndex());
            }
        });
        ROW++;

        //Components of the property: Clip Box.
        layout.addInnerPanel(pDataProperties, pClipBox, bClipBox, lClipBox, cClipBox, MessagesGED.clip_box, ROW);
        ROW=ROW+2;
        bClipBox.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pClipBox.setVisible(!bClipBox.isSelected());
                HidePolyline.checkAllButtons();
            }
        });
        int rowClipBox = 0;
        int LCClipBox = 0; //left column - clip box
        int RCClipBox = 1; //rigth column - clip box
        int LMClipBox = 0; //left margin - clip box
        //Clip Box Upper
        layout.addJLabel(pClipBox, lClipBoxUpper, MessagesGED.upper_left, LCClipBox, rowClipBox, LMClipBox);
        layout.addJTextField(pClipBox, cClipBoxUpper, true, RCClipBox, rowClipBox++);
        cClipBoxUpper.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxUpper.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });
        //Clip Box Point
        layout.addJLabel(pClipBox, lClipBoxPoint, MessagesGED.point, LCClipBox, rowClipBox, LMClipBox);
        layout.addJTextField(pClipBox, cClipBoxPoint, true, RCClipBox, rowClipBox++);
        cClipBoxPoint.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxPoint.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });
        //Clip Box Width
        layout.addJLabel(pClipBox, lClipBoxWidth, MessagesGED.width, LCClipBox, rowClipBox, LMClipBox);
        layout.addJTextField(pClipBox, cClipBoxWidth, true, RCClipBox, rowClipBox++);
        cClipBoxWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });
        //Clip Box Height
        layout.addJLabel(pClipBox, lClipBoxHeight, MessagesGED.height, LCClipBox, rowClipBox, LMClipBox);
        layout.addJTextField(pClipBox, cClipBoxHeight, true, RCClipBox, rowClipBox);
        cClipBoxHeight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxHeight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });

        //Components of the property: Data.
        layout.addJLabel(pDataProperties, lData, MessagesGED.data, 1, ROW, LEFTMARGIN);
        layout.addDataField(pDataProperties, pData, bData, cData, 2, ROW, currentpolyline);
        bData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateDataTable();
                dataTableDialog.setVisible(true);
            }
        });
        ROW++;

        //Components of the property: Tag.
        layout.addJLabel(pDataProperties, lTag, MessagesGED.tag, LEFTCOLUMN, ROW, LEFTMARGIN);
        layout.addJTextField(pDataProperties, cTag, true, RIGHTCOLUMN, ROW);
        cTag.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTag();
            }
        });
        cTag.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTag();
            }
        });
    }

    /**
     * Implement Data Editor.
     */
    private void dataDialog() {
        layout.addDataDialog(dataTableDialog, dataScroll, dataTable, append, delete, refresh, ok, currentpolyline);
        dataTableDialog.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent evt) {
                dataTableDialogWindowClosing(evt);
            }

            private void dataTableDialogWindowClosing(WindowEvent evt) {
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
    * Loads the current properties of group: Data Properties.
    * @param objectID Enters the identification of polyline.
    */
    @Override
    public final void values(String objectID) {
        if (objectID != null) {
            currentpolyline = objectID;

            //Get the current status of the property: Clip Box
            Double[] currentClipBox = (Double []) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_CLIP_BOX__);
            cClipBoxUpper.setText(currentClipBox[0].toString());
            cClipBoxPoint.setText(currentClipBox[1].toString());
            cClipBoxWidth.setText(currentClipBox[2].toString());
            cClipBoxHeight.setText(currentClipBox[3].toString());
            titleClipBox();

            // Get the current status of the property: Clip State
            cClipState.setSelectedIndex((Integer) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_CLIP_STATE__));

            // Get the current status of the property: Data
            updateDataTable();

            // Get the current status of the property: Tag
            cTag.setText((String) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_TAG__));
        }
    }

    /**
    * Inserts the current situation of the clip box.
    */
    public void titleClipBox() {
        String titleUpper = cClipBoxUpper.getText();
        String titlePoint = cClipBoxPoint.getText();
        String titleWidth = cClipBoxWidth.getText();
        String titleHeight = cClipBoxHeight.getText();
        String titleClipBox = titleUpper + " , " + titlePoint +
                              " , " + titleWidth + " , " + titleHeight;
        cClipBox.setText(" [" + titleClipBox + "]");
    }

    /**
    * Updates the property: Clip Box.
    * upper:0  point:1  width:2 height: 3
    */
    private void updateClipBox() {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cClipBoxUpper.getText());
        value[1] = Double.parseDouble(cClipBoxPoint.getText());
        value[2] = Double.parseDouble(cClipBoxWidth.getText());
        value[3] = Double.parseDouble(cClipBoxHeight.getText());
        GraphicController.getController()
                .setProperty(currentpolyline, GraphicObjectProperties.__GO_CLIP_BOX__, value);
        titleClipBox();
    }

    /**
    * Get all data from a polyline.
    */
    public Object[][] getData() {
        double[] dataX = (double[]) PolylineData.getDataX(currentpolyline);
        double[] dataY = (double[]) PolylineData.getDataY(currentpolyline);
        double[] dataZ = (double[]) PolylineData.getDataZ(currentpolyline);
        Object[][] data = new Object[dataX.length][3];
        for (int i=0; i<dataX.length; i++){
            data[i][0] = dataX[i];
            data[i][1] = dataY[i];
            data[i][2] = dataZ[i];
        }
        return data;
    }

    /**
    * Checks if the polyline has 3 dimensions.
    */
    private boolean is3D(Object[][] data) {
        boolean is3D = false;
        for (int i=0; i<data.length; i++) {
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
        if(dataTable.getSelectedRow() != -1) {
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
               PolylineData.setPointValue(currentpolyline, dataTable.getSelectedRow(), (Double)xValue, (Double)yValue, 0.0);
        }
    }

    /**
    * Updates the property: Tag.
    */
    private void updateTag() {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_TAG__,
                cTag.getText()
                );
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pDataProperties.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pDataProperties.setVisible(visible);
        bDataProperties.setSelected(!visible);
    }
}