#include "eggp.h"
#include "evolutionary_algorithm.h"
#include "algorithms.h"
#include "math.h"
#include "utils.h"

double median_double(const double *an_array, const int length);
void sort_double_array(double *array, const int length) ;
int cmp_dbl(const void * a, const void * b);

double median_double(const double *an_array, const int length) {

	int i;
	double *copy_array = (double*)malloc(length * sizeof(double));
	double median;

	/* make a copy of the array */
	for (i = 0; i < length; i++) {
		copy_array[i] = an_array[i];
	}

	/* sort the copy array */
	sort_double_array(copy_array, length);

	/* if even */
	if (length % 2 == 0) {
		median = (copy_array[(length / 2)] + copy_array[(length / 2) - 1] ) / 2;
	}

	/* if odd */
	else {
		median = copy_array[ (length - 1) / 2];
	}

	free(copy_array);

	return median;
}


/*
	sort double array using qsort
*/
void sort_double_array(double *array, const int length) {

	qsort(array, length, sizeof(double), cmp_dbl);
}

/*
	used by qsort in sort_double_array
*/
int cmp_dbl(const void * a, const void * b) {

	if ( *(double*)a < * (double*)b) {
		return -1;
	}
	if ( *(double*)a == *(double*)b ) {
		return 0;
	}
	else {
		return 1;
	}
}

int main(void){
  //Reset random seed.
  srand(time(NULL));

  //Generate a function set AND OR NAND NOR
  Function_Set* fset = get_common_fset("add,sub,mul,div,sin,cos,log,exp");
  print_fset(fset);

  //Load the local 1-Bit adder dataset.
  GP_Dataset* dataset = load_data_set("Datasets/GraphPaper/nguyen3_train.csv", 2, 0, 0, 0, 1, 20);
  GP_Dataset* test_dataset = load_data_set("Datasets/GraphPaper/nguyen3_test.csv", 2, 0, 0, 0, 1, 20);

  int repeats = 1;
  double* fitness_res = malloc(repeats * sizeof(double));
  double* evaluation_res = malloc(repeats * sizeof(double));
  double* test_res = malloc(repeats * sizeof(double));
  for(int r = 0; r < repeats; r++){
    printf("Run %d\n", r);
    exp_result* res = one_plus_lambda(dataset, test_dataset, fset, 4, 0.04, 25000);
    fitness_res[r] = res->final_fitness;
    evaluation_res[r] = (double)res->evaluations;
    test_res[r] = res->test_fitness;
    printf("Median evaluations %lf, fitness %lf, test %lf\n", median_double(evaluation_res, r+1), median_double(fitness_res, r+1), median_double(test_res, r+1));
    free(res);
  }

  freeFset(fset);
  freeDataset(dataset);
  freeDataset(test_dataset);
  free(fitness_res);
  free(evaluation_res);
  free(test_res);
}
