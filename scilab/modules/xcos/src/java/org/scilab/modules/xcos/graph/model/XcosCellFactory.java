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

package org.scilab.modules.xcos.graph.model;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.synchronousScilabExec;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.Optional;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosView;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.CommandControlLink;
import org.scilab.modules.xcos.link.ExplicitLink;
import org.scilab.modules.xcos.link.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;
import java.lang.reflect.Constructor;
import java.util.EnumMap;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.custom.RoundBlock;
;

/**
 * Ease the creation of any {@link Kind} of graphical object
 */
public final class XcosCellFactory {

    /** Size compatibility for user defined blocks */
    private static final double DEFAULT_SIZE_FACTOR = 20.0;
    private static final Logger LOG = Logger.getLogger(XcosCellFactory.class.getName());

    /** Default singleton constructor */
    private XcosCellFactory() {
        // This class is a static singleton
    }

    /**
     * This is a notify method mapped as a Scilab gateway used to alert with the loaded UID
     *
     * @param uid
     *            the loaded UID
     * @param kind
     *            the kind of the created object (as an int)
     */
    @ScilabExported(module = "xcos", filename = "XcosCellFactory.giws.xml")
    public static synchronized void created(long uid, int kind) {
        lastCreated = new ScicosObjectOwner(uid, Kind.values()[kind]);

    }

    private static ScicosObjectOwner lastCreated = null;

    /**
     * Retrieve and clear the last created object (<pre>xcosCellCreated</pre> call)
     * @return the last created object
     */
    public static synchronized ScicosObjectOwner getLastCreated() {
        ScicosObjectOwner last = lastCreated;
        lastCreated = null;
        return last;
    }

    /*
     * Diagram management
     */

    /**
     * Allocate a Java XcosDiagram from a COSF file.
     *
     * This method execute the file and register a
     *
     * @param controller
     *            the controller
     * @param filename
     *            the file to execute
     * @return an allocated XcosDiagram
     */
    public static XcosDiagram createDiagramFromCOSF(final JavaController controller, String filename) {
        XcosView view = (XcosView) JavaController.lookup_view(Xcos.class.getName());
        JavaController.unregister_view(view);

        XcosDiagram diagram;
        try {
            synchronousScilabExec(
                "function f(), " + buildCall("exec", filename, -1) + "; " + buildCall("xcosCellCreated", "scs_m".toCharArray()) + "endfunction; f();");

            ScicosObjectOwner last = getLastCreated();
            if (last.getKind() == Kind.DIAGRAM) {
                String[] strUID = new String[1];
                controller.getObjectProperty(last.getUID(), last.getKind(), ObjectProperties.UID, strUID);

                diagram = new XcosDiagram(controller, last.getUID(), last.getKind(), strUID[0]);
                insertChildren(controller, diagram);
            } else {
                diagram = null;
            }
        } catch (InterpreterException e) {
            diagram = null;
        } finally {
            JavaController.register_view(Xcos.class.getName(), view);
        }

        return diagram;
    }

