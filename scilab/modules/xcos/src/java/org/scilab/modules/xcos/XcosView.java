/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.xcos;

import java.util.ArrayList;
import java.util.EnumMap;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;

import javax.swing.SwingUtilities;

/**
 * Generic view to dispatch common GUI update to the right JGraphX component
 */
public class XcosView extends View {

    private final static class Entry {
        XcosViewListener listener;
        boolean onCallerThread;
        EnumSet<ObjectProperties> listenedProperties;

        public Entry(final boolean onCallerThread, final EnumSet<ObjectProperties> properties, final XcosViewListener listener) {
            this.listener = listener;
            this.onCallerThread = onCallerThread;
            this.listenedProperties = properties;
        }
    }

    private final EnumMap<Kind, ArrayList<Entry>> registeredListeners;
    private final HashMap<Long, Object> visibleObjects;

    /**
     * Default constructor
     */
    public XcosView() {
        registeredListeners = new EnumMap<>(Kind.class);
        visibleObjects = new HashMap<Long, Object>();
    }

    /**
     * @return all visible Java objects
     */
    public HashMap<Long, Object> getVisibleObjects() {
        return visibleObjects;
    }

    /*
     * Implement the MVC View interface by dispatch-ing to the listeners
     */

    @Override
    public final void objectCreated(long uid, Kind kind) {
        List<Entry> listeners = registeredListeners.get(kind);
        if (listeners == null) {
            return;
        }

        for (Entry e : listeners) {
            if (e.onCallerThread) {
                e.listener.objectCreated(uid, kind);
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        e.listener.objectCreated(uid, kind);
                    }
                });
            }
        }
    }

    @Override
    public final void objectReferenced(long uid, Kind kind) {
        List<Entry> listeners = registeredListeners.get(kind);
        if (listeners == null) {
            return;
        }

        for (Entry e : listeners) {
            if (e.onCallerThread) {
                e.listener.objectReferenced(uid, kind);
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        e.listener.objectReferenced(uid, kind);
                    }
                });
            }
        }
    }

    @Override
    public final void objectUnreferenced(long uid, Kind kind) {
        List<Entry> listeners = registeredListeners.get(kind);
        if (listeners == null) {
            return;
        }

        for (Entry e : listeners) {
            if (e.onCallerThread) {
                e.listener.objectUnreferenced(uid, kind);
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        e.listener.objectUnreferenced(uid, kind);
                    }
                });
            }
        }
    }

    @Override
    public final void objectDeleted(long uid, Kind kind) {
        List<Entry> listeners = registeredListeners.get(kind);
        if (listeners == null) {
            return;
        }

        for (Entry e : listeners) {
            if (e.onCallerThread) {
                e.listener.objectDeleted(uid, kind);
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        e.listener.objectDeleted(uid, kind);
                    }
                });
            }
        }
    }

    @Override
    public final void propertyUpdated(long uid, Kind kind, ObjectProperties property, UpdateStatus status) {
        List<Entry> listeners = registeredListeners.get(kind);
        if (listeners == null) {
            return;
        }

        for (Entry e : listeners) {
            if (e.onCallerThread) {
                if (e.listenedProperties.contains(property)) {
                    e.listener.propertyUpdated(uid, kind, property, status);
                }
            } else {
                if (e.listenedProperties.contains(property)) {
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            e.listener.propertyUpdated(uid, kind, property, status);
                        }
                    });
                }
            }
        }
    }

    /*
     * Helpers to manage the listeners on the EDT
     */

    public void addXcosViewListener(final XcosViewListener listener) {
        for (Kind kind : EnumSet.allOf(Kind.class)) {
            ArrayList<Entry> l = registeredListeners.get(kind);
            if (l == null) {
                l = new ArrayList<>();
                registeredListeners.put(kind, l);
            }

            l.add(new Entry(false, EnumSet.allOf(ObjectProperties.class), listener));
        }
    }

    public void addXcosViewListener(final Kind kind, final XcosViewListener listener) {
        ArrayList<Entry> l = registeredListeners.get(kind);
        if (l == null) {
            l = new ArrayList<>();
            registeredListeners.put(kind, l);
        }

        l.add(new Entry(false, EnumSet.allOf(ObjectProperties.class), listener));
    }

    public void addXcosViewListener(final Kind kind, final ObjectProperties property, final XcosViewListener listener) {
        ArrayList<Entry> l = registeredListeners.get(kind);
        if (l == null) {
            l = new ArrayList<>();
            registeredListeners.put(kind, l);
        }

        l.add(new Entry(false, EnumSet.of(property), listener));
    }

    public void addXcosViewListener(final Kind kind, boolean onCallerThread, final EnumSet<ObjectProperties> properties, final XcosViewListener listener) {
        ArrayList<Entry> l = registeredListeners.get(kind);
        if (l == null) {
            l = new ArrayList<>();
            registeredListeners.put(kind, l);
        }

        l.add(new Entry(onCallerThread, properties, listener));
    }
}
