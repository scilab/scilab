/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import junit.framework.TestCase;
import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.read.H5Read;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.scilab.modules.hdf5.write.H5Write;

public class testScilabGraph extends TestCase {

    public static void main(String[] args) throws NullPointerException, HDF5Exception {
	testScilabGraph test = new testScilabGraph();
	test.testFakeMetanetGraph();
    }

    public void testFakeMetanetGraph() throws NullPointerException, HDF5Exception {
	/* PROFILES */
	ScilabList PROFILES = new ScilabList();
	PROFILES.add(new ScilabDouble(1.2));
	double [][]profilesArc = {{0,0},
		{0.1, 0.48},
		{0.2, 0.8533333},
		{0.3, 1.12},
		{0.4, 1.28},
		{0.5, 1.3333333}, 
		{0.6, 1.28},
		{0.7, 1.12}, 
		{0.8, 0.8533333},
		{0.9, 0.48},
		{1,0}};
	PROFILES.add(new ScilabDouble(profilesArc));

	/*
	 * NODEDATA
	 */
	String[] nodeDataTypes = {"nodedata"};
	ScilabMList NODEDATA = new ScilabMList(nodeDataTypes);

	/*
	 * EDGEDATA
	 */
	String[] edgeDataTypes = {"edgedata"};
	ScilabMList EDGEDATA = new ScilabMList(edgeDataTypes);


	/* 
	 * NODEDEFAULTS
	 */
	String[] nodeDefaultsTypes = {"nodedefs", "type", "diam", "border", "colors", "font"};
	ScilabTList NODEDEFAULTS = new ScilabTList(nodeDefaultsTypes);
	NODEDEFAULTS.add(new ScilabDouble(0));
	NODEDEFAULTS.add(new ScilabDouble(15));
	NODEDEFAULTS.add(new ScilabDouble(1));
	double[][] nodeDefaultColor = {{-1}, {-2}};
	NODEDEFAULTS.add(new ScilabDouble(nodeDefaultColor));
	double [][]nodeDefaultFont = {{3}, {8}, {-1}};
	NODEDEFAULTS.add(new ScilabDouble(nodeDefaultFont));


	/* 
	 * EDGEDEFAULTS
	 */
	String[] edgeDefaultsTypes = {"edgedefs", "width", "foreground", "font", "profile_index"};
	ScilabTList EDGEDEFAULTS = new ScilabTList(edgeDefaultsTypes);
	EDGEDEFAULTS.add(new ScilabDouble(1));
	EDGEDEFAULTS.add(new ScilabDouble(-1));
	double [][]edgeDefaultFont = {{3}, {8}, {-1}};
	EDGEDEFAULTS.add(new ScilabDouble(edgeDefaultFont));
	EDGEDEFAULTS.add(new ScilabDouble(2));

	/* NODEGRAPHIC */
	String[] nodeGraphicTypes = {"ngraphic", "display", "defaults", "name", "x", "y", "type", "diam", "border", "colors", "font", "displaymode"};
	ScilabMList NODEGRAPHIC = new ScilabMList(nodeGraphicTypes);
	NODEGRAPHIC.add(new ScilabString("name"));
	NODEGRAPHIC.add(NODEDEFAULTS);
	String [][]nodesName = {{"1", "2", "3", "4", "5"}};
	NODEGRAPHIC.add(new ScilabString(nodesName));
	//double[][] nodesX = {{-138.38667, 97.746665, 99.799998, -142.49334, -20.320002}};
	double[][] nodesX = {{0, 200, 200, 0, 100}};
	NODEGRAPHIC.add(new ScilabDouble(nodesX));
	//double[][] nodesY = {{183.57333, 182.54667, -29.973335, -24.840001, 79.879999}};
	double[][] nodesY = {{200, 200, 0, 0, 100}};
	NODEGRAPHIC.add(new ScilabDouble(nodesY));
	NODEGRAPHIC.add(new ScilabDouble(zeros(1, 5)));
	//NODEGRAPHIC.add(new ScilabDouble(zeros(1, 5)));
	double[][] nodeDiams = {{15,20,25,30,35}};
	NODEGRAPHIC.add(new ScilabDouble(nodeDiams));
	NODEGRAPHIC.add(new ScilabDouble(zeros(1, 5)));
	NODEGRAPHIC.add(new ScilabDouble(zeros(2, 5)));
	NODEGRAPHIC.add(new ScilabDouble(zeros(3, 5)));
	NODEGRAPHIC.add(new ScilabString("center"));


	/* 
	 * EDGEGRAPHIC 
	 */
	String[] edgeGraphicTypes = {"egraphic", "display", "defaults", "profiles", "name", "width", "foreground", "font", "profile_index", "displaymode"};
	ScilabMList EDGEGRAPHIC = new ScilabMList(edgeGraphicTypes);
	EDGEGRAPHIC.add(new ScilabString("name"));
	EDGEGRAPHIC.add(EDGEDEFAULTS);
	EDGEGRAPHIC.add(PROFILES);
	String [][]edgesNames = {{"1->2", "2->3", "3->4", "4->1", "5->1", "5->2", "5->3", "5->4"}};
	EDGEGRAPHIC.add(new ScilabString(edgesNames));
	EDGEGRAPHIC.add(new ScilabDouble(zeros(1,8)));
	EDGEGRAPHIC.add(new ScilabDouble(zeros(1,8)));
	EDGEGRAPHIC.add(new ScilabDouble(zeros(3,8)));
	EDGEGRAPHIC.add(new ScilabDouble(zeros(1,8)));
	EDGEGRAPHIC.add(new ScilabString("tangeant"));


	/* 
	 * NODES
	 */
	String[] nodesTypes = {"nodes", "number", "graphics", "data"};
	ScilabMList NODES = new ScilabMList(nodesTypes);
	NODES.add(new ScilabDouble(5));
	NODES.add(NODEGRAPHIC);
	NODES.add(NODEDATA);


	/* 
	 * EDGES
	 */
	String[] edgesTypes = {"edges", "tail", "head", "graphics", "data"};
	ScilabMList EDGES = new ScilabMList(edgesTypes);
	double [][] head = {{2, 3, 4, 1, 1, 2, 3, 4}};
	double [][] tail = {{1, 2, 3, 4, 5, 5, 5, 5}};
	EDGES.add(new ScilabDouble(tail));
	EDGES.add(new ScilabDouble(head));
	EDGES.add(EDGEGRAPHIC);
	EDGES.add(EDGEDATA);


	/* 
	 * GRAPH
	 */
	String[] graphTypes = {"graph", "version", "name", "directed", "nodes", "edges"}; 
	ScilabTList GRAPH = new ScilabTList(graphTypes);
	GRAPH.add(new ScilabString("5.2"));
	GRAPH.add(new ScilabString("Il grande Blouno !!!"));
	GRAPH.add(new ScilabDouble(1));
	GRAPH.add(NODES);
	GRAPH.add(EDGES);


	int fileId = H5Write.createFile("/tmp/fakeGraph.h5");
	H5Write.writeInDataSet(fileId, "FakeGraph", GRAPH);
	H5Write.closeFile(fileId);
	
	ScilabTList data = new ScilabTList();
	fileId = H5Read.openFile("/tmp/fakeGraph.h5");
	H5Read.readDataFromFile(fileId, data);
	H5Write.closeFile(fileId);
    }

    private double[][] zeros(int rows, int cols) {
	double [][]out = new double[rows][cols];

	for (int i = 0 ; i < rows ; ++i) {
	    for (int j = 0 ; j < cols ; ++j) {
		out[i][j] = 0;
	    }
	}
	return out;
    }

}
