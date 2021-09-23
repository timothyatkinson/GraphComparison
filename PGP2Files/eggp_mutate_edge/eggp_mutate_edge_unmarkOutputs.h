#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"
#include "common.h"

bool matcheggp_mutate_edge_unmarkOutputs(Morphism *morphism);

void applyeggp_mutate_edge_unmarkOutputs(Morphism *morphism, bool record_changes);
bool fillpoteggp_mutate_edge_unmarkOutputs(MorphismPot *pot, Morphism *morphism);

