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

/**
 * Dummy abstract implementation of an XcosView listener.
 */
public abstract class XcosViewListener {
    public void objectCreated(long uid, Kind kind) {};
    public void objectDeleted(long uid, Kind kind) {};
    public void objectReferenced(long uid, Kind kind, long refCount) {};
    public void objectUnreferenced(long uid, Kind kind, long refCount) {};
    public void propertyUpdated(long uid, Kind kind, ObjectProperties property, UpdateStatus status) {};
}
