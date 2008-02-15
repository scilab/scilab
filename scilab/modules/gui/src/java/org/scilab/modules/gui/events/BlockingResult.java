package org.scilab.modules.gui.events;

public class BlockingResult {

	private String theResult;
	
	private static BlockingResult me;
	
	private BlockingResult() {
		theResult = null;
	}

	public static BlockingResult getInstance() {
		if (me == null) {
			me = new BlockingResult();	
		}
		return me;
	}
	
	public String getResult() {
		synchronized (me) {
			try {
				me.wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return me.theResult;
	}

	public void setResult(String theResult) {
		this.theResult = theResult;
		synchronized (me) {
			me.notify();
		}
	}
	
	
	
}
