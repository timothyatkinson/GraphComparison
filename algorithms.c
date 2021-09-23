#include "algorithms.h"
#include "eggp.h"
#include "graph.h"

double mean_absolute_error(Graph* individual, GP_Dataset* dataset, Function_Set* f_set);
double mean_absolute_error(Graph* individual, GP_Dataset* dataset, Function_Set* f_set){
  double score = gp_evaluate(individual, dataset, f_set) / (double)(dataset->outputs * dataset->rows);
  if(isnan(score) || isinf(score)){
    return 1e20;
  }
  return score;
}

exp_result* make_result(int evaluations, double fitness, double test_fitness);
exp_result* make_result(int evaluations, double fitness, double test_fitness){
  exp_result* res = malloc(sizeof(exp_result));
  res->evaluations = evaluations;
  res->final_fitness = fitness;
  res->test_fitness = test_fitness;
  return res;
}

void print_node_as_string(Graph* graph, Node* node);
void print_node_as_string(Graph* graph, Node* node){
  HostLabel label = node->label;
  HostListItem *item = label.list->first;
  HostListItem *item2 = label.list->last;
  if(strcmp(item2->atom.str, "IN") == 0){
    printf(" i%d ", item->atom.num);
  }
  else{
    printf(" %s(", item2->atom.str);
    Node** targets = malloc(node->outdegree * sizeof(Node*));

    int counter;
    for(counter = 0; counter < node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(graph, node, counter);
      if(host_edge == NULL) continue;
      HostLabel edge_label = host_edge->label;
      HostListItem *edge_item = edge_label.list->first;
      targets[edge_item->atom.num] = getNode(graph, host_edge->target);
    }
    for(counter = 0; counter < node->outdegree; counter++){
      if(counter > 0){
        printf(", ");
      }
      print_node_as_string(graph, targets[counter]);
    }

    free(targets);

    printf(") ");
  }
}

void print_graph_as_string(Graph* graph);
void print_graph_as_string(Graph* graph){
  for(int n = 0; n < graph->nodes.size; n++){
    Node* v = getNode(graph, n);
    if(v != NULL && v->index != -1){
      HostLabel label = v->label;

      HostListItem *item = label.list->first;
      HostListItem *item2 = label.list->last;
      if(item->atom.type != 'i') continue;
      if(item2->atom.type != 's') continue;
      if(strcmp(item2->atom.str, "OUT") == 0){
        printf("o%d = ", item->atom.num);
        int counter;
        for(counter = 0; counter < v->out_edges.size + 2; counter++)
        {
          Edge *host_edge = getNthOutEdge(graph, v, counter);
          if(host_edge == NULL) continue;
          int target = host_edge->target;
    			print_node_as_string(graph, getNode(graph, target));
        }
        printf("\n");
      }
    }
  }
}

static int lam_update_rate = 4000;
static int gen_update_rate = 20;

exp_result* one_plus_lambda(GP_Dataset* dataset, GP_Dataset* test_dataset, Function_Set* f_set, int lambda, double mutation_rate, int max_generations){

  int pop_size = 1 + lambda;

  EGGP_init_env* init_env = default_eggp_init_env(dataset, f_set);
  init_env->pop_size = pop_size;
  Graph** population = eggp_init((uintptr_t) init_env);
  double* pop_scores = malloc(pop_size * sizeof(double));
  double best_score = -1.0;
  int best_index = -1;

  printf("Initial population scores:\n");
  for(int l = 0; l < pop_size; l++){
    pop_scores[l] = mean_absolute_error(population[l], dataset, f_set);
    printf("%lf", pop_scores[l]);
    if(l < pop_size - 1){
      printf(", ");
    }
    if(best_score == -1.0 || pop_scores[l] < best_score){
      best_score = pop_scores[l];
      best_index = l;
    }
  }
  printf(" with best %lf (%d)\n", best_score, best_index);

  int generation = 1;
  while(best_score > 1e-6 && generation < max_generations){
    if(generation % lam_update_rate == 0){
      printf("Generation %d, pop scores:", generation);
      for(int l = 0; l < pop_size; l++){
        printf("%lf", pop_scores[l]);
        if(l < pop_size - 1){
          printf(", ");
        }
      }
      printf(" with best %lf (%d - %lf)\n", best_score, best_index, mean_absolute_error(population[best_index], dataset, f_set));
    }

    int new_best_index = best_index;
    double new_best_score = best_score;
    for(int l = 0; l < pop_size; l++){
      if(l != best_index){
        freeGraph(population[l]);
        population[l] = eggp_mutate(population[best_index], f_set, mutation_rate);
        pop_scores[l] = mean_absolute_error(population[l], dataset, f_set);
        if(pop_scores[l] <= new_best_score + 1e-7){
          new_best_score = pop_scores[l];
          new_best_index = l;
        }
      }
    }
    best_index = new_best_index;
    best_score = new_best_score;

    generation += 1;
  }


  //printfGraph(population[best_index]);
  double test_performance = mean_absolute_error(population[best_index], test_dataset, f_set);
  //printfGraph(population[best_index]);
  //print_graph_as_string(population[best_index]);
  //gp_print_evaluate(population[best_index], test_dataset, f_set);
  free_graph_array(population, pop_size);
  free(pop_scores);
  free(init_env);

  printf("Terminated at generation %d with fitness %lf, %lf\n", generation, best_score, test_performance);

  return make_result(generation * lambda, best_score, test_performance);
}

