#include "fast_genops.h"
#include "label.h"
#include "utils.h"

int node_mutation_fast(Graph* p1, Function_Set* fset){

  int inputs = count_inputs(p1);
  int outputs = count_outputs(p1);

  //printf("Mutate, %d\n", p1->nodes.size);
  //printfGraph(p1);


  //Pick an node to mutate uniformly at random. O(1)
  int node = random_int(0, p1->number_of_nodes - (inputs + outputs));

  //Find this Node. O(v)
  Node* v = NULL;
  int up_to = 0;
  for(int i = 0; i < p1->nodes.size; i++){
    Node* v2 = getNode(p1, i);
    if(v2 == NULL || v2->index == -1) continue;
    HostLabel label = v2->label;
    HostListItem *item = label.list->last;
    if(item->atom.type != 's') break;
    if(strcmp(item->atom.str, "OUT") == 0 || strcmp(item->atom.str, "IN") == 0) continue;
    if(up_to == node){
       v = v2;
     }
     up_to++;
   }

   if(v == NULL){
     printf("Exception: %dth node not found!\n", node);
     exit(0);
   }


   HostLabel label = v->label;
   HostListItem *item = label.list->last;
   if(item->atom.type != 's'){
     printf("Exception: %dth node's final label item is not a string\n", node);
     exit(0);
   }

   char* func = item->atom.str;
   int new = random_int(0, fset->functionCount - 1);

   Function* old_func = NULL;
   Function* new_func = NULL;

   up_to = 0;
   Function* first = fset->first;
   while(first != NULL){
     if(strcmp(first->name, func) == 0){
       old_func = first;
     }
     else{
       if(up_to == new){
         new_func = first;
       }
       up_to++;
     }
     first = first->next;
   }

   if(old_func == NULL){
     printf("Exception: could not find function represented by %s\n", func);
     exit(0);
   }
   if(new_func == NULL){
     printf("Exception: could not find new function %d\n", new);
     exit(0);
   }

   mark_active_blue(p1);
   int active = 0;
   if(v->label.mark == 3){
     active = 1;
   }
   unmark_graph(p1);

   //Perform node mutation.
   free(item->atom.str);
   item->atom.str = strdup(new_func->name);

   //Delete random edges.
   if(v->outdegree > new_func->arity){
     while(v->outdegree > new_func->arity){
       //Pick edge.
       int delete = random_int(0, v->outdegree);
       //Find edge.
       up_to = 0;
       Edge* del_edge = NULL;
       for(int counter = 0; counter < v->out_edges.size + 2; counter++)
       	{
       		Edge *host_edge = getNthOutEdge(p1, v, counter);
       		if(host_edge == NULL || host_edge->index == -1) continue;
          if(up_to == delete){
            del_edge = host_edge;
          }
          up_to++;
       	}
       if(del_edge == NULL){
         printf("Exception: could not find %d edge to delete\n", delete);
         exit(0);
       }
       //Delete edge.
       removeEdge(p1, del_edge->index);
     }
   }
   //Add random edges. We must preserve acyclicty and not targetting outputs.
   else if(v->outdegree < new_func->arity){

     Node* actual_source = v;
     int mutate_source = v->index;
     HostLabel source_label = actual_source->label;

     //Topological sort will allow us to find all invalid mutations in linear time O(v + e). It also handily gives us a enumeratable array of actual nodes, ignoring holes.
     int* top = reverse_topological_sort(p1);
     int* invalid = malloc(p1->nodes.size * sizeof(int));
     int num_valid = p1->number_of_nodes - 2;
     //Initially all nodes are valid. O(v)
     for(int i = 0; i < p1->nodes.size; i++){
       invalid[i] = 0;
     }


      //Find index of source in topological sort. Linear time O(v)
      int top_source = -1;
      for(int i = 0; i < p1->number_of_nodes; i++){
        if(top[i] == mutate_source){
          top_source = i;
        }
      }

      if(top_source == -1){
        printf("Exception %dth node not found!\n", mutate_source);
        exit(0);
      }

     invalid[mutate_source] = 1;

     //Move rightwards in the reverse topological sorting (up the jungle), eliminating any node v with a path to the source and any output node. Linear time O(v + e)
     for(int i = 0; i < p1->number_of_nodes; i++){
       //Find the node
        Node* v = getNode(p1, top[i]);
        HostLabel label = v->label;

       //Any node with a path to the source is invalid. So we can propagate based on incoming edges. Because we have topological sorted, i must be greater than the topological index of the source.
       if(i >= top_source && invalid[top[i]] == 1){
         for(int counter = 0; counter < v->in_edges.size + 2; counter++)
          {
            Edge *host_edge = getNthInEdge(p1, v, counter);
            if(host_edge == NULL || host_edge->index == -1) continue;
            if(invalid[host_edge->source] != 1){
              invalid[host_edge->source] = 1;
              num_valid--;
            }
          }
       }

       //All output nodes are invalid.
       HostListItem *item = label.list->last;
       if(item->atom.type == 's' && strcmp(item->atom.str, "OUT") == 0 && invalid[top[i]] != 1){
         invalid[top[i]] = 1;
         num_valid--;
       }
     }

     while(v->outdegree < new_func->arity){
        //Only proceed if we can manage to do a valid addition.
        if(num_valid > 0){
          int new_target = random_int(0, num_valid);

          Node* new = NULL;
          //Find the new target. O(n)
          up_to = 0;
          for(int i = 0; i < p1->nodes.size; i++){
            Node* node = getNode(p1, i);
            if(node == NULL || node->index == -1 || invalid[i] == 1) continue;
            if(up_to == new_target){
              new = node;
            }
            up_to++;
          }

          if(new == NULL){
            printf("Error: could not find %dth valid node.\n", new_target);
            exit(0);
          }

          //Actually perform the insertion.

          HostAtom array2[1];
          HostLabel label2;
          array2[0].type = 'i';
          array2[0].num = 0;
          HostList *list2 = makeHostList(array2, 1, false);
          label2 = makeHostLabel(0, 1, list2);
          addEdge(p1, label2, mutate_source, new->index);
        }
        else{
          printf("Exception: could not insert new edge!\n");
          printf("Mutate Node %d; %s->%s\n", v->index, old_func->name, new_func->name);
          printfGraph(p1);
          exit(0);
        }
      }
      free(top);
      free(invalid);
   }

   //Shuffle the node's inputs.
   int* new_order = random_order(v->outdegree);
   up_to = 0;
   Edge* del_edge = NULL;
   for(int counter = 0; counter < v->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(p1, v, counter);
      if(host_edge == NULL || host_edge->index == -1) continue;
      HostList* l = host_edge->label.list;
      HostListItem* i = l->first;
      i->atom.num = new_order[up_to];
      up_to++;
    }

   free(new_order);

   return active;
}

