/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * This object is used to map an element ID to an UIElement
 * It is used when we want to access a Java object from Scilab which knows an object just thru its ID
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

package org.scilab.modules.gui.utils;

import java.util.HashMap;

import org.scilab.modules.gui.uielement.UIElement;

/**
 * This object is used to map an element ID to an UIElement
 * It is used when we want to access a Java object from Scilab which knows an object just thru its ID
 * @author Vincent COUVERT
 */
public final class UIElementMapper {

    /**
     * The hashmap containing the mapping between UIElement id (int) and UIElement
     */
    private static HashMap<Integer, UIElement> mapper = new HashMap<Integer, UIElement>();

    /**
     * The highest Id already used to map an UIElement
     */
    private static int maxId;

    /**
     * Default constructor
     * This is a utility class, there should not be any instance
     */
    private UIElementMapper() {	}

    /**
     * Add a new UIElement
     * @param uielement corresponding UIelement
     * @return uielementId id of the UIElement
     */
    public static int add(UIElement uielement) {
        maxId++;
        mapper.put(maxId, uielement);
        return maxId;
    }

    /**
     * Remove a UIElement from the mapping
     * @param uielementId id of the UIElement
     */
    public static void removeMapping(int uielementId) {
        mapper.remove(uielementId);
    }

    /**
     * Get the element corresponding an ID
     * @param uielementId id of the UIElement
     * @return the corresponding element
     */
    public static UIElement getCorrespondingUIElement(int uielementId) {
        return mapper.get(uielementId);
    }
}
