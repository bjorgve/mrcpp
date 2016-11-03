/*
 *
 *
 *  \date Oct 15, 2009
 *  \author Jonas Juselius <jonas.juselius@uit.no> \n
 *          CTCC, University of Tromsø
 *
 * \breif
 */

#ifndef SCALINGBASIS_H
#define SCALINGBASIS_H

#include <vector>

#include "TelePrompter.h"
#include "constants.h"
#include "Polynomial.h"

class ScalingBasis {
public:
    ScalingBasis(int k, int t)
            : type(t),
              order(k) {
        if (this->order < 1) MSG_FATAL("Invalid scaling order");
        int q_order = getQuadratureOrder();
        this->quadVals = Eigen::MatrixXd::Zero(q_order, q_order);
    }
    virtual ~ScalingBasis() { }

    void evalf(const double *r, Eigen::MatrixXd &vals) const {
        if (vals.rows() != this->funcs.size()) MSG_ERROR("Invalid argument");

        for (int d = 0; d < vals.cols(); d++) {
            for (int k = 0; k < vals.rows(); k++) {
                vals(k, d) = getFunc(k).evalf(r[d]);
            }
        }
    }

    Polynomial &getFunc(int k) { return this->funcs[k]; }
    const Polynomial &getFunc(int k) const { return this->funcs[k]; }

    int getScalingType() const { return this->type; }
    int getScalingOrder() const { return this->order; }
    int getQuadratureOrder() const { return this->order + 1; }
    const Eigen::MatrixXd &getQuadratureValues() const { return this->quadVals; }

    bool operator==(const ScalingBasis &basis) const {
        if (this->type != basis.type) return false;
        if (this->order != basis.order) return false;
        return true;
    }
    bool operator!=(const ScalingBasis &basis) const {
        if (this->type != basis.type) return true;
        if (this->order != basis.order) return true;
        return false;
    }

    friend std::ostream& operator<<(std::ostream &o, const ScalingBasis &bas) {
        o << "*ScalingBasis:" << std::endl;
        o << "  order           = " << bas.getScalingOrder() << std::endl;
        if (bas.getScalingType() == Legendre) {
            o << "  type            = Legendre";
        } else if (bas.getScalingType() == Interpol) {
            o << "  type            = Interpolating";
        } else {
            o << "  type            = Unknown";
        }
        return o;
    }
protected:
    const int type;
    const int order;
    Eigen::MatrixXd quadVals;
    std::vector<Polynomial> funcs;
};

#endif /* SCALINGBASIS_H */
