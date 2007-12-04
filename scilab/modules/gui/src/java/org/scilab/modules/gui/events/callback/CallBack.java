package org.scilab.modules.gui.events.callback;


import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public abstract class CallBack implements ActionListener {
	
	public static int UNTYPED = -1;
	public static int SCILAB_INSTRUCTION = 0;
	public static int C_FORTRAN = 1;
	public static int SCILAB_FUNCTION = 2;
	
	public abstract void callBack();
	
	public abstract void actionPerformed(ActionEvent e);
}
