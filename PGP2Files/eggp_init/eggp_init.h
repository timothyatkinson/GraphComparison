#include "common.h"
#include "graph.h"
#include "morphism.h"

extern Graph *eggp_init_host;
extern int *eggp_init_node_map;
extern MorphismPot *eggp_init_pot;

int eggp_init_execute(Graph* host_graph);
