package org.scilab.modules.xcos.actions;

import java.awt.Container;
import java.awt.event.ActionEvent;
import java.beans.XMLEncoder;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;


import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;

public class ExportToXMLAction extends DefaultAction {

	private static final long serialVersionUID = 1L;
	private XcosDiagram diagram;
	
	public ExportToXMLAction(ScilabGraph scilabGraph) {
		super("Export to XML",scilabGraph);
	}
	
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu("Export to XML", null, new ExportToXMLAction(scilabGraph), null);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		
		diagram = (XcosDiagram)getGraph(e);
		String[] properties = { "label" };

		//GraphToXml.writeDiagramToXML("/home/allan/Bureau/testexport.xml", diagram);
		XMLEncoder encoder;;
		//XML
		
		try {

			encoder = new XMLEncoder(new BufferedOutputStream(
					new FileOutputStream(("/home/allan/Bureau/testexport.xml"))));
			//configureEncoder(encoder);
			encoder.writeObject(diagram);
			encoder.close();
		} catch (Exception e1) {
			e1.printStackTrace();
			//JOptionPane.showMessageDialog(diagram, e1.getMessage(), "Error",
					//JOptionPane.ERROR_MESSAGE);
		}
	
	}
	
}
