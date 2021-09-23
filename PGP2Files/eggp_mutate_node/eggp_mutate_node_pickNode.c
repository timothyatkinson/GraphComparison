#include "eggp_mutate_node_pickNode.h"

#include "eggp_mutate_node.h"

/* AND */
/* Gen b0 */
bool eggp_mutate_node_b0 = true;
/* Gen b1 */
bool eggp_mutate_node_b1 = true;

static bool evaluateCondition(void)
{
   return (eggp_mutate_node_b0 && eggp_mutate_node_b1);
}

static void evaluatePredicateeggp_mutate_node_0(Morphism *morphism)
{
   Assignment assignment_0 = getAssignment(morphism, 0);
   /* If the variable is not yet assigned, return. */
   if(assignment_0.type == 'n') return;
   string var_0 = getStringValue(morphism, 0);

   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_0;

   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   array1[index1++].str = "IN";

   if(!equalHostLists(array0, array1, list_length0, list_length1)) eggp_mutate_node_b0 = true;
   else eggp_mutate_node_b0 = false;
}

static void evaluatePredicateeggp_mutate_node_1(Morphism *morphism)
{
   Assignment assignment_0 = getAssignment(morphism, 0);
   /* If the variable is not yet assigned, return. */
   if(assignment_0.type == 'n') return;
   string var_0 = getStringValue(morphism, 0);

   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_0;

   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   array1[index1++].str = "OUT";

   if(!equalHostLists(array0, array1, list_length0, list_length1)) eggp_mutate_node_b1 = true;
   else eggp_mutate_node_b1 = false;
}

static bool match_n0(Morphism *morphism);

bool matcheggp_mutate_node_pickNode(Morphism *morphism)
{
   if(1 > eggp_mutate_node_host->number_of_nodes || 0 > eggp_mutate_node_host->number_of_edges) return false;
   if(match_n0(morphism)) {
      return true;
   }
   else
   {
      initialiseMorphism(morphism, eggp_mutate_node_host);
      return false;
   }
}

static bool match_n0(Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < eggp_mutate_node_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(eggp_mutate_node_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching string variable 0. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 0, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateeggp_mutate_node_0(morphism);
            evaluatePredicateeggp_mutate_node_1(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               eggp_mutate_node_b0 = true;
               eggp_mutate_node_b1 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            return true;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applyeggp_mutate_node_pickNode(Morphism *morphism, bool record_changes)
{
   int host_node_index = lookupNode(morphism, 0);
   HostLabel label_n0 = getNodeLabel(eggp_mutate_node_host, host_node_index);
   if(record_changes) pushRemarkedNode(host_node_index, label_n0.mark);
   changeNodeMark(eggp_mutate_node_host, host_node_index, 3);

   /* Reset the morphism. */
   initialiseMorphism(morphism, eggp_mutate_node_host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);

bool fillpoteggp_mutate_node_pickNode(MorphismPot *pot, Morphism *morphism)
{
   if(1 > eggp_mutate_node_host->number_of_nodes || 0 > eggp_mutate_node_host->number_of_edges) return false;
   int oldPotsize = potSize(pot);
   morphism->weight = 1.000000;
   fillpot_n0(pot, morphism);
   if(potSize(pot) > oldPotsize){
      initialiseMorphism(morphism, eggp_mutate_node_host);
      return true;
   }
   else
   {
      initialiseMorphism(morphism, eggp_mutate_node_host);
      return false;
   }
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < eggp_mutate_node_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(eggp_mutate_node_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching string variable 0. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 0, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateeggp_mutate_node_0(morphism);
            evaluatePredicateeggp_mutate_node_1(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               eggp_mutate_node_b0 = true;
               eggp_mutate_node_b1 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "eggp_mutate_node_pickNode", eggp_mutate_node_host);
            removeNodeMap(morphism, 0);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

