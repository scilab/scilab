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
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Imageplot properties: DATAMAPPING
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Imageplot extends ContentLayout {

    /**
     * Components of the property: Data Mapping (only to Grayplot).
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void dataMapping(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lDataMapping = new JLabel();
        final JComboBox cDataMapping = new JComboBox();
        this.addLabelComboBox(panel, lDataMapping, MessagesGED.data_mapping,
                              cDataMapping, new String[] {MessagesGED.scaled, MessagesGED.direct},
                              LEFTMARGIN, COLUMN, ROW++);
        cDataMapping.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController()
                .setProperty(UID, GraphicObjectProperties.__GO_DATA_MAPPING__,
                             cDataMapping.getSelectedIndex());
            }
        });
        // Get the current status of the property: Data Mapping
        cDataMapping.setSelectedIndex((Integer) GraphicController.getController()
                                      .getProperty(UID, GraphicObjectProperties.__GO_DATA_MAPPING__));
    }
}