int edge_mutation_fast(Graph* p1){

  //Pick an edge to mutate uniformly at random. O(1)
  int edge = random_int(0, p1->number_of_edges);

  //Find this edge. O(e)
  Edge* e = NULL;
  int up_to = 0;
  for(int i = 0; i < p1->edges.size; i++){
    Edge* e2 = getEdge(p1, i);
    if(e2 == NULL || e2->index == -1) continue;
    if(up_to == edge){
       e = e2;
     }
     up_to++;
   }

   if(e == NULL){
     printf("Exception: %dth edge not found!\n", edge);
     exit(0);
   }

   int mutate_source = e->source;
   int mutate_target = e->target;

   Node* actual_source = getNode(p1, mutate_source);
   mark_active_blue(p1);
   int active = 0;
   if(actual_source->label.mark == 3){
     active = 1;
   }
   unmark_graph(p1);
   HostLabel source_label = actual_source->label;

   //Topological sort will allow us to find all invalid mutations in linear time O(v + e). It also handily gives us a enumeratable array of actual nodes, ignoring holes.
   int* top = reverse_topological_sort(p1);
   int* invalid = malloc(p1->nodes.size * sizeof(int));
   int num_valid = p1->number_of_nodes - 2;
   //Initially all nodes are valid. O(v)
   for(int i = 0; i < p1->nodes.size; i++){
     invalid[i] = 0;
   }

   //printf("Checking \n");
   //printfGraph(p1);
   //printf("%d -> %d\n", mutate_source, mutate_target);


    //Find index of source in topological sort. Linear time O(v)
    int top_source = -1;
    int top_target = -1;
    for(int i = 0; i < p1->number_of_nodes; i++){
      if(top[i] == mutate_source){
        top_source = i;
      }
      if(top[i] == mutate_target){
        top_target = i;
      }
    }

    if(top_source == -1){
      printf("Exception %dth source node not found!\n", mutate_source);
      exit(0);
    }
    if(top_target == -1){
      printf("Exception %dth target node not found!\n", mutate_target);
      exit(0);
    }


   invalid[mutate_source] = 1;
   invalid[mutate_target] = 1;

   //Move rightwards in the reverse topological sorting (up the jungle), eliminating any node v with a path to the source, any output node. Linear time O(v + e)
   for(int i = 0; i < p1->number_of_nodes; i++){
     //Find the node
      Node* v = getNode(p1, top[i]);
      HostLabel label = v->label;

     //Any node with a path to the source is invalid. So we can propagate based on incoming edges. Because we have topological sorted, i must be greater than the topological index of the source.
     if(i >= top_source && invalid[top[i]] == 1){
       for(int counter = 0; counter < v->in_edges.size + 2; counter++)
        {
          Edge *host_edge = getNthInEdge(p1, v, counter);
          if(host_edge == NULL || host_edge->index == -1) continue;
          if(invalid[host_edge->source] != 1){
            invalid[host_edge->source] = 1;
            num_valid--;
          }
        }
     }

     //All output nodes are invalid.
     HostListItem *item = label.list->last;
     if(item->atom.type == 's' && strcmp(item->atom.str, "OUT") == 0 && invalid[top[i]] != 1){
       invalid[top[i]] = 1;
       num_valid--;
     }
   }

   //Only proceed if we can manage to do a valid mutation.
   if(num_valid > 0){
     int new_target = random_int(0, num_valid);

     Node* new = NULL;
     //Find the new target. O(n)
     int up_to = 0;
     for(int i = 0; i < p1->nodes.size; i++){
       Node* node = getNode(p1, i);
       if(node == NULL || node->index == -1 || invalid[node->index] == 1) continue;
       if(up_to == new_target){
         new = node;
       }
       up_to++;
     }

     if(new == NULL){
       exit(0);
     }

     //Actually perform the mutation.
     HostLabel lbcpy = makeHostLabel(e->label.mark, e->label.length, copyHostList(e->label.list));
     removeEdge(p1, e->index);
     addEdge(p1, lbcpy, mutate_source, new->index);
   }
   free(top);
   free(invalid);
   return active;
}

