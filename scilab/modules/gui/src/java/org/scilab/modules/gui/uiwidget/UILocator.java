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

package org.scilab.modules.gui.uiwidget;

import java.awt.Component;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

/**
 * Class to help to locate a component given its path composed of ids
 */
public final class UILocator {

    private static final Map<String, UIComponent> roots = new HashMap<String, UIComponent>();
    private static final Map<String, UIComponent> pathToUI = new HashMap<String, UIComponent>();
    private static final Map<Integer, UIComponent> uids = new HashMap<Integer, UIComponent>();

    /**
     * Clear the caches
     */
    public static void clearCache() {
        roots.clear();
        pathToUI.clear();
        uids.clear();
    }

    /**
     * Add a root component
     * @param ui the root
     */
    public static void addRoot(final UIComponent ui) {
        String id = ui.getId();
        if (id != null && !id.isEmpty()) {
            if (!roots.containsKey(id)) {
                roots.put(id, ui);
            }
        }
    }

    public static UIComponent findWindow(final UIComponent ui) {
        if (ui.getComponent() instanceof Component) {
            Component c = (Component) ui.getComponent();
            JFrame win = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, c);
            if (win != null) {
                for (Map.Entry<String, UIComponent> entry : roots.entrySet()) {
                    if (entry.getValue().component == win) {
                        return entry.getValue();
                    }
                }
            }
        }

        return null;
    }

    /**
     * Register a component by its uid
     * @param ui the component to register
     */
    public static void add(final UIComponent ui) {
        if (ui.isRoot()) {
            addRoot(ui);
        }
        uids.put(ui.getUid(), ui);
    }

    /**
     * Paths to access components are cached, so give a way to remove the paths pointing to
     * the component from the cache.
     * @param ui the component to remove from the paths cache
     */
    public static void removeFromCachedPaths(final UIComponent ui) {
        final String id = ui.getId();
        if (ui.isRoot() && id != null) {
            roots.remove(id);
        }
        List<String> toRemove = new LinkedList<String>();
        for (Map.Entry<String, UIComponent> entry : pathToUI.entrySet()) {
            if (entry.getValue() == ui) {
                toRemove.add(entry.getKey());
            }
        }

        for (String key : toRemove) {
            pathToUI.remove(key);
        }
    }

    /**
     * Test if an uid has been registered
     * @param uid the uid
     * @return true if the uid has been registered
     */
    public static boolean isValid(final int uid) {
        return uids.get(uid) != null;
    }

    /**
     * Remove a component from caches
     * @param ui the component to remove
     */
    public static void remove(final UIComponent ui) {
        removeFromCachedPaths(ui);
        uids.remove(ui.getUid());
    }

    /**
     * Remove all components
     */
    public static void removeAll() {
        UIComponent[] uis = (UIComponent[]) roots.keySet().toArray();
        for (UIComponent ui : uis) {
            remove(ui);
        }
    }

    /**
     * Get a component according to its uid
     * @param uid the uid
     * @return the corresponding component
     */
    public static UIComponent get(final int uid) {
        return uids.get(uid);
    }

    /**
     * Get the roots component
     * @return an array containing the root components.
     */
    public static UIComponent[] getRoots() {
        Collection<UIComponent> values = roots.values();
        return values.toArray(new UIComponent[values.size()]);
    }

    /**
     * Get a component corresponding to the given path.
     * The path is constituted of ids separated by '/'.
     * It is possible to use '..' and '*' to make the navigation easier.
     * @param path a path
     * @return the corresponding component.
     */
    public static UIComponent get(final String path) {
        boolean putInCache = true;
        UIComponent ui = pathToUI.get(path);
        if (ui != null) {
            return ui;
        }

        List<String> pathElements = splitPath(path);
        if (pathElements.isEmpty()) {
            return null;
        }

        String base = pathElements.get(0);
        UIComponent root = roots.get(base);

        if (root == null) {
            // Not a root element: we look after it in the tree
            root = search(base, roots);
            if (root == null) {
                return null;
            }
        }

        for (int i = 1; i < pathElements.size(); i++) {
            String el = pathElements.get(i);
            if (el.equals("..")) {
                root = root.parent;
            } else if (el.equals("*")) {
                if (i != pathElements.size() - 1) {
                    el = pathElements.get(++i);
                    if (root.children != null) {
                        root = search(el, root.children);
                    } else {
                        return null;
                    }
                } else {
                    root = null;
                }
            } else if (!el.equals(".")) {
                final UIComponent prev = root;
                if (root.children != null) {
                    root = root.children.get(el);
                } else {
                    root = null;
                }
                if (root == null) {
                    int n;
                    try {
                        n = Integer.parseInt(el) - 1;
                        if (prev.childrenList != null && n >= 0 && n < prev.childrenList.size()) {
                            root = prev.childrenList.get(n);
                            // we don't put the path in the cache since the children could change
                            // so a valid number can become invalid !
                            putInCache = false;
                        }
                    } catch (NumberFormatException e) { }
                }
            }
            if (root == null) {
                return null;
            }
        }

        if (putInCache) {
            pathToUI.put(path, root);
        }

        return root;
    }

    /**
     * Search a component with an id in a children map
     * @param base the id of the component
     * @param map a children map
     * @return the corresponding component or null
     */
    private static final UIComponent search(final String base, final Map<String, UIComponent> map) {
        UIComponent c = map.get(base);
        if (c != null) {
            return c;
        }

        for (UIComponent uic : map.values()) {
            Map<String, UIComponent> cmap = uic.children;
            if (cmap != null) {
                c = search(base, cmap);
                if (c != null) {
                    return c;
                }
            }
        }

        return null;
    }

    /**
     * Split a path.
     * @param path the path to split
     * @return a List containing the path components
     */
    private static final List<String> splitPath(final String path) {
        final char[] c = path.toCharArray();
        final List<String> list = new ArrayList<String>(16);
        int start = 0;
        int end = c.length - 1;

        for (; start < c.length; start++) {
            if (c[start] != '/' && c[start] != ' ') {
                break;
            }
        }

        for (; end >= 0; end--) {
            if (c[end] != '/' && c[end] != ' ') {
                break;
            }
        }

        for (int i = start; i < end; i++) {
            if (c[i] == '/' && i - start > 0) {
                list.add(new String(c, start, i - start));
                start = i + 1;
            }
        }

        if (end + 1 - start > 0) {
            list.add(new String(c, start, end + 1 - start));
        }

        return list;
    }
}
