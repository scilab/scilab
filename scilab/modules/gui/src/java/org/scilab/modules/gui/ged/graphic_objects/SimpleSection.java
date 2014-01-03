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
package org.scilab.modules.gui.ged.graphic_objects;

import javax.swing.JPanel;
import javax.swing.JToggleButton;
import org.scilab.modules.gui.ged.ContentLayout;

/**
* Interface for sections of property list (GED).
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public abstract class SimpleSection extends ContentLayout {
    private JToggleButton bSection;
    private JPanel pSection;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param sectionName name of section
    * @param objectName name of object
    */
    public SimpleSection(String sectionName, String objectName) {
        bSection = new JToggleButton();
        pSection = new JPanel();
        initMainPanel(sectionName, objectName);
    }

    /**
    * Insert show/hide button, title and main JPanel of group.
    * @param sectionName name of section
    * @param objectName name of object
    */
    private void initMainPanel(String sectionName, String objectName) {
        this.addHeader(this, pSection, bSection, sectionName, objectName);
    }

    /**
    * Initilialize the components of the section.
    * @param objectID uid.
    */
    public abstract void initComponents(Integer objectID);

    /**
    * Set Jpanel of Section.
    * @param panel JPanel
    */
    public void setSectionPanel(JPanel panel) {
        pSection = panel;
    }

    /**
    * Get Jpanel of Section.
    * @return jpanel
    */
    public JPanel getSectionPanel() {
        return pSection;
    }

    /**
    * Set Show/Hide Button of Section.
    * @param button JToggleButton
    */
    public void setSectionButton(JToggleButton button) {
        bSection = button;
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public boolean getStatus() {
        return pSection.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public void setVisibility(boolean visible) {
        pSection.setVisible(visible);
        bSection.setSelected(!visible);
    }
}