#include <time.h>
#include "common.h"
#include "debug.h"
#include "graphStacks.h"
#include "parser.h"
#include "eggp_mutate_edge.h"

#include "eggp_mutate_edge_pickEdge.h"
Morphism *M_eggp_mutate_edge_pickEdge = NULL;
#include "eggp_mutate_edge_markOutputs.h"
Morphism *M_eggp_mutate_edge_markOutputs = NULL;
#include "eggp_mutate_edge_pickNewTarget.h"
Morphism *M_eggp_mutate_edge_pickNewTarget = NULL;
#include "eggp_mutate_edge_mutateEdge.h"
Morphism *M_eggp_mutate_edge_mutateEdge = NULL;
#include "eggp_mutate_edge_unmarkOutputs.h"
Morphism *M_eggp_mutate_edge_unmarkOutputs = NULL;

static void eggp_mutate_edge_freeMorphisms(void)
{
   freeMorphism(M_eggp_mutate_edge_pickEdge);
   freeMorphism(M_eggp_mutate_edge_markOutputs);
   freeMorphism(M_eggp_mutate_edge_pickNewTarget);
   freeMorphism(M_eggp_mutate_edge_mutateEdge);
   freeMorphism(M_eggp_mutate_edge_unmarkOutputs);
}

Graph* eggp_mutate_edge_host = NULL;
int* eggp_mutate_edge_node_map = NULL;
MorphismPot* eggp_mutate_edge_pot = NULL;

static void eggp_mutate_edge_garbageCollect(void)
{
   eggp_mutate_edge_freeMorphisms();
   freePot(eggp_mutate_edge_pot);
}

bool eggp_mutate_edge_success = true;

int eggp_mutate_edge_execute(Graph* host_graph)
{
   eggp_mutate_edge_host = host_graph;
   eggp_mutate_edge_success = true;
   eggp_mutate_edge_pot = makeMorphismPot();
   emptyPot(eggp_mutate_edge_pot);
   M_eggp_mutate_edge_pickEdge = makeMorphism(2, 1, 3);
   M_eggp_mutate_edge_markOutputs = makeMorphism(2, 1, 3);
   M_eggp_mutate_edge_pickNewTarget = makeMorphism(1, 0, 2);
   M_eggp_mutate_edge_mutateEdge = makeMorphism(3, 1, 5);
   M_eggp_mutate_edge_unmarkOutputs = makeMorphism(1, 0, 1);

   /* Try Statement */
   /* Condition */
   int restore_point0 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
   do
   {
      /* Rule Call */
      emptyPot(eggp_mutate_edge_pot);
      fillpoteggp_mutate_edge_pickEdge(eggp_mutate_edge_pot, M_eggp_mutate_edge_pickEdge);
      if(potSize(eggp_mutate_edge_pot) > 0){
         MorphismHolder *holder = drawFromPot(eggp_mutate_edge_pot);
         duplicateMorphism(holder->morphism, M_eggp_mutate_edge_pickEdge, eggp_mutate_edge_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyeggp_mutate_edge_pickEdge(M_eggp_mutate_edge_pickEdge, true);
         eggp_mutate_edge_success = true;
      }
      else
      {
         eggp_mutate_edge_success = false;
         break;
      }
      emptyPot(eggp_mutate_edge_pot);
      /* Loop Statement */
      while(eggp_mutate_edge_success)
      {
         /* Rule Call */
         if(matcheggp_mutate_edge_markOutputs(M_eggp_mutate_edge_markOutputs))
         {
            applyeggp_mutate_edge_markOutputs(M_eggp_mutate_edge_markOutputs, true);
            eggp_mutate_edge_success = true;
         }
         else
         {
            eggp_mutate_edge_success = false;
         }
      }
      eggp_mutate_edge_success = true;
      /* Rule Call */
      emptyPot(eggp_mutate_edge_pot);
      fillpoteggp_mutate_edge_mutateEdge(eggp_mutate_edge_pot, M_eggp_mutate_edge_mutateEdge);
      if(potSize(eggp_mutate_edge_pot) > 0){
         MorphismHolder *holder = drawFromPot(eggp_mutate_edge_pot);
         duplicateMorphism(holder->morphism, M_eggp_mutate_edge_mutateEdge, eggp_mutate_edge_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyeggp_mutate_edge_mutateEdge(M_eggp_mutate_edge_mutateEdge, true);
         eggp_mutate_edge_success = true;
      }
      else
      {
         eggp_mutate_edge_success = false;
         break;
      }
      emptyPot(eggp_mutate_edge_pot);
      /* Loop Statement */
      while(eggp_mutate_edge_success)
      {
         /* Rule Call */
         if(matcheggp_mutate_edge_unmarkOutputs(M_eggp_mutate_edge_unmarkOutputs))
         {
            applyeggp_mutate_edge_unmarkOutputs(M_eggp_mutate_edge_unmarkOutputs, true);
            eggp_mutate_edge_success = true;
         }
         else
         {
            eggp_mutate_edge_success = false;
         }
      }
      eggp_mutate_edge_success = true;
   } while(false);

   /* Then Branch */
   if(eggp_mutate_edge_success)
   {
      discardChanges(restore_point0);
      /* Skip Statement */
      eggp_mutate_edge_success = true;
   }
   /* Else Branch */
   else
   {
      undoChanges(eggp_mutate_edge_host, restore_point0);
      eggp_mutate_edge_success = true;
      /* Skip Statement */
      eggp_mutate_edge_success = true;
   }
   eggp_mutate_edge_garbageCollect();
   return 0;
}

