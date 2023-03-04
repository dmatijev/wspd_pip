#include <fstream>
#include <assert.h> 
#include <vector>
#include "wsp.h"

int FindWSP2(tree_node *tnode1, tree_node *tnode2, double s, int dim, 
            dumbell_list& dumbells);
int wellsep(tree_node *tnode1, tree_node *tnode2, double s, 
            int dim, double& dist, double &actual_s);

//extern std::ofstream output_file;

/***************/
/*** FindWSP ***/
/***************/

int FindWSP(tree_node *tnode, double s, int dim, 
           dumbell_list& dumbells){

  if (tnode->pt != NULL) {
    /* printf("tnode->pt != NULL!!!\n"); */
    return (0);
  }
  else {
    return (FindWSP(tnode->lchild, s, dim, dumbells)
	    + FindWSP(tnode->rchild, s, dim, dumbells)
	    + FindWSP2(tnode->lchild, tnode->rchild, s, dim, dumbells)); 
  }
}

int dfs(tree_node *tnode, vector<double* >& pt)
{
	if (tnode->lchild == NULL && tnode->rchild == NULL){
		pt.push_back(tnode->pt->coord.data());
		return 1;
	}
	return dfs(tnode->lchild, pt) + dfs(tnode->rchild, pt);
}

// EDIT: get index information
using Points = vector<point*>;

int dfs(tree_node *tnode, Points& pts)
{
	if (tnode->lchild == NULL && tnode->rchild == NULL){
		pts.push_back(tnode->pt);
		return 1;
	}
	return dfs(tnode->lchild, pts) + dfs(tnode->rchild, pts);
}


/****************/
/*** FindWSP2 ***/
/****************/

// EDIT:

void save_indices(Points& l, Points& r, 
                 dumbell_list& dumbells) {
 
    pair<vector<int>, vector<int>> dumbel;
    for (auto p: l)  dumbel.first.push_back(p->index);
    for (auto p: r) dumbel.second.push_back(p->index);
    dumbells.push_back(dumbel);
    
    
    //for (auto p : l) cout << p->index << " ";
    //cout  << " | ";
    //for (auto p : r) cout << p->index << " ";
    //cout  << "\n";
}

void save_info(double sep, tree_node *l, tree_node *r, 
              dumbell_list& dumbells) {
  //output_file << sep << " "
            //<< *l->center << " "
  //          << l->radius << " "
            //<< *r->center << " "
  //          << r->radius << " | ";
}

int FindWSP2(tree_node *tnode1, tree_node *tnode2, double s, int dim, 
            dumbell_list& dumbells) {
  double distance, actual_s;
  if (wellsep(tnode1, tnode2, s, dim, distance, actual_s)) {
    // EDIT:
	  Points tnode1_pts, tnode2_pts;
	  dfs(tnode1, tnode1_pts);
	  dfs(tnode2, tnode2_pts);
          save_info(actual_s,tnode1,tnode2, dumbells);
          save_indices(tnode1_pts,tnode2_pts, dumbells);
	  return (1);
  }
  else {
    if (tnode1->radius > tnode2->radius)
      return (FindWSP2(tnode1->lchild, tnode2, s, dim, dumbells) 
	      + FindWSP2(tnode1->rchild, tnode2, s, dim, dumbells));
    else
      return (FindWSP2(tnode1, tnode2->lchild, s, dim, dumbells) 
	      + FindWSP2(tnode1, tnode2->rchild, s, dim, dumbells));
  }
}

/***************/
/*** wellsep ***/
/***************/

int wellsep(tree_node *tnode1, tree_node *tnode2, double s, 
            int dim, double& dist, double &actual_s){
  int i;
  double radius, distance;

  radius = std::max(tnode1->radius, tnode2->radius);

  distance = 0.0;
  for (i=0; i<dim; i++) {
    distance += (tnode1->center[i] - tnode2->center[i])*
      (tnode1->center[i] - tnode2->center[i]);
  }

  dist = distance = sqrt(distance);
  distance = distance - 2*radius;
  actual_s = radius == 0 ? -1 : distance / radius;

  if (distance > s*radius)
    return (1);
  else
    return (0);
}
