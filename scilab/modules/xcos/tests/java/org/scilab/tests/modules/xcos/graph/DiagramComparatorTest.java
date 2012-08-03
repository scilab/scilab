/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.tests.modules.xcos.graph;

import java.awt.GraphicsEnvironment;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.PriorityQueue;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.DiagramComparator;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;

public class DiagramComparatorTest {

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
    }

    @Test
    public void addXcosDiagrams() {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        final PriorityQueue<XcosDiagram> sorted = new PriorityQueue<XcosDiagram>(1, DiagramComparator.getInstance());
        final ArrayList<XcosDiagram> testVector = new ArrayList<XcosDiagram>();

        /*
         * Init test vector
         */
        for (int i = 0; i < 20; i++) {
            testVector.add(new XcosDiagram());
        }

        /*
         * test
         */
        sorted.addAll(testVector);

        assert sorted.size() == testVector.size();

        /*
         * Assert
         */
        for (XcosDiagram ref : testVector) {
            assert sorted.contains(ref);
        }

        assertOnDiagramCollection(sorted);
    }

    @Test
    public void addSuperBlocksDiagrams() {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        final PriorityQueue<XcosDiagram> sorted = new PriorityQueue<XcosDiagram>(1, DiagramComparator.getInstance());
        final ArrayList<XcosDiagram> testVector = new ArrayList<XcosDiagram>();

        /*
         * Init test vector
         */
        for (int i = 0; i < 20; i++) {
            testVector.add(new SuperBlockDiagram());
        }

        /*
         * test
         */
        sorted.addAll(testVector);

        assert sorted.size() == testVector.size();

        /*
         * Assert
         */
        for (XcosDiagram ref : testVector) {
            assert sorted.contains(ref);
        }
    }

    @Test
    public void checkOneHierarchy() {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        final Collection<XcosDiagram> sorted = Xcos.getInstance().createDiagramCollection();
        final ArrayList<XcosDiagram> testVector = new ArrayList<XcosDiagram>();

        /*
         * Init test vector
         */
        XcosDiagram root1 = new XcosDiagram();
        root1.installListeners();
        XcosDiagram root2 = new XcosDiagram();
        root2.installListeners();

        /*
         * First child
         */
        SuperBlock r1b1 = new SuperBlock();
        root1.addCell(r1b1);

        SuperBlockDiagram r1diag1 = new SuperBlockDiagram(r1b1);
        r1diag1.installListeners();
        r1diag1.installSuperBlockListeners();

        SuperBlock r1diag1b1 = new SuperBlock();
        r1diag1.addCell(r1diag1b1);

        SuperBlockDiagram r1diag1b1diag1 = new SuperBlockDiagram(r1diag1b1);
        r1diag1b1diag1.installListeners();
        r1diag1b1diag1.installSuperBlockListeners();

        /*
         * test vector
         */
        testVector.add(root1);
        testVector.add(r1diag1);
        testVector.add(r1diag1b1diag1);
        testVector.add(root2);

        /*
         * test
         */
        sorted.addAll(testVector);

        ArrayList<XcosDiagram> expected = new ArrayList<XcosDiagram>();
        expected.add(root1);
        expected.add(root2);
        expected.add(r1diag1);
        expected.add(r1diag1b1diag1);

        assertOnDiagramCollection(sorted);

        sorted.clear();
        Collections.shuffle(testVector);
        sorted.addAll(testVector);
        assertOnDiagramCollection(sorted);
    }

    private void assertOnDiagramCollection(Collection<XcosDiagram> diags) {
        int depth = 0;

        for (XcosDiagram d : diags) {
            int currentDepth = 0;

            while (d instanceof SuperBlockDiagram) {
                currentDepth++;
                d = ((SuperBlockDiagram) d).getContainer().getParentDiagram();
            }

            assert currentDepth >= depth;
            depth = currentDepth;
        }
    }

    @Test
    public void checkTwoFilesHierarchy() {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        final Collection<XcosDiagram> sorted = Xcos.getInstance().createDiagramCollection();
        final ArrayList<XcosDiagram> testVector = new ArrayList<XcosDiagram>();

        /*
         * Init test vector
         */
        XcosDiagram root1 = new XcosDiagram();
        root1.installListeners();
        XcosDiagram root2 = new XcosDiagram();
        root2.installListeners();

        /*
         * First child
         */
        SuperBlock r1b1 = new SuperBlock();
        root1.addCell(r1b1);
        SuperBlock r2b1 = new SuperBlock();
        root1.addCell(r2b1);

        SuperBlockDiagram r1diag1 = new SuperBlockDiagram(r1b1);
        r1diag1.installListeners();
        r1diag1.installSuperBlockListeners();
        SuperBlockDiagram r2diag1 = new SuperBlockDiagram(r2b1);
        r2diag1.installListeners();
        r2diag1.installSuperBlockListeners();

        /*
         * Second child
         */
        SuperBlock r1b2 = new SuperBlock();
        root1.addCell(r1b2);
        SuperBlock r2b2 = new SuperBlock();
        root1.addCell(r2b2);

        SuperBlockDiagram r1diag2 = new SuperBlockDiagram(r1b2);
        r1diag2.installListeners();
        r1diag2.installSuperBlockListeners();
        SuperBlockDiagram r2diag2 = new SuperBlockDiagram(r2b2);
        r2diag2.installListeners();
        r2diag2.installSuperBlockListeners();

        /*
         * test vector
         */
        testVector.add(root1);
        testVector.add(r1diag1);
        testVector.add(root2);
        testVector.add(r1diag2);
        testVector.add(r2diag1);
        testVector.add(r2diag2);

        /*
         * test
         */
        sorted.addAll(testVector);
        assert sorted.size() == testVector.size();

        assertOnDiagramCollection(sorted);
        assertOnTwoFilesHierarchy(sorted, root1, root2);

        /*
         * Shuffle then test n times
         */
        Collections.shuffle(testVector);
        for (int i = 0; i < testVector.size(); i++) {
            Collections.rotate(testVector, i);
            sorted.clear();
            sorted.addAll(testVector);

            assertOnDiagramCollection(sorted);
            assertOnTwoFilesHierarchy(sorted, root1, root2);
        }

    }

    private void assertOnTwoFilesHierarchy(final Collection<XcosDiagram> sorted, XcosDiagram root1, XcosDiagram root2) {
        final Iterator<XcosDiagram> it = sorted.iterator();
        XcosDiagram first = it.next();
        XcosDiagram second = it.next();

        assert first == root1 || first == root2;
        assert second == root1 || second == root2;

        while (it.hasNext()) {
            XcosDiagram diag = it.next();
            assert diag instanceof SuperBlockDiagram;
        }
    }

    @Test
    public void checkValidHierarchy() {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        final Collection<XcosDiagram> sorted = Xcos.getInstance().createDiagramCollection();
        final ArrayList<XcosDiagram> testVector = new ArrayList<XcosDiagram>();

        /*
         * Init test vector
         */
        XcosDiagram root = new XcosDiagram();
        root.installListeners();

        /*
         * First child
         */
        SuperBlock b1 = new SuperBlock();
        root.addCell(b1);

        SuperBlockDiagram diag1 = new SuperBlockDiagram(b1);
        diag1.installListeners();
        diag1.installSuperBlockListeners();

        /*
         * Second child
         */
        SuperBlock b2 = new SuperBlock();
        root.addCell(b2);

        SuperBlockDiagram diag2 = new SuperBlockDiagram(b2);
        diag2.installListeners();
        diag2.installSuperBlockListeners();

        /*
         * test vector
         */
        testVector.add(root);
        testVector.add(diag1);
        testVector.add(diag2);

        /*
         * test
         */
        sorted.addAll(testVector);

        assert sorted.size() == testVector.size();

        /*
         * Assert
         */
        final Iterator<XcosDiagram> it = sorted.iterator();
        assert it.next() == root;
        if (it.next() == diag1) {
            assert it.next() == diag2;
        } else {
            assert it.next() == diag1;
        }

        assertOnDiagramCollection(sorted);
    }
}
