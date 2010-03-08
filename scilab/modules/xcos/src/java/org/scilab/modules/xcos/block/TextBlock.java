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

import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;

import com.mxgraph.util.mxConstants;

/**
 * 
 */
public final class TextBlock extends BasicBlock {
    
	/** After investigations, the 1pt of scicos is equivalent to a 4 real pt */
	private static final int SCICOS_FONTSIZE_RATIO = 4;
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
	
	/**
	 * Default constructor
	 * @param name the font name
	 */
	private Font(String name) {
	    this.name = name;
	}
	
	/**
	 * @return the font name
	 */
	public String getName() {
	    return name;
	}
    }

    /**
     * Default constructor
     */
	public TextBlock() {
		super();
	}

	/**
	 * Constructor with value
	 * @param label the current value
	 */
	protected TextBlock(String label) {
		this();
		setValue(label);
	}

	/**
	 * Initialize the block with the default values
	 */
	@Override
	protected void setDefaultValues() {
		super.setDefaultValues();
		setInterfaceFunctionName("TEXT_f");
		setStyle(getInterfaceFunctionName());
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
	return Font.values()[number - 1];
    }

    /**
     * @return the fontSize
     */
    private int getFontSize() {
	return (Integer.parseInt(((ScilabString) getExprs()).getData()[2][0]) * SCICOS_FONTSIZE_RATIO);
    }
    
    /**
     * Apply style on setExprs
     * @param exprs the current expression value
     */
    @Override
    public void setExprs(ScilabType exprs) {
        super.setExprs(exprs);
        
        StyleMap map = new StyleMap(getStyle());
        map.put(mxConstants.STYLE_FONTFAMILY, getFont().getName());
        map.put(mxConstants.STYLE_FONTSIZE, Integer.toString(getFontSize()));
        
        setStyle(map.toString());
    }
    
    /**
     * Disabling BlockSettings action
     * @param context the current context
     */
    @Override
    public void openBlockSettings(String[] context) {
	// NOTHING TO BE DONE
    }
    
    /**
     * Disabling BlockSettings action
     * @param modifiedBlock the updated values
     */
    @Override
    public void updateBlockSettings(BasicBlock modifiedBlock) {
	// NOTHING TO BE DONE
    }
    
    /**
     * Customize menu
     * @param menuList the current menus to modifiate
     */
    @Override
    protected void customizeMenu(
            Map<Class< ? extends DefaultAction>, Menu> menuList) {
        menuList.get(BlockParametersAction.class).setEnabled(false);
        menuList.get(RegionToSuperblockAction.class).setEnabled(false);
    }
}
