/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
package org.scilab.modules.xcos.graph.model;

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
import org.scilab.modules.xcos.block.custom.BigSom;
import org.scilab.modules.xcos.block.custom.GroundBlock;
import org.scilab.modules.xcos.block.custom.Product;
import org.scilab.modules.xcos.block.custom.RoundBlock;
import org.scilab.modules.xcos.block.custom.Summation;
import org.scilab.modules.xcos.block.custom.VoltageSensorBlock;

/**
 * List the specific block interface function name. <BR>
 * <BR>
 * <EM>Specific instance must be registered after the most generic one in order
 * to serialized all the non-default values.</EM>
 */
public enum BlockInterFunction {
    /** @see TextBlock */
    TEXT_f(TextBlock.class),
    /** @see SuperBlock */
    SUPER_f(SuperBlock.class),
    /** @see SuperBlock */
    DSUPER(SuperBlock.class),
    /** @see AfficheBlock */
    AFFICH_m(AfficheBlock.class),
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
