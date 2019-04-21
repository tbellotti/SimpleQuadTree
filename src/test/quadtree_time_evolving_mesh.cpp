
#include "../../include/quadtree.h"


class MyTimeVaryingLevelSet : public LipschitzFunction<double>
{
private:
    double t_m = 0.0;
public:
    MyTimeVaryingLevelSet() : LipschitzFunction<double>(), t_m(double(0.0)) {}
    MyTimeVaryingLevelSet(double lipschitz) : LipschitzFunction<double>(lipschitz) {}
    ~MyTimeVaryingLevelSet() = default;

    void setTime(double new_time) { t_m = new_time; }

    double operator()(Point<double> arg) const override
    {
        Point<double> tmp1 = arg+Point<double>(-0.52+0.2*t_m, -0.55+0.15*t_m);
        Point<double> tmp2 = arg+Point<double>(-0.23-0.3*t_m, -0.25+0.05*t_m);
        return std::min(tmp1.abs() - 0.070*(1.0+0.5*t_m), tmp2.abs() - 0.13*(1.0+0.3*t_m));
    }
};


int main ()
{

    QuadTree<double> my_tree_level_set(Point<double>(0.0, 0.0), 1.0, 1.0, 1, 7.0);
    my_tree_level_set.buildUniform(3);
    MyTimeVaryingLevelSet ls_crit(1.2);

    unsigned int N = 20;
    double dt = 1.0/N;
    double t = 0.0;

    for (unsigned int n = 0; n < N; n++)    {

        std::ostringstream fname;
        fname <<"/home/thomas/SimpleQuadTree/media/output/test/quadtree_time_evolving_mesh/frame"<<n<<".tex";
        std::string filename = fname.str();

        my_tree_level_set.updateWithLevelSet(ls_crit);
        my_tree_level_set.exportMeshTikz(filename, false);

        t = t+dt;
        ls_crit.setTime(t);
    }


    return 0;
}