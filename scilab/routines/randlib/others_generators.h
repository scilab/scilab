#ifndef SCI_OTHER_GEN
#define SCI_OTHER_GEN

/* header for the Mersenne Twister */
unsigned long randmt();
int set_state_mt_simple(double s);
int set_state_mt(double seed_array[]);
void get_state_mt(double state[]);

/* header for kiss */
unsigned long kiss();
int set_state_kiss(double g1, double g2, double g3, double g4);
void get_state_kiss(double g[]);

/* header for clcg2 */
unsigned long clcg2();
int set_state_clcg2(double g1, double g2);
void get_state_clcg2(double g[]);

/* header for scilab urand */
unsigned long urandc();
int set_state_urand(double g);
void get_state_urand(double g[]);

/* header for scilab fsultra */
unsigned long fsultra();
int set_state_fsultra(double g[]);
int set_state_fsultra_simple(double g1,double g2);
void get_state_fsultra(double g[]);

#endif /** SCI_OTHER_GEN   **/




