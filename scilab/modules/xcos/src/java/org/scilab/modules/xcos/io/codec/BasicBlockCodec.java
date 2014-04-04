/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.codec;

import java.util.Map;
import java.util.logging.Logger;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCellCodec;
import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxUtils;

/**
 * Codec for any Block.
 */
public class BasicBlockCodec extends XcosObjectCodec {

    private static final String BASIC_BLOCK = BasicBlock.class.getSimpleName();
    private static final String SIMULATION_FUNCTION_TYPE = "simulationFunctionType";
    private static final String[] IGNORED_FIELDS = new String[] { SIMULATION_FUNCTION_TYPE, "locked", "parametersPCS" };
    private static final Logger LOG = Logger.getLogger(BasicBlockCodec.class.getName());

    /**
     * The constructor used on for configuration
     *
     * @param template
     *            Prototypical instance of the object to be encoded/decoded.
     * @param exclude
     *            Optional array of fieldnames to be ignored.
     * @param idrefs
     *            Optional array of fieldnames to be converted to/from
     *            references.
     * @param mapping
     *            Optional mapping from field- to attributenames.
     */
    public BasicBlockCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);
    }

    /**
     * Register all known codecs on the {@link mxCodecRegistry}.
     */
    public static void register() {
        mxCodecRegistry.addPackage("org.scilab.modules.xcos.block");
        mxCodecRegistry.addPackage("org.scilab.modules.xcos.block.io");
        mxCodecRegistry.addPackage("org.scilab.modules.xcos.block.positionning");

        for (BlockInterFunction function : BlockFactory.BlockInterFunction.values()) {
            XcosObjectCodec codec = new BasicBlockCodec(function.getSharedInstance(), IGNORED_FIELDS, REFS, null);
            mxCodecRegistry.register(codec);
        }

        XcosObjectCodec basicBlockCodec = new BasicBlockCodec(new BasicBlock(), IGNORED_FIELDS, REFS, null);
        mxCodecRegistry.register(basicBlockCodec);

        mxCellCodec cellCodec = new mxCellCodec(new mxCell(), null, REFS, null);
        mxCodecRegistry.register(cellCodec);

        /*
         * per block specific codec setup
         */
        BasicBlockCodec codec = (BasicBlockCodec) mxCodecRegistry.getCodec("AfficheBlock");
        codec.exclude.add("printTimer");
        codec.exclude.add("updateAction");

        /*
         * Compat. to remove old specific implementations
         *
         * These implementation was available from Scilab-5.2.0 to Scilab-5.3.3.
         */
        mxCodecRegistry.addAlias("ConstBlock", BASIC_BLOCK);
        mxCodecRegistry.addAlias("GainBlock", BASIC_BLOCK);
        mxCodecRegistry.addAlias("PrintBlock", BASIC_BLOCK);
    }

    /**
     * Things to do before encoding
     *
     * @param enc
     *            Codec that controls the encoding process.
     * @param obj
     *            Object to be encoded.
     * @param node
     *            XML node to encode the object into.
     * @return Returns the object to be encoded by the default encoding.
     * @see com.mxgraph.io.mxObjectCodec#beforeEncode(com.mxgraph.io.mxCodec,
     *      java.lang.Object, org.w3c.dom.Node)
     */
    @Override
    public Object beforeEncode(mxCodec enc, Object obj, Node node) {
        ((Element) node).setAttribute(SIMULATION_FUNCTION_TYPE, String.valueOf(((BasicBlock) obj).getSimulationFunctionType()));

        /*
         * Log some information
         */
        final BasicBlock b = (BasicBlock) obj;

        for (int i = 0; i < b.getChildCount(); i++) {
            final mxICell o = b.getChildAt(i);

            // switch instanceof(o)
            if (o instanceof BasicPort) {
                if (o.getParent() != b) {
                    trace(enc, node, "Inconsistent parent");
                }
                continue;
            }
            if (o instanceof mxCell) {
                // this is a comment
                continue;
            }

            trace(enc, node, "Inconsistent child %s at %d", o, i);
        }

        if (b.getEdgeCount() > 0) {
            trace(enc, node, "Has %d links", b.getEdgeCount());
        }

        return super.beforeEncode(enc, obj, node);
    }

    /**
     * {@inheritDoc}
     *
     * Shortcut method to avoid old-XML-compat (before jgraphx-1.4) from
     * {@link mxCellCodec}.
     *
     * Without this shortcut, alias does not works (Class name String
     * comparaison).
     *
     * @see http://forum.jgraph.com/questions/1467/aliases-are-not-handled-on-
     *      mxcellcodecbeforedecode
     */
    @Override
    public Node beforeDecode(mxCodec dec, Node node, Object obj) {
        return node;
    }

    /**
     * Apply compatibility pattern to the decoded object
     *
     * @param dec
     *            Codec that controls the decoding process.
     * @param node
     *            XML node to decode the object from.
     * @param obj
     *            Object decoded.
     * @return The Object transformed
     * @see org.scilab.modules.xcos.io.codec.XcosObjectCodec#afterDecode(com.mxgraph.io.mxCodec,
     *      org.w3c.dom.Node, java.lang.Object)
     */
    @Override
    public Object afterDecode(mxCodec dec, Node node, Object obj) {
        if (!(obj instanceof BasicBlock)) {
            LOG.severe("Unable to decode " + obj);
            return obj;
        }
        final BasicBlock block = (BasicBlock) obj;

        block.setSimulationFunctionType(SimulationFunctionType.DEFAULT);

        String functionType = (((Element) node).getAttribute(SIMULATION_FUNCTION_TYPE));
        if (functionType != null && functionType.compareTo("") != 0) {
            SimulationFunctionType type = BasicBlock.SimulationFunctionType.valueOf(functionType);
            if (type != null) {
                block.setSimulationFunctionType(type);
            }
        }

        // Re associate the diagram container
        if (block instanceof SuperBlock) {
            final SuperBlock superBlock = (SuperBlock) block;
            if (superBlock.getChild() != null) {
                superBlock.getChild().setContainer(superBlock);
            }
            superBlock.invalidateRpar();
        }

        // update TextBlock due to a wrong serialization of the CSS properties
        if (block instanceof TextBlock) {
            block.setValue(mxUtils.getBodyMarkup(String.valueOf(block.getValue()), true));
        }

        // update style to replace direction by rotation and add the
        // default style if absent
        StyleMap map = new StyleMap(((Element) node).getAttribute(STYLE));
        formatStyle(map, (BasicBlock) obj);
        block.setStyle(map.toString());
        block.updateFieldsFromStyle();

        /*
         * Compat. to remove old specific implementations
         *
         * These implementation was available from Scilab-5.2.0 to Scilab-5.3.3.
         *
         * Set default values stolen from the old implementation in case of
         * default value.
         */
        if (node.getNodeName().equals("ConstBlock")) {
            if (block.getInterfaceFunctionName().equals(BasicBlock.DEFAULT_INTERFACE_FUNCTION)) {
                block.setInterfaceFunctionName("CONST_m");
            }
            if (block.getSimulationFunctionName().equals(BasicBlock.DEFAULT_SIMULATION_FUNCTION)) {
                block.setSimulationFunctionName("cstblk4");
            }
            if (block.getValue() == null) {
                block.setValue("1");
            }
        }
        if (node.getNodeName().equals("GainBlock")) {
            if (block.getInterfaceFunctionName().equals(BasicBlock.DEFAULT_INTERFACE_FUNCTION)) {
                block.setInterfaceFunctionName("GAINBLK_f");
            }
        }
        // PrintBlock has no default values

        return super.afterDecode(dec, node, obj);
    }

    /**
     * Format the style value
     *
     * @param map
     *            the read style value
     * @param obj
     *            the block instance
     */
    private void formatStyle(StyleMap map, BasicBlock obj) {

        // Append the bloc style if not present.
        String name = obj.getInterfaceFunctionName();
        if (!map.containsKey(name)) {
            map.put(name, null);
        }

        formatStyle(map);
    }

    /**
     * To force serialisation/deserialisation of Scilab values, return null
     * instead of any ScilabType instance.
     *
     * {@inheritDoc}
     */
    @Override
    protected Object getFieldTemplate(Object obj, String fieldname, Node child) {
        final Object template = getFieldValue(obj, fieldname);

        if (template instanceof ScilabType) {
            return null;
        } else {
            return super.getFieldTemplate(obj, fieldname, child);
        }
    }
}
