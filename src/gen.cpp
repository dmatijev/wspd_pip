
#include "wsp.h"

/**********************/
/*** GeneratePoints ***/
/**********************/

vector<point> GeneratePoints(int num, int dim){
  int i, j;
  double r,  r_scale;
  bool found;
  vector<point> point_set(num);
  double temp_sum;
  vector<double> temp_coord;

  //point_set = (point *) malloc(sizeof(point) * num);
  for (i = 0; i < num; i++) {
    //point_set[i].coord = (double *) malloc(sizeof(double)*dim);
    point_set[i].coord.resize(dim);
  }

  /* Kuzmin model */
  temp_coord.resize(dim);
  for (i=0; i<num; i++) {
    found = false;
    while (found == false) {
      temp_sum = 0.0;
      for (j=0; j<dim; j++) {
	//temp_coord[j] = 2*(drand48() - .5);
  double tmp_rnd = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  temp_coord[j] = 2*(tmp_rnd - .5);
	temp_sum += temp_coord[j]*temp_coord[j];
      }
      if (temp_sum <= 1)
	found = true;
    }
    r = sqrt(temp_sum);
    r_scale = 1.0;
    for (j=0; j<dim; j++) {
      r_scale *= (1-r);
    }
    for (j=0; j<dim; j++) {
      point_set[i].coord[j] = (temp_coord[j]/r)*((1.0/r_scale)-1.0);
    }
    //point_set[i].lists = (list_elt **) malloc(sizeof(list_elt *) * dim);
    point_set[i].lists.resize(dim);
  }
  
  return point_set;
}
