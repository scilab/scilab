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

package org.scilab.modules.xcos.io.sax;

import java.util.Arrays;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.block.custom.BigSom;
import org.scilab.modules.xcos.block.custom.GroundBlock;
import org.scilab.modules.xcos.block.custom.Product;
import org.scilab.modules.xcos.block.custom.RoundBlock;
import org.scilab.modules.xcos.block.custom.Summation;
import org.scilab.modules.xcos.block.custom.VoltageSensorBlock;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.io.HandledElement;
import org.xml.sax.Attributes;

class BlockHandler implements ScilabHandler {

    private final XcosSAXHandler saxHandler;

    /**
     * Default constructor
     * @param saxHandler the shared sax handler
     */
    BlockHandler(XcosSAXHandler saxHandler) {
        this.saxHandler = saxHandler;
    }

    @Override
    public BasicBlock startElement(HandledElement found, Attributes atts) {
        String v;
        BasicBlock block;
        Kind kind;
        if (found == HandledElement.TextBlock) {
            kind = Kind.ANNOTATION;
        } else {
            kind = Kind.BLOCK;
        }
        final long uid = saxHandler.controller.createObject(kind);

        String value = atts.getValue("value");
        if (value != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.DESCRIPTION, value);
            if (kind == Kind.BLOCK && saxHandler.validCIdentifier.matcher(value).matches()) {
                saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.LABEL, value);
            }
        }

        String style = atts.getValue("style");
        if (style != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.STYLE, style);
        }

        String strUID = atts.getValue("id");
        if (strUID != null) {
            saxHandler.allChildren.peek().put(strUID, uid);
        }

        switch (found) {
            case AfficheBlock:
                block = new AfficheBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case BasicBlock:
                block = new BasicBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case BigSom:
                block = new BigSom(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case ConstBlock:
                block = new BasicBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case EventInBlock:
                block = new EventInBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case EventOutBlock:
                block = new EventOutBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case ExplicitInBlock:
                block = new ExplicitInBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case ExplicitOutBlock:
                block = new ExplicitOutBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case GainBlock:
                block = new BasicBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case GroundBlock:
                block = new GroundBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case ImplicitInBlock:
                block = new ImplicitInBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case ImplicitOutBlock:
                block = new ImplicitOutBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case PrintBlock:
                block = new BasicBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case Product:
                block = new Product(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case RoundBlock:
                block = new RoundBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case SplitBlock:
                block = new SplitBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case Summation:
                block = new Summation(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case SuperBlock:
                block = new SuperBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case TextBlock:
                block = new TextBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            case VoltageSensorBlock:
                block = new VoltageSensorBlock(saxHandler.controller, uid, kind, value, null, style, strUID);
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Set the attributes
         */
        v = atts.getValue("interfaceFunctionName");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, v);
        } else if (kind == Kind.BLOCK) {
            // Setup the interface function accordingly to the mapping table
            // by default it is empty and might not be serialized (depending on
            // the class)
            String defaultInterfaceFunction = Arrays.stream(BlockInterFunction.values()).filter(n -> n.getKlass().equals(block.getClass())).map(e -> e.name())
                                              .findFirst().get();
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, defaultInterfaceFunction);
        }
        v = atts.getValue("simulationFunctionName");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_NAME, v);
        }
        v = atts.getValue("blockType");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.SIM_BLOCKTYPE, v);
        }
        VectorOfInt vecOfInt = new VectorOfInt(2);
        v = atts.getValue("dependsOnU");
        if ("1".equals(v)) {
            vecOfInt.set(0, 1);
        }
        v = atts.getValue("dependsOnT");
        if ("1".equals(v)) {
            vecOfInt.set(1, 1);
        }
        saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.SIM_DEP_UT, vecOfInt);
        v = atts.getValue("simulationFunctionType");
        if (v != null) {
            SimulationFunctionType type = SimulationFunctionType.valueOf(v);
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_API, type.getValue());
        }

        saxHandler.insertChild(block);
        return block;
    }

    @Override
    public void endElement(HandledElement found) {
    }
}