int* tournament(double* fitness, int pop_size, int tournament_size);
int* tournament(double* fitness, int pop_size, int tournament_size){
  int* tournament_results = malloc(2 * sizeof(int));
  //int* visited = malloc(pop_size * sizeof(int));
  //for(int p = 0; p < pop_size; p++){
  //  visited[p] = 0;
  //}
  double winner_fitness = -1.0;
  double loser_fitness = -1.0;
  int winner_index = -1;
  int loser_index = -1;
  int candidate_1 = random_int(0, pop_size);
  int candidate_2 = random_int(0, pop_size);
  while(candidate_1 == candidate_2){
    candidate_2 = random_int(0, pop_size);
  }
  if(fitness[candidate_1] < fitness[candidate_2]){
    winner_index = candidate_1;
    winner_fitness = fitness[candidate_1];
    loser_index = candidate_2;
    loser_fitness = fitness[candidate_2];
  }
  else{
    winner_index = candidate_2;
    winner_fitness = fitness[candidate_2];
    loser_index = candidate_1;
    loser_fitness = fitness[candidate_1];
  }
  //visited[candidate_1] = 1;
  //visited[candidate_2] = 1;
  for(int i = 0; i < tournament_size - 2; i++){
    int candidate = random_int(0, pop_size);
    //while(visited[candidate] == 1){
    //  candidate = random_int(0, pop_size);
    //}
    double candidate_fitness = fitness[candidate];
    if(candidate_fitness < winner_fitness){
      winner_fitness = candidate_fitness;
      winner_index = candidate;
    }
    else if(candidate_fitness > loser_fitness){
      loser_fitness = candidate_fitness;
      loser_index = candidate;
    }
    //visited[candidate] = 1;
  }
  //free(visited);
  tournament_results[0] = winner_index;
  tournament_results[1] = loser_index;
  return tournament_results;
}

exp_result* generational(GP_Dataset* dataset, GP_Dataset* test_dataset, Function_Set* f_set, int pop_size, int tournament_size, double mutation_rate, int max_generations){

  EGGP_init_env* init_env = default_eggp_init_env(dataset, f_set);
  init_env->pop_size = pop_size;
  Graph** population = eggp_init((uintptr_t) init_env);
  double* pop_scores = malloc(pop_size * sizeof(double));
  double best_score = -1.0;
  int best_index = -1;

  printf("Initial population scores:\n");
  for(int l = 0; l < pop_size; l++){
    pop_scores[l] = mean_absolute_error(population[l], dataset, f_set);
    if(l < 10){
      printf("%lf", pop_scores[l]);
      if(l < pop_size - 1){
        printf(", ");
      }
    }
    else if (l == 10){
      printf("...");
    }
    if(best_score == -1.0 || pop_scores[l] < best_score){
      best_score = pop_scores[l];
      best_index = l;
    }
  }
  printf(" Best %lf (%d)\n", best_score, best_index);

  int generation = 1;

  while(best_score > 1e-6 && generation < max_generations){
    if(generation % gen_update_rate == 0){
      printf("Generation %d, pop scores: ", generation);
      for(int l = 0; l < pop_size; l++){
        if(l < 10){
          printf("%lf", pop_scores[l]);
          if(l < pop_size - 1){
            printf(", ");
          }
        }
        else if (l == 10){
          printf("...");
        }
      }
      printf(" Best %lf (%d)\n", best_score, best_index);
    }

    Graph** new_population = malloc(pop_size * sizeof(Graph*));
    double* new_scores = malloc(pop_size * sizeof(double));
    int new_best_index = best_index;
    double new_best_fitness = best_score;
    for(int p = 0; p < pop_size; p++){
      if(p == best_index){
        new_population[p] = duplicate_graph(population[p]);
        new_scores[p] = best_score;
      }
      else{
        int* tourn_results = tournament(pop_scores, pop_size, tournament_size);
        int winner = tourn_results[0];
        int loser = tourn_results[1];
        new_population[p] = eggp_mutate(population[winner], f_set, mutation_rate);
        new_scores[p] = mean_absolute_error(new_population[p], dataset, f_set);
        if (new_scores[p] <= new_best_fitness + 1e-7){
          new_best_index = p;
          new_best_fitness = new_scores[p];
        }
        free(tourn_results);
      }
    }
    free_graph_array(population, pop_size);
    free(pop_scores);
    population = new_population;
    pop_scores = new_scores;
    best_index = new_best_index;
    best_score = new_best_fitness;
    generation += 1;
  }

  double test_performance = mean_absolute_error(population[best_index], test_dataset, f_set);

  free_graph_array(population, pop_size);
  free(pop_scores);
  free(init_env);

  return make_result(generation * pop_size, best_score, test_performance);
}


