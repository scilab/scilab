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
        TEXT_f(new TextBlock()),
        /** @see SuperBlock */
        DSUPER(new SuperBlock(true)),
        /** @see SuperBlock */
        SUPER_f(new SuperBlock()),
        /** @see AfficheBlock */
        AFFICH_m(new AfficheBlock()),
        /** @see AfficheBlock */
        AFFICH_f(AFFICH_m.getSharedInstance()),
        /** @see ExplicitInBlock */
        IN_f(new ExplicitInBlock()),
        /** @see ExplicitOutBlock */
        OUT_f(new ExplicitOutBlock()),
        /** @see ImplicitInBlock */
        INIMPL_f(new ImplicitInBlock()),
        /** @see ImplicitOutBlock */
        OUTIMPL_f(new ImplicitOutBlock()),
        /** @see EventInBlock */
        CLKINV_f(new EventInBlock()),
        /** @see EventOutBlock */
        CLKOUTV_f(new EventOutBlock()),
        /** @see EventOutBlock */
        CLKOUT_f(CLKOUTV_f.getSharedInstance()),
        /** @see SplitBlock */
        SPLIT_f(new SplitBlock()),
        /** @see SplitBlock */
        IMPSPLIT_f(SPLIT_f.getSharedInstance()),
        /** @see SplitBlock */
        CLKSPLIT_f(SPLIT_f.getSharedInstance()),
        /** @see GroundBlock */
        Ground(new GroundBlock()),
        /** @see VoltageSensorBlock */
        VoltageSensor(new VoltageSensorBlock()),
        /** @see RoundBlock */
        SUM_f(new RoundBlock("SUM_f")),
        /** @see RoundBlock */
        PROD_f(new RoundBlock("PROD_f")),
        /** @see RoundBlock */
        CLKSOM_f(new RoundBlock("CLKSOM_f")),
        /** @see RoundBlock */
        CLKSOMV_f(new RoundBlock("CLKSOMV_f")),
        /** @see BigSom */
        BIGSOM_f(new BigSom()),
        /** @see Summation */
        SUMMATION(new Summation()),
        /** @see Product */
        PRODUCT(new Product());

        private BasicBlock block;

        /**
         * Default constructor
         *
         * @param block
         *            The reference instance
         */
        private BlockInterFunction(BasicBlock block) {
            this.block = block;
        }

        /**
         * Create a block instance
         *
         * @return The new block instance
         */
        private BasicBlock createInstance() {
            BasicBlock clone = null;

            if (block != null) {
                clone = (BasicBlock) BlockFactory.createClone(block);
            }
            return clone;
        }

        /**
         * Get the reference shared block instance for this BlockInterFunction.
         *
         * @return The shared block instance
         */
        public BasicBlock getSharedInstance() {
            return block;
        }
    }

    // CSON: ClassDataAbstractionCoupling

    /** Default singleton constructor */
    private BlockFactory() {
        // This class is a static singleton
    }

    /**
     * Instantiate a new block with the specified interface function name.
     *
     * @param label
     *            The interface function name.
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(String label) {
        BasicBlock block = null;

        for (BlockInterFunction func : BlockInterFunction.values()) {
            if (label.compareTo(func.name()) == 0) {
                block = func.createInstance();
                break;
            }
        }

        // Not specific block
        if (block == null) {
            block = new BasicBlock(label);
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
        return func.createInstance();
    }

    /**
     * Create a clone for a block
     *
     * @param block
     *            The block to be cloned
     * @return the clone
     */
    public static Object createClone(BasicBlock block) {
        try {
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