    /**
     * Insert the diagram MVC children into the JGraphX model
     *
     * @param controller
     *            the shared controller
     * @param diagram
     *            the current diagram instance
     */
    public static void insertChildren(JavaController controller, XcosDiagram diagram) {
        /*
         * Retrieve the children
         */
        VectorOfScicosID children = new VectorOfScicosID();
        controller.getObjectProperty(diagram.getUID(), diagram.getKind(), ObjectProperties.CHILDREN, children);
        final int childrenLen = children.size();

        /*
         * Allocation some pre-sized stash data
         */
        final ArrayList<BasicLink> links = new ArrayList<>(childrenLen / 2);
        final HashMap<Long, BasicPort> ports = new HashMap<>(childrenLen);

        /*
         * Create the XcosCell objects and store some of them for later use
         */
        XcosCell[] cells = new XcosCell[childrenLen];
        for (int i = 0; i < childrenLen; i++) {
            final long uid = children.get(i);
            final Kind kind = controller.getKind(uid);

            switch (kind) {
                case ANNOTATION:
                case BLOCK:
                    BasicBlock b = createBlock(controller, uid, kind);
                    cells[i] = b;
                    BlockPositioning.updatePortsPosition(diagram, b);
                    b.getTypedChildrenIndexes(BasicPort.class).stream()
                    .map(index -> b.getChildAt(index))
                    .filter(c -> c instanceof BasicPort)
                    .map(c -> (BasicPort) c)
                    .forEach(c -> ports.put(c.getUID(), c));
                    break;
                case LINK:
                    BasicLink l = createLink(controller, uid, kind);
                    cells[i] = l;
                    links.add(l);
                    break;
                default:
                    break;
            }
        }

        /*
         * Relink the links on the XcosCell part
         */
        for (BasicLink l : links) {
            long[] src = new long[1];
            controller.getObjectProperty(l.getUID(), l.getKind(), ObjectProperties.SOURCE_PORT, src);

            long[] dest = new long[1];
            controller.getObjectProperty(l.getUID(), l.getKind(), ObjectProperties.DESTINATION_PORT, dest);

            BasicPort srcPort = ports.get(src[0]);
            if (srcPort != null) {
                l.setSource(srcPort);
            } else {
                LOG.severe("Unable to connect link " + l.getId() + " : invalid source " + src[0]);
            }

            BasicPort destPort = ports.get(dest[0]);
            if (destPort != null) {
                l.setTarget(destPort);
            } else {
                LOG.severe("Unable to connect link " + l.getId() + " : invalid target " + dest[0]);
            }
        }

        // re-add the children cells without duplicating them
        children.clear();
        controller.setObjectProperty(diagram.getUID(), diagram.getKind(), ObjectProperties.CHILDREN, children);

        // add all the children using the diagram modification tracking features
        diagram.addCells(cells);

        // each cell has been referenced twice (CHILDREN insert and addCells), derefence them all by one
        Arrays.stream(cells).forEach(c -> controller.deleteObject(c.getUID()));
    }

    /*
     * Block and Annotation management
     */

