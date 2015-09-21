/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph.model;

import java.lang.reflect.InvocationTargetException;
import java.util.Arrays;
import java.util.EnumSet;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
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
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxICell;

/**
 * Ease the creation of any {@link Kind} of graphical object
 */
public final class XcosCellFactory {

    // DAC: As this is the constructor for all the block classes, this class is
    // very coupled with *Block classes
    // CSOFF: ClassDataAbstractionCoupling
    /**
     * List the specific block interface function name. <BR>
     * <BR>
     * <EM>Specific instance must be registered before generic ones in order
     * to serialized all the non-default values.</EM>
     */
    public static enum BlockInterFunction {
        /** @see TextBlock */
        TEXT_f(TextBlock.class),
        /** @see SuperBlock */
        DSUPER(SuperBlock.class),
        /** @see SuperBlock */
        SUPER_f(SuperBlock.class),
        /** @see AfficheBlock */
        AFFICH_m(AfficheBlock.class),
        /** @see AfficheBlock */
        AFFICH_f(AfficheBlock.class),
        /** @see ExplicitInBlock */
        IN_f(ExplicitInBlock.class),
        /** @see ExplicitOutBlock */
        OUT_f(ExplicitOutBlock.class),
        /** @see ImplicitInBlock */
        INIMPL_f(ImplicitInBlock.class),
        /** @see ImplicitOutBlock */
        OUTIMPL_f(ImplicitOutBlock.class),
        /** @see EventInBlock */
        CLKINV_f(EventInBlock.class),
        /** @see EventOutBlock */
        CLKOUTV_f(EventOutBlock.class),
        /** @see EventOutBlock */
        CLKOUT_f(EventOutBlock.class),
        /** @see SplitBlock */
        SPLIT_f(SplitBlock.class),
        /** @see SplitBlock */
        IMPSPLIT_f(SplitBlock.class),
        /** @see SplitBlock */
        CLKSPLIT_f(SplitBlock.class),
        /** @see GroundBlock */
        Ground(GroundBlock.class),
        /** @see VoltageSensorBlock */
        VoltageSensor(VoltageSensorBlock.class),
        /** @see RoundBlock */
        SUM_f(RoundBlock.class),
        /** @see RoundBlock */
        PROD_f(RoundBlock.class),
        /** @see RoundBlock */
        CLKSOM_f(RoundBlock.class),
        /** @see RoundBlock */
        CLKSOMV_f(RoundBlock.class),
        /** @see BigSom */
        BIGSOM_f(BigSom.class),
        /** @see Summation */
        SUMMATION(Summation.class),
        /** @see Product */
        PRODUCT(Product.class),
        /** @see BasicBlock */
        BASIC_BLOCK(BasicBlock.class);

        private final Class<? extends BasicBlock> klass;

        /**
         * Default constructor
         *
         * @param block
         *            The reference instance
         */
        private BlockInterFunction(Class<? extends BasicBlock> klass) {
            this.klass = klass;
        }

        /**
         * @return the class to instantiate
         */
        public Class<? extends BasicBlock> getKlass() {
            return klass;
        }
    }

    // CSON: ClassDataAbstractionCoupling

    /** Default singleton constructor */
    private XcosCellFactory() {
        // This class is a static singleton
    }

    /*
     * Block and Annotation management
     */

