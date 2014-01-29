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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_SCROLL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import javax.swing.JTabbedPane;

import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.gui.SwingViewObject;

public class SwingScilabTabGroup extends JTabbedPane implements SwingViewObject {
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

        switch (property) {
            case __GO_VISIBLE__ :
                setVisible((Boolean) value);
                break;
            case __GO_UI_TITLE_POSITION__ :
                Integer pos = (Integer)value;
                switch (Uicontrol.TitlePositionType.intToEnum(pos)) {
                    case BOTTOM:
                        setTabPlacement(JTabbedPane.BOTTOM);
                        break;
                    case LEFT:
                        setTabPlacement(JTabbedPane.LEFT);
                        break;
                    case RIGHT:
                        setTabPlacement(JTabbedPane.RIGHT);
                        break;
                    case TOP:
                    default:
                        setTabPlacement(JTabbedPane.TOP);
                        break;
                }
                break;
            case __GO_UI_TITLE_SCROLL__ :
                if ((Boolean)value) {
                    setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);
                } else {
                    setTabLayoutPolicy(JTabbedPane.WRAP_TAB_LAYOUT);
                }
                break;
        }
    }


}
