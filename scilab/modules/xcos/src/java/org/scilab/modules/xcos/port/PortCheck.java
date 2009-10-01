package org.scilab.modules.xcos.port;

import java.util.Collection;

import com.mxgraph.view.mxGraph;
import com.mxgraph.view.mxMultiplicity;

public class PortCheck extends mxMultiplicity {

    public PortCheck(boolean source, String type, String attr, String value,
	    int min, String max, Collection validNeighbors, String countError,
	    String typeError, boolean validNeighborsAllowed) {
	super(source, type, attr, value, min, max, validNeighbors, countError,
		typeError, validNeighborsAllowed); 
    }

    public String check(mxGraph graph, Object edge, Object source, Object target, int sourceOut, int targetIn)
    {
	System.err.println("Calling check : "+source.getClass().getSimpleName()+" -> "+target.getClass().getSimpleName());
	if (isTypeCompatible(source, target)) { return null; }

	return typeError;
    }
    
    public boolean checkType(mxGraph graph, Object value, String type)
    {
	System.err.println("Calling checkType (3)");
	return false;
    }

    public boolean checkType(mxGraph graph, Object value, String type,
		String attr, String attrValue)
    {
	System.err.println("Calling checktype(5)");
	return false;
    }
    public boolean isUnlimited() {
	System.err.println("Calling isunlimited()");
	return false;
    }
    public int getMaxValue() {
	System.err.println("Calling getMaxValue()");
	return 2;
    }
    private boolean isTypeCompatible(Object firstPort, Object secondPort) {
	if(firstPort.getClass().getSimpleName().equals(type) &&
		secondPort.getClass().getSimpleName().equals(value)) { return true; }
	return true;
    }

}
