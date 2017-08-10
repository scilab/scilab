/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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
package org.scilab.tests.modules.xcos.graph.model;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.EnumSet;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
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
    public void createOneSpecificBlock() throws ScilabInterpreterManagement.InterpreterException {
        final String interfaceFunction = "BIGSOM_f";
        BasicBlock blk = XcosCellFactory.createBlock(interfaceFunction);

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
