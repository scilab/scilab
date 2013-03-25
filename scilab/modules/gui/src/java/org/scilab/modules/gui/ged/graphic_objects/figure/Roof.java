/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.figure;

import javax.swing.JPanel;

/**
 * Highest class - Prepares methods dependent on JPanel.
 * @author cardinot
 */
public class Roof extends JPanel {

    /**
    * Build methods of overwriting.
    */
    public Roof() {
        basePropertiesComponents();
        controlComponents();
        dataComponents();
        styleComponents();
        initLayout();
        dialogColor();
    }

    /**
    * It has all the components of the section Base Properties.
    */
    public void basePropertiesComponents() { };

    /**
    * It has all the components of the section Control.
    */
    public void controlComponents() { };

    /**
    * It has all the components of the section Data.
    */
    public void dataComponents() { };

    /**
    * It has all the components of the section Style/Appareance.
    */
    public void styleComponents() { };

    /**
    * Creates layout of all sections.
    */
    public void initLayout() { };

    /**
    * JDialog - Selection of background colors.
    */
    public void dialogColor() { };
}