    /**
     * Instantiate a new block with the specified interface function.
     *
     * @param func
     *            the interface function
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(BlockInterFunction func) throws InterpreterException {
        return createBlock(func, func.name());
    }

    /**
     * Instantiate a new block with the specified UID value and interface function
     *
     * @param uid
     *            The associated UID value
     * @param interfaceFunction
     *            the interface function
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(String interfaceFunction) throws InterpreterException {
        Optional<BlockInterFunction> func = EnumSet.allOf(BlockInterFunction.class).stream().filter(f -> f.name().equals(interfaceFunction)).findFirst();

        final BasicBlock block;
        if (func.isPresent()) {
            block = createBlock(func.get());
        } else {
            block = createBlock(BlockInterFunction.BASIC_BLOCK, interfaceFunction);
        }
        block.setStyle(interfaceFunction);

        return block;
    }

    private static BasicBlock createBlock(BlockInterFunction func, String interfaceFunction) throws InterpreterException {
        return createBlock(new JavaController(), func, interfaceFunction);
    }

    private static BasicBlock createBlock(final JavaController controller, BlockInterFunction func, String interfaceFunction) throws InterpreterException {
        BasicBlock block;
        ScicosObjectOwner last;

        if (BlockInterFunction.BASIC_BLOCK.name().equals(interfaceFunction)) {
            // deliver all the MVC speed for the casual case
            last = new ScicosObjectOwner(controller.createObject(Kind.BLOCK), Kind.BLOCK);
        } else {
            // allocate an empty block that will be filled later
            synchronousScilabExec("xcosCellCreated(" + interfaceFunction + "(\"define\")); ");
            last = getLastCreated();
        }

        // defensive programming
        if (last == null) {
            throw new InterpreterException("XcosCellFactory#createBlock : unable to allocate " + interfaceFunction);
        }

        if (EnumSet.of(Kind.BLOCK, Kind.ANNOTATION).contains(last.getKind())) {
            block = createBlock(controller, func, interfaceFunction, last.getUID(), last.getKind());
        } else {
            block = null;
        }

        return block;
    }

    public static BasicBlock createBlock(final JavaController controller, long uid, Kind kind) {
        String[] interfaceFunction = new String[1];
        if (kind == Kind.BLOCK) {
            controller.getObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);
        } else { // ANNOTATION
            interfaceFunction[0] = "TEXT_f";
        }

        final BlockInterFunction func = lookForInterfunction(interfaceFunction[0]);

        return createBlock(controller, func, interfaceFunction[0], uid, kind);
    }

    public static BlockInterFunction lookForInterfunction(String interfaceFunction) {
        Optional<BlockInterFunction> optFunc = EnumSet.allOf(BlockInterFunction.class).stream().filter(f -> f.name().equals(interfaceFunction)).findFirst();

        final BlockInterFunction func;
        if (optFunc.isPresent()) {
            func = optFunc.get();
        } else {
            func = BlockInterFunction.BASIC_BLOCK;
        }
        return func;
    }

    /**
     * Instantiate a new block with the specified interface function and uid.
     *
     * @param controller
     *            the Java controller to use
     * @param func
     *            the interface function as an enum
     * @param interfaceFunction
     *            the interface function name
     * @param uid
     *            the allocated uid
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(final JavaController controller, BlockInterFunction func, String interfaceFunction, long uid, Kind kind) {

        final EnumMap<ObjectProperties, Integer> properties = new EnumMap<>(ObjectProperties.class);
        properties.put(ObjectProperties.INPUTS, 0);
        properties.put(ObjectProperties.OUTPUTS, 0);
        properties.put(ObjectProperties.EVENT_INPUTS, 0);
        properties.put(ObjectProperties.EVENT_OUTPUTS, 0);


        /*
         * Retrieve the JGraphX data before cell creation
         */
        String[] strUID = new String[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.UID, strUID);

        String[] style = new String[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.STYLE, style);
        if (style[0].isEmpty()) {
            style[0] = interfaceFunction;
        }

        String value;
        String[] description = new String[] { "" };
        controller.getObjectProperty(uid, kind, ObjectProperties.DESCRIPTION, description);
        value = description[0];

        VectorOfDouble geom = new VectorOfDouble(4);
        controller.getObjectProperty(uid, kind, ObjectProperties.GEOMETRY, geom);

        double x = geom.get(0);
        double y = geom.get(1);
        double w = geom.get(2);
        double h = geom.get(3);
        mxGeometry geometry = new mxGeometry(x, y, w, h);

