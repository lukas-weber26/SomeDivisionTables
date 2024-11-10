#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

typedef struct division_table {
	double minx;
	int ndx;
	double dx;
	double * data;
} division_table;

division_table division_table_create(double minx, int ndx) {
	assert(minx > 0);
	division_table new_table = {minx, ndx, 1.0};
	new_table.data = malloc(sizeof(double)*ndx*ndx);

	for (int i = 0; i < ndx; i ++) {
		for (int j = 0; j < ndx; j ++) {
			new_table.data[j + ndx*i] = ((double)j*1.0+ minx)/((double)i*1.0+ minx);
		}
	}

	return new_table;
}

void division_table_free(division_table table) {
	free(table.data);	
}

double division_table_a_div_b_interpolated(division_table table, double a, double b) {

	int index_a = (int)(0.5 + a-table.minx);
	int index_b = (int)(0.5 + b-table.minx);
	
	return table.data[index_a + table.ndx*index_b]; 

}

double division_table_a_div_b_smart(division_table table, double a, double b) {
	int index_a = (int)(a-table.minx);
	int index_b = (int)(b-table.minx);

	if (index_b >= table.minx+table.ndx - 1) {
		double left =  division_table_a_div_b_smart(table, a,(((int)b) >> 1));
		double right = 0.5;
		//printf("Returning (b) %lf/%lf = %lf\n", a, b, left*right);
		return  left*right; 	
	}

	if (index_a >= table.minx+table.ndx - 1) {
		double left =  division_table_a_div_b_smart(table,(((int)a) >> 1) ,b);
		double right =  2.0; 
		//printf("Returning (a) %lf/%lf = %lf\n", a, b, left*right);
		return  left*right; 	
	}
	
	//printf("Returning %lf/%lf = %lf\n", a, b, table.data[index_a + table.ndx*index_b]);
	return table.data[index_a + table.ndx*index_b]; 

}

double division_table_a_div_b(division_table table, double a, double b) {
	int index_a = (int)(a-table.minx);
	int index_b = (int)(b-table.minx);
	
	return table.data[index_a + table.ndx*index_b];
}

void time_table () {
	double simple = 0;
	int n = 0;

	division_table table = division_table_create(1.0, 9);
	
	for (double i = 1.0; i < 1000.0; i += 0.01) {
		for (double j = 1.0; j < 1000.0; j += 0.01) {
			simple = simple + division_table_a_div_b_smart(table, i, j); 
			//simple = simple + 0.0; //(i/j);//simple + division_table_a_div_b(table, i, j); 
			n++;
		}
	}

	printf("Iterations: %d, %lf\n", n, simple);

	division_table_free(table);
}

void get_error() {
	double loss = 0.0;
	int n = 0;

	division_table table = division_table_create(1.0, 9);


	for (double i = 1.0; i < 1000.0; i += 0.01) {
		for (double j = 1.0; j < 1000.0; j += 0.01) {
			double smart = fabs(division_table_a_div_b_smart(table, i, j) - (i/j))/fabs(i/j);	
			loss += smart;			
			n++;
		}
		printf("oof %lf\n", i);
	}

	printf("Error : %lf.\n", loss/n);

	division_table_free(table);
}

int main() {
	time_table();
}








