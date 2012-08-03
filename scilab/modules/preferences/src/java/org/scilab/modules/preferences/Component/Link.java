/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.preferences.Component;

import java.awt.Color;
import java.awt.Cursor;

import org.scilab.modules.preferences.XConfigManager;

import org.w3c.dom.Node;

/**
 * Implementation of Link compliant with extended management.
 * @author Vincent COUVERT
 */
public class Link extends Label {

    private static final long serialVersionUID = -8915028261763299611L;

    /**
     * Constructor.
     * @param peer : associated view DOM node.
     */
    public Link(final Node peer) {
        super(peer);
        setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        refresh(peer);
    }

    /**
     * Refresh the component by the use of actuators.
     * @param peer the corresponding view DOM node
     */
    public void refresh(final Node peer) {
        super.refresh(peer);
        /* Force text to be underlined */
        String text = "<HTML><U>" + XConfigManager.getAttribute(peer , "text") + "</U></HTML>";
        if (!text.equals(text())) {
            text(text);
        }

        String enable = XConfigManager.getAttribute(peer , "enable", "true");
        if (enable.equals("true")) {
            setForeground(Color.BLUE);
        } else {
            if (getParent() != null) {
                setForeground(getParent().getBackground().darker());
            }
        }
    }
}

