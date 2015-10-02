/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.sax;

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
enum HandledElement {
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
    XcosDiagram(HandledElementsCategory.CUSTOM),;

    static Map<String, HandledElement> getMap() {
        Map<String, HandledElement> map = new HashMap<>(HandledElement.values().length);
        Stream.of(HandledElement.values())
        .forEach(e -> map.put(e.name(), e));

        return Collections.unmodifiableMap(map);
    }

    final HandledElementsCategory category;

    private HandledElement(final HandledElementsCategory category) {
        this.category = category;
    }
}