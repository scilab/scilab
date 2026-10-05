/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.tests.modules.xcos.nonreg_tests;

import static org.junit.Assert.assertEquals;

import org.junit.Test;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.model.XcosCell;

import com.mxgraph.model.mxGeometry;

/**
 * Non-regression test for issue 17571 (https://gitlab.com/scilab/scilab/-/work_items/17571):
 * the "Unique name" of a SUPER_f block lost its accented characters and a
 * name made of digits became empty.
 */
public class issue_17571 {
    private static final String[][] CASES = {
        {"1234", "_1234"},
        {"12ab", "_12ab"},
        {"Réseau&Grid", "Reseau_Grid"},
        {"my block", "my_block"},
        {"  a - b  ", "a_b"},
        {"a_&b", "a_b"},
        {"_x", "_x"},
        {"Nom_valide", "Nom_valide"},
        {"&&", ""}
    };

    @Test
    public void toValidCIdentifier() {
        for (String[] c : CASES) {
            assertEquals(c[0], c[1], XcosCell.toValidCIdentifier(c[0]));
        }
    }

    @Test
    public void superBlockName() {
        JavaController controller = new JavaController();
        long uid = controller.createObject(Kind.BLOCK);
        try {
            SuperBlock block = new SuperBlock(controller, uid, Kind.BLOCK, "", new mxGeometry(), "", "issue_17571");
            for (int k = 0; k < 3; k++) {
                block.setValue(CASES[k][0]);
                assertEquals(CASES[k][0], CASES[k][1], block.getValue());
            }
        } finally {
            controller.deleteObject(uid);
        }
    }
}
