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
#include "project.h"
#include "Gaussian.h"
#include "RepresentableFunction.h"
#include "GaussFunc.h"
#include <pybind11/functional.h>

using namespace mrcpp;
namespace py = pybind11;

template <int D>
class PyRepresentableFunction : public RepresentableFunction<D> {
public:
    using RepresentableFunction<D>::RepresentableFunction;

    double evalf(const double *r) const override {
        PYBIND11_OVERLOAD_PURE(
            double,
            RepresentableFunction<D>,
            evalf,
            r
        );
    }

};


PYBIND11_MODULE(pymrcpp, m) {
py::class_ <ScalingBasis> scalingbasis(m, "ScalingBasis");
    scalingbasis.def(py::init<int, int>());

py::class_<InterpolatingBasis> (m, "InterpolatingBasis", scalingbasis)
    .def(py::init<int>());

py::class_<BoundingBox<3>> (m, "BoundingBox")
    .def(py::init<int, py::array_t<const int>, py::array_t <const int>>())
    .def("getScale", &BoundingBox<3>::getScale);


py::class_<MultiResolutionAnalysis<3>> (m, "MultiResolutionAnalysis")
    .def(py::init<BoundingBox<3>, ScalingBasis, int>())
    .def("getOrder", &MultiResolutionAnalysis<3>::getOrder)
    .def("getMaxDepth", &MultiResolutionAnalysis<3>::getMaxDepth)
    .def("getMaxScale", &MultiResolutionAnalysis<3>::getMaxScale);

py::class_<MWTree<3>> mwtree(m, "MWTree");
    mwtree.def(py::init<MultiResolutionAnalysis<3>>());

py::class_<FunctionTree<3>> (m, "FunctionTree")
    .def(py::init<const MultiResolutionAnalysis<3>>())
    .def("integrate", &FunctionTree<3>::integrate);

py::class_<RepresentableFunction<3>, PyRepresentableFunction<3>> repfunc(m, "RepresentableFunction");
    repfunc
    .def(py::init<>())
    .def("evalf", &RepresentableFunction<3>::evalf);



py::class_<Gaussian<3>> gaussian(m, "Gaussian", repfunc);

py::class_<GaussFunc<3>>(m, "GaussFunc", gaussian)
    .def(py::init<double, double, py::array_t <double>, py::array_t <double>>())
    .def("evalf", py::overload_cast<py::array_t <double>>(&GaussFunc<3>::evalf));

    m.def("project", py::overload_cast<double, FunctionTree<3> &, RepresentableFunction<3> &, int>(&project<3>));
    m.def("project3D", py::overload_cast<double, FunctionTree<3> &, std::function<double (double, double, double)>, int>(&project3D));
}
