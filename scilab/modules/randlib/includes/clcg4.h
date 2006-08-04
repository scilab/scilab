#ifndef H_CLCG4_H
#define H_CLCG4_H

#define Maxgen  100

typedef  enum {InitialSeed, LastSeed, NewSeed}  SeedType;

int set_seed_clcg4(int g, double s0, double s1, double s2, double s3);

void get_state_clcg4(int g, double s[4]);

void init_generator_clcg4(int g, SeedType Where);

void advance_state_clcg4(int g, int k);

int set_initial_seed_clcg4(double s0, double s1, double s2, double s3);

unsigned long clcg4(int g);

#endif
