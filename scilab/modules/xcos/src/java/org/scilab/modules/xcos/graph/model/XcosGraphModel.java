/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2016 - Scilab Enterprises - Clement DAVID
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