Graph* init_fast(int num_inputs, int num_nodes, int num_outputs, Function_Set* fset){
  int total = num_inputs + num_nodes + num_outputs;
  int* node_array = malloc(total * sizeof(int));
  Graph* host = build_empty_host_graph();

  int up_to = 0;
  for(int i = 0; i < num_inputs; i++){
    //Add a node for each input labelled i:"IN" where i is the number of the input and "IN" indicates that it is indeed an input
    HostAtom array[2];
    HostLabel label;
    array[0].type = 'i';
    array[0].num = i;
    array[1].type = 's';
    array[1].str = "IN";
    HostList *list = makeHostList(array, 2, false);
    label = makeHostLabel(0, 2, list);
    node_array[up_to] = addNode(host, 0, label);
    up_to += 1;
  }

  /* OUTPUTS */
  for(int i = 0; i < num_outputs; i++){
    //Add a node for each output labelled i:"OUT" where i is the number of the output and "OUT" indicates that it is indeed an output
    HostAtom array[2];
    HostLabel label;
    array[0].type = 'i';
    array[0].num = i;
    array[1].type = 's';
    array[1].str = "OUT";
    HostList *list = makeHostList(array, 2, false);
    label = makeHostLabel(0, 2, list);
    int o_node = addNode(host, 0, label);
    node_array[up_to] = o_node;
    up_to += 1;
  }

  for(int i = 0; i < num_nodes; i++){
    HostAtom array[1];
    HostLabel label;

    int func_index = random_int(0, fset->functionCount);

    int up_to_func = 0;
    Function* first = fset->first;
    Function* func = NULL;
    while(first != NULL && func == NULL){
      if(up_to_func == func_index){
        func = first;
      }
      up_to_func++;
      first = first->next;
    }

    array[0].type = 's';
    array[0].str = func->name;
    HostList *list = makeHostList(array, 1, false);
    label = makeHostLabel(0, 1, list);
    int f_node = addNode(host, 0, label);
    node_array[up_to] = f_node;

    for(int a = 0; a < func->arity; a++){
      int target = random_int(0, up_to - num_outputs);
      if(target >= num_inputs){
        target += num_outputs;
      }
      HostAtom edge_array[1];
      HostLabel edge_label;
      edge_array[0].type = 'i';
      edge_array[0].num = a;
      HostList *edge_list = makeHostList(edge_array, 1, false);
      edge_label = makeHostLabel(0, 1, edge_list);
      addEdge(host, edge_label, f_node, node_array[target]);
    }
    up_to += 1;
  }

  for(int i = 0; i < num_outputs; i++){
    int o_node = node_array[num_inputs + i];
    int target = random_int(0, num_inputs + num_nodes);
    if(target >= num_inputs){
      target += num_outputs;
    }
    HostAtom edge_array[1];
    HostLabel edge_label;
    edge_array[0].type = 'i';
    edge_array[0].num = 0;
    HostList *edge_list = makeHostList(edge_array, 1, false);
    edge_label = makeHostLabel(0, 1, edge_list);
    addEdge(host, edge_label, o_node, node_array[target]);
  }

  free(node_array);

  return host;
}
