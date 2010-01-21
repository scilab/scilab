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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
	 * HashMap associated to this mapper
	 */
	private static class UIElementMap extends HashMap<Integer, UIElement> {	

		private static final long serialVersionUID = 1L;

		/**
		 * Default Constructor
		 */
		public UIElementMap() {
			super();
		}
	}
	
	/**
	 * ID for an UIElement which in not yet in the UIElementMapper (not yet created)
	 */
	private static final int DEFAULT_UIELEMENT_ID = -1;
	
	/**
	 * The hashmap containing the mapping between UIElement id (int) and UIElement
	 */
	private static UIElementMap mapper = new UIElementMap();
	
	/**
	 * The highest Id already used to map an UIElement
	 */
	private static int maxId;
	
	/**
	 * The ID of the tab which contains the console (known as root object or 0 from Scilab)
	 */
	private static int consoleId;
	
	/**
	 * Default constructor
	 * This is a utility class, there should not be any instance
	 */
	private UIElementMapper() {	}
	
	/**
	 * Add a new mapping between a UIElement and its Id
	 * @param uielementId id of the UIElement
	 * @param uielement corresponding UIelement
	 */
	public static void addMapping(int uielementId, UIElement uielement) {
		mapper.put(uielementId, uielement);
	}
	
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
	
	/**
	 * Returns true if the UIElement already exists in the hashmap
	 * @param uielement UIElement object to be searched
	 * @return true if the figure already exists in the hashmap, false otherwise
	 */
	public static boolean containsUIElement(UIElement uielement) {
		return mapper.containsValue(uielement);
	}
	
	/**
	 * Returns true if the figure already exists in the hashmap
	 * @return last Id associated to an UIElement
	 */
	public static int getMaxId() {
		return maxId;
	}
	
	/**
	 * Get the default id for an UIElement
	 * @return the id for a not created UIElement (not in the UIElementMapper)
	 */
	public static int getDefaultId() {
		return DEFAULT_UIELEMENT_ID;
	}
	
	/**
	 * Set the console id for current Scilab sessions
	 * @param id the id of the tab which contains the console
	 */
	public static void setConsoleId(int id) {
		consoleId = id;
	}
	
	/**
	 * Get the console id for current Scilab sessions
	 * @return id the id of the tab which contains the console
	 */
	public static int getConsoleId() {
		return consoleId;
	}
}