exp_result* steady_state(GP_Dataset* dataset, GP_Dataset* test_dataset, Function_Set* f_set, int pop_size, int tournament_size, double mutation_rate, int max_generations){

  EGGP_init_env* init_env = default_eggp_init_env(dataset, f_set);
  init_env->pop_size = pop_size;
  Graph** population = eggp_init((uintptr_t) init_env);
  double* pop_scores = malloc(pop_size * sizeof(double));
  double best_score = -1.0;
  int best_index = -1;

  printf("Initial population scores:\n");
  for(int l = 0; l < pop_size; l++){
    pop_scores[l] = mean_absolute_error(population[l], dataset, f_set);
    if(l < 10){
      printf("%lf", pop_scores[l]);
      if(l < pop_size - 1){
        printf(", ");
      }
    }
    else if (l == 10){
      printf("...");
    }
    if(best_score == -1.0 || pop_scores[l] < best_score){
      best_score = pop_scores[l];
      best_index = l;
    }
  }
  printf(" Best %lf (%d)\n", best_score, best_index);

  int generation = 1;

  while(best_score > 1e-6 && generation < max_generations){
    if(generation % gen_update_rate == 0){
      printf("Generation %d, pop scores: ", generation);
      for(int l = 0; l < pop_size; l++){
        if(l < 10){
          printf("%lf", pop_scores[l]);
          if(l < pop_size - 1){
            printf(", ");
          }
        }
        else if (l == 10){
          printf("...");
        }
      }
      printf(" Best %lf (%d)\n", best_score, best_index);
    }
    int new_best_index = best_index;
    double new_best_fitness = best_score;
    for(int p = 0; p < pop_size / 2; p++){
      int* tourn_results1 = tournament(pop_scores, pop_size, tournament_size);
      int* tourn_results2 = tournament(pop_scores, pop_size, tournament_size);
      int winner1 = tourn_results1[0];
      int loser1 = tourn_results1[1];
      int winner2 = tourn_results2[0];
      int loser2 = tourn_results2[1];
      while (loser1 == loser2 || winner1 == winner2){
        free(tourn_results1);
        free(tourn_results2);
        tourn_results1 = tournament(pop_scores, pop_size, tournament_size);
        tourn_results2 = tournament(pop_scores, pop_size, tournament_size);
        winner1 = tourn_results1[0];
        loser1 = tourn_results1[1];
        winner2 = tourn_results2[0];
        loser2 = tourn_results2[1];
      }
      Graph* new1 = eggp_mutate(population[winner1], f_set, mutation_rate);
      Graph* new2 = eggp_mutate(population[winner2], f_set, mutation_rate);
      freeGraph(population[loser1]);
      freeGraph(population[loser2]);
      population[loser1] = new1;
      population[loser2] = new2;
      pop_scores[loser1] = mean_absolute_error(population[loser1], dataset, f_set);
      if (pop_scores[loser1] <= new_best_fitness + 1e-7){
        new_best_index = loser1;
        new_best_fitness = pop_scores[loser1];
      }
      pop_scores[loser2] = mean_absolute_error(population[loser2], dataset, f_set);
      if (pop_scores[loser2] <= new_best_fitness + 1e-7){
        new_best_index = loser2;
        new_best_fitness = pop_scores[loser2];
      }
      free(tourn_results1);
      free(tourn_results2);
    }
    best_index = new_best_index;
    best_score = new_best_fitness;
    generation += 1;
  }

  double test_performance = mean_absolute_error(population[best_index], test_dataset, f_set);

  free_graph_array(population, pop_size);
  free(pop_scores);
  free(init_env);

  return make_result(generation * pop_size, best_score, test_performance);
}
