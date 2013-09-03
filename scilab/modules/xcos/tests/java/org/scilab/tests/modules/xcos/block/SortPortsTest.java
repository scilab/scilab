/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.tests.modules.xcos.block;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

public class SortPortsTest {

    @Test
    public void checkEmpty() {
        final ArrayList<Object> children = new ArrayList<Object>();
        BasicBlock.sort(children);

        Assert.assertEquals(children, Collections.EMPTY_LIST);
        assertRightOrder(children);
    }

    @Test
    public void checkAlone() {
        checkSingleInstance(new ExplicitInputPort());
        checkSingleInstance(new ExplicitOutputPort());
        checkSingleInstance(new ControlPort());
        checkSingleInstance(new CommandPort());
    }

    private void checkSingleInstance(Object instance) {
        final ArrayList<Object> children = new ArrayList<Object>();
        children.add(instance);

        BasicBlock.sort(children);

        Assert.assertEquals(children, Collections.singletonList(instance));
        assertRightOrder(children);
    }

    @Test
    public void checkMulti() throws InstantiationException, IllegalAccessException {
        checkMultiInstance(ExplicitInputPort.class);
        checkMultiInstance(ExplicitOutputPort.class);
        checkMultiInstance(ControlPort.class);
        checkMultiInstance(CommandPort.class);

    }

    private void checkMultiInstance(final Class <? extends BasicPort > klass) throws InstantiationException, IllegalAccessException {
        final ArrayList<Object> children = new ArrayList<Object>();

        final int size = (int) Math.random() * 10;
        for (int i = 0; i < size; i++) {
            children.add(klass.newInstance());
        }

        final ArrayList<Object> sorted = new ArrayList<Object>(children);
        BasicBlock.sort(sorted);

        Assert.assertEquals(children, sorted);
        assertRightOrder(sorted);
    }

    @Test
    public void sortPortsTwice() {
        final ArrayList<Object> children = new ArrayList<Object>();
        fillRandomly(children);

        final ArrayList<Object> sorted = new ArrayList<Object>(children);
        BasicBlock.sort(sorted);
        assertRightOrder(sorted);

        final ArrayList<Object> sortedTwice = new ArrayList<Object>(sorted);
        BasicBlock.sort(sortedTwice);

        Assert.assertEquals(sorted, sortedTwice);
    }

    private void fillRandomly(final List<Object> children) {
        final int size = (int) (Math.random() * 10) + 3;

        for (int i = 0; i < size; i++) {
            final int type = (int) (Math.random() * 6);

            final BasicPort p;
            switch (type) {
                case 0:
                    p = new ExplicitInputPort();
                    break;
                case 1:
                    p = new ImplicitInputPort();
                    break;
                case 2:
                    p = new ExplicitOutputPort();
                    break;
                case 3:
                    p = new ImplicitOutputPort();
                    break;
                case 4:
                    p = new ControlPort();
                    break;
                case 5:
                    p = new CommandPort();
                    break;
                default:
                    p = null;
                    break;
            }

            children.add(p);
        }
    }

    private void assertRightOrder(final List<Object> children) {
        int typeIndex = 0;

        for (Object object : children) {
            switch (typeIndex) {
                case 0:
                    if (object instanceof InputPort) {
                        break;
                    }
                    typeIndex++;
                case 1:
                    if (object instanceof OutputPort) {
                        break;
                    }
                    typeIndex++;
                case 2:
                    if (object instanceof ControlPort) {
                        break;
                    }
                    typeIndex++;
                case 3:
                    if (object instanceof CommandPort) {
                        break;
                    }
                    typeIndex++;

                default:
                    Assert.fail("children is not ordered");
                    break;
            }
        }
    }
}
