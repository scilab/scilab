package org.scilab.modules.gui.events;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

public class GlobalEventFilter {

	public static void filter(KeyEvent event) {
		// Do nothing because we do not care of that kind of object.
		//System.out.println("[KEY]** Got an event : "+event.toString());
		synchronized (ClickInfos.getInstance()) {
			ClickInfos.getInstance().setMouseButtonNumber(event.getKeyChar()+SCILAB_OFFSET);
			ClickInfos.getInstance().notify();
		}
	}
	
	public static void filter(ActionEvent event, String command) {
		// DEBUG
		// System.out.println("[GENERIC ACTION] : "+event.toString());
		synchronized (ClickInfos.getInstance()) {
			ClickInfos.getInstance().setMenuCallback(command);
			ClickInfos.getInstance().notify();
		}
	}	
	
	public static void filter(MouseEvent mouseEvent, SwingScilabCanvas source) {
		// Do nothing because we do not care of that kind of object.
		// System.out.println("[ACTION on TextBox]** Got an event : "+mouseEvent.toString());
		synchronized (ClickInfos.getInstance()) {
			ClickInfos.getInstance().setWindowID(-1);
			ClickInfos.getInstance().notify();
		}
	}

	private static final int SCILAB_OFFSET = 32;
}
