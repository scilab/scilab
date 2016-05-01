/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
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

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Map;
import java.util.WeakHashMap;

import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCellCodec;
import com.mxgraph.io.mxCodec;
import com.mxgraph.model.mxICell;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * Codec for any xcos object
 */
public class XcosObjectCodec extends mxCellCodec {
    /**
     * Refs field for codecs
     */
    protected static final String[] REFS = { "parent", "source", "target" };

    /*
     * Cache fields and accessors
     */
    protected Map<Class<?>, Map<String, Field>> fields = new WeakHashMap<>();
    protected Map<Class<?>, Map<Field, Method>> getters = new WeakHashMap<>();
    protected Map<Class<?>, Map<Field, Method>> setters = new WeakHashMap<>();

    /**
     * Attribute name containing {@link com.mxgraph.model.mxCell} style.
     */
    protected static final String STYLE = "style";
    private static final String DIRECTION = "direction";
    private static final String WEST = "west";
    private static final String SOUTH = "south";
    private static final String EAST = "east";

    private static final int DIRECTION_STEP = 90;

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
    public XcosObjectCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);

    }

    /**
     * {@inheritDoc}
     *
     * Overridden for performance issues.
     */
    @Override
    protected Method getAccessor(Object obj, Field field, boolean isGetter) {
        /*
         * Cache the object
         */
        final Class<?> type = obj.getClass();

        Map<Field, Method> map;
        if (isGetter) {
            map = getters.get(type);
        } else {
            map = setters.get(type);
        }
        if (map == null) {
            map = new WeakHashMap<Field, Method>();
            if (isGetter) {
                getters.put(type, map);
            } else {
                setters.put(type, map);
            }
        }

        Method m = map.get(field);
        if (m != null) {
            return m;
        }

        /*
         * Cache is empty, look for the accessor
         */
        m = super.getAccessor(obj, field, isGetter);
        if (m != null) {
            map.put(field, m);
        }

        return m;
    }

    /**
     * {@inheritDoc}
     *
     * Overridden for performance issues.
     */
    @Override
    protected Field getField(Object obj, String fieldname) {
        /*
         * Cache the object
         */
        final Class<?> type = obj.getClass();

        Map<String, Field> map = fields.get(type);
        if (map == null) {
            map = new WeakHashMap<String, Field>();
            fields.put(type, map);
        }

        Field f = map.get(fieldname);
        if (f != null) {
            return f;
        }

        /*
         * Cache is empty, look for the field
         */
        f = super.getField(obj, fieldname);
        if (f != null) {
            map.put(fieldname, f);
        }

        return f;
    }

    /**
     * {@inheritDoc}
     *
     * Clone the object while preserving UID / Kind allocation
     *
     */
    @Override
    protected Object cloneTemplate(Node node) {
        Object obj = null;

        try {
            if (template.getClass().isEnum()) {
                obj = template.getClass().getEnumConstants()[0];
            } else {
                if (XcosCell.class.isAssignableFrom(template.getClass())) {
                    JavaController controller = new JavaController();
                    Kind kind = ((XcosCell) template).getKind();

                    Constructor<? extends Object> cstrs = template.getClass().getConstructor(Long.TYPE);
                    obj = cstrs.newInstance(controller.createObject(kind));
                } else {
                    obj = template.getClass().newInstance();
                }
            }

        } catch (ReflectiveOperationException e) {
            // ignore
            e.printStackTrace();
        } catch (SecurityException e) {
            // ignore
            e.printStackTrace();
        }

        return obj;

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
        if (obj instanceof mxICell) {
            final mxICell cell = (mxICell) obj;

            final Node id = node.getAttributes().getNamedItem("id");
            if (id != null) {
                cell.setId(id.getNodeValue());
            }

        }
        return super.afterDecode(dec, node, obj);
    }

    /**
     * @param style
     *            the style to be formatted
     */
    public void formatStyle(StyleMap style) {
        if (style.containsKey(DIRECTION)) {
            String direction = style.get(DIRECTION);

            int angle = 0;
            do {
                if (direction.compareTo(EAST) == 0) {
                    break;
                }
                angle += DIRECTION_STEP;
                if (direction.compareTo(SOUTH) == 0) {
                    break;
                }
                angle += DIRECTION_STEP;
                if (direction.compareTo(WEST) == 0) {
                    break;
                }
                angle += DIRECTION_STEP;
            } while (false);

            style.remove(DIRECTION);
            style.put(XcosConstants.STYLE_ROTATION, Integer.toString(angle));

        }

        if (!style.containsKey(ScilabGraphConstants.STYLE_FLIP)) {
            style.put(XcosConstants.STYLE_FLIP, Boolean.FALSE.toString());
        }

        if (!style.containsKey(ScilabGraphConstants.STYLE_MIRROR)) {
            style.put(XcosConstants.STYLE_MIRROR, Boolean.FALSE.toString());
        }
    }

    /**
     * Trace any msg to the xml document.
     *
     * @param enc
     *            the current encoder
     * @param node
     *            the current node
     * @param msg
     *            the message
     * @param format
     *            the format
     */
    protected void trace(mxCodec enc, Node node, String msg, Object... format) {
        node.appendChild(enc.getDocument().createComment(String.format(msg, format)));
    }
}
