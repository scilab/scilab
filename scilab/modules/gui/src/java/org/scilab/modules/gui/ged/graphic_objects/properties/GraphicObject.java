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
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* GraphicObject properties: TAG, USERDATA, VISIBLE
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class GraphicObject extends ContentLayout {

    /**
     * Components of the property: Tag.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void tag(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lTag = new JLabel();
        final JTextField cTag = new JTextField();
        addLabelTextField(panel, lTag, MessagesGED.tag,
                          cTag, true, LEFTMARGIN, COLUMN, ROW++);
        cTag.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController()
                .setProperty(UID, GraphicObjectProperties.__GO_TAG__, cTag.getText());
            }
        });
        cTag.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                GraphicController.getController()
                .setProperty(UID, GraphicObjectProperties.__GO_TAG__, cTag.getText());
            }
        });

        // Get the current status of the property: Tag
        String tag = (String) GraphicController.getController()
                     .getProperty(UID, GraphicObjectProperties.__GO_TAG__);
        cTag.setText(tag);
    }

    /**
     * Components of the property: User Data.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     */
    public void userData(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN) {
        /*
        JLabel lUserData = new JLabel();
        JLabel cUserData = new JLabel();
        JButton bUserData = new JButton();
        JPanel pUserData = new JPanel();
        addJLabel(panel, lUserData, MessagesGED.user_data, COLUMN, ROW, LEFTMARGIN);
        addDataField(panel, pUserData, bUserData, cUserData, COLUMN + 1, ROW);
        cUserData.setText("editvar");
        bUserData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                startEditVar();
            }
        });
        */
    }

    /**
     * Components of the property: Visible.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void visible(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        final JLabel lVisible = new JLabel();
        final JComboBox cVisible = new JComboBox();
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        addLabelComboBox(panel, lVisible, MessagesGED.visible,
                         cVisible, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cVisible.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_VISIBLE__,
                    cVisible.getSelectedIndex() == 0 ? false : true);
            }
        });

        // Get the current status of the property: Visible
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_VISIBLE__);
        cVisible.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Start the variable editor with USERDATA value.
     */
    public void startEditVar() {
        try {
            ScilabInterpreterManagement.asynchronousScilabExec(null,
                    "user_data_f1g2h3ncvaj01=gcf();"
                    + "user_data_f1g2h3ncvaj02=user_data_f1g2h3ncvaj01.user_data;"
                    + "editvar(\"user_data_f1g2h3ncvaj02\");");
        } catch (InterpreterException e1) {
            System.err.println("An error in the interpreter has been catched: " + e1.getLocalizedMessage());
        }
    }

    public final void updateUserData() {
        try {
            ScilabInterpreterManagement.asynchronousScilabExec(null,
                    "user_data_f1g2h3ncvaj01.user_data=user_data_f1g2h3ncvaj02;"
                    + "clear user_data_f1g2h3ncvaj01;"
                    + "clear user_data_f1g2h3ncvaj02;");
        } catch (InterpreterException e1) {
            System.err.println("An error in the interpreter has been catched: " + e1.getLocalizedMessage());
        }
    }
}