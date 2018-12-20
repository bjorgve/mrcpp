/**
 *
 *
 * \date May 25, 2010
 * \author Stig Rune Jensen
 *		   CTCC, University of Tromsø
 *
 */

#include "Gaussian.h"
#include "trees/NodeIndex.h"

using namespace Eigen;

namespace mrcpp {

template<int D>
Gaussian<D>::Gaussian(double a, double c, const double r[D], const int p[D])
            : screen(false),
              coef(c),
              squareNorm(-1.0) {
    this->alpha.fill(a);
    for (int d = 0; d < D; d++) {
        if (r == nullptr) {
            this->pos[d] = 0.0;
        } else {
            this->pos[d] = r[d];
        }
        if (p == nullptr) {
            this->power[d] = 0;
        } else {
            this->power[d] = p[d];
        }
    }
}

template<int D>
Gaussian<D>::Gaussian(double a, double c, const Coord<D> &r,
                      const std::array<int, D> &p)
            : screen(false),
              coef(c),
              power(p),
              pos(r),
              squareNorm(-1.0) {
    this->alpha.fill(a);
}

template<int D>
Gaussian<D>::Gaussian(const std::array<double, D> &a, double c, const Coord<D> &r,
                      const std::array<int, D> &p)
            : screen(false),
              coef(c),
              power(p),
              alpha(a),
              pos(r),
              squareNorm(-1.0) {}

template<int D>
Gaussian<D>::~Gaussian() {
}

template<int D>
void Gaussian<D>::multPureGauss(const Gaussian<D> &lhs,	const Gaussian<D> &rhs) {

    auto newAlpha = std::array<double, D> {};
    auto mju = std::array<double, D> {};
    for (auto d = 0; d < D; d++) {
        newAlpha[d] = lhs.alpha[d] + rhs.alpha[d];
        mju[d] = (lhs.alpha[d] * rhs.alpha[d]) / newAlpha[d];
    }
    auto newPos = std::array<double, D> {};
    auto relPos = std::array<double, D> {};

    double newCoef = 1.0;
    for (int d = 0; d < D; d++) {
        newPos[d] = (lhs.alpha[d]*lhs.pos[d] + rhs.alpha[d]*rhs.pos[d])/newAlpha[d];
        relPos[d] = lhs.pos[d] - rhs.pos[d];
        newCoef *= std::exp(-mju[d] * std::pow(relPos[d], 2.0));
    }
    setExp(newAlpha);
    setPos(newPos);
    this->squareNorm = -1.0;
    setCoef(newCoef);
}

template<int D>
void Gaussian<D>::calcScreening(double nStdDev) {
    assert(nStdDev > 0);
    if (not this->isBounded()) {
        this->bounded = true;
        this->A = new double[D];
        this->B = new double[D];
    }
    for (int d = 0; d < D; d++) {
        double limit = std::sqrt(nStdDev/this->alpha[d]);
        this->A[d] = this->pos[d] - limit;
        this->B[d] = this->pos[d] + limit;
    }
    screen = true;
}

template<int D>
bool Gaussian<D>::checkScreen(int n, const int *l) const {
    if (not getScreen()) {
        return false;
    }
    double length = std::pow(2.0, -n);
    const double *A = this->getLowerBounds();
    const double *B = this->getUpperBounds();
    for (int d = 0; d < D; d++) {
        double a = length * l[d];
        double b = length * (l[d] + 1);
        if (a > B[d] or b < A[d]) {
            return true;
        }
    }
    return false;
}

template<int D>
bool Gaussian<D>::isVisibleAtScale(int scale, int nQuadPts) const {

    for (auto& alp : this->alpha) {
        double stdDeviation = std::pow(2.0*alp, -0.5);
        int visibleScale = int(-std::floor(std::log2(nQuadPts*2.0*stdDeviation)));

        if (scale < visibleScale) {
            return false;
        }
    }

    return true;
}

template<int D>
bool Gaussian<D>::isZeroOnInterval(const double *a, const double *b) const {
    for (int i = 0; i < D; i++) {
        double stdDeviation = std::pow(2.0*this->alpha[i], -0.5);
        double gaussBoxMin = this->pos[i] - 5.0*stdDeviation;
        double gaussBoxMax = this->pos[i] + 5.0*stdDeviation;
        if (a[i] > gaussBoxMax or b[i] < gaussBoxMin) {
            return true;
        }
    }
    return false;
}

template<int D>
void Gaussian<D>::evalf(const MatrixXd &points, MatrixXd &values) const {
    assert(points.cols() == D);
    assert(points.cols() == values.cols());
    assert(points.rows() == values.rows());
    for (int d = 0; d < D; d++) {
        for (int i = 0; i < points.rows(); i++) {
            values(i, d) = evalf(points(i, d), d);
        }
    }
}

template class Gaussian<1>;
template class Gaussian<2>;
template class Gaussian<3>;

} // namespace mrcpp
