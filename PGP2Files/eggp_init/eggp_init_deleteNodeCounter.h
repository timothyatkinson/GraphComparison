#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"
#include "common.h"

bool matcheggp_init_deleteNodeCounter(Morphism *morphism);

void applyeggp_init_deleteNodeCounter(Morphism *morphism, bool record_changes);
bool fillpoteggp_init_deleteNodeCounter(MorphismPot *pot, Morphism *morphism);

