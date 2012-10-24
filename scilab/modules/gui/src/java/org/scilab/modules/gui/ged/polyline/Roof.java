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
package org.scilab.modules.gui.ged.polyline;

import java.awt.Dimension;
import org.scilab.modules.gui.ged.ContentLayout;

/**
 * Highest class - Prepares methods dependent on JPanel.
 * @author cardinot
 */
public class Roof extends ContentLayout {

    /**
    * Build methods of overwriting.
    */
    public Roof() {
        setPreferredSize(new Dimension(100, 800));
        
        basePropertiesComponents();
        dataPropertiesComponents();
        positionComponents();
        styleComponents();
        dialogBackgroundColor();
        dialogForegroundColor();
        dialogMarkBackground();
        dialogMarkForeground();
    }

    /**
    * It has all the components of the section Base Properties.
    */
    public void basePropertiesComponents() { };

    /**
    * It has all the components of the section Base Properties.
    */
    public void dataPropertiesComponents() { };

    /**
    * It has all the components of the section Position.
    */
    public void positionComponents() { };

    /**
    * It has all the components of the section Style/Appearance.
    */
    public void styleComponents() { };

    /**
    * JDialog - Selection of background colors.
    */
    public void dialogBackgroundColor() { };

    /**
    * JDialog - Selection of foreground colors.
    */
    public void dialogForegroundColor() { };

    /**
    * JDialog - Selection of mark background colors.
    */
    public void dialogMarkBackground() { };

    /**
    * JDialog - Selection of mark foreground colors.
    */
    public void dialogMarkForeground() { };
}
