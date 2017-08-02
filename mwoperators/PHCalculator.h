#ifndef PHCALCULATOR_H
#define PHCALCULATOR_H

#pragma GCC system_header
#include <Eigen/Core>

#include "TreeCalculator.h"

class PHCalculator : public TreeCalculator<2> {
public:
    PHCalculator(const ScalingBasis &basis, int n);
    virtual ~PHCalculator() { }

protected:
    const int diff_order;
    Eigen::MatrixXd S_m1;
    Eigen::MatrixXd S_0;
    Eigen::MatrixXd S_p1;

    virtual void calcNode(MWNode<2> &node);
    void readSMatrix(const ScalingBasis &basis, char n);
};

#endif // PHCALCULATOR_H