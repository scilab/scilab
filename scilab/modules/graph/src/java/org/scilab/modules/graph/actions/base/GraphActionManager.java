/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions.base;

import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Map;
import java.util.Set;

import org.scilab.modules.graph.ScilabGraph;

/**
 * Implement construction methods for Actions. 
 */
public final class GraphActionManager {
	private static Map<ScilabGraph, Set<DefaultAction>> perGraphAction = 
		new Hashtable<ScilabGraph, Set<DefaultAction>>();
	private static Set<DefaultAction> nullGraphAction = new HashSet<DefaultAction>();

	/**
	 * Static class so private constructor 
	 */
	private GraphActionManager() { }
	
	/**
	 * As each action is unique per graph, get the action instance for the
	 * graph.
	 * 
	 * If the action has never been instantiated, use the
	 * DefaultAction(ScilabGraph) constructor and put it on the set.
	 * 
	 * @param graph
	 *            the graph to work on
	 * @param action
	 *            the action class we are looking for.
	 * @param <T> the type to work with.
	 * @return the instance or null on error.
	 */
	public static < T extends DefaultAction> T getInstance(ScilabGraph graph, Class<T> action) {
		Set<DefaultAction> actionSet = getActionSet(graph);
		
		for (DefaultAction defaultAction : actionSet) {
			if (defaultAction.getClass() == action) {
				// Here we can safely cast the action according to the test.
				return (T) defaultAction;
			}
		}
		
		T instance = null;
		try {
			instance = action.getConstructor(ScilabGraph.class).newInstance(graph);
			actionSet.add(instance);
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
		
		return instance;
	}

	/**
	 * Get the action set attached to the graph
	 * @param graph the graph where to search
	 * @return the {@link DefaultAction} set
	 */
	private static Set<DefaultAction> getActionSet(ScilabGraph graph) {
		Set<DefaultAction> actionSet;
		if (graph == null) {
			actionSet = nullGraphAction;
		} else {
			if (perGraphAction.containsKey(graph)) {
				actionSet = perGraphAction.get(graph);
			} else {
				actionSet = new HashSet<DefaultAction>();
				perGraphAction.put(graph, actionSet);
			}
		}
		return actionSet;
	}
	
	/**
	 * As each action is unique per graph, get the action instance for the
	 * graph.
	 * 
	 * If the action has never been instantiated, return null.
	 * 
	 * @param graph
	 *            the graph to work on
	 * @param action
	 *            the action class we are looking for.
	 * @param <T> the type to work with.
	 * @return the instance or null if not found.
	 */
	public static  < T extends DefaultAction> T get(ScilabGraph graph, Class<T> action) {
		Set<DefaultAction> actionSet = getActionSet(graph);

		for (DefaultAction defaultAction : actionSet) {
			if (defaultAction.getClass() == action) {
				// Here we can safely cast the action according to the test.
				return (T) defaultAction;
			}
		}
			
		return null;
	}
	
	/**
	 * Enable or disable action on all registered graph.
	 * @param actionKlass The action type to enable
	 * @param enable the enable status
	 */
	public static void setEnable(Class< ? extends DefaultAction> actionKlass, boolean enable) {
		// Handle null graph
		for (DefaultAction action : nullGraphAction) {
			if (actionKlass.isInstance(action)) {
				action.setEnabled(enable);
			}
		}
		
		// Handle non-null graph
		for (Set<DefaultAction> actions : perGraphAction.values()) {
			for (DefaultAction action : actions) {
				if (actionKlass.isInstance(action)) {
					action.setEnabled(enable);
				}
			}
		}
	}
	
	/**
	 * Get status action on all registered graph.
	 * @param actionKlass The action type to enable
	 * @return the status (true if all action are enabled, false otherwise)
	 */
	public static boolean getEnable(Class< ? extends DefaultAction> actionKlass) {
		boolean result = true;
		
		// Handle null graph
		for (DefaultAction action : nullGraphAction) {
			if (actionKlass.isInstance(action)) {
				result &= action.isEnabled();
			}
		}
		
		// Handle non-null graph
		for (Set<DefaultAction> actions : perGraphAction.values()) {
			for (DefaultAction action : actions) {
				if (actionKlass.isInstance(action)) {
					result &= action.isEnabled();
				}
			}
		}
		
		return result;
	}
}
