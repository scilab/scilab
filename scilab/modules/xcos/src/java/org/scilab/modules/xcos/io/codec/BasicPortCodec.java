/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan Simon
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

import java.util.Map;
import java.util.logging.Logger;

import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;
import com.mxgraph.io.mxObjectCodec;
import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;

/**
 * Codec for any Port.
 *
 * This class doesn't pas the Data Abstraction Coupling (DAC) as we perform some
 * template initialization on the {@link #register()} method.
 */
// CSOFF: ClassDataAbstractionCoupling
public class BasicPortCodec extends XcosObjectCodec {

    private static final Logger LOG = Logger.getLogger(BasicPortCodec.class.getName());
    private static final String DATA_TYPE = "dataType";
    private static final String[] IGNORED_FIELDS = new String[] { DATA_TYPE, "connectedLinkId" };

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
    public BasicPortCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);
    }

    /**
     * Register all the know codecs on the {@link mxCodecRegistry}
     */
    public static void register() {
        JavaController controller = new JavaController();

        XcosObjectCodec explicitOutputPortCodec = new BasicPortCodec(new ExplicitOutputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null), IGNORED_FIELDS, REFS, null);
        mxCodecRegistry.register(explicitOutputPortCodec);
        XcosObjectCodec explicitInputPortCodec = new BasicPortCodec(new ExplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null), IGNORED_FIELDS, REFS, null);
        mxCodecRegistry.register(explicitInputPortCodec);
        XcosObjectCodec implicitOutputPortCodec = new BasicPortCodec(new ImplicitOutputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null), IGNORED_FIELDS, REFS, null);
        mxCodecRegistry.register(implicitOutputPortCodec);
        XcosObjectCodec implicitInputPortCodec = new BasicPortCodec(new ImplicitInputPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null), IGNORED_FIELDS, REFS, null);
        mxCodecRegistry.register(implicitInputPortCodec);
        XcosObjectCodec commandPortCodec = new BasicPortCodec(new CommandPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null), IGNORED_FIELDS, REFS, null);
        mxCodecRegistry.register(commandPortCodec);
        XcosObjectCodec controlPortCodec = new BasicPortCodec(new ControlPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, null), IGNORED_FIELDS, REFS, null);
        mxCodecRegistry.register(controlPortCodec);
        mxCodecRegistry.register(new mxObjectCodec(Orientation.EAST));
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
        final BasicPort b = (BasicPort) obj;

        if (b.getParent() == null) {
            trace(enc, node, "Invalid parent");
        }

        for (int i = 0; i < b.getChildCount(); i++) {
            final mxICell o = b.getChildAt(i);

            // switch instanceof(o)
            if (o instanceof mxCell) {
                // this is a comment
                continue;
            }

            trace(enc, node, "Inconsistent child %s at %d", o, i);
        }

        switch (b.getEdgeCount()) {
            case 0:
                break;
            case 1:
                final mxCell link = (mxCell) b.getEdgeAt(0);
                if (link.getSource() != b && link.getTarget() != b) {
                    trace(enc, node, "Inconsistent source or target at %s", link);
                }
                break;
            default:
                trace(enc, node, "Too much links");
                break;
        }

        return super.beforeEncode(enc, obj, node);
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
        if (!(obj instanceof BasicPort)) {
            LOG.severe("Unable to decode " + obj);
            return obj;
        }
        final BasicPort port = (BasicPort) obj;
        final String attr = ((Element) node).getAttribute(DATA_TYPE);

        // update connectable flag
        port.setConnectable(true);

        // update style from version to version.
        StyleMap map = new StyleMap(((Element) node).getAttribute(STYLE));
        formatStyle(map, (BasicPort) obj);
        port.setStyle(map.toString());

        return super.afterDecode(dec, node, obj);
    }

    /**
     * Format the style value
     *
     * @param map
     *            The style as a map
     * @param obj
     *            the associated obj
     */
    private void formatStyle(StyleMap map, BasicPort obj) {

        // Append the bloc style if not present.
        String name = obj.getClass().getSimpleName();
        if (!map.containsKey(name)) {
            map.put(name, null);
        }

        // Replace direction by rotation
        formatStyle(map);

        // Update the rotation value according to the Orientation
        updateRotationFromOrientation(map, obj);
    }

    /**
     * Update the rotation value when the block has been rotated on 5.2.0
     * format. Update it according to the Orientation field added 2010/01/08
     * between 5.2.0 and 5.2.1.
     *
     * @param map
     *            The previous style value
     * @param obj
     *            The port we are working on
     */
    private void updateRotationFromOrientation(StyleMap map, BasicPort obj) {
        final Orientation orientation = obj.getOrientation();
        int rotation = 0;
        boolean flipped = false;
        boolean mirrored = false;

        if (map.get(mxConstants.STYLE_ROTATION) != null) {
            rotation = Double.valueOf(map.get(mxConstants.STYLE_ROTATION)).intValue();
        } else {
            rotation = 0;
        }

        /*
         * Protect against a not set parent
         */
        if (obj.getParent() == null || !(obj.getParent() instanceof BasicBlock)) {
            return;
        }

        StyleMap parentBlockMap = new StyleMap(obj.getParent().getStyle());
        flipped = Boolean.parseBoolean(parentBlockMap.get(ScilabGraphConstants.STYLE_FLIP));
        mirrored = Boolean.parseBoolean(parentBlockMap.get(ScilabGraphConstants.STYLE_MIRROR));

        // Calculate the rotation for this kind of port.
        rotation = orientation.getAbsoluteAngle(obj.getClass(), flipped, mirrored);

        map.put(mxConstants.STYLE_ROTATION, Integer.toString(rotation));
    }
}
// CSON: ClassDataAbstractionCoupling