        /*
         * Instanciate the block
         */
        BasicBlock block = null;
        try {
            Constructor<? extends BasicBlock> cstr = func.getKlass().getConstructor(JavaController.class, Long.TYPE, Kind.class, Object.class, mxGeometry.class, String.class, String.class);
            block = cstr.newInstance(controller, uid, kind, value, geometry, style[0], strUID[0]);
        } catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException | NoSuchMethodException
                     | SecurityException e) {
            // Something goes wrong, print it.
            e.printStackTrace();
            return block;
        }

        /*
         * Synchronize model information back to the JGraphX data
         *
         * Annotations have no inputs/outputs
         */

        if (block.getKind() == Kind.BLOCK) {
            insertPortChildren(controller, properties, block);
        }


        /*
         * Compatibility to ease user definition :
         *   * split are not updated as they have an hard-coded fixed-size
         *   * round blocks : ports globally layout around the block
         *   * generic case : layout the ports per kind per block-side
         */
        boolean convertGeometry;
        if (block instanceof SplitBlock) {
            convertGeometry = false;
        } else if (block instanceof RoundBlock) {
            int numberOfPorts = properties.get(ObjectProperties.INPUTS) +
                                properties.get(ObjectProperties.OUTPUTS) +
                                properties.get(ObjectProperties.EVENT_INPUTS) +
                                properties.get(ObjectProperties.EVENT_OUTPUTS);
            convertGeometry = (2 * w + 2 * h) < (numberOfPorts * BasicPort.DEFAULT_PORTSIZE);
        } else {
            convertGeometry = h < (properties.get(ObjectProperties.INPUTS) * BasicPort.DEFAULT_PORTSIZE) |
                              h < (properties.get(ObjectProperties.OUTPUTS) * BasicPort.DEFAULT_PORTSIZE) |
                              w < (properties.get(ObjectProperties.EVENT_INPUTS) * BasicPort.DEFAULT_PORTSIZE) |
                              w < (properties.get(ObjectProperties.EVENT_OUTPUTS) * BasicPort.DEFAULT_PORTSIZE);
        }

        if (convertGeometry) {
            w = w * DEFAULT_SIZE_FACTOR;
            h = h * DEFAULT_SIZE_FACTOR;

            /*
             * Invert the y-axis value and translate it.
             */
            y = -y - h;

            block.setGeometry(new mxGeometry(x, y, w, h));
        }

        return block;
    }

    /**
     * Instantiate a new block for an already created MVC object
     *
     * @param lastCreated the owned MVC object
     * @return a block or null
     */
    public static BasicBlock createBlock(final JavaController controller, ScicosObjectOwner lastCreated) {
        // pre-condition
        if (lastCreated.getKind() != Kind.ANNOTATION && lastCreated.getKind() != Kind.BLOCK) {
            return null;
        }

        String[] interfaceFunction = new String[1];
        BlockInterFunction func = lookForInterfunction(interfaceFunction[0]);

        return createBlock(controller, func, interfaceFunction[0], lastCreated.getUID(), lastCreated.getKind());
    }

    /*
     * Port management
     */

    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller
     *            is the shared controller instance
     * @param properties
     *            specify the kind of port to insert and should be some of :
     *            <UL>
     *            <LI>{@link ObjectProperties#INPUTS}
     *            <LI>{@link ObjectProperties#OUTPUTS}
     *            <LI>{@link ObjectProperties#EVENT_INPUTS}
     *            <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     *            </UL>
     *            This method will fill the value with the number of added ports
     * @param parent
     *            is the parent {@link mxCell} to modify
     */
    private static void insertPortChildren(final JavaController controller, final EnumMap<ObjectProperties, Integer> properties, final XcosCell parent) {
        for (ObjectProperties property : properties.keySet()) {
            properties.put(property, insertPortChildren(controller, property, parent));
        }
    }

    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller
     *            is the shared controller instance
     * @param property
     *            specify the kind of port to insert and should be one of :
     *            <UL>
     *            <LI>{@link ObjectProperties#INPUTS}
     *            <LI>{@link ObjectProperties#OUTPUTS}
     *            <LI>{@link ObjectProperties#EVENT_INPUTS}
     *            <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @param parent
     *            is the parent {@link mxCell} to modify
     * @return the number of inserted children
     */
    private static int insertPortChildren(final JavaController controller, final ObjectProperties property, final XcosCell parent) {
        VectorOfScicosID modelChildren = new VectorOfScicosID();
        controller.getObjectProperty(parent.getUID(), parent.getKind(), property, modelChildren);

        XcosCell[] children = new XcosCell[modelChildren.size()];
        for (int i = 0; i < children.length; i++) {
            XcosCell child = createPort(controller, modelChildren.get(i), property);
            children[i] = child;
        }

        modelChildren.clear();
        controller.setObjectProperty(parent.getUID(), parent.getKind(), property, modelChildren);

        Arrays.stream(children).forEach(c -> {
            parent.insert(c);
            controller.deleteObject(c.getUID());
        });

        return children.length;
    }

    /**
     * Create a port for a specific uid
     *
     * @param controller
     *            is the shared controller instance
     * @param uid
     *            represent the allocated UID on the MVC
     * @param property
     *            specify the kind of port to create and should be one of :
     *            <UL>
     *            <LI>{@link ObjectProperties#INPUTS}
     *            <LI>{@link ObjectProperties#OUTPUTS}
     *            <LI>{@link ObjectProperties#EVENT_INPUTS}
     *            <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @return a newly allocated port
     */
    private static final BasicPort createPort(final JavaController controller, long uid, final ObjectProperties property) {
        BasicPort port;
        boolean[] isImplicit = { false };

        String[] strUID = new String[] { "" };
        controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.UID, strUID);

        String[] style = new String[] { "" };
        controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.STYLE, style);

        String[] value = new String[] { "" };
        controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.LABEL, value);

        switch (property) {
            case INPUTS:
                controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);
                if (isImplicit[0]) {
                    port = new ImplicitInputPort(controller, uid, Kind.PORT, value[0], style[0], strUID[0]);
                } else {
                    port = new ExplicitInputPort(controller, uid, Kind.PORT, value[0], style[0], strUID[0]);
                }
                break;
            case OUTPUTS:
                controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);
                if (isImplicit[0]) {
                    port = new ImplicitOutputPort(controller, uid, Kind.PORT, value[0], style[0], strUID[0]);
                } else {
                    port = new ExplicitOutputPort(controller, uid, Kind.PORT, value[0], style[0], strUID[0]);
                }
                break;
            case EVENT_INPUTS:
                port = new ControlPort(controller, uid, Kind.PORT, value[0], style[0], strUID[0]);
                break;
            case EVENT_OUTPUTS:
                port = new CommandPort(controller, uid, Kind.PORT, value[0], style[0], strUID[0]);
                break;
            default:
                return null;
        }

        /*
         * Setup JGraphX properties
         */

        return port;
    }

    /*
     * Link management
     */

    private static BasicLink createLink(JavaController controller, long uid, Kind kind) {
        int[] type = new int[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.KIND, type);

        /*
         * Synchronize model information back to the JGraphX data
         */

        String[] value = new String[] { "" };
        controller.getObjectProperty(uid, kind, ObjectProperties.LABEL, value);

        String[] style = new String[] { "" };
        controller.getObjectProperty(uid, kind, ObjectProperties.STYLE, style);

        String[] strUID = new String[] { "" };
        controller.getObjectProperty(uid, kind, ObjectProperties.UID, strUID);

        VectorOfDouble controlPoints = new VectorOfDouble();
        controller.getObjectProperty(uid, kind, ObjectProperties.CONTROL_POINTS, controlPoints);
        final int pointsLen = controlPoints.size() / 2;

        mxGeometry geom = new mxGeometry();

        // as the link is supposed to be connected and accordingly to the JGraphX rules : do not add the origin and destination point
        ArrayList<mxPoint> points = new ArrayList<>();
        int i = 0;
        // ignore origin
        i++;
        // loop for points
        for (; i < pointsLen - 1; i++) {
            points.add(new mxPoint(controlPoints.get(2 * i), controlPoints.get(2 * i + 1)));
        }
        // ignore destination
        i++;

        geom.setPoints(points);

        /*
         * Allocate the link
         */
        BasicLink link;
        switch (type[0]) {
            case -1:
                link = new CommandControlLink(controller, uid, kind, value[0], geom, style[0], strUID[0]);
                break;
            case 1:
                link = new ExplicitLink(controller, uid, kind, value[0], geom, style[0], strUID[0]);
                break;
            case 2:
                link = new ImplicitLink(controller, uid, kind, value[0], geom, style[0], strUID[0]);
                break;
            default:
                return null;
        }

        return link;
    }
}
