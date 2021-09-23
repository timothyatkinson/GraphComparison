#ifndef UTILS_H
#define UTILS_H

//C Libraries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//P-GP2 Libraries
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

//Util graph marking functions
void mark_active_blue(Graph* hostG);
void mark_active_children_blue(Graph* hostG, Node *node);
void unmark_graph(Graph* hostG);
//Counts the number of nodes connected to an output in a computational network
int count_active_nodes(Graph* hostG, int inputs, int outputs);
//Counts the number of edges connected to an output in a computational network
int count_active_edges(Graph* hostG);
//Returns the red component of a graph
Graph* get_red(Graph* red_blue);
//Returns the blue component of a graph
Graph* get_blue(Graph* red_blue);
//Returns the argument mark component of a graph, marked the target_mark argument.
Graph* get_mark(Graph* multi_mark, int mark, int target_mark);
//Duplicates a graph
Graph* build_empty_host_graph();

void free_graph_array(Graph** array, int graphs);
void free_graph_data(Graph* graph);
int random_int(int min, int max);
int* random_order(int entries);
void write_results(char* file_name, double* fitness, double* test, double* evals, int num_runs);

//Performs a topological sort on a graph in O(v + e) time.
int* topological_sort(Graph* host);

int count_inputs(Graph* hostG);

int count_outputs(Graph* hostG);

//Performs a reverse topological sort in O(v + e) time. Uses the topological_sort function and then reverses the result.
int* reverse_topological_sort(Graph* host);
#endif
