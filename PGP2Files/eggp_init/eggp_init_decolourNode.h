#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"
#include "common.h"

bool matcheggp_init_decolourNode(Morphism *morphism);

void applyeggp_init_decolourNode(Morphism *morphism, bool record_changes);
bool fillpoteggp_init_decolourNode(MorphismPot *pot, Morphism *morphism);

