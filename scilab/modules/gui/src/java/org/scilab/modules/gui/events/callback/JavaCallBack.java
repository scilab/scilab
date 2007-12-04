package org.scilab.modules.gui.events.callback;


import java.awt.event.ActionEvent;

public abstract class JavaCallBack extends CallBack {
	
	public void actionPerformed(ActionEvent e) {
		callBack();
	}
}
