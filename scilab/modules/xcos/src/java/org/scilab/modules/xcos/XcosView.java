/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
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


        addXcosViewListener(new UpdateStyleFromInterfunctionAdapter());
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
    public final void objectReferenced(long uid, Kind kind, long refCount) {
        List<Entry> listeners = registeredListeners.get(kind);
        if (listeners == null) {
            return;
        }

        for (Entry e : listeners) {
            if (e.onCallerThread) {
                e.listener.objectReferenced(uid, kind, refCount);
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        e.listener.objectReferenced(uid, kind, refCount);
                    }
                });
            }
        }
    }

    @Override
    public final void objectUnreferenced(long uid, Kind kind, long refCount) {
        List<Entry> listeners = registeredListeners.get(kind);
        if (listeners == null) {
            return;
        }

        for (Entry e : listeners) {
            if (e.onCallerThread) {
                e.listener.objectUnreferenced(uid, kind, refCount);
            } else {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        e.listener.objectUnreferenced(uid, kind, refCount);
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
        addXcosViewListener(listener, EnumSet.allOf(Kind.class), false, EnumSet.allOf(ObjectProperties.class));
    }

    public void addXcosViewListener(final XcosViewListener listener, final Kind kind) {
        addXcosViewListener(listener, EnumSet.of(kind), false, EnumSet.allOf(ObjectProperties.class));
    }

    public void addXcosViewListener(final XcosViewListener listener, final Kind kind, final ObjectProperties property) {
        addXcosViewListener(listener, EnumSet.of(kind), false, EnumSet.of(property));
    }

    public void addXcosViewListener(final XcosViewListener listener, boolean onCallerThread) {
        addXcosViewListener(listener, EnumSet.allOf(Kind.class), onCallerThread, EnumSet.allOf(ObjectProperties.class));
    }

    public void addXcosViewListener(final XcosViewListener listener, EnumSet<Kind> kinds, boolean onCallerThread, EnumSet<ObjectProperties> properties) {
        for (Kind kind : kinds) {
            ArrayList<Entry> l = registeredListeners.get(kind);
            if (l == null) {
                l = new ArrayList<>();
                registeredListeners.put(kind, l);
            }

            l.add(new Entry(onCallerThread, properties, listener));
        }
    }

    public void removeXcosViewListener(final XcosViewListener listener) {
        for (Kind kind : EnumSet.allOf(Kind.class)) {
            ArrayList<Entry> l = registeredListeners.get(kind);
            if (l != null) {
                l.removeIf(e -> e.listener == listener);
            }
        }
    }
}
