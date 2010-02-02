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

import org.scilab.modules.gui.menu.Menu;

import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;

import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.utils.StyleMap;

import com.mxgraph.util.mxConstants;

/**
 * A textblock is used to annotate diagrams.
 */
public final class TextBlock extends BasicBlock {
	private static final long serialVersionUID = -4279562884443733433L;
	
	/**
	 * The factor between a real point and a scicos point
	 */
	private static final int FONT_FACTOR = 10;
	private static final String INTERFUNCTION_NAME = "TEXT_f";
	
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
		 * Cstr
		 * @param name the name of the font
		 */
		private Font(String name) {
		    this.name = name;
		}
		
		/**
		 * @return the name of the font
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
	 * Constructor with a default text
	 * @param label The default text
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
		setInterfaceFunctionName(INTERFUNCTION_NAME);
		setStyle(INTERFUNCTION_NAME);
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
	// After investigations, the 1pt of scicos is equivalent to a 10 real pt
	return (Integer.parseInt(((ScilabString) getExprs()).getData()[2][0]) * FONT_FACTOR);
   }
    
    /**
     * Apply style on setExprs
     * @param exprs the expression to be parsed
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
     * @param modifiedBlock the updated block
    */
    @Override
    public void updateBlockSettings(BasicBlock modifiedBlock) {
	// NOTHING TO BE DONE
    }
    
    /**
     * Customize menu
     * @param menuList the menuList to work on
     */
    @Override
    protected void customizeMenu(
            Map<Class< ? extends DefaultAction>, Menu> menuList) {
        menuList.get(BlockParametersAction.class).setEnabled(false);
        menuList.get(RegionToSuperblockAction.class).setEnabled(false);
    }
}
