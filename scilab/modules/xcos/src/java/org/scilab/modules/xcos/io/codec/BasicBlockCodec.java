/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.io.codec;

import java.util.EnumSet;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;
import java.util.stream.Collectors;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
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
    private static final String[] IGNORED_FIELDS = new String[] { };
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
        try {

            mxCodecRegistry.addPackage("org.scilab.modules.xcos.block");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.block.io");
            mxCodecRegistry.addPackage("org.scilab.modules.xcos.block.custom");


            Map<Class <? extends BasicBlock>, List<BlockInterFunction>> customBlocks = EnumSet.allOf(BlockInterFunction.class).stream()
                    .collect(Collectors.groupingBy(BlockInterFunction::getKlass));

            for (Class<? extends BasicBlock> blockKlass : customBlocks.keySet()) {
                final String interfaceFunction = customBlocks.get(blockKlass).get(0).name();
                XcosObjectCodec codec = new BasicBlockCodec(XcosCellFactory.createBlock(interfaceFunction), IGNORED_FIELDS, REFS, null);
                mxCodecRegistry.register(codec);
            }

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

        } catch (Exception e) {
            e.printStackTrace();
        }
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
        JavaController controller = new JavaController();

        // update TextBlock due to a wrong serialization of the CSS properties
        if (block instanceof TextBlock) {
            block.setValue(mxUtils.getBodyMarkup(String.valueOf(block.getValue()), true));
        }

        // update style to replace direction by rotation and add the
        // default style if absent
        StyleMap map = new StyleMap(((Element) node).getAttribute(STYLE));
        formatStyle(map, (BasicBlock) obj, controller);
        block.setStyle(map.toString());

        /*
         * Compat. to remove old specific implementations
         *
         * These implementation was available from Scilab-5.2.0 to Scilab-5.3.3.
         *
         * Set default values stolen from the old implementation in case of
         * default value.
         */
        if (node.getNodeName().equals("ConstBlock")) {
            String[] strData = new String[1];

            controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, strData);
            if (BasicBlock.DEFAULT_INTERFACE_FUNCTION.equals(strData[0])) {
                controller.setObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, "CONST_m");
            }
            controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_NAME, strData);
            if (BasicBlock.DEFAULT_SIMULATION_FUNCTION.equals(strData[0])) {
                controller.setObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_NAME, "cstblk4");
            }
            if (block.getValue() == null) {
                block.setValue("1");
            }
        }
        if (node.getNodeName().equals("GainBlock")) {
            String[] strData = new String[1];

            controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, strData);
            if (BasicBlock.DEFAULT_INTERFACE_FUNCTION.equals(strData[0])) {
                controller.setObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, "GAINBLK_f");
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
     * @param controller the current java controller used to update the data
     */
    private void formatStyle(StyleMap map, BasicBlock obj, JavaController controller) {
        String[] strData = new String[1];
        controller.getObjectProperty(obj.getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, strData);

        // Append the bloc style if not present.
        String name = strData[0];
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
