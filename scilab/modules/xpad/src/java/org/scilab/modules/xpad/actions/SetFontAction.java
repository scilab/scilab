package org.scilab.modules.xpad.actions;

import java.awt.Font;
import java.io.File;
import java.util.ArrayList;

import javax.swing.JFileChooser;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class SetFontAction extends DefaultAction {

    public SetFontAction(Xpad editor) {
	super("Set Font...", editor);
    }
    
    public void doAction() {
    	SwingScilabFontChooser _fontChooser = new SwingScilabFontChooser(ConfigXpadManager.getFont());
    	_fontChooser.displayAndWait();
    	
    	Font newFont =_fontChooser.getSelectedFont() ;
        
    	if (newFont != null ){
    	
    		ArrayList<String> listStylesName = ConfigXpadManager.getAllStyleName() ;
    		
    		getEditor().getTextPane().setFont(newFont);
    		
    		/*we need to loop on every style , if not after the second change, styles will not change anymore
    		  except default*/
    		
    		for (int i = 0 ; i < listStylesName.size() ; i++ )
    		{
    			Style tempStyle = getEditor().getTextPane().getStyledDocument().getStyle(listStylesName.get(i));

    	    	
    	    	StyleConstants.setFontFamily(tempStyle ,newFont.getFamily() );
    	    	StyleConstants.setFontSize(tempStyle, newFont.getSize());
    	    	StyleConstants.setBold(tempStyle, newFont.isBold());
    	    	//StyleConstants.setItalic(tempStyle, newFont.isItalic());
    	    	

    			
    		}
    		

    		
    		/*insert update refresh the styles without needing to type text*/
	    	((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).insertUpdate(null);
	    	getEditor().getTextPane().requestFocus();
	    	
	    	ConfigXpadManager.saveFont(newFont);
    	}
    	
    }

}
