/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

public class UINodeList extends UIComponent {

    protected List<UINode> list;

    public UINodeList(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        list = new ArrayList<UINode>();

        return list;
    }

    public void add(UIComponent comp) throws UIWidgetException {
        if (comp instanceof UINode) {
            list.add((UINode) comp);
        }
    }
}