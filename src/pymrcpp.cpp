//
// Created by magnar on 08.03.18.
//


#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/eigen.h>
#include "ScalingBasis.h"
#include "InterpolatingBasis.h"
#include "BoundingBox.h"
#include "MultiResolutionAnalysis.h"
#include "MWTree.h"
#include "FunctionTree.h"
using namespace mrcpp;
namespace py = pybind11;


PYBIND11_MODULE(pymrcpp, m) {
py::class_ <ScalingBasis> scalingbasis(m, "ScalingBasis");
    scalingbasis.def(py::init<int, int>());

py::class_<InterpolatingBasis> (m, "InterpolatingBasis", scalingbasis)
    .def(py::init<int>());

py::class_<BoundingBox<3>> (m, "BoundingBox")
    .def(py::init<int, py::array_t<int>, py::array_t <int>>())
    .def("getScale", &BoundingBox<3>::getScale);


py::class_<MultiResolutionAnalysis<3>> (m, "MultiResolutionAnalysis")
    .def(py::init<BoundingBox<3>, ScalingBasis, int>())
    .def("getOrder", &MultiResolutionAnalysis<3>::getOrder)
    .def("getMaxDepth", &MultiResolutionAnalysis<3>::getMaxDepth)
    .def("getMaxScale", &MultiResolutionAnalysis<3>::getMaxScale);

py::class_<MWTree<3>> mwtree(m, "MWTree");
    mwtree.def(py::init<MultiResolutionAnalysis<3>>());

py::class_<FunctionTree<3>> (m, "FunctionTree")
    .def(py::init<const MultiResolutionAnalysis<3>>());
}