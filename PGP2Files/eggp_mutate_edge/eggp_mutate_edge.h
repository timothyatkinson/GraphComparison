#include "common.h"
#include "graph.h"
#include "morphism.h"

extern Graph *eggp_mutate_edge_host;
extern int *eggp_mutate_edge_node_map;
extern MorphismPot *eggp_mutate_edge_pot;

int eggp_mutate_edge_execute(Graph* host_graph);
