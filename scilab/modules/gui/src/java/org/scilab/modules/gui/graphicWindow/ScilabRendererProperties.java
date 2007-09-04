/*------------------------------------------------------------------------*/
/* file: ScilabRendererProperties.java                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Properties needed by a figure from its rendering canvas         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.gui.graphicWindow;

import javax.media.opengl.GL;

import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.figureDrawing.RendererProperties;


public class ScilabRendererProperties implements RendererProperties {

	/** Rendering canvas */
	private Canvas parentCanvas;
	/** Enclosing tab */
	private Tab parentTab;
	
	public ScilabRendererProperties(Tab parentTab, Canvas parentCanvas) {
		this.parentCanvas = parentCanvas;
		this.parentTab = parentTab;
	}
	
	public void forceDisplay() {
		parentCanvas.repaint();
	}

	public int getCanvasHeight() {
		return parentCanvas.getDims().getHeight();
	}

	public int getCanvasWidth() {
		return parentCanvas.getDims().getWidth();
	}

	public GL getGLPipeline() {
		return parentCanvas.getGL();
	}

	public String getInfoMessage() {
		return parentTab.getName();
	}

	public int getWindowHeight() {
		return parentTab.getDims().getHeight();
	}

	public int getWindowPosX() {
		return parentTab.getPosition().getX();
	}

	public int getWindowPosY() {
		return parentTab.getPosition().getY();
	}

	public int getWindowWidth() {
		return parentTab.getDims().getWidth();
	}

	public void setCanvasSize(int width, int height) {
		parentCanvas.setDims(new Size(width,height));
	}

	public void setInfoMessage(String infoMessage) {
		parentTab.setName(infoMessage);
	}

	public void setWindowPosition(int posX, int posY) {
		parentTab.setPosition(new Position(posX, posY));
	}

	public void setWindowSize(int width, int height) {
		parentTab.setDims(new Size(width, height));
		
	}

	public void setPixmapMode(boolean onOrOff) {
		// pixmap "on" means auto swap buffer off.
		parentCanvas.setAutoSwapBufferMode(!onOrOff);
	}
    
    public boolean getPixmapMode() {
    	// pixmap "on" means auto swap buffer off.
    	return !(parentCanvas.getAutoSwapBufferMode());
    }
	
}
