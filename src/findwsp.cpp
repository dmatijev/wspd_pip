#include <fstream>
#include <assert.h> 
#include <vector>
#include "wsp.h"

//extern std::ofstream output_file;

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

/*******************/
/*** FindWSP2_cb ***/
/*******************/

int dfs(tree_node *tnode, vector<double* >& pt)
{
	if (tnode->lchild == NULL && tnode->rchild == NULL){
		pt.push_back(tnode->pt->coord.data());
		return 1;
	}
	return dfs(tnode->lchild, pt) + dfs(tnode->rchild, pt);
}

// EDIT: get index information

int dfs(tree_node *tnode, Points& pts)
{
	if (tnode->lchild == NULL && tnode->rchild == NULL){
		pts.push_back(tnode->pt);
		return 1;
	}
	return dfs(tnode->lchild, pts) + dfs(tnode->rchild, pts);
}

int FindWSP2_cb(tree_node *tnode1, tree_node *tnode2, double s, int dim,
                pair_callback& cb) {
  double distance, actual_s;
  if (wellsep(tnode1, tnode2, s, dim, distance, actual_s)) {
    Points pts1, pts2;
    dfs(tnode1, pts1);
    dfs(tnode2, pts2);
    cb(pts1, pts2);
    return 1;
  } else {
    if (tnode1->radius > tnode2->radius)
      return (FindWSP2_cb(tnode1->lchild, tnode2, s, dim, cb)
              + FindWSP2_cb(tnode1->rchild, tnode2, s, dim, cb));
    else
      return (FindWSP2_cb(tnode1, tnode2->lchild, s, dim, cb)
              + FindWSP2_cb(tnode1, tnode2->rchild, s, dim, cb));
  }
}

/******************/
/*** FindWSP_cb ***/
/******************/

int FindWSP_cb(tree_node *tnode, double s, int dim, pair_callback& cb) {
  if (tnode->pt != NULL)
    return 0;
  return (FindWSP_cb(tnode->lchild, s, dim, cb)
          + FindWSP_cb(tnode->rchild, s, dim, cb)
          + FindWSP2_cb(tnode->lchild, tnode->rchild, s, dim, cb));
}
