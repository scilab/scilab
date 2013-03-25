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
package org.scilab.modules.gui.ged.graphic_objects.surface.grayplot;

import javax.swing.JPanel;

/**
 * Highest class - Prepares methods dependent on JPanel.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class Roof extends JPanel {

    /**
    * Build methods of overwriting.
    */
    public Roof() {
        basePropertiesComponents();
        initLayout();
    }

    /**
    * It has all the components of the section Base Properties.
    */
    public void basePropertiesComponents() { };

    /**
    * Creates layout of all sections(Base Proprerties and Style/Appareance).
    */
    public void initLayout() { };
}