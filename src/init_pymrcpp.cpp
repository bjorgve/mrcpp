/*
 *  \date Mar 08, 2018
 *  \author Magnar Bjørgve <magnar.bjorgve@uit.no> \n
 *          Hylleraas Centre for Quantum Molecular Sciences \n
 *          UiT - The Arctic University of Norway
 */

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
#include "ABGVOperator.h"
#include "PoissonOperator.h"
#include "multiply.h"

//#include <sstream> //Does not seem to be needed
using namespace mrcpp;
namespace py = pybind11;


template<int D>
void init_pymrcpp(py::module &m) {

//MWFunctions

    std::stringstream funcTreeVecName;
    funcTreeVecName << "FunctionTreeVector" << D << "D";
py::class_<FunctionTreeVector<D>> (m, funcTreeVecName.str().data())
    .def(py::init<>())
    .def("size", &FunctionTreeVector<D>::size)
    .def("push_back", py::overload_cast<double, FunctionTree<D> *>(&FunctionTreeVector<D>::push_back))
    .def("push_back", py::overload_cast<FunctionTree<D> *>(&FunctionTreeVector<D>::push_back));


    std::stringstream boundBoxName;
    boundBoxName << "BoundingBox" << D << "D";
py::class_<BoundingBox<D>> (m, boundBoxName.str().data())
    .def(py::init<int, py::array_t<const int>, py::array_t <const int>>())
    .def(py::init<int, int *,  int *>()) //1D cases can be initialized without array type input
    .def("getScale", &BoundingBox<D>::getScale);

    std::stringstream multResAnaName;
    multResAnaName << "MultiResolutionAnalysis" << D << "D";
py::class_<MultiResolutionAnalysis<D>> (m, multResAnaName.str().data())
    .def(py::init<BoundingBox<D>, ScalingBasis, int>())
    .def("getOrder", &MultiResolutionAnalysis<D>::getOrder)
    .def("getMaxDepth", &MultiResolutionAnalysis<D>::getMaxDepth)
    .def("getMaxScale", &MultiResolutionAnalysis<D>::getMaxScale);

    std::stringstream mwTreeName;
    mwTreeName << "MWTree" << D << "D";
py::class_<MWTree<D>> mwtree(m, mwTreeName.str().data());
    mwtree
    .def(py::init<MultiResolutionAnalysis<D>>())
    .def("getSquareNorm", &MWTree<D>::getSquareNorm);

    std::stringstream funcTreeName;
    funcTreeName << "FunctionTree" << D << "D";
py::class_<FunctionTree<D>> (m, funcTreeName.str().data(), mwtree)
    .def(py::init<const MultiResolutionAnalysis<D>>())
    .def("integrate", &FunctionTree<D>::integrate);

    std::stringstream repFuncName;
    repFuncName << "RepresentableFunction" << D << "D";
py::class_<RepresentableFunction<D>, PyRepresentableFunction<D>> repfunc(m, repFuncName.str().data());
    repfunc
    .def(py::init<>())
    .def("evalf", &RepresentableFunction<D>::evalf);



    std::stringstream ABGVOperatorName;
    ABGVOperatorName << "ABGVOperator" << D << "D";
py::class_ <ABGVOperator<D>> (m, ABGVOperatorName.str().data())
    .def(py::init< MultiResolutionAnalysis<D> &, double, double >());

    if (D==3){
    py::class_ <PoissonOperator> (m, "PoissonOperator")
        .def(py::init< MultiResolutionAnalysis<3> &, double >());
    }

    m.def("add", py::overload_cast<double, FunctionTree<D> &, double , FunctionTree<D> &, double, FunctionTree<D> &, int>(&add<D>));

    m.def("project", py::overload_cast<double, FunctionTree<D> &, RepresentableFunction<D> &, int>(&project<D>));
    m.def("project", py::overload_cast<double, FunctionTree<1> &, std::function<double (double)>, int>(&project1D));
    m.def("project", py::overload_cast<double, FunctionTree<2> &, std::function<double (double, double)>, int>(&project2D));
    m.def("project", py::overload_cast<double, FunctionTree<3> &, std::function<double (double, double, double)>, int>(&project3D));

    m.def("multiply", py::overload_cast<double, FunctionTree<D> &, double, FunctionTree<D> &, FunctionTree<D> &, int >(&multiply<D>));
    m.def("multiply", py::overload_cast<double, FunctionTree<D> &, FunctionTreeVector<D> &, int >(&multiply<D>));
//
//Gaussians

    std::stringstream gaussianName;
    gaussianName << "Gaussian" << D << "D";
py::class_<Gaussian<D>> gaussian(m, gaussianName.str().data(), repfunc);

    std::stringstream gausFuncName;
    gausFuncName << "GaussFunc" << D << "D";
py::class_<GaussFunc<D>>(m, gausFuncName.str().data(), gaussian)
    .def(py::init<double, double, py::array_t <double>, py::array_t <double>>())
    .def("evalf", py::overload_cast<py::array_t <double>>(&GaussFunc<D>::evalf))
    .def("calcCoulombEnergy", &GaussFunc<D>::calcCoulombEnergy);
}

template void init_pymrcpp<1>(py::module &m);
template void init_pymrcpp<2>(py::module &m);
template void init_pymrcpp<3>(py::module &m);
