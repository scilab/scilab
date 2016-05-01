/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Stream;

/**
 * Contains all the handled elements to Xcos by category to ease dispatch.
 *
 * <p>
 * Note: the names correspond to each Element localName.
 */
public enum HandledElement {
    add(HandledElementsCategory.RAW_DATA),
    AfficheBlock(HandledElementsCategory.BLOCK),
    Array(HandledElementsCategory.RAW_DATA),
    BasicBlock(HandledElementsCategory.BLOCK),
    BigSom(HandledElementsCategory.BLOCK),
    CommandControlLink(HandledElementsCategory.LINK),
    CommandPort(HandledElementsCategory.PORT),
    ConstBlock(HandledElementsCategory.BLOCK),
    ControlPort(HandledElementsCategory.PORT),
    data(HandledElementsCategory.RAW_DATA),
    EventInBlock(HandledElementsCategory.BLOCK),
    EventOutBlock(HandledElementsCategory.BLOCK),
    ExplicitInBlock(HandledElementsCategory.BLOCK),
    ExplicitInputPort(HandledElementsCategory.PORT),
    ExplicitLink(HandledElementsCategory.LINK),
    ExplicitOutBlock(HandledElementsCategory.BLOCK),
    ExplicitOutputPort(HandledElementsCategory.PORT),
    GainBlock(HandledElementsCategory.BLOCK),
    GroundBlock(HandledElementsCategory.BLOCK),
    ImplicitInBlock(HandledElementsCategory.BLOCK),
    ImplicitInputPort(HandledElementsCategory.PORT),
    ImplicitLink(HandledElementsCategory.LINK),
    ImplicitOutBlock(HandledElementsCategory.BLOCK),
    ImplicitOutputPort(HandledElementsCategory.PORT),
    mxCell(HandledElementsCategory.JGRAPHX),
    mxGeometry(HandledElementsCategory.JGRAPHX),
    mxPoint(HandledElementsCategory.JGRAPHX),
    Orientation(HandledElementsCategory.CUSTOM),
    PrintBlock(HandledElementsCategory.BLOCK),
    Product(HandledElementsCategory.BLOCK),
    RoundBlock(HandledElementsCategory.BLOCK),
    ScilabBoolean(HandledElementsCategory.RAW_DATA),
    ScilabDouble(HandledElementsCategory.RAW_DATA),
    ScilabInteger(HandledElementsCategory.RAW_DATA),
    ScilabString(HandledElementsCategory.RAW_DATA),
    SplitBlock(HandledElementsCategory.BLOCK),
    Summation(HandledElementsCategory.BLOCK),
    SuperBlock(HandledElementsCategory.BLOCK),
    SuperBlockDiagram(HandledElementsCategory.CUSTOM),
    TextBlock(HandledElementsCategory.BLOCK),
    VoltageSensorBlock(HandledElementsCategory.BLOCK),
    XcosDiagram(HandledElementsCategory.CUSTOM);

    public static Map<String, HandledElement> getMap() {
        Map<String, HandledElement> map = new HashMap<>(HandledElement.values().length);
        Stream.of(HandledElement.values())
        .forEach(e -> map.put(e.name(), e));

        return Collections.unmodifiableMap(map);
    }

    final HandledElementsCategory category;

    private HandledElement(final HandledElementsCategory category) {
        this.category = category;
    }

    public HandledElementsCategory getCategory() {
        return category;
    }
}