    /**
     * Instantiate a new block with the specified UID value.
     *
     * @param uid
     *            The associated UID value
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(long uid) {
        BasicBlock block = null;

        JavaController controller = new JavaController();
        String[] interfaceFunction = new String[1];
        controller.getObjectProperty(uid, Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

        for (BlockInterFunction func : BlockInterFunction.values()) {
            if (func.name().equals(interfaceFunction)) {
                block = createBlock(controller, func, uid);
                break;
            }
        }

        if (block == null) {
            block = createBlock(controller, BlockInterFunction.BASIC_BLOCK, uid);
            block.setStyle(interfaceFunction[0]);
        }

        return block;
    }

    /**
     * Instantiate a new block with the specified interface function.
     *
     * @param func
     *            the interface function
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(BlockInterFunction func) {
        JavaController controller = new JavaController();

        long uid = controller.createObject(Kind.BLOCK);
        return createBlock(controller, func, uid);
    }

    /**
     * Instantiate a new block with the specified interface function and uid.
     *
     * @param controller the Java controller to use
     * @param func the interface function
     * @param uid the allocated uid
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(final JavaController controller, BlockInterFunction func, long uid) {
        BasicBlock block = null;
        try {
            block = func.getKlass().getConstructor(Long.TYPE).newInstance(uid);
        } catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException	| NoSuchMethodException | SecurityException e) {
            // Something goes wrong, print it.
            e.printStackTrace();
            return block;
        }

        insertPortChildren(controller, block);
        return block;
    }

    /*
     * Port management
     */


    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller is the shared controller instance
     * @param parent is the parent {@link mxCell} to modify
     */
    public static void insertPortChildren(final JavaController controller, final XcosCell parent) {
        final EnumSet<ObjectProperties> properties = EnumSet.of(ObjectProperties.INPUTS, ObjectProperties.OUTPUTS, ObjectProperties.EVENT_INPUTS, ObjectProperties.EVENT_OUTPUTS);
        insertPortChildren(controller, properties, parent);
    }


    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller is the shared controller instance
     * @param properties specify the kind of port to insert and should be some of : <UL>
     *        <LI>{@link ObjectProperties#INPUTS}
     *        <LI>{@link ObjectProperties#OUTPUTS}
     *        <LI>{@link ObjectProperties#EVENT_INPUTS}
     *        <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @param parent is the parent {@link mxCell} to modify
     */
    public static void insertPortChildren(final JavaController controller, final EnumSet<ObjectProperties> properties, final XcosCell parent) {
        for (ObjectProperties property : properties) {
            insertPortChildren(controller, property, parent);
        }
    }

    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller is the shared controller instance
     * @param property specify the kind of port to insert and should be one of : <UL>
     *        <LI>{@link ObjectProperties#INPUTS}
     *        <LI>{@link ObjectProperties#OUTPUTS}
     *        <LI>{@link ObjectProperties#EVENT_INPUTS}
     *        <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @param parent is the parent {@link mxCell} to modify
     */
    public static void insertPortChildren(final JavaController controller, final ObjectProperties property, final XcosCell parent) {
        if (parent.getKind() != Kind.BLOCK) {
            return;
        }

        VectorOfScicosID modelChildren = new VectorOfScicosID();
        controller.getObjectProperty(parent.getUID(), parent.getKind(), property, modelChildren);

        mxICell[] children = new mxICell[modelChildren.size()];
        for (int i = 0; i < children.length; i++) {
            children[i] = createPort(controller, modelChildren.get(i), property);
        }
        Arrays.stream(children).forEach(c -> parent.insert(c));
    }

    /**
     * Create a port for a specific uid
     *
     * @param controller is the shared controller instance
     * @param uid represent the allocated UID on the MVC
     * @param property specify the kind of port to create and should be one of : <UL>
     *        <LI>{@link ObjectProperties#INPUTS}
     *        <LI>{@link ObjectProperties#OUTPUTS}
     *        <LI>{@link ObjectProperties#EVENT_INPUTS}
     *        <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @return a newly allocated port
     */
    private static final BasicPort createPort(final JavaController controller, long uid, final ObjectProperties property) {
        boolean[] isImplicit = {false};

        switch (property) {
            case INPUTS:
                controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);
                if (isImplicit[0]) {
                    return new ImplicitInputPort(uid);
                } else {
                    return new ExplicitInputPort(uid);
                }
            case OUTPUTS:
                controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);
                if (isImplicit[0]) {
                    return new ImplicitOutputPort(uid);
                } else {
                    return new ExplicitOutputPort(uid);
                }
            case EVENT_INPUTS:
                return new ControlPort(uid);
            case EVENT_OUTPUTS:
                return new CommandPort(uid);
            default:
                return null;
        }
    }
}
