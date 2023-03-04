#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>
//#include "Timer.h"
using namespace std;

typedef enum HALF {lower,upper} half;

/******************************************************************/
struct point;

struct list_elt{
  list_elt *next, *prev;      /* next/prev list element    */
  point *pt;                  /* point represented         */
};

#define list mylist

struct list{
  list_elt *mem;                     /* pointer to memory to free */
  list_elt *first, *last;            /* first and last elements   */
};                                   /*    of sorted list         */



//typedef list *list_set;              /* set of d=dim lists        */
using list_set = list*;
using dumbell_list = vector< pair< vector<int>, vector<int> > >;
struct list_set_elt {
  list_set data;                     /* set of d=dim lists        */
  int count;                         /* how many items in this list */
  list_set_elt *next;   	      /* next list set             */
};

struct point {
  point(){}
  point(vector<double>& pt):coord{pt}{
    int dim = pt.size();
    lists.resize(dim);
  }
  bool operator<(const point& p) const {return this->coord < p.coord; }
  bool operator==(const point& p) const { return this->coord == p.coord; }
  //double *coord;                     /* location of point         */
  vector<double> coord;
  list_set new_list;                 /* lists to be inserted into */
  //list_elt **lists;                  /* where pt is currently at  */
  vector<list_elt*> lists;
  int sort_dim;                      /* dim along which points    */
  // EDIT: row of the point
  int index;
  vector<double>& coordinates(){return coord;}
};              	               /*    are being sorted       */

struct tree_node{
  tree_node *lchild, *rchild; /* children of tree node     */
  double *center;                    /* center of pts contained   */
  double radius;                     /* radius containing all pts */
  point *pt;                         /* if leaf node, point       */
  int nr_pt;
};       	                  /*    represented */

/******************************************************************/

dumbell_list run_wspd(int num, int dim, double sep_const, 
              vector<point>& pts);
vector<point> GeneratePoints(int num, int dim);
void SortPoints(point *, int num, int dim, list *sorted_list);
void ListCopy(list *l_orig, list *l_copy, int num);
void DoubleLink(list_elt *elt_array, int num, list *linked_list);
tree_node *BuildTree(list_set ls, int num, int dim);

int FindWSP(tree_node *tnode, double s, int dim, 
           dumbell_list& dumbells);
double upper_bound(int n, int d, double s);
