/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import javax.swing.JTabbedPane;

import org.scilab.modules.gui.SwingViewObject;

public class SwingScilabTabGroup extends JTabbedPane implements SwingViewObject{
    private Integer id;
    
    public SwingScilabTabGroup() {
        super();
    }
    
    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getId() {
        return id;
    }

    public void update(int property, Object value) {
        if (property == __GO_VISIBLE__) {
            setVisible((Boolean) value);
        }
    }


}
