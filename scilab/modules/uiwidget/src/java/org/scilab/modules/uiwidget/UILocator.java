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

package org.scilab.modules.uiwidget;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

public final class UILocator {

    private static final Map<String, UIComponent> roots = new HashMap<String, UIComponent>();
    private static final Map<String, UIComponent> pathToUI = new HashMap<String, UIComponent>();
    private static final Map<Integer, UIComponent> uids = new HashMap<Integer, UIComponent>();

    public static void addRoot(final UIComponent ui) {
        String id = ui.getId();
        if (id != null && !id.isEmpty()) {
            if (!roots.containsKey(id)) {
                roots.put(id, ui);
            }
        }
    }

    public static void add(final UIComponent ui) {
        if (ui.isRoot()) {
            addRoot(ui);
        }
        uids.put(ui.getUid(), ui);
    }

    public static void removeFromCachedPaths(final UIComponent ui) {
        final String id = ui.getId();
        if (ui.isRoot()) {
            roots.remove(id);
        }
        Set<String> toRemove = new TreeSet<String>();
        for (Map.Entry<String, UIComponent> entry : pathToUI.entrySet()) {
            if (entry.getValue() == ui) {
                toRemove.add(entry.getKey());
            }
        }

        for (String key : toRemove) {
            pathToUI.remove(key);
        }
    }

    public static void remove(final UIComponent ui) {
        removeFromCachedPaths(ui);
        uids.remove(ui.getUid());
    }

    public static UIComponent get(final int uid) {
        return uids.get(uid);
    }

    public static UIComponent get(final String path) {
        UIComponent ui = pathToUI.get(path);
        int start = 0;
        if (ui != null) {
            return ui;
        }

        List<String> pathElements = splitPath(path);
        if (pathElements.isEmpty()) {
            return null;
        }

        UIComponent root;
        String base = pathElements.get(0);
        root = roots.get(base);

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
            } else if (!el.equals(".")) {
                root = root.children.get(el);
            }
            if (root == null) {
                return null;
            }
        }

        pathToUI.put(path, root);

        return root;
    }

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

