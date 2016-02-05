/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block;

import com.mxgraph.model.mxGeometry;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.block.actions.CodeGenerationAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskCreateAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskCustomizeAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskRemoveAction;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * A SuperBlock contains an entire diagram on it. Thus it can be easily
 * customized by the user.
 *
 * A SuperBlock can be created from any part of the diagram y selecting blocks
 * and applying the
 * {@link org.scilab.modules.xcos.block.actions.RegionToSuperblockAction}.
 *
 * It can also appear to users as a normal block by applying a mask on it. In
 * this case the creator can use any SuperBlock context defined variable on a
 * prompt to the user.
 *
 * @see SuperBlockDiagram
 * @see SuperblockMaskCreateAction
 * @see SuperblockMaskCustomizeAction
 * @see SuperblockMaskRemoveAction
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
@SuppressWarnings(value = { "serial" })
public final class SuperBlock extends BasicBlock {
    /**
     * The interfunction name (linked to Xcos-core)
     */
    public static final String INTERFUNCTION_NAME = "SUPER_f";

    /**
     * The simulation name (linked to Xcos-core)
     */
    private static final String SIMULATION_NAME = "super";
    /**
     * The simulation name on a masked status (linked to Xcos-core)
     */
    private static final String MASKED_SIMULATION_NAME = "csuper";
    /**
     * The interfunction name on a masked status (linked to Xcos-core)
     */
    private static final String MASKED_INTERFUNCTION_NAME = "DSUPER";

    public SuperBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);

        controller.setObjectProperty(uid, kind, ObjectProperties.SIM_FUNCTION_NAME, SIMULATION_NAME);
    }

    /**
     * @param graph
     *            parent diagram
     */
    @Override
    public void openContextMenu(ScilabGraph graph) {
        ContextMenu menu = null;

        menu = createContextMenu(graph);
        menu.getAsSimpleContextMenu().addSeparator();
        menu.add(CodeGenerationAction.createMenu(graph));

        Menu maskMenu = ScilabMenu.createMenu();
        maskMenu.setText(XcosMessages.SUPERBLOCK_MASK);

        if (isMasked()) {
            maskMenu.add(SuperblockMaskRemoveAction.createMenu(graph));
            menu.add(maskMenu);
        } else {
            maskMenu.add(SuperblockMaskCreateAction.createMenu(graph));
        }
        maskMenu.add(SuperblockMaskCustomizeAction.createMenu(graph));
        menu.add(maskMenu);

        menu.setVisible(true);
    }

    /**
     * Mask the SuperBlock
     */
    public void mask(final JavaController controller) {
        controller.setObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, MASKED_INTERFUNCTION_NAME);
        controller.setObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_NAME, MASKED_SIMULATION_NAME);

        VectorOfInt ipar = new VectorOfInt(1);
        ipar.set(0, 1);
        controller.setObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.IPAR, ipar);
    }

    /**
     * Unmask the SuperBlock
     */
    public void unmask(final JavaController controller) {
        controller.setObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, INTERFUNCTION_NAME);
        controller.setObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_NAME, SIMULATION_NAME);
    }

    /**
     * @return True is the SuperBlock is masked, false otherwise
     */
    public boolean isMasked() {
        JavaController controller = new JavaController();

        String[] interfaceFunction = new String[1];
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

        return !INTERFUNCTION_NAME.equals(interfaceFunction[0]);
    }

    /**
     * Customize the parent diagram on name change
     *
     * @param value
     *            the new name
     * @see com.mxgraph.model.mxCell#setValue(java.lang.Object)
     */
    @Override
    public void setValue(Object value) {
        if (value != null) {
            super.setValue(FileUtils.toValidCIdentifier(value.toString()));
        } else {
            super.setValue(value);
        }
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
