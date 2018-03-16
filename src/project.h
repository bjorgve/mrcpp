#pragma once

#include <functional>

#include "mrcpp_declarations.h"


namespace mrcpp {
template<int D> void project(double prec, FunctionTree<D> &out, RepresentableFunction<D> &inp, int maxIter = -1);
template<int D> void project(double prec, FunctionTree<D> &out, std::function<double (const double *r)> func, int maxIter = -1);
void project3D(double prec, FunctionTree<3> &out, std::function<double (double x, double y, double z)> func, int maxIter = -1);
void project2D(double prec, FunctionTree<2> &out, std::function<double (double x, double y)> func, int maxIter = -1);

}
