/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
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
 * Default view for the Xcos java side.
 */
public class XcosView extends View {

    public XcosView() {
    }

    public void objectCreated(long uid, Kind k) {
        System.err.println("XcosView::objectCreated(" + uid + ", " + k + ")");
    }

    public void objectDeleted(long uid) {
        System.err.println("XcosView::objectDeleted(" + uid + ")");
    }

    public void objectUpdated(long uid, Kind k) {
        System.err.println("XcosView::objectUpdated(" + uid + ", " + k + ")");
    }

    public void propertyUpdated(long uid, Kind k, ObjectProperties p) {
        System.err.println("XcosView::propertyUpdated(" + uid + ", " + k + ", " + p + ")");
    }
}
