#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "wsp.h"

 

namespace py = pybind11;


PYBIND11_MODULE(wspd, m)
{
    
    m.doc() = "Well-separated pair decomposition (WSPD) plugin"; 
    m.def("build_wspd", &run_wspd, "The function that computes WSPD following the Callahan and Kosaraju O(n log(n)) algorithm");
    py::class_<point>(m, "point")
        .def(py::init<vector<double>&>())
        .def("coord", &point::coordinates);
}
