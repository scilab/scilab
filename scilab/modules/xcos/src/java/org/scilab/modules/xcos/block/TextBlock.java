/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import java.util.Map;

import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.Font;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.utils.XcosMessages;

import static org.scilab.modules.xcos.io.scicos.AbstractElement.getIndexes;
import static org.scilab.modules.xcos.io.scicos.AbstractElement.canGet;

import com.mxgraph.util.mxConstants;

/**
 * A textblock is used to annotate diagrams.
 */
@SuppressWarnings(value = { "serial" })
public final class TextBlock extends BasicBlock {
    private static final String INTERFUNCTION_NAME = "TEXT_f";

    /**
     * Default constructor
     */
    public TextBlock() {
        super();
    }

    /**
     * Initialize the block with the default values
     */
    @Override
    protected void setDefaultValues() {
        super.setDefaultValues();
        setInterfaceFunctionName(INTERFUNCTION_NAME);
        setStyle(INTERFUNCTION_NAME);

        setValue(XcosMessages.DOTS);
    }

    /**
     * @return the fontNumber
     */
    private Font getFont() {
        final ScilabString exprs = getLocalExprs();
        int number;

        final boolean isColumnDominant = exprs.getHeight() >= exprs.getWidth();
        final int[] indexes = getIndexes(1, isColumnDominant);
        if (canGet(exprs, indexes)) {
            number = Integer.parseInt(exprs.getData()[indexes[0]][indexes[1]]);
        } else {
            number = 0;
        }
        return Font.getFont(number);
    }

    /**
     * @return the fontSize
     */
    private int getFontSize() {
        final ScilabString exprs = getLocalExprs();
        int number;

        final boolean isColumnDominant = exprs.getHeight() >= exprs.getWidth();
        final int[] indexes = getIndexes(2, isColumnDominant);
        if (canGet(exprs, indexes)) {
            number = Integer.parseInt(exprs.getData()[indexes[0]][indexes[1]]);
        } else {
            number = 0;
        }
        return Font.getSize(number);
    }

    /**
     * Format exprs as a Scilab valid one
     */
    @Override
    public ScilabType getExprs() {
        final String[][] data = new String[][] { new String[] { getValue().toString(), "2", "1" } };
        return new ScilabString(data);
    }

    /**
     * Exprs accessor
     */
    private ScilabString getLocalExprs() {
        return (ScilabString) super.getExprs();
    }

    @Override
    public ScilabType getRealParameters() {
        return new ScilabString(getValue().toString());
    }

    @Override
    public ScilabType getIntegerParameters() {
        final double[][] data = new double[][] { new double[] { 2, 1 } };
        return new ScilabDouble(data);
    }

    /**
     * Apply style on setExprs
     *
     * @param exprs
     *            the expression to be parsed
     */
    @Override
    public void setExprs(ScilabType exprs) {
        super.setExprs(exprs);

        final StyleMap map = new StyleMap(getStyle());
        map.put(mxConstants.STYLE_FONTFAMILY, getFont().getName());
        map.put(mxConstants.STYLE_FONTSIZE, Integer.toString(getFontSize()));
        setStyle(map.toString());

        setValue(getLocalExprs().getData()[0][0]);
    }

    /**
     * Disabling BlockSettings action
     *
     * @param context
     *            the current context
     */
    @Override
    public void openBlockSettings(String[] context) {
        // NOTHING TO BE DONE
    }

    /**
     * Disabling BlockSettings action
     *
     * @param modifiedBlock
     *            the updated block
     */
    @Override
    public void updateBlockSettings(BasicBlock modifiedBlock) {
        // NOTHING TO BE DONE
    }

    /**
     * Customize menu
     *
     * @param menuList
     *            the menuList to work on
     */
    @Override
    protected void customizeMenu(Map < Class <? extends DefaultAction > , Menu > menuList) {
        menuList.get(BlockParametersAction.class).setEnabled(false);
        menuList.get(RegionToSuperblockAction.class).setEnabled(false);
    }
}
