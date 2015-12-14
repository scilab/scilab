/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.tests.modules.xcos.graph.model;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.EnumSet;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;

public class XcosCellFactoryTest {
    private JavaController controller;

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
        controller = new JavaController();
    }

    @Test
    public void createOneSpecificBlock() {
        final String interfaceFunction = "BIGSOM_f";
        BasicBlock blk = XcosCellFactory.createBlock(controller, XcosCellFactory.lookForInterfunction(interfaceFunction), interfaceFunction,
                         controller.createObject(Kind.BLOCK));

        assert blk.getStyle().contains(interfaceFunction);
    }

    @Test
    public void createAllSpecificBlocks() throws NoSuchMethodException, SecurityException, InstantiationException, IllegalAccessException,
        IllegalArgumentException, InvocationTargetException {
        EnumSet<BlockInterFunction> blocks = EnumSet.allOf(BlockInterFunction.class);
        blocks.remove(BlockInterFunction.BASIC_BLOCK);

        for (BlockInterFunction b : blocks) {
            Constructor<? extends BasicBlock> cstr = b.getKlass().getConstructor(Long.TYPE);
            BasicBlock blk = cstr.newInstance(controller.createObject(Kind.BLOCK));

            // the block should have no children
            assert blk.getChildCount() == 0;
        }
    }
}
