/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.graph.actions.base;

import java.lang.reflect.InvocationTargetException;
import java.util.Collections;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.WeakHashMap;

import javax.swing.Action;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;

/**
 * Implement construction methods for Actions.
 */
public final class GraphActionManager {
    private static Map<ScilabGraph, Set<DefaultAction>> perGraphAction = new WeakHashMap<ScilabGraph, Set<DefaultAction>>();
    private static Set<DefaultAction> nullGraphAction = Collections.newSetFromMap(new WeakHashMap<DefaultAction, Boolean>());

    /**
     * Static class so private constructor
     */
    private GraphActionManager() {
    }

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
     * @param <T>
     *            the type to work with.
     * @return the instance or null on error.
     */
    public static <T extends DefaultAction> T getInstance(ScilabGraph graph, Class<T> action) {
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
     *
     * @param graph
     *            the graph where to search
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
                actionSet = Collections.newSetFromMap(new WeakHashMap<DefaultAction, Boolean>());
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
     * @param <T>
     *            the type to work with.
     * @return the instance or null if not found.
     */
    public static <T extends DefaultAction> T get(ScilabGraph graph, Class<T> action) {
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
     *
     * @param actionKlass
     *            The action type to enable
     * @param enable
     *            the enable status
     */
    public static void setEnable(Class <? extends DefaultAction > actionKlass, boolean enable) {
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
     *
     * @param actionKlass
     *            The action type to enable
     * @return the status (true if all action are enabled, false otherwise)
     */
    public static boolean getEnable(Class <? extends DefaultAction > actionKlass) {
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

    /**
     * Update the {@link KeyStroke} of all actions of class.
     *
     * @param actionKlass
     *            the action class to update
     * @param key
     *            the {@link KeyStroke} to set
     */
    public static void updateActionKeyStroke(Class <? extends DefaultAction > actionKlass, KeyStroke key) {
        /*
         * Per graph action
         */
        final Set<Entry<ScilabGraph, Set<DefaultAction>>> entrySet = perGraphAction.entrySet();
        for (Entry<ScilabGraph, Set<DefaultAction>> entry : entrySet) {
            setKeyStroke(entry.getValue(), actionKlass, key);
        }

        /*
         * Out of graphs actions
         */
        setKeyStroke(nullGraphAction, actionKlass, key);
    }

    private static final void setKeyStroke(final Set<DefaultAction> actions, final Class <? extends DefaultAction > actionKlass, final KeyStroke key) {
        for (final DefaultAction a : actions) {
            if (a.getClass() == actionKlass) {
                a.putValue(Action.ACCELERATOR_KEY, key);
            }
        }
    }
}
