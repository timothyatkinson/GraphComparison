#ifndef fast_genops_h
#define fast_genops_h

#include "graph.h"
#include "common_functions.h"

int node_mutation_fast(Graph* p1, Function_Set* fset);
int edge_mutation_fast(Graph* p1);
Graph* init_fast(int num_inputs, int num_nodes, int num_outputs, Function_Set* fset);
#endif
