/* Copyright ENPC */

/* Data structure to deal with a set of choices */

typedef struct {
  char *name;
  char *cbinfo ; 
  Widget toggle;
} SciData;

typedef struct {
  struct {
    char *name;
    char *text;
    int   num_toggles;
    int   columns;
    int  (*function)();
    int  default_toggle; /* and is set dynamically to the selected value */
    Widget label;
  } choice;
  SciData *data;
} SciStuff;

extern int SciChoiceCreate();
extern int SciChoiceFree();
extern Widget create_choices();
extern SciStuff **Everything ;



