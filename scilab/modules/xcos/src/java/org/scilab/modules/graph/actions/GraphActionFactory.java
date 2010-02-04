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

package org.scilab.modules.graph.actions;

import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Map;
import java.util.Set;

import org.scilab.modules.graph.ScilabGraph;

/**
 * Implement construction methods for Actions. 
 */
public final class GraphActionFactory {
	private static Map<ScilabGraph, Set<DefaultAction>> perGraphAction = 
		new Hashtable<ScilabGraph, Set<DefaultAction>>();
	private static Set<DefaultAction> nullGraphAction = new HashSet<DefaultAction>();

	/**
	 * Static class so private constructor 
	 */
	private GraphActionFactory() { }
	
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
	 * @return the instance or null on error.
	 */
	public static DefaultAction getInstance(ScilabGraph graph, Class< ? extends DefaultAction> action) {
		Set<DefaultAction> actionSet = getActionSet(graph);
		
		for (DefaultAction defaultAction : actionSet) {
			if (defaultAction.getClass() == action) {
				return defaultAction;
			}
		}
		
		DefaultAction instance = null;
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
	 * @return the instance or null if not found.
	 */
	public static DefaultAction get(ScilabGraph graph, Class< ? extends DefaultAction> action) {
		Set<DefaultAction> actionSet = getActionSet(graph);

		for (DefaultAction defaultAction : actionSet) {
			if (defaultAction.getClass() == action) {
				return defaultAction;
			}
		}
			
		return null;
	}
}
