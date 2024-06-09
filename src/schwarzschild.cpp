#include <Eigen/Dense>
#include <cmath>
#include "schwarzschild.h"

inline double Schwarzschild::g_tt(double r, double theta) {
    return -(1 - 2.0 / r);
}

inline double Schwarzschild::g_rr(double r, double theta) {
    return 1 / (1 - 2.0 / r);
}

Vector8d Schwarzschild::geodesic_eq_rhs(const Vector8d& y) {
    Vector8d derivatives;

    double r = y(1);
    double theta = y(2);
    double phi = y(3);
    double p0 = y(4);
    double p1 = y(5);
    double p2 = y(6);
    double p3 = y(7);

    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);

    derivatives(0) = p0;
    derivatives(1) = p1;
    derivatives(2) = p2;
    derivatives(3) = p3;

    derivatives(4) = -2.0/(r*r*(1 - 2.0/r))*p0*p1;
    derivatives(5) = -1.0/(r*r)*(1 - 2.0/r)*p0*p0 + 1.0/(r*r*(1 - 2.0/r))*p1*p1 + r*(1 - 2.0/r)*p2*p2 + (1 - 2.0/r)*r*std::pow(std::sin(theta), 2)*p3*p3;
    derivatives(6) = -2.0/r*p1*p2 + std::sin(theta)*std::cos(theta)*p3*p3;
    derivatives(7) = -2.0/r*p1*p3 - 2.0*std::cos(theta)/std::sin(theta)*p2*p3;

    return derivatives;
}

bool Schwarzschild::break_integration(const Vector8d& y, bool &outside_celestial_sphere, bool &below_EH, bool &inside_disk) {
        double z = y(1)*std::cos(y(2));
        
        below_EH = y(1) <= 1.1*r_EH;
        outside_celestial_sphere = y(1) >= 100.0;
        inside_disk = (std::abs(z) <= 0.1) and (y(1) >= 5.0) and (y(1) <= 10.0);

        return (outside_celestial_sphere or below_EH or inside_disk);
    }