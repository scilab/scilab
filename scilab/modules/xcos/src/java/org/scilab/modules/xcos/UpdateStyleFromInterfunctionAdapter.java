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

import org.scilab.modules.graph.utils.StyleMap;

/**
 * Update the source block when the interface function change.
 */
public final class UpdateStyleFromInterfunctionAdapter extends XcosViewListener {
    public UpdateStyleFromInterfunctionAdapter() { }

    /**
     * Put the interfunction at the start of the style map to preserve
     * style modification.
     *
     * oldStyle="SUPER_f;fillColor=red" newStyle="DSUPER;fillColor=red"
     *
     * and not newStyle="fillColor=red;DSUPER"
     */
    @Override
    public void propertyUpdated(long uid, Kind kind, ObjectProperties property, UpdateStatus status) {
        if (status != UpdateStatus.SUCCESS || kind != Kind.BLOCK) {
            return;
        }

        // prevent any unrelated property change, safety code
        if (property != ObjectProperties.INTERFACE_FUNCTION && property != ObjectProperties.STYLE) {
            return;
        }

        JavaController controller = new JavaController();

        String[] interfaceFunction = new String[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

        String[] style = new String[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.STYLE, style);


        final StyleMap styleMap = new StyleMap(interfaceFunction[0]);
        styleMap.putAll(style[0]);

        controller.setObjectProperty(uid, kind, ObjectProperties.STYLE, styleMap.toString());
    }
}
