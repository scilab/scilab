/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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
package org.scilab.modules.gui.ged.graphic_objects.properties;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* ClippableContouredObject properties: CLIPBOX, CLIPSTATE
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class ClippableContouredObject extends ContentLayout {
    private JComboBox cClipState;
    private JTextField cClipBox;
    private JTextField cClipBoxUpper;
    private JTextField cClipBoxPoint;
    private JTextField cClipBoxWidth;
    private JTextField cClipBoxHeight;

    /**
     * Components of the property: Clip State.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void clipState(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lClipState = new JLabel();
        cClipState = new JComboBox();
        addLabelComboBox(panel, lClipState, MessagesGED.clip_state,
                         cClipState, new String[] {MessagesGED.off, MessagesGED.clipgrf, MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cClipState.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_CLIP_STATE__,
                    cClipState.getSelectedIndex());
            }
        });
        // Get the current status of the property: Clip State
        cClipState.setSelectedIndex((Integer) GraphicController.getController()
                                    .getProperty(UID, GraphicObjectProperties.__GO_CLIP_STATE__));
    }

    /**
     * Components of the property: Clip Box.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void clipBox(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        final JPanel pClipBox = new JPanel();
        final JToggleButton bClipBox = new JToggleButton();
        JLabel lClipBox = new JLabel();
        JLabel lClipBoxUpper = new JLabel();
        JLabel lClipBoxPoint = new JLabel();
        JLabel lClipBoxWidth = new JLabel();
        JLabel lClipBoxHeight = new JLabel();
        cClipBox = new JTextField();
        cClipBoxUpper = new JTextField();
        cClipBoxPoint = new JTextField();
        cClipBoxWidth = new JTextField();
        cClipBoxHeight = new JTextField();

        addInnerPanel(panel, pClipBox, bClipBox, lClipBox, cClipBox, MessagesGED.clip_box, ROW);
        bClipBox.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pClipBox.setVisible(!bClipBox.isSelected());
            }
        });
        int rowClipBox = 0;
        //Clip Box Upper
        addLabelTextField(pClipBox, lClipBoxUpper, MessagesGED.upper_left,
                          cClipBoxUpper, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxUpper.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox(UID);
            }
        });
        cClipBoxUpper.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox(UID);
            }
        });
        //Clip Box Point
        addLabelTextField(pClipBox, lClipBoxPoint, MessagesGED.point,
                          cClipBoxPoint, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxPoint.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox(UID);
            }
        });
        cClipBoxPoint.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox(UID);
            }
        });
        //Clip Box Width
        addLabelTextField(pClipBox, lClipBoxWidth, MessagesGED.width,
                          cClipBoxWidth, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox(UID);
            }
        });
        cClipBoxWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox(UID);
            }
        });
        //Clip Box Height
        addLabelTextField(pClipBox, lClipBoxHeight, MessagesGED.height,
                          cClipBoxHeight, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxHeight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox(UID);
            }
        });
        cClipBoxHeight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox(UID);
            }
        });
        getClipBox(UID);
    }

    /**
    * Inserts the current situation of the clip box.
    */
    private void titleClipBox() {
        String titleUpper = cClipBoxUpper.getText();
        String titlePoint = cClipBoxPoint.getText();
        String titleWidth = cClipBoxWidth.getText();
        String titleHeight = cClipBoxHeight.getText();
        String titleClipBox = titleUpper + " , " + titlePoint +
                              " , " + titleWidth + " , " + titleHeight;
        cClipBox.setText(" [" + titleClipBox + "]");
    }

    /**
    * Get status of the property: Clip Box.
    */
    private void getClipBox(Integer UID) {
        Double[] currentClipBox = (Double []) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_CLIP_BOX__);
        cClipBoxUpper.setText(currentClipBox[0].toString());
        cClipBoxPoint.setText(currentClipBox[1].toString());
        cClipBoxWidth.setText(currentClipBox[2].toString());
        cClipBoxHeight.setText(currentClipBox[3].toString());
        titleClipBox();
    }

    /**
    * Updates the property: Clip Box.
    */
    private void updateClipBox(Integer UID) {
        try {
            if (cClipState.getSelectedIndex() == 0) {
                cClipState.setSelectedIndex(2);
            }
            Double[] value = new Double[4];
            value[0] = Double.parseDouble(cClipBoxUpper.getText());
            value[1] = Double.parseDouble(cClipBoxPoint.getText());
            value[2] = Double.parseDouble(cClipBoxWidth.getText());
            value[3] = Double.parseDouble(cClipBoxHeight.getText());
            GraphicController.getController()
            .setProperty(UID, GraphicObjectProperties.__GO_CLIP_BOX__, value);
            titleClipBox();
        } catch (NumberFormatException e) {
            getClipBox(UID);
        }
    }
}