#include <time.h>
#include "common.h"
#include "debug.h"
#include "graphStacks.h"
#include "parser.h"
#include "eggp_init.h"

#include "eggp_init_initNodeCounter.h"
Morphism *M_eggp_init_initNodeCounter = NULL;
#include "eggp_init_addNode.h"
Morphism *M_eggp_init_addNode = NULL;
#include "eggp_init_addEdgeToArity.h"
Morphism *M_eggp_init_addEdgeToArity = NULL;
#include "eggp_init_decolourNode.h"
Morphism *M_eggp_init_decolourNode = NULL;
#include "eggp_init_connectOutput.h"
Morphism *M_eggp_init_connectOutput = NULL;
#include "eggp_init_deleteNodeCounter.h"
Morphism *M_eggp_init_deleteNodeCounter = NULL;

static void eggp_init_freeMorphisms(void)
{
   freeMorphism(M_eggp_init_initNodeCounter);
   freeMorphism(M_eggp_init_addNode);
   freeMorphism(M_eggp_init_addEdgeToArity);
   freeMorphism(M_eggp_init_decolourNode);
   freeMorphism(M_eggp_init_connectOutput);
   freeMorphism(M_eggp_init_deleteNodeCounter);
}

Graph* eggp_init_host = NULL;
int* eggp_init_node_map = NULL;
MorphismPot* eggp_init_pot = NULL;

static void eggp_init_garbageCollect(void)
{
   eggp_init_freeMorphisms();
   freePot(eggp_init_pot);
}

bool eggp_init_success = true;

int eggp_init_execute(Graph* host_graph)
{
   eggp_init_host = host_graph;
   eggp_init_success = true;
   eggp_init_pot = makeMorphismPot();
   emptyPot(eggp_init_pot);
   M_eggp_init_initNodeCounter = makeMorphism(1, 0, 1);
   M_eggp_init_addNode = makeMorphism(2, 0, 3);
   M_eggp_init_addEdgeToArity = makeMorphism(3, 0, 5);
   M_eggp_init_decolourNode = makeMorphism(1, 0, 1);
   M_eggp_init_connectOutput = makeMorphism(2, 0, 3);
   M_eggp_init_deleteNodeCounter = makeMorphism(1, 0, 1);

   /* Rule Call */
   if(matcheggp_init_initNodeCounter(M_eggp_init_initNodeCounter))
   {
      applyeggp_init_initNodeCounter(M_eggp_init_initNodeCounter, false);
      eggp_init_success = true;
   }
   else
   {
      printf("No output graph: rule eggp_init_initNodeCounter not applicable.\n");
      eggp_init_garbageCollect();
      return 0;
   }
   /* Loop Statement */
   int restore_point0 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
   while(eggp_init_success)
   {
      /* Rule Call */
      emptyPot(eggp_init_pot);
      fillpoteggp_init_addNode(eggp_init_pot, M_eggp_init_addNode);
      if(potSize(eggp_init_pot) > 0){
         MorphismHolder *holder = drawFromPot(eggp_init_pot);
         duplicateMorphism(holder->morphism, M_eggp_init_addNode, eggp_init_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyeggp_init_addNode(M_eggp_init_addNode, true);
         eggp_init_success = true;
      }
      else
      {
         eggp_init_success = false;
         undoChanges(eggp_init_host, restore_point0);
      }
      emptyPot(eggp_init_pot);
      if(!eggp_init_success) break;

      /* Loop Statement */
      while(eggp_init_success)
      {
         /* Rule Call */
         emptyPot(eggp_init_pot);
         fillpoteggp_init_addEdgeToArity(eggp_init_pot, M_eggp_init_addEdgeToArity);
         if(potSize(eggp_init_pot) > 0){
            MorphismHolder *holder = drawFromPot(eggp_init_pot);
            duplicateMorphism(holder->morphism, M_eggp_init_addEdgeToArity, eggp_init_host);
            freeMorphism(holder->morphism);
            free(holder);
            applyeggp_init_addEdgeToArity(M_eggp_init_addEdgeToArity, true);
            eggp_init_success = true;
         }
         else
         {
            eggp_init_success = false;
         }
         emptyPot(eggp_init_pot);
      }
      eggp_init_success = true;
      if(!eggp_init_success) break;

      /* Rule Call */
      if(matcheggp_init_decolourNode(M_eggp_init_decolourNode))
      {
         applyeggp_init_decolourNode(M_eggp_init_decolourNode, true);
         eggp_init_success = true;
      }
      else
      {
         eggp_init_success = false;
         undoChanges(eggp_init_host, restore_point0);
      }
      /* Graph changes from loop body may not have been used.
         Discard them so that future graph roll backs are uncorrupted. */
      if(eggp_init_success) discardChanges(restore_point0);
   }
   eggp_init_success = true;
   /* Loop Statement */
   while(eggp_init_success)
   {
      /* Rule Call */
      emptyPot(eggp_init_pot);
      fillpoteggp_init_connectOutput(eggp_init_pot, M_eggp_init_connectOutput);
      if(potSize(eggp_init_pot) > 0){
         MorphismHolder *holder = drawFromPot(eggp_init_pot);
         duplicateMorphism(holder->morphism, M_eggp_init_connectOutput, eggp_init_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyeggp_init_connectOutput(M_eggp_init_connectOutput, false);
         eggp_init_success = true;
      }
      else
      {
         eggp_init_success = false;
      }
      emptyPot(eggp_init_pot);
   }
   eggp_init_success = true;
   /* Rule Call */
   if(matcheggp_init_deleteNodeCounter(M_eggp_init_deleteNodeCounter))
   {
      applyeggp_init_deleteNodeCounter(M_eggp_init_deleteNodeCounter, false);
      eggp_init_success = true;
   }
   else
   {
      printf("No output graph: rule eggp_init_deleteNodeCounter not applicable.\n");
      eggp_init_garbageCollect();
      return 0;
   }
   eggp_init_garbageCollect();
   return 0;
}

