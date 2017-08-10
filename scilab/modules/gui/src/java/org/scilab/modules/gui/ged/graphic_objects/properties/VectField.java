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

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* VectField properties: ARROWSIZE
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class VectField extends ContentLayout {

    /**
     * Components of the property: Arrow Size
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void arrowSize(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lArrowSize = new JLabel();
        final JTextField cArrowSize = new JTextField();
        addLabelTextField(panel, lArrowSize, MessagesGED.arrow_size,
                          cArrowSize, true, LEFTMARGIN, COLUMN, ROW);
        cArrowSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setArrowSize(cArrowSize, UID);
            }
        });
        cArrowSize.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setArrowSize(cArrowSize, UID);
            }
        });
        // Get the current status of the property: Arrow Size
        cArrowSize.setText(Double.toString((Double) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_ARROW_SIZE__)));
    }

    /**
    * Set Arrow Size
    * @param size arrow size.
    */
    private void setArrowSize(JTextField cArrowSize, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_ARROW_SIZE__,
                Double.parseDouble(cArrowSize.getText()));
        } catch (NumberFormatException e) {
            cArrowSize.setText(Double.toString((Double) GraphicController.getController()
                                               .getProperty(UID, GraphicObjectProperties.__GO_ARROW_SIZE__)));
        }
    }
}