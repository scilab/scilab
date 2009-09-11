package org.scilab.modules.xpad.actions;

import java.awt.Font;
import java.util.ArrayList;

import javax.swing.text.Style;
import javax.swing.text.StyleConstants;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class ResetFontAction extends DefaultAction {

    private ResetFontAction(Xpad editor) {
	super("Reset default font", editor);
    }
    
    public void doAction() {

    	
    	Font oldFont = ConfigXpadManager.getDefaultFont() ;
        
    	
    		ArrayList<String> listStylesName = ConfigXpadManager.getAllStyleName() ;

    		getEditor().getTextPane().setFont(oldFont);
    		
    		/*we need to loop on every style , if not after the second change, styles will not change anymore
    		  except default*/
    		
    		for (int i = 0 ; i < listStylesName.size() ; i++ )
    		{
    			Style tempStyle = getEditor().getTextPane().getStyledDocument().getStyle(listStylesName.get(i));

    	    	
    	    	StyleConstants.setFontFamily(tempStyle ,oldFont.getFamily() );
    	    	StyleConstants.setFontSize(tempStyle, oldFont.getSize());
    	    	StyleConstants.setBold(tempStyle, oldFont.isBold());
    	    	//StyleConstants.setItalic(tempStyle, newFont.isItalic());
    	    	

    			
    		}
	    	

    		
    		/*insert update refresh the styles without needing to type text*/
	    	((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).insertUpdate(null);
	    	getEditor().getTextPane().requestFocus();
	    	
	    	ConfigXpadManager.saveFont(oldFont);
    	}
    
    public static MenuItem createMenu(Xpad editor) {
	return createMenu("Reset default font", null, new ResetFontAction(editor), null);
 }
    	
    }
