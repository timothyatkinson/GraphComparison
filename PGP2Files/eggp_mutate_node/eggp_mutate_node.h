#include "common.h"
#include "graph.h"
#include "morphism.h"

extern Graph *eggp_mutate_node_host;
extern int *eggp_mutate_node_node_map;
extern MorphismPot *eggp_mutate_node_pot;

int eggp_mutate_node_execute(Graph* host_graph);
