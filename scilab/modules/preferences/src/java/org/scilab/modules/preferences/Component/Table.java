/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.preferences.Component;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;


/** Implementation of Select compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
@SuppressWarnings(value = { "serial" })
public class Table extends Panel implements XComponent, XChooser, ListSelectionListener, ActionListener {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -6127289363733321914L;

    private final CustomTableCellRenderer customTableCellRenderer = new CustomTableCellRenderer();
    private Model model;
    private JTable table;
    private JScrollPane scrollPane;

    /** The attribute column indicates which column has to be chosen.
     *    if this attribute is not set, the chosen value is the selected index (as a string).
     */
    private String column;

    //begin Dynamic_controller
    // TODO separate class.
    private ImageIcon icons[] = {
        new ImageIcon(Icon.SCILAB_XICONS + "list-add.png"),
        new ImageIcon(Icon.SCILAB_XICONS + "go-up.png"),
        new ImageIcon(Icon.SCILAB_XICONS + "media-playback-stop.png"),
        new ImageIcon(Icon.SCILAB_XICONS + "go-down.png"),
        new ImageIcon(Icon.SCILAB_XICONS + "list-remove.png")
    };
    JButton controls[] = {
        new JButton(icons[0]),
        new JButton(icons[1]),
        new JButton(icons[2]),
        new JButton(icons[3]),
        new JButton(icons[4])
    };
    boolean openMask[] = {
        true,
        true,
        true,
        true,
        true
    };

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Table(final Node peer) {
        super(peer);
        model = new Model(peer);
        table = new JTable(model);

        table.getSelectionModel().addListSelectionListener(this);
        table.getTableHeader().setReorderingAllowed(false);
        table.setDragEnabled(false);
        setupOpenMask(peer);
        setupControls(peer);

        column = XCommonManager.getAttribute(peer , "column");

        if (XCommonManager.getAttribute(peer , "mode").equals("select")) {
            table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        }

        if (XCommonManager.getAttribute(peer , "mode").equals("cell")) {
            table.setCellSelectionEnabled(true);
            table.setColumnSelectionAllowed(false);
            table.setRowSelectionAllowed(true);
        }
        //checkContent();
        scrollPane = new JScrollPane(table);
        this.add(scrollPane);
    }

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String[] actuators = {"item"};
        return actuators;
    }

    public void setupOpenMask(Node peer) {
        if (XCommonManager.getAttribute(peer , "location").equals("fixed")) {
            openMask[1] = false;
        } else {
            openMask[1] = model.getRowCount() > 1;
        }
        if (XCommonManager.getAttribute(peer , "mode").equals("select")) {
            openMask[2] = false;
        }
        if (XCommonManager.getAttribute(peer , "location").equals("fixed")) {
            openMask[3] = false;
        } else {
            openMask[3] = model.getRowCount() > 1;
        }
        openMask[4] = model.getRowCount() != 0;
    }

    public void setupControls(Node peer) {
        if (XCommonManager.getAttribute(peer , "size").equals("dynamic")) {
            JPanel panel = new JPanel(new GridLayout(5, 1));
            for (int i = 0; i < controls.length; i++) {
                panel.add(controls[i]);
                controls[i].addActionListener((ActionListener) this);
                controls[i].setEnabled(false);
            }
            if (model.getRowCount() == 0) {
                controls[0].setEnabled(true);
            }
            this.add(panel, BorderLayout.EAST);
        }
    }

    public void openControls() {
        for (int i = 0; i < controls.length; i++) {
            controls[i].setEnabled(openMask[i]);
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        int id;
        JButton src = (JButton) e.getSource();
        for ( id = 0; id < controls.length; id++) {
            if (controls[id] == src) {
                break;
            }
        }
        switch (id) {
            case 0:
                // Add new row
                if (actionListener != null) {
                    ActionEvent transmit  = new ActionEvent(this, 0, "tableAdd", e.getWhen(), 0);
                    actionListener.actionPerformed(transmit);
                }
                break;
            case 1:
                // Move row upper
                break;
            case 2:
                table.getSelectionModel().clearSelection();
                for (int i = 0; i < controls.length; i++) {
                    controls[i].setEnabled(false);
                }
                break;
            case 3:
                break;
            case 4:
                // Delete row
                System.out.println("[DEBUG] calling actionPerformed(deleteRow)");
                if (actionListener != null) {
                    ActionEvent transmit = new ActionEvent(this, 0, "tableDel", e.getWhen(), 0);
                    actionListener.actionPerformed(transmit);
                }
                break;
        }
    }
    //end Dynamic_controller

    private final class CustomTableCellRenderer extends DefaultTableCellRenderer {
        public Component getTableCellRendererComponent (JTable table,
                Object obj, boolean isSelected, boolean hasFocus, int row, int column) {
            Component cell = super.getTableCellRendererComponent(table, obj, isSelected, hasFocus, row, column);
            if (obj instanceof java.awt.Color) {
                cell.setBackground((java.awt.Color) obj);
                cell.setForeground((java.awt.Color) obj);
            }

            return cell;
        }
    }

    public void addTableModelListener(TableModelListener listener) {
        model.addTableModelListener(listener);
        addActionListener((ActionListener) listener);
    }

    @SuppressWarnings("unused")
    private void checkContent() {
        for (int j = 0; j < model.getColumnCount(); j++) {
            System.out.print("  " + model.getColumnName(j));
        }
        System.out.print("\n");
        if (model.getRowCount() == 0) {
            System.out.println("-- no tableRow items!");
        }
        for (int i = 0; i < model.getRowCount(); i++) {
            for (int j = 0; j < model.getColumnCount(); j++) {
                System.out.print("  " + model.getValueAt(i, j));
            }
            System.out.print("\n");
        }
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        model.setNodeList(peer.getChildNodes());
        String item = XCommonManager.getAttribute(peer , "item", "-1");
        setupOpenMask(peer);
        if (!item.equals(item())) {
            item(item);
        }
        for (int j = 0; j < model.getColumnCount(); j++) {
            table.getColumnModel().getColumn(j).setCellRenderer(new CustomTableCellRenderer());
        }
        repaint();
    }

    /** Row selection management.
     *
     */
    private ActionListener actionListener = null;

    /** Actual response read by the listener.
     *
     * @return the chosen value.
     */
    public final Object choose() {
        if (! column.equals(XCommonManager.NAV)) {
            int row      = table.getSelectedRow();
            Node record  = model.getRowRecord(row);
            String value = XCommonManager.getAttribute(record , column);
            return value;
        }
        return item();
    }

    /** Registration of a single listener.
     * @param listener for the unique actionListener.
     */
    public final void addActionListener(final ActionListener listener) {
        actionListener = listener;
    }

    boolean externalChange;

    public void valueChanged(ListSelectionEvent e) {
        //table.valueChanged(e);
        if (externalChange && !e.getValueIsAdjusting()) {
            openControls();
            if (actionListener != null) {
                ActionEvent transmit = new ActionEvent(this, 0, "tableSelect", System.currentTimeMillis(), 0);
                actionListener.actionPerformed(transmit);
            }
        }
    }

    /** Sensor for 'item' attribute.
     *
     * @return the attribute value.
     */
    public final String item() {
        return "" + (table.getSelectedRow() + 1);
    }

    /** Actuator for 'item' attribute.
     *
     * @param text : the attribute value.
     */
    public final void item(final String item) {
        int selectedRow = Integer.parseInt(item);
        externalChange  = false;
        if ((selectedRow != -1) && (selectedRow != (table.getSelectedRow() + 1))) {
            table.setRowSelectionInterval(selectedRow - 1, selectedRow - 1);
            openControls();
        }
        externalChange  = true;
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "Table ...";
        return signature;
    }

    /** Enrich action description with event and model information.
     *
     * @param event embeds the description of interaction in view
     * @param action embeds the description of interaction in model
     */
    public static void processModelEvent(TableModelEvent event, Node [] actions) {
        Model src = (Model) event.getSource();
        for (int i = 0; i < actions.length; i++) {
            Element element = (Element) actions[i];
            int row = event.getFirstRow();
            int col = event.getColumn();
            String value    = src.getValueAt(row, col).toString();
            String attr     = src.getColumnAttr(col);
            String context  = element.getAttribute("context");
            element.setAttribute("set", attr);
            element.setAttribute("value", value);
            // subtle reuse of the 'set' behavior!
            element.setAttribute("context", context + (row + 1) + "/");
        }
    }

}

