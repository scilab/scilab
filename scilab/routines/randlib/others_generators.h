#ifndef SCI_OTHER_GEN
#define SCI_OTHER_GEN

/* header for the Mersenne Twister */
double randmt();
int set_state_mt_simple(double s);
int set_state_mt(double seed_array[]);
void get_state_mt(double state[]);

/* header for kiss */
double kiss();
int set_state_kiss(double g1, double g2, double g3, double g4);
void get_state_kiss(double g[]);

/* header for clcg2 */
double clcg2();
int set_state_clcg2(double g1, double g2);
void get_state_clcg2(double g[]);

/* header for scilab urand */
extern double C2F(urand)(integer *);

#endif /** SCI_OTHER_GEN   **/




