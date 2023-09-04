#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "wsp.h"

 

namespace py = pybind11;


PYBIND11_MODULE(wspd, m)
{
    
    m.doc() = "Well-separated pair decomposition (WSPD) plugin"; 
    m.def("build_wspd", &run_wspd, "Well Separated Pair Decomposition (WSPD): The build_wspd() function computes WSPD following the original Callahan and Kosaraju O(n log(n)) algorithm. \n Parameters: \n    \t arg0: int  - the size of the dataset  \n  \t arg1: int - the dimension of the dataset \n \t arg2: float - the separation constant S \n  \t arg3: List[point] - list of input points. Note that point is an internal wspd point (wspd.point). \n \n Output: The function outputs the Python list of tuples. Each tuple represents the WSPD realization link, sometimes called the dumbbell. ");
    py::class_<point>(m, "point")
        .def(py::init<vector<double>&>())
        .def("coord", &point::coordinates);
}