/** Scans DOM nodes and translate it into table model.
 *
 * @author gradit
 *
 */
class Model extends AbstractTableModel {

    /** Serialization ID.
     *
     */
    private static final long serialVersionUID = -4786321481195930071L;

    /** Data source.
     *
     */
    private NodeList nodelist;
    private NodeList prevC, prevR;
    private int rows;
    private int cols;

    public Model(Node peer) {
        setNodeList(peer.getChildNodes());
    }

    /** Set a new root for value scanning.
     *
     * @param input is the data source
     */
    public void setNodeList(final NodeList input) {
        nodelist = input;
    }

    @Override
    public int getColumnCount() {
        if (nodelist != prevC) {
            cols = 0;
            for (int i = 0; i < nodelist.getLength(); i++) {
                Node node = nodelist.item(i);
                if (node.getNodeName().equals("tableCol")) {
                    cols++;
                }
            }
            prevC = nodelist;
        }

        return cols;
    }

    @Override
    public int getRowCount() {
        if (nodelist != prevR) {
            rows = 0;
            for (int i = 0; i < nodelist.getLength(); i++) {
                Node node = nodelist.item(i);
                if (node.getNodeName().equals("tableRow")) {
                    rows++;
                }
            }
            prevR = nodelist;
        }

        return rows;
    }

