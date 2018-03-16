//
// Created by magnar on 08.03.18.
//


#include <pybind11/pybind11.h>

#include "ScalingBasis.h"
#include "InterpolatingBasis.h"
#include "LegendreBasis.h"

using namespace mrcpp;
namespace py = pybind11;


void init_pymrcppClasses(py::module &m) {


py::class_<ScalingBasis> scalingbasis(m, "ScalingBasis");
    scalingbasis.def(py::init<int, int>());

py::class_<InterpolatingBasis> (m, "InterpolatingBasis", scalingbasis)
    .def(py::init<int>());

py::class_<LegendreBasis> (m, "LegendreBasis", scalingbasis)
    .def(py::init<int>());
}
