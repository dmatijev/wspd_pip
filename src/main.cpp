#include "wsp.h"
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
//#include "Timer.h"


//std::ofstream output_file;

int main(int argc, char *argv[]){
  int rows{-1}; // counting rows, must be less than num to start
  int num{0}, dim, i, j, num_wsp;
  double sep_const;
  tree_node *root;
  vector<point> pts;
//  list_set orders;

//  Timer timer;
  if (argc < 3) {
    fprintf(stdout, "Usage 1: wsp num_points num_dim sep_constant\n");
    fprintf(stdout, "Usage 2: wsp file_name sep_constant\n");

    exit(-1);
  }

  if (argc == 4)
  {
    num = atoi(argv[1]);
    dim = atoi(argv[2]);
    sep_const = atof(argv[3]);
    /* generate points */
    /*  printf("Generating points...\n"); */
    /* pts = GeneratePointsDave(num, dim); */
    printf("Genereting points... ");
//    timer.start();
    pts = GeneratePoints(num, dim);
    for (int i= 0; i<pts.size(); i++)
        pts[i].index = i;
  //  timer.stop();
   // cout << " done in " << timer.msecs() << " msecs" << endl;

  }

  string file_name ;
  
  if (argc == 3) // read from the file 
  {
    file_name = argv[1];
    string output_file_name = string(file_name) + ".wsp_out.txt";
    cout << "wsp: " << file_name << " --> " << output_file_name << endl;
    //output_file.open(file_name + ".output.txt");
    //output_file.open(output_file_name);
    sep_const = atof(argv[2]);
    // read file to pts data structure

    cout << "Reading file: " << file_name << ", target sep: " << sep_const << endl;
//    timer.start();
    ifstream file(file_name);
    bool first_line = true;
    vector<double> coords;
    
    //int rows  = 0; // counting rows
    if (file.is_open()) {
      // EDIT:
      while (!file.eof() && rows < num) {
        string line;
        getline (file, line); // reads a line
        //cout << line << endl;
        istringstream iss(line);
        // EDIT: peel of the header comment marker
        // TODO: Make sure the file is read right
        if (iss.peek() == '#') iss.get();
        if (first_line)
        {
          rows = 0;
          iss >> num;
          iss >> dim;
          first_line = false;
          
          // allocate points data-structure!!!
          //pts = (point *) malloc(sizeof(point) * num);	
          pts.resize(num);
          for (i = 0; i < num; i++) {
            //pts[i].coord = (double *) malloc(sizeof(double)*dim);
            pts[i].coord.resize(dim);
            //pts[i].lists = (list_elt **) malloc(sizeof(list_elt *) * dim);
            pts[i].lists.resize(dim);
          }
          cout << "nr_points = " << num << " and dim = " << dim << endl;
        }else
        {
          //cout << "first_line = false" << endl;
          for (j=0; j<dim; j++) 
          {
            double  sub;
            iss >> sub; // parting the each number in the line
            pts[rows].coord[j] = sub;
            //cout << "part-string: (i,j): " <<rows << ", " << j << " : " <<  sub << endl;
          }
          pts[rows].index = rows;
          rows++;
        }
      }
      file.close();
    }

//    timer.stop();
//    cout << "done in " << timer.secs() << " secs" <<  endl;          
    
  }
//  timer.start();
/*  pts.clear();
  vector<double> p(2);
  p[0] = 1; p[1] = 1;
  point p1(p);
  p[0] = 2; p[1] = 2;
  point p2(p);
  p[0] = 10; p[1] = 10;
  point p3(p);
  p[0] = 9; p[1] = 9;
  point p4(p);
  p[0] = 1; p[1] = 10;
  point p5(p);
  p[0] = 2; p[1] = 10;
  point p6(p);
  pts.push_back(p1);
  pts.push_back(p2);
  pts.push_back(p3);
  pts.push_back(p4);
  pts.push_back(p5);
  pts.push_back(p6);
  
  sep_const = 4.0;
  num = 6;
  dim = 2;
  
  */
  dumbell_list dumbells =  run_wspd(num, dim, sep_const, pts);    
//  timer.stop();
//  cout << "WSPD computed in " << timer.secs() << " secs" <<  endl;
  cout << "Nr. of dumbells is: " << dumbells.size() << endl;
/*  
  for (auto d : dumbells)
  {
      for (auto idx: d.first)
        cout << idx  << "  " ; 
      cout << " | " ;
      for (auto idx: d.second)
          cout << idx << "  " ;
      cout <<endl;
  }
  */
}

