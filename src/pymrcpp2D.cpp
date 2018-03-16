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


void init_pymrcpp2D(py::module &m) {

//MWFunctions
py::class_<FunctionTreeVector<2>> (m, "FunctionTreeVector2D")
    .def(py::init<>())
    .def("size", &FunctionTreeVector<2>::size)
    .def("push_back", py::overload_cast<double, FunctionTree<2> *>(&FunctionTreeVector<2>::push_back))
    .def("push_back", py::overload_cast<FunctionTree<2> *>(&FunctionTreeVector<2>::push_back));


py::class_<BoundingBox<2>> (m, "BoundingBox2D")
    .def(py::init<int, py::array_t<const int>, py::array_t <const int>>())
    .def("getScale", &BoundingBox<2>::getScale);

py::class_<MultiResolutionAnalysis<2>> (m, "MultiResolutionAnalysis2D")
    .def(py::init<BoundingBox<2>, ScalingBasis, int>())
    .def("getOrder", &MultiResolutionAnalysis<2>::getOrder)
    .def("getMaxDepth", &MultiResolutionAnalysis<2>::getMaxDepth)
    .def("getMaxScale", &MultiResolutionAnalysis<2>::getMaxScale);

py::class_<MWTree<2>> mwtree(m, "MWTree2D");
    mwtree
    .def(py::init<MultiResolutionAnalysis<2>>())
    .def("getSquareNorm", &MWTree<2>::getSquareNorm);
py::class_<FunctionTree<2>> (m, "FunctionTree2D", mwtree)
    .def(py::init<const MultiResolutionAnalysis<2>>())
    .def("integrate", &FunctionTree<2>::integrate);

py::class_<RepresentableFunction<2>, PyRepresentableFunction<2>> repfunc(m, "RepresentableFunction2D");
    repfunc
    .def(py::init<>())
    .def("evalf", &RepresentableFunction<2>::evalf);

    m.def("add", py::overload_cast<double, FunctionTree<2> &, double , FunctionTree<2> &, double, FunctionTree<2> &, int>(&add<2>));
    m.def("project", py::overload_cast<double, FunctionTree<2> &, RepresentableFunction<2> &, int>(&project<2>));
    m.def("project", py::overload_cast<double, FunctionTree<2> &, std::function<double (double, double)>, int>(&project2D));

//Gaussians
py::class_<Gaussian<2>> gaussian(m, "Gaussian2D", repfunc);

py::class_<GaussFunc<2>>(m, "GaussFunc2D", gaussian)
    .def(py::init<double, double, py::array_t <double>, py::array_t <double>>())
    .def("evalf", py::overload_cast<py::array_t <double>>(&GaussFunc<2>::evalf));
}
