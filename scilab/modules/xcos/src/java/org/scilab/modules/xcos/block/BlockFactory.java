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

package org.scilab.modules.xcos.block;

import java.lang.reflect.InvocationTargetException;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
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

import com.mxgraph.model.mxICell;

/**
 * Ease the creation of blocks
 */
public final class BlockFactory {

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
        PRODUCT(Product.class);

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
    private BlockFactory() {
        // This class is a static singleton
    }

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
                block = createBlock(func, uid);
                break;
            }
        }

        // Not specific block
        if (block == null) {
            block = new BasicBlock(uid);
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
        return createBlock(func, uid);
    }

    /**
     * Instantiate a new block with the specified interface function and uid.
     *
     * @param func the interface function
     * @param uid the allocated uid
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(BlockInterFunction func, long uid) {
        BasicBlock block = null;
        try {
            block = func.getKlass().getConstructor(Long.TYPE).newInstance(uid);
        } catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException	| NoSuchMethodException | SecurityException e) {
            // Something goes wrong, print it.
            e.printStackTrace();
        }

        return block;
    }

    /**
     * Create a clone for a block
     *
     * @param block
     *            The block to be cloned
     * @return the clone
     */
    public static Object createClone(BasicBlock block) {
        JavaController controller = new JavaController();

        try {
            // FIXME implement the MVC part
            //        	long uid = controller.cloneObject(block.getUID(), true);

            BasicBlock clone = (BasicBlock) block.clone();

            /* Clone children */
            for (int i = 0; i < block.getChildCount(); i++) {
                mxICell port = block.getChildAt(i);
                clone.insert((mxICell) port.clone());
            }

            return clone;
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
            return null;
        }
    }
}
