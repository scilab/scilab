/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * This object is used to map an element ID to an UIElement
 * It is used when we want to access a Java object from Scilab which knows an object just thru its ID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
