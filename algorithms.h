#ifndef algorithms_h
#define algorithms_h


#include "common_functions.h"
#include "generic_operators.h"

typedef struct exp_result{
  int evaluations;
  double final_fitness;
  double test_fitness;
} exp_result;

exp_result* one_plus_lambda(GP_Dataset* dataset, GP_Dataset* test_dataset, Function_Set* f_set, int lambda, double mutation_rate, int max_generations);
exp_result* generational(GP_Dataset* dataset, GP_Dataset* test_dataset, Function_Set* f_set, int pop_size, int tournament_size, double mutation_rate, int max_generations);
exp_result* steady_state(GP_Dataset* dataset, GP_Dataset* test_dataset, Function_Set* f_set, int pop_size, int tournament_size, double mutation_rate, int max_generations);
#endif
