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

import java.util.Arrays;

import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
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
import org.scilab.modules.xcos.block.positionning.BigSom;
import org.scilab.modules.xcos.block.positionning.GroundBlock;
import org.scilab.modules.xcos.block.positionning.Product;
import org.scilab.modules.xcos.block.positionning.RoundBlock;
import org.scilab.modules.xcos.block.positionning.Summation;
import org.scilab.modules.xcos.block.positionning.VoltageSensorBlock;
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

        switch (found) {
            case AfficheBlock:
                block = new AfficheBlock(uid);
                break;
            case BasicBlock:
                block = new BasicBlock(uid);
                break;
            case BigSom:
                block = new BigSom(uid);
                break;
            case ConstBlock:
                // FIXME: why not needed anymore
                block = new BasicBlock(uid);
                break;
            case EventInBlock:
                block = new EventInBlock(uid);
                break;
            case EventOutBlock:
                block = new EventOutBlock(uid);
                break;
            case ExplicitInBlock:
                block = new ExplicitInBlock(uid);
                break;
            case ExplicitOutBlock:
                block = new ExplicitOutBlock(uid);
                break;
            case GainBlock:
                // FIXME: why not needed anymore
                block = new BasicBlock(uid);
                break;
            case GroundBlock:
                block = new GroundBlock(uid);
                break;
            case ImplicitInBlock:
                block = new ImplicitInBlock(uid);
                break;
            case ImplicitOutBlock:
                block = new ImplicitOutBlock(uid);
                break;
            case PrintBlock:
                // FIXME: why not needed anymore
                block = new BasicBlock(uid);
                break;
            case Product:
                block = new Product(uid);
                break;
            case RoundBlock:
                block = new RoundBlock(uid);
                break;
            case SplitBlock:
                block = new SplitBlock(uid);
                break;
            case Summation:
                block = new Summation(uid);
                break;
            case SuperBlock:
                block = new SuperBlock(uid);
                break;
            case TextBlock:
                block = new TextBlock(uid);
                break;
            case VoltageSensorBlock:
                block = new VoltageSensorBlock(uid);
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Set the attributes
         */
        v = atts.getValue("id");
        if (v != null) {
            block.setId(v);
            saxHandler.allChildren.peek().put(v, uid);
        }

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
        v = atts.getValue("simulationFunctionType");
        if (v != null) {
            SimulationFunctionType type = SimulationFunctionType.valueOf(v);
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_API, type.getValue());
        }

        v = atts.getValue("style");
        if (v != null) {
            saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.STYLE, v);
        }

        v = atts.getValue("value");
        if (v != null) {
            if (kind == Kind.BLOCK) {
                saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.LABEL, v);
            } else { // ANNOTATION
                saxHandler.controller.setObjectProperty(uid, kind, ObjectProperties.DESCRIPTION, v);
            }
        }

        saxHandler.insertChild(block);
        return block;
    }

    @Override
    public void endElement(HandledElement found) {
    }
}