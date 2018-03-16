//
// Created by magnar on 08.03.18.
//


#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>

#include "BoundingBox.h"
#include "MultiResolutionAnalysis.h"
#include "MWTree.h"
#include "FunctionTree.h"
#include "project.h"
#include "Gaussian.h"
#include "RepresentableFunction.h"
#include "PyRepresentableFunction.h"
#include "GaussFunc.h"
#include "add.h"
#include "FunctionTreeVector.h"

using namespace mrcpp;
namespace py = pybind11;


void init_pymrcpp1D(py::module &m) {

//MWFunctions
py::class_<FunctionTreeVector<1>> (m, "FunctionTreeVector1D")
    .def(py::init<>())
    .def("size", &FunctionTreeVector<1>::size)
    .def("push_back", py::overload_cast<double, FunctionTree<1> *>(&FunctionTreeVector<1>::push_back))
    .def("push_back", py::overload_cast<FunctionTree<1> *>(&FunctionTreeVector<1>::push_back));


py::class_<BoundingBox<1>> (m, "BoundingBox1D")
    .def(py::init<int, py::array_t<const int>, py::array_t <const int>>())
    .def(py::init<int, int *,  int *>()) //1D cases can be initialized without array type input
    .def("getScale", &BoundingBox<1>::getScale);

py::class_<MultiResolutionAnalysis<1>> (m, "MultiResolutionAnalysis1D")
    .def(py::init<BoundingBox<1>, ScalingBasis, int>())
    .def("getOrder", &MultiResolutionAnalysis<1>::getOrder)
    .def("getMaxDepth", &MultiResolutionAnalysis<1>::getMaxDepth)
    .def("getMaxScale", &MultiResolutionAnalysis<1>::getMaxScale);

py::class_<MWTree<1>> mwtree(m, "MWTree1D");
    mwtree
    .def(py::init<MultiResolutionAnalysis<1>>())
    .def("getSquareNorm", &MWTree<1>::getSquareNorm);
py::class_<FunctionTree<1>> (m, "FunctionTree1D", mwtree)
    .def(py::init<const MultiResolutionAnalysis<1>>())
    .def("integrate", &FunctionTree<1>::integrate);

py::class_<RepresentableFunction<1>, PyRepresentableFunction<1>> repfunc(m, "RepresentableFunction1D");
    repfunc
    .def(py::init<>())
    .def("evalf", &RepresentableFunction<1>::evalf);

    m.def("add", py::overload_cast<double, FunctionTree<1> &, double , FunctionTree<1> &, double, FunctionTree<1> &, int>(&add<1>));
    m.def("project", py::overload_cast<double, FunctionTree<1> &, RepresentableFunction<1> &, int>(&project<1>));
    m.def("project", py::overload_cast<double, FunctionTree<1> &, std::function<double (double)>, int>(&project1D));

//Gaussians
py::class_<Gaussian<1>> gaussian(m, "Gaussian1D", repfunc);

py::class_<GaussFunc<1>>(m, "GaussFunc1D", gaussian)
    .def(py::init<double, double, py::array_t <double>, py::array_t <double>>())
    .def("evalf", py::overload_cast<py::array_t <double>>(&GaussFunc<1>::evalf));
}
