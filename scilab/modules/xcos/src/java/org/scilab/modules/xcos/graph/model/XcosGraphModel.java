/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph.model;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGraphModel;
import java.rmi.server.UID;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

/**
 * Implement a custom model for Xcos
 */
public class XcosGraphModel extends mxGraphModel {

    public XcosGraphModel(JavaController controller, long diagramId, Kind kind, String uid) {
        super(new mxCell());

        XcosCell parent = new XcosCell(controller, diagramId, kind, null, null, null, uid);
        ((mxCell) getRoot()).insert(parent);
    }

    @Override
    public final String createId(Object cell) {
        return new UID().toString();
    }

}
