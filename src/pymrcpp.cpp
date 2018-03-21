/*
 *  \date Mar 08, 2018
 *  \author Magnar Bjørgve <magnar.bjorgve@uit.no> \n
 *          Hylleraas Centre for Quantum Molecular Sciences \n
 *          UiT - The Arctic University of Norway
 */


#include <pybind11/pybind11.h>

namespace py = pybind11;

template <int D> void init_pymrcpp(py::module &);
void init_pymrcppClasses(py::module &);
template<int D> void init_pymethods(py::module &);
void init_pyProject1D(py::module &);
void init_pyProject2D(py::module &);
void init_pyProject3D(py::module &);

template<int D> void init_pyOperators(py::module &);

PYBIND11_MODULE(pymrcpp, m) {

    init_pyProject1D(m);
    init_pyProject2D(m);
    init_pyProject3D(m);
    init_pymrcpp<1>(m);
    init_pymrcpp<2>(m);
    init_pymrcpp<3>(m);
    init_pymrcppClasses(m);
    init_pymethods<1>(m);
    init_pymethods<2>(m);
    init_pymethods<3>(m);

    init_pyOperators<1>(m);
    init_pyOperators<2>(m);
    init_pyOperators<3>(m);

}
