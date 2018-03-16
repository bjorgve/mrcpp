//
// Created by magnar on 08.03.18.
//


#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_pymrcpp3D(py::module &);
void init_pymrcppClasses(py::module &);

PYBIND11_MODULE(pymrcpp, m) {

    init_pymrcpp3D(m);
    init_pymrcppClasses(m);

}
