#include <bits/stdc++.h>
#define PI 3.1415926535

using namespace std;
double h, r;
double eps = 1e-4;
double a, b;

double circleIntersection(double c1, double c2, double radius){
    const double distance = c2 - c1;
    if (distance > 2* radius){
        return 0.0;
    }
    double x =  sqrt( pow(radius, 2) -  pow(distance/2, 2)); // x^2 = r ^2 + (d/2)^2
    double angle = fmod( atan2(x, distance / 2) * 2 + 2 * PI, 2 * PI);
    return 2 * pow(radius, 2) / 2.0 * (angle -  sin(angle));
}

double f(double y){
    double temp_r = r*(h - y)/h;
    double c1 = (0 - y/h)*a;
    double c2 = (0 + y/h)*b;
    return circleIntersection(c1, c2, temp_r);
}

double height(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3){
    double e1 = (y1 - y0)*(y3 - y2)*(x2 - x0);
    double e2 = y0 * (x1 - x0) * (y3 - y2);
    double e3 = y2 * (x3 - x2) * (y1 - y0);
    double e4 = (x1 - x0) * (y3 - y2) - (x3 - x2)*(y1 - y0);
    return (e1 + e2 - e3)/e4;
}

double simpsons_rule(double x, double y) {
    double fx0 = f(x);
    double fx1 = f((x + y) / 2);
    double fx2 = f(y);
    double dx = (y - x)/2;
    return (fx0 + 4*fx1 + fx2)*dx/3;
}

double integration(double x, double y){
    double m = (x+y)/2;
    double l = simpsons_rule(x,m);
    double r = simpsons_rule(m,y);
    double whole = simpsons_rule(x,y);
    if (fabs(l+r-whole ) < eps)
        return whole;
    return integration(x,m) + integration(m,y);
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        cin >> r >> h >> a >> b;
        double hei = height(0 - a, h, r, 0, b, h, 0 - r, 0);
        double cone_vol = r*r*h*PI/3;
        double intersect_vol = integration(0, hei);
        double out = 2*cone_vol - intersect_vol;
        ostringstream os;
        os << out;
        string s = os.str();
        s = s.substr(0, s.find('.') + 2);
        cout << s << endl;
    }
    return 0;
}