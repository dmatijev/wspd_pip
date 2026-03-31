#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "wsp.h"
#undef list

 

namespace py = pybind11;


PYBIND11_MODULE(wspd, m)
{
    
    m.doc() = "Well-separated pair decomposition (WSPD) plugin"; 
    m.def("build_wspd", &run_wspd, "Well Separated Pair Decomposition (WSPD): The build_wspd() function computes WSPD following the original Callahan and Kosaraju O(n log(n)) algorithm. \n Parameters: \n    \t arg0: int  - the size of the dataset  \n  \t arg1: int - the dimension of the dataset \n \t arg2: float - the separation constant S \n  \t arg3: List[point] - list of input points. Note that point is an internal wspd point (wspd.point). \n \n Output: The function outputs the Python list of tuples. Each tuple represents the WSPD realization link, sometimes called the dumbbell. ");
    m.def("build_wspd_np",
        [](int num, int dim, double sep_const, vector<point>& pts) -> py::list {
            py::list result;
            run_wspd_cb(num, dim, sep_const, pts,
                [&result](Points& l, Points& r) {
                    py::array_t<int> arr1(l.size());
                    py::array_t<int> arr2(r.size());
                    auto* buf1 = arr1.mutable_data();
                    auto* buf2 = arr2.mutable_data();
                    for (size_t i = 0; i < l.size(); i++) buf1[i] = l[i]->index;
                    for (size_t i = 0; i < r.size(); i++) buf2[i] = r[i]->index;
                    result.append(py::make_tuple(arr1, arr2));
                });
            return result;
        },
        "Well Separated Pair Decomposition (WSPD): The build_wspd_np() function computes WSPD following the original Callahan and Kosaraju O(n log(n)) algorithm and returns index arrays as numpy arrays. \n Parameters: \n    \t arg0: int  - the size of the dataset  \n  \t arg1: int - the dimension of the dataset \n \t arg2: float - the separation constant S \n  \t arg3: List[point] - list of input points. Note that point is an internal wspd point (wspd.point). \n \n Output: The function outputs the Python list of tuples. Each tuple represents the WSPD realization link, sometimes called the dumbbell. Unlike build_wspd(), the two index sets in each tuple are returned as numpy arrays instead of lists. ");
    py::class_<point>(m, "point", "Class point is an internal point representation. It accepts the Python list of floats as point coordinates. Attribute coord can be used to fetch point coordinates when needed.")
        .def(py::init<vector<double>&>())
        .def("coord", &point::coordinates);
}
