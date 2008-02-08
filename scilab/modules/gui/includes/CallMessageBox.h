#ifndef __CALLMESSAGEBOX_H__
#define __CALLMESSAGEBOX_H__

/* Copyright INRIA */
/* Vincent COUVERT 2008 */

/**
 * Create a new MessageBox
 *
 * @param void  
 * @return the ID of the created MessageBox
 */
int createMessageBox(void);

/**
 * Set the title of the MessageBox
 *
 * @param ID the ID of the MessageBox    
 * @param title the title to set
 */
void setMessageBoxTitle(int ID, char * title);

/**
 * Set the message of the MessageBox
 *
 * @param ID the ID of the MessageBox    
 * @param title the message to set
 */
void setMessageBoxMessage(int ID, char * message);

/**
 * Display the MessageBox and Wait for a user action
 *
 * @param ID the ID of the MessageBox  
 */
void messageBoxDisplayAndWait(int ID);

/**
 * Get the index of the selected button
 *
 * @param ID the ID of the MessageBox  
 * @return the index of the selected button
 */
int getMessageBoxSelectedButton(int ID);

/**
 * Set the labels of the buttons
 *
 * @param ID the ID of the MessageBox  
 * @param labels the labels of the buttons
 * @param nbLabels the number of labels in labels parameter
 */
void setMessageBoxButtonsLabels(int ID, char** labels, int nbLabels);


#endif /* !__CALLMESSAGEBOX_H__ */

