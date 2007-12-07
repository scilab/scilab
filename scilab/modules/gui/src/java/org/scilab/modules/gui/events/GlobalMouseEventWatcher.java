package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

public class GlobalMouseEventWatcher implements AWTEventListener {

	public void eventDispatched(AWTEvent mouseEvent) {
		ClickInfos.getInstance().setXCoordinate(((MouseEvent) mouseEvent).getPoint().getX());
		ClickInfos.getInstance().setYCoordinate(((MouseEvent) mouseEvent).getPoint().getY());
		if (mouseEvent.getID() == MouseEvent.MOUSE_CLICKED) {
			if (mouseEvent.getSource() instanceof SwingScilabCanvas) {
				GlobalEventFilter.filter((MouseEvent) mouseEvent, (SwingScilabCanvas) mouseEvent.getSource());
			}
		}
		//System.out.println("Mouse Event : "+((MouseEvent) mouseEvent).toString());
	}
}
