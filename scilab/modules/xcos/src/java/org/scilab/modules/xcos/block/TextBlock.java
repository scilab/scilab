/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import java.util.Map;

import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;

import com.mxgraph.util.mxConstants;

public final class TextBlock extends BasicBlock {
    
    private static final long serialVersionUID = -4279562884443733433L;

    /**
     * Font list from http://www.w3.org/TR/CSS2/fonts.html#generic-font-families
     * Scicos has a number descriptor < 7 and > 0 
     */
    enum Font {
	SERIF("serif"),
	SANS_SERIF("sans-serif"),
	CURSIVE("cursive"),
	FANTASY("fantasy"),
	MONOSPACE("monospace"),
	ARIAL("Arial");
	
	private String name;
	
	private Font(String name) {
	    this.name = name;
	}
	
	public String getName() {
	    return name;
	}
    }

    public TextBlock() {
	super();
	setVertex(false);
	setVisible(false);
    }

    public TextBlock(String label) {
	super(label);
	setInterfaceFunctionName("TEXT_f");
    }

    /**
     * @return the text
     */
    public String getText() {
	return ((ScilabString) getExprs()).getData()[0][0];
    }
    
    /**
     * @return the fontNumber
     */
    private Font getFont() {
	int number = Integer.parseInt(((ScilabString) getExprs()).getData()[1][0]);
	return Font.values()[number-1];
    }

    /**
     * @return the fontSize
     */
    private int getFontSize() {
	// After investigations, the 1pt of scicos is equivalent to a 4 real pt
	return (Integer.parseInt(((ScilabString) getExprs()).getData()[2][0])*4);
    }
    
    /**
     * Apply style on setExprs
     */
    @Override
    public void setExprs(ScilabType exprs) {
        super.setExprs(exprs);
        
        StringBuilder str = new StringBuilder();
        str.append("[;");
        str.append(mxConstants.STYLE_FONTFAMILY);
        str.append("=");
        str.append(getFont().getName());
        str.append(";");
        str.append(mxConstants.STYLE_FONTSIZE);
        str.append("=");
        str.append(getFontSize());
        str.append(";]");
        
        setStyle(str.toString());
    }
    
    /**
     * Disabling BlockSettings action
     */
    @Override
    public void openBlockSettings(String context[]) {
	// NOTHING TO BE DONE
    }
    
    /**
     * Disabling BlockSettings action
     */
    @Override
    public void updateBlockSettings(BasicBlock modifiedBlock) {
	// NOTHING TO BE DONE
    }
    
    /**
     * Customize menu
     */
    @Override
    protected void customizeMenu(
            Map<Class<? extends DefaultAction>, Menu> menuList) {
        menuList.get(BlockParametersAction.class).setEnabled(false);
        menuList.get(RegionToSuperblockAction.class).setEnabled(false);
    }
    
    @Override
    public String getStyle() {
        String style = super.getStyle();

        /*
         * Automatically add mxConstants.STYLE_SHAPE if not present  
         */
        if (!style.contains(mxConstants.STYLE_SHAPE)) {
            StringBuilder str = new StringBuilder(style);
            str.append(";");
            str.append(mxConstants.STYLE_SHAPE);
            str.append("=");
            str.append(mxConstants.SHAPE_LABEL);
            str.append(";");
            style = str.toString();
        }
        
        return style;
    }
}
