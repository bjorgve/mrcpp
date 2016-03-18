#ifndef MULTIPLICATIONCALCULATOR_H
#define MULTIPLICATIONCALCULATOR_H

#include "TreeCalculator.h"
#include "MultiplicationVector.h"

template<int D> class MWMultiplier;

template<int D>
class MultiplicationCalculator : public TreeCalculator<D> {
public:
    friend class MWMultiplier<D>;
protected:
    MultiplicationCalculator(MultiplicationVector<D> &inp) : prod_vec(&inp) { }
    virtual ~MultiplicationCalculator() { }

    virtual void calcNode(MWNode<D> &node_o) const {
        const NodeIndex<D> &idx = node_o.getNodeIndex();
        Eigen::VectorXd &vec_o = node_o.getCoefs();
        vec_o.setConstant(1.0);
        for (int i = 0; i < this->prod_vec->size(); i++) {
            FunctionTree<D> &func_i = this->prod_vec->getFunc(i);
            MWNode<D> node_i = func_i.getNode(idx); // Copy node
            node_i.mwTransform(Reconstruction);
            node_i.cvTransform(Forward);
            const Eigen::VectorXd &vec_i = node_i.getCoefs();
            vec_o = vec_o.array() * vec_i.array();
        }
        node_o.cvTransform(Backward);
        node_o.mwTransform(Compression);
        node_o.setHasCoefs();
        node_o.calcNorms();
    }

private:
    MultiplicationVector<D> *prod_vec;
};

#endif // MULTIPLICATIONCALCULATOR_H