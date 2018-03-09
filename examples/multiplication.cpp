#include "MRCPP/MWFunctions"
#include "MRCPP/Gaussians"
#include "MRCPP/Printer"
#include "MRCPP/Timer"

const int min_scale = -4;
const int max_depth = 25;

const int order = 7;
const double prec = 1.0e-5;

int main(int argc, char **argv) {
    mrcpp::Timer timer;

    // Initialize printing
    int printlevel = 0;
    mrcpp::Printer::init(printlevel);
    mrcpp::Printer::printEnvironment();
    mrcpp::Printer::printHeader(0, "Multiplying MW functions");

    // Constructing world box
    int corner[3] = {-1,-1,-1};
    int boxes[3]  = { 2, 2, 2};
    mrcpp::BoundingBox<3> world(min_scale, corner, boxes);

    // Constructing basis and MRA
    mrcpp::InterpolatingBasis basis(order);
    mrcpp::MultiResolutionAnalysis<3> MRA(world, basis, max_depth);

    // Setting up analytic Gaussians
    double beta = 20.0;
    double alpha = pow(beta/mrcpp::pi, 3.0/2.0);
    double f_pos[3] = {0.0, 0.0,  0.1};
    double g_pos[3] = {0.0, 0.0, -0.1};
    mrcpp::GaussFunc<3> f_func(beta, alpha, f_pos);
    mrcpp::GaussFunc<3> g_func(beta, alpha, g_pos);

    // Initialize MW functions
    mrcpp::FunctionTree<3> f_tree(MRA);
    mrcpp::FunctionTree<3> g_tree(MRA);
    mrcpp::FunctionTree<3> h_tree(MRA);

    // Projecting f and g
    mrcpp::project(prec, f_tree, f_func);
    mrcpp::project(prec, g_tree, g_func);

    // h = f*g
    mrcpp::multiply(prec, h_tree, 1.0, f_tree, g_tree);

    double integral = h_tree.integrate();
    double sq_norm = h_tree.getSquareNorm();
    mrcpp::Printer::printDouble(0, "Integral", integral);
    mrcpp::Printer::printDouble(0, "Square norm", sq_norm);

    timer.stop();
    mrcpp::Printer::printFooter(0, timer, 2);

    return 0;
}
