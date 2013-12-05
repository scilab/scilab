/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
* Champ properties:
* Colored
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Champ extends ContentLayout {

    /**
     * Components of the property: Colored
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID objectID
     */
    public void colored(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lColored = new JLabel();
        final JComboBox cColored = new JComboBox();
        addLabelComboBox(panel, lColored, MessagesGED.colored,
                         cColored, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cColored.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_COLORED__,
                    cColored.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Colored
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_COLORED__);
        cColored.setSelectedIndex(enable ? 1 : 0);
    }
}