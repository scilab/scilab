/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

public class SuperblockMaskCustomizeAction extends DefaultAction {

	private SuperblockMaskCustomizeAction(ScilabGraph scilabGraph) {
		super(XcosMessages.CUSTOMIZE, scilabGraph);
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.CUSTOMIZE, null,
				new SuperblockMaskCustomizeAction(scilabGraph), null);
	}

	public void actionPerformed(ActionEvent e) {
		SuperBlock block = (SuperBlock) ((XcosDiagram) getGraph(e))
				.getSelectionCell();

		CustomizeFrame frame = new CustomizeFrame();
		frame.setBlock(block);
		frame.importFromModel();
		frame.setVisible(true);
	}

	/**
	 * Frame used to customize fields and variables default values.
	 */
	private static class CustomizeFrame extends JFrame {
		private SuperBlock block;
		private CustomizeFrameListener listener;

		public CustomizeFrame() {
			setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
			listener = new CustomizeFrameListener();
			initComponents();
		}

		/**
		 * Register the block used to get/set the scicos values
		 * @param block the registered block
		 */
		public void setBlock(SuperBlock block) {
			this.block = block;
		}

		private SuperBlock getBlock() {
			return block;
		}

		/**
		 * Export the table models to the block exprs.
		 */
		private void exportToModel() {
			final Vector customModel = ((DefaultTableModel) varCustomizeTable.getModel()).getDataVector();
			final Vector valuesModel = ((DefaultTableModel) defaultValueTable.getModel()).getDataVector();
			
			/* We have one content that is not a variable : Window Title*/
			final int nbOfVar = valuesModel.size()-1;
			
			final String[] values = new String[nbOfVar];
			final String[] varNames = new String[nbOfVar];
			final String[] varDesc = new String[nbOfVar+1];
			final ScilabList polFields = new ScilabList();
			
			/* Title */
			varDesc[0] = (String) ((Vector) valuesModel.get(0)).get(1);
			
			/* Other fields */
			for (int i = 0; i < nbOfVar; i++) {
				values[i] = (String) ((Vector) valuesModel.get(i+1)).get(1);
				varNames[i] = (String) ((Vector) customModel.get(i+1)).get(1);
				varDesc[i+1] = (String) ((Vector) customModel.get(i+1)).get(2);
				
				/* reconstruct pol fields
				 * TODO: what are these fields ?  
				 */
				polFields.add(new ScilabString("pol"));
				polFields.add(new ScilabDouble(-1.0));
			}
			
			/* Construct fields from data */
			ScilabList exprs = new ScilabList() {
				{
					add(new ScilabString(values));
					add(new ScilabList() {
						{
							add(new ScilabString(varNames));
							add(new ScilabString(varDesc));
							add(polFields);
						}
					});
				}
			};
			
			getBlock().setExprs(exprs);
		}

		/**
		 * Import the model exprs to the table models.
		 */
		private void importFromModel() {
			ScilabType rawExprs = getBlock().getExprs();
			DefaultTableModel customModel = (DefaultTableModel) varCustomizeTable.getModel();
			DefaultTableModel valuesModel = (DefaultTableModel) defaultValueTable.getModel();
			
			/*
			 * rawExprs can be instance of ScilabList when value has been
			 * previously set or instance of ScilabDouble if expression is
			 * empty. Only import expression when non empty.
			 */
			if (rawExprs instanceof ScilabList) {
				ScilabList exprs = (ScilabList) getBlock().getExprs();
				ScilabString values = (ScilabString) exprs.get(0);
				ScilabString varNames = (ScilabString) ((ScilabList) exprs.get(1)).get(0);
				ScilabString varDesc = (ScilabString) ((ScilabList) exprs.get(1)).get(1);
				
				/*
				 * Check if the data are stored as columns or as row.
				 */
				if (varDesc.getHeight() >= varDesc.getWidth()) {

					/* Title */
					valuesModel.setValueAt(varDesc.getData()[0][0], 0, 1);

					/* Loop all over the data */
					for (int i = 1; i < varDesc.getHeight(); i++) {
						customModel.addRow(new Object[] {
								i+1, varNames.getData()[i-1][0], varDesc.getData()[i][0], true
						});
						valuesModel.setValueAt(values.getData()[i-1][0], i, 1);
					}
				} else {
					/* Title */
					valuesModel.setValueAt(varDesc.getData()[0][0], 0, 1);

					/* Loop all over the data */
					for (int i = 1; i < varDesc.getHeight(); i++) {
						customModel.addRow(new Object[] {
								i+1, varNames.getData()[0][i-1], varDesc.getData()[0][i], true
						});
						valuesModel.setValueAt(values.getData()[0][i-1], i, 1);
					}
				}
			}
		}

		/**
		 * Construct the UI and install the listeners.
		 */
		private void initComponents() {

	        mainPanel = new javax.swing.JPanel();
	        tabbedPane = new javax.swing.JTabbedPane();
	        varSettings = new javax.swing.JPanel();
	        customizeMainPanel = new javax.swing.JPanel();
	        customizeScrollPane = new javax.swing.JScrollPane();
	        varCustomizeTable = new javax.swing.JTable();
	        tableManagement = new javax.swing.JPanel();
	        insert = new javax.swing.JButton();
	        delete = new javax.swing.JButton();
	        buttonBlob = new javax.swing.JPanel();
	        moveUp = new javax.swing.JButton();
	        moveDown = new javax.swing.JButton();
	        rowManagement = new javax.swing.JPanel();
	        rowLabel = new javax.swing.JLabel();
	        rowSpinner = new javax.swing.JSpinner();
	        defaultValues = new javax.swing.JPanel();
	        defaultValuesScrollPane = new javax.swing.JScrollPane();
	        defaultValueTable = new javax.swing.JTable();
	        validationPanel = new javax.swing.JPanel();
	        okButton = new javax.swing.JButton();
	        cancelButton = new javax.swing.JButton();

	        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);

	        mainPanel.setLayout(new java.awt.BorderLayout());
	        mainPanel.setBorder(BorderFactory.createEmptyBorder(2, 2, 2, 2));

	        varSettings.setLayout(new javax.swing.BoxLayout(varSettings, javax.swing.BoxLayout.PAGE_AXIS));

	        varCustomizeTable.setModel(listener.getModel().customizeTableModel);
	        customizeScrollPane.setViewportView(varCustomizeTable);
	        varCustomizeTable.setAutoCreateRowSorter(true);
	        
	        /* Update the default value table */ 
	        varCustomizeTable.getModel().addTableModelListener(listener.updateValuesTable);
	        
	        /* Activate and deactivate insertion/deletion sensible buttons/spinner */
	        varCustomizeTable.getModel().addTableModelListener(listener.updateButtonsSensibleForModifications);
	        
	        /* Activate and deactivate selection sensible buttons */
	        varCustomizeTable.getSelectionModel().addListSelectionListener(listener.updateButtonsSensibleForSelectionChange);
	        
	        customizeMainPanel.add(customizeScrollPane);

	        tableManagement.setBorder(javax.swing.BorderFactory.createEmptyBorder(2, 2, 2, 2));
	        tableManagement.setLayout(new java.awt.GridLayout(5, 1));

	        insert.setMnemonic('n');
	        insert.setText("Insert");
	        tableManagement.add(insert);
	        insert.addActionListener(listener.insertActionListener );

	        delete.setMnemonic('l');
	        delete.setText("Delete");
	        tableManagement.add(delete);
	        delete.addActionListener(listener.deleteActionListener );

	        tableManagement.add(buttonBlob);

	        moveUp.setMnemonic('u');
	        moveUp.setText("Move Up");
	        tableManagement.add(moveUp);
	        moveUp.addActionListener(listener.moveUpActionListener );

	        moveDown.setMnemonic('w');
	        moveDown.setText("Move Down");
	        tableManagement.add(moveDown);
	        moveDown.addActionListener(listener.moveDownActionListener );

	        customizeMainPanel.add(tableManagement);

	        varSettings.add(customizeMainPanel);

	        rowLabel.setText("Rows :");
	        rowManagement.add(rowLabel);

	        rowSpinner.setModel(new javax.swing.SpinnerNumberModel(1, 1, Integer.MAX_VALUE, 1));
	        rowSpinner.setEditor(new javax.swing.JSpinner.NumberEditor(rowSpinner, "######0"));
	        rowSpinner.setValue(varCustomizeTable.getModel().getRowCount());
	        rowManagement.add(rowSpinner);
	        rowSpinner.addChangeListener(listener.rowSpinnerChangeListener );

	        varSettings.add(rowManagement);

	        tabbedPane.addTab("Variable settings", varSettings);

	        defaultValueTable.setModel(listener.getModel().valuesTableModel);
	        defaultValuesScrollPane.setViewportView(defaultValueTable);

	        defaultValueTable.setAutoCreateRowSorter(false);
	        
	        defaultValues.add(defaultValuesScrollPane);

	        tabbedPane.addTab("Default Values", defaultValues);

	        mainPanel.add(tabbedPane, java.awt.BorderLayout.CENTER);

	        okButton.setText("OK");
	        validationPanel.add(okButton);
	        okButton.addActionListener(listener.okActionListener );

	        cancelButton.setText("Cancel");
	        validationPanel.add(cancelButton);
	        cancelButton.addActionListener(listener.cancelActionListener );

	        mainPanel.add(validationPanel, java.awt.BorderLayout.PAGE_END);

	        add(mainPanel);
	        
	        pack();
	        
	        cancelButton.requestFocusInWindow();
	        setResizable(false);
	    }

		private javax.swing.JPanel buttonBlob;
		private javax.swing.JButton cancelButton;
		private javax.swing.JPanel customizeMainPanel;
		private javax.swing.JScrollPane customizeScrollPane;
		private javax.swing.JTabbedPane tabbedPane;
		private javax.swing.JTable defaultValueTable;
		private javax.swing.JPanel defaultValues;
		private javax.swing.JScrollPane defaultValuesScrollPane;
		private javax.swing.JButton delete;
		private javax.swing.JButton insert;
		private javax.swing.JPanel mainPanel;
		private javax.swing.JButton moveDown;
		private javax.swing.JButton moveUp;
		private javax.swing.JButton okButton;
		private javax.swing.JLabel rowLabel;
		private javax.swing.JPanel rowManagement;
		private javax.swing.JSpinner rowSpinner;
		private javax.swing.JPanel tableManagement;
		private javax.swing.JPanel validationPanel;
		private javax.swing.JTable varCustomizeTable;
		private javax.swing.JPanel varSettings;
		
		/**
		 * Implements the models used on the frame.
		 */
		private class CustomizeFrameModel {
			
			/**
			 * Model used on the customize table.
			 */
			public final DefaultTableModel customizeTableModel = new javax.swing.table.DefaultTableModel(
		            new Object [][] {
			            	new Object[] {1, "WinTitle", "Window title", false}
			            },
			            new String [] {
			                "Row", "Variable name", "Variable description", "Editable"
			            }
			    ) {
			        Class[] types = new Class [] {
			            java.lang.Integer.class, java.lang.String.class, java.lang.String.class, java.lang.Boolean.class
			        };
			        boolean[] canEdit = new boolean [] {
			            false, true, true, true
			        };

			        public Class getColumnClass(int columnIndex) {
			            return types [columnIndex];
			        }

			        public boolean isCellEditable(int rowIndex, int columnIndex) {
			          	if (rowIndex != 0)
			           		return canEdit [columnIndex];
			           	return false;
			        }
			    };
			
		    /**
		     * Model used for the values table
		     */
			public final DefaultTableModel valuesTableModel = new javax.swing.table.DefaultTableModel(
		            new Object [][] {
		            		new Object[] {"Window title", ""}
		            },
		            new String [] {
		                "Variable names", "Values"
		            }
		        ) {
		            Class[] types = new Class [] {
		                java.lang.String.class, java.lang.String.class
		            };
		            boolean[] canEdit = new boolean [] {
		                false, true
		            };

		            public Class getColumnClass(int columnIndex) {
		                return types [columnIndex];
		            }

		            public boolean isCellEditable(int rowIndex, int columnIndex) {
		                return canEdit [columnIndex];
		            }
		        };
		}
		
		/**
		 * Implement the action listener for the frame
		 */
		private class CustomizeFrameListener {
			private CustomizeFrameModel model;
			
			public CustomizeFrameListener() {
				model = new CustomizeFrameModel();
			}
			
			public CustomizeFrameModel getModel() {
				return model;
			}
			
			public final ActionListener cancelActionListener = new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					dispose();					
				}
			};

			public final ActionListener okActionListener = new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					exportToModel();
				}
			};

			public final ChangeListener rowSpinnerChangeListener = new ChangeListener() {
				public void stateChanged(ChangeEvent e) {
					int rowCount = varCustomizeTable.getRowCount();
					int value = (Integer) rowSpinner.getModel().getValue();
					DefaultTableModel model = (DefaultTableModel) varCustomizeTable.getModel();
					
					for (; rowCount < value; rowCount++) {
						model.addRow(
								new Object[] {rowCount+1, "", true}
						);
					}
					
					for (; rowCount > value; rowCount--) {
						model.removeRow(rowCount-1);
					}
				}
			};

			public final ActionListener moveDownActionListener = new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					int selectedRow = varCustomizeTable.getSelectedRow();
					Vector<Vector> model = ((DefaultTableModel) varCustomizeTable.getModel()).getDataVector();
					
					if (selectedRow > 0 && selectedRow < varCustomizeTable.getRowCount()-1) {
						Vector current = (Vector) model.get(selectedRow);
						Vector next = (Vector) model.get(selectedRow +1);
						
						/* Inverting data*/
						model.set(selectedRow +1, current);
						model.set(selectedRow, next);
						
						/* Update the index field */
						current.set(0, ((Integer) current.get(0)) +1);
						next.set(0, ((Integer) next.get(0)) -1);
						
						/* Keep the same row selected */
						varCustomizeTable.changeSelection(selectedRow +1, varCustomizeTable.getSelectedColumn(), false, false);
					}
				}
			};

			public final ActionListener moveUpActionListener = new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					int selectedRow = varCustomizeTable.getSelectedRow();
					Vector<Vector> model = ((DefaultTableModel) varCustomizeTable.getModel()).getDataVector();
					
					if (selectedRow > 1) {
						Vector current = (Vector) model.get(selectedRow);
						Vector next = (Vector) model.get(selectedRow-1);
						
						/* Inverting data*/
						model.set(selectedRow-1, current);
						model.set(selectedRow, next);
						
						/* Update the index field */
						current.set(0, ((Integer) current.get(0)) -1);
						next.set(0, ((Integer) next.get(0)) +1);
						
						/* Keep the same row selected */
						varCustomizeTable.changeSelection(selectedRow -1, varCustomizeTable.getSelectedColumn(), false, false);
					}
				}
			};

			public final ActionListener deleteActionListener = new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					int selected = varCustomizeTable.getSelectedRow();
					int nbOfRows = varCustomizeTable.getRowCount();
					boolean hasChanged = false;
					
					if (selected > 1) {
						((DefaultTableModel) varCustomizeTable.getModel()).removeRow(selected);
						varCustomizeTable.changeSelection(selected-1, 1, false, false);
						hasChanged = true;
					} else if (nbOfRows > 1) {
						((DefaultTableModel) varCustomizeTable.getModel()).removeRow(nbOfRows -1);
						varCustomizeTable.changeSelection(nbOfRows -2, 1 , false, false);
						hasChanged = true;
					}
					
					if (hasChanged) {
						if (selected > 1) {
							/* Update the others index */
							nbOfRows--;
							for (int i = selected; i < nbOfRows; i++) {
								varCustomizeTable.getModel().setValueAt(i + 1,
										i, 0);
							}
						}
					}
				}
			};

			public final ActionListener insertActionListener = new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					((DefaultTableModel) varCustomizeTable.getModel()).addRow(
							new Object[] {varCustomizeTable.getRowCount()+1, "", true}
					);
					varCustomizeTable.changeSelection(varCustomizeTable.getRowCount()-1, 1, false, false);
				}
			};

			/**
			 * Update the buttons on selection change.  
			 */
			public final ListSelectionListener updateButtonsSensibleForSelectionChange = new ListSelectionListener() {
				public void valueChanged(ListSelectionEvent e) {
					/* We cannot move up anymore */
					boolean isFirst = false;
					/* We cannot move down anymore */
					boolean isLast = false;
					
					int selectedRow = varCustomizeTable.getSelectedRow();
					int rowCount = varCustomizeTable.getRowCount();
					
					isFirst = selectedRow <= 1;
					isLast = selectedRow == rowCount-1;
					
					moveUp.setEnabled(!isFirst);
					moveDown.setEnabled(!isLast);
				}
			};

			/**
			 * Update the buttons/spinner on modifications  
			 */
			public final TableModelListener updateButtonsSensibleForModifications = new TableModelListener() {
	        	public void tableChanged(TableModelEvent e) {
					/* We cannot delete anymore */
					boolean canDelete = false;
					
					int rowCount = model.customizeTableModel.getRowCount();
					
					canDelete = rowCount > 1;
					
					delete.setEnabled(canDelete);
					rowSpinner.setValue(rowCount);
				}
			};
			
			/**
			 * Update the values table on change on the customize table.
			 */
			public final TableModelListener updateValuesTable = new TableModelListener() {
				public void tableChanged(TableModelEvent e) {
					DefaultTableModel valuesModel = model.valuesTableModel;
					DefaultTableModel customModel = model.customizeTableModel;
					int row = e.getFirstRow();
					int column = e.getColumn();
					
					switch (e.getType()) {
					case TableModelEvent.INSERT:
						valuesModel.addRow(new Object[] {customModel.getValueAt(row, 1), ""});
						break;
						
					case TableModelEvent.DELETE:
						valuesModel.removeRow(row);
						break;
						
					case TableModelEvent.UPDATE:
						if (column == 1) {
							valuesModel.setValueAt(customModel.getValueAt(row, 1), row, 0);
						}
						break;

					default:
						break;
					}
				}
			};
		}
	}
	
	/**
	 * Ease the development of the UI (debug). 
	 * @param args
	 */
	public static void main(String args[]) {
		java.awt.EventQueue.invokeLater(new Runnable() {
			public void run() {
				new CustomizeFrame().setVisible(true);
			}
		});
	}
}
