#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"
#include "common.h"

bool matcheggp_mutate_edge_pickNewTarget(Morphism *morphism);

void applyeggp_mutate_edge_pickNewTarget(Morphism *morphism, bool record_changes);
bool fillpoteggp_mutate_edge_pickNewTarget(MorphismPot *pot, Morphism *morphism);