    /** browse DOM model.
     *
     * @return tablePrototype DOM node
     */
    public Node getPrototypeRecord() {
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().equals("tablePrototype")) {
                return node;
            }
        }
        return null;
    }

    /** browse DOM model.
     *
     * @param col is the column identifier
     * @return DOM for column
     */
    public Node getColumnRecord(final int col) {
        int count = col;
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().equals("tableCol")) {
                if (count == 0) {
                    return node;
                }
                count--;
            }
        }
        return null;
    }

    /** browse DOM model.
     *
     * @param row is the row identifier
     * @return DOM for row
     */
    public Node getRowRecord(final int row) {
        int count = row;
        for (int i = 0; i < nodelist.getLength(); i++) {
            Node node = nodelist.item(i);
            if (node.getNodeName().equals("tableRow")) {
                if (count == 0) {
                    return node;
                }
                count--;
            }
        }
        return null;
    }

    /** browse DOM model.
     *
     * @param col is the column identifier
     * @return attribute name for column.
     */
    public String getColumnAttr(final int col) {
        Node record = getColumnRecord(col);
        if (record != null) {
            return XCommonManager.getAttribute(record , "attr");
        }
        return XCommonManager.NAV;
    }

    @Override
    public String getColumnName(final int col) {
        Node record = getColumnRecord(col);
        if (record != null) {
            return XCommonManager.getAttribute(record , "title");
        }
        return XCommonManager.NAV;
    }

    @Override
    public Object getValueAt(final int row, final int col) {
        String attr  = getColumnAttr(col);
        Node record  = getRowRecord(row);
        String value = XCommonManager.getAttribute(record , attr);
        if (value.startsWith("#")) {
            return XCommonManager.getColor(value);
        }
        return value;
    }

    @Override
    public void setValueAt(final Object value, final int row, final int col) {
        // temporary storage of value
        String attr     = getColumnAttr(col);
        Element record  = (Element) getRowRecord(row);
        record.setAttribute(attr, value.toString());
        // event firing
        TableModelEvent event = new TableModelEvent(this, row, row, col);
        fireTableChanged(event);
        return;
    }

    @Override
    public boolean isCellEditable(final int row, final int col) {
        Node record  = getColumnRecord(col);
        String value = XCommonManager.getAttribute(
                           record , "editable", "false");
        return value == "true";
    }

}
