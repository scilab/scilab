/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2010-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block.custom;

import com.mxgraph.model.mxGeometry;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxICell;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

/**
 * The Ground block has only one specificity : it's port position.
 */
@SuppressWarnings(value = { "serial" })
public class GroundBlock extends BasicBlock {

    /** Default constructor */
    public GroundBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);
    }

    /**
     * Set the orientation before calling parent method.
     *
     * @param child
     *            the port to add.
     * @param index
     *            the index of the add
     * @return the inserted cell
     */
    @Override
    public mxICell insert(mxICell child, int index) {
        if (child instanceof BasicPort) {
            ((BasicPort) child).setOrientation(Orientation.NORTH);
        }
        return super.insert(child, index);
    }
}
