#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "wsp.h"

namespace py = pybind11;

PYBIND11_MODULE(wspd, m)
{
    
    m.doc() = "Well-separated pair decomposition (WSPD) plugin"; 
    m.def("build_wspd",
        [](int num, int dim, double sep_const, vector<point>& pts) -> py::list {
            if (num < 1 || dim < 1 || pts.size() < 1) throw std::invalid_argument("num and dim must be positive integers");
            
            py::list result(pts.size() - 1);

            run_wspd_cb(num, dim, sep_const, pts,
                [&result](Points& l, Points& r) {
                    py::list arr1, arr2;
                    for (auto* p : l) arr1.append(p->index);
                    for (auto* p : r) arr2.append(p->index);
                    result.append(py::make_tuple(arr1, arr2));
                });
            return result;
        },
        "Well Separated Pair Decomposition (WSPD): The build_wspd() function computes WSPD following the original Callahan and Kosaraju O(n log(n)) algorithm. \n Parameters: \n    \t arg0: int  - the size of the dataset  \n  \t arg1: int - the dimension of the dataset \n \t arg2: float - the separation constant S \n  \t arg3: List[point] - list of input points. Note that point is an internal wspd point (wspd.point). \n \n Output: The function outputs the Python list of tuples. Each tuple represents the WSPD realization link, sometimes called the dumbbell. ");
    m.def("build_wspd_np",
        [](int num, int dim, double sep_const, vector<point>& pts) -> py::list {
            if (num < 1 || dim < 1 || pts.size() < 1) throw std::invalid_argument("num and dim must be positive integers");

            py::list result(pts.size() - 1);

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
    m.def("build_wspd_tup_np",
        [](int num, int dim, double sep_const, vector<point>& pts) -> py::tuple {
            if (num < 1 || dim < 1 || pts.size() < 1) throw std::invalid_argument("num and dim must be positive integers");
            
            py::list a_pairs(pts.size() - 1);
            py::list b_pairs(pts.size() - 1);
            
            run_wspd_cb(num, dim, sep_const, pts,
                [&a_pairs, &b_pairs](Points& l, Points& r) {
                    py::array_t<int> arr1(l.size());
                    py::array_t<int> arr2(r.size());
                    auto* buf1 = arr1.mutable_data();
                    auto* buf2 = arr2.mutable_data();
                    for (size_t i = 0; i < l.size(); i++) buf1[i] = l[i]->index;
                    for (size_t i = 0; i < r.size(); i++) buf2[i] = r[i]->index;
                    a_pairs.append(arr1);
                    b_pairs.append(arr2);
                });
            return py::make_tuple(a_pairs, b_pairs);
        },
        "Well Separated Pair Decomposition (WSPD): The build_wspd_tup_np() function computes WSPD following the original Callahan and Kosaraju O(n log(n)) algorithm and returns a tuple of two lists of numpy arrays. \n Parameters: \n    \t arg0: int  - the size of the dataset  \n  \t arg1: int - the dimension of the dataset \n \t arg2: float - the separation constant S \n  \t arg3: List[point] - list of input points. Note that point is an internal wspd point (wspd.point). \n \n Output: The function outputs a tuple (A, B) where A is a list of numpy arrays for the first element of each WSPD pair and B is a list of numpy arrays for the second element of each WSPD pair. The ith array of A and the ith array of B together form the ith WSPD dumbbell. ");
    m.def("build_wspd_flat_np",
        [](int num, int dim, double sep_const, vector<point>& pts) -> py::tuple {
            // Accumulate index vectors in a single pass via the callback API.
            vector<int> a_flat_vec(pts.size() - 1), b_flat_vec(pts.size() - 1);
            vector<int> a_off_vec(pts.size() - 1), b_off_vec(pts.size() - 1);
            
            run_wspd_cb(num, dim, sep_const, pts,
                [&](Points& l, Points& r) {
                    a_off_vec.push_back(static_cast<int>(a_flat_vec.size()));
                    b_off_vec.push_back(static_cast<int>(b_flat_vec.size()));
                    for (auto* p : l) a_flat_vec.push_back(p->index);
                    for (auto* p : r) b_flat_vec.push_back(p->index);
                });

            // Copy into numpy arrays.
            py::array_t<int> a_flat(a_flat_vec.size());
            py::array_t<int> b_flat(b_flat_vec.size());
            py::array_t<int> a_offsets(a_off_vec.size());
            py::array_t<int> b_offsets(b_off_vec.size());
            std::copy(a_flat_vec.begin(), a_flat_vec.end(), a_flat.mutable_data());
            std::copy(b_flat_vec.begin(), b_flat_vec.end(), b_flat.mutable_data());
            std::copy(a_off_vec.begin(),  a_off_vec.end(),  a_offsets.mutable_data());
            std::copy(b_off_vec.begin(),  b_off_vec.end(),  b_offsets.mutable_data());

            return py::make_tuple(a_flat, b_flat, a_offsets, b_offsets);
        },
        "Well Separated Pair Decomposition (WSPD): The build_wspd_flat_np() function computes WSPD following the original Callahan and Kosaraju O(n log(n)) algorithm and returns four 1-D numpy integer arrays (a_flat, b_flat, a_offsets, b_offsets). \n Parameters: \n    \t arg0: int  - the size of the dataset  \n  \t arg1: int - the dimension of the dataset \n \t arg2: float - the separation constant S \n  \t arg3: List[point] - list of input points. Note that point is an internal wspd point (wspd.point). \n \n Output: a_flat and b_flat are the concatenations of all A and B point-index arrays across every well-separated pair. a_offsets[i] and b_offsets[i] give the start position of the i-th pair within a_flat and b_flat, respectively. This is equivalent to the result of build_wspd_tup_np but stored in a flat (CSR-style) layout.");
    py::class_<point>(m, "point", "Class point is an internal point representation. It accepts the Python list of floats as point coordinates. Attribute coord can be used to fetch point coordinates when needed.")
        .def(py::init<vector<double>&>())
        .def("coord", &point::coordinates);
}
