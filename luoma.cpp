#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long double x1,y1,x2,y2,x3,y3;
    if (!(cin >> x1 >> y1)) return 0;
    cin >> x2 >> y2 >> x3 >> y3;
    const long double PI = acosl(-1.0L);
    const long double EPS = 1e-8L;

    // Circumcenter using determinant formula
    long double a1 = x1, b1 = y1;
    long double a2 = x2, b2 = y2;
    long double a3 = x3, b3 = y3;
    long double d = 2.0L*(a1*(b2-b3) + a2*(b3-b1) + a3*(b1-b2));
    if (fabsl(d) < EPS) {
        // Collinear or nearly so - no finite circumcircle for three distinct collinear points.
        // Problem constraints likely avoid this; output 0.0 as fallback.
        cout.setf(std::ios::fixed); cout<<setprecision(8)<<0.0L<<"\n";
        return 0;
    }
    long double a1s = a1*a1 + b1*b1;
    long double a2s = a2*a2 + b2*b2;
    long double a3s = a3*a3 + b3*b3;
    long double cx = (a1s*(b2-b3) + a2s*(b3-b1) + a3s*(b1-b2)) / d;
    long double cy = (a1s*(a3-a2) + a2s*(a1-a3) + a3s*(a2-a1)) / d;

    auto ang = [&](long double x, long double y)->long double {
        return atan2l(y - cy, x - cx);
    };
    long double A0 = ang(x1,y1);
    long double A1 = ang(x2,y2);
    long double A2 = ang(x3,y3);
    // normalize to [0, 2pi)
    auto norm2pi = [&](long double t)->long double {
        long double r = fmodl(t, 2.0L*PI);
        if (r < 0) r += 2.0L*PI;
        return r;
    };
    A0 = norm2pi(A0);
    A1 = norm2pi(A1);
    A2 = norm2pi(A2);

    long double R = hypotl(x1 - cx, y1 - cy);
    long double best = 1e300L;
    for (int n = 3; n <= 100; ++n) {
        long double step = 2.0L*PI / n;
        // check differences from A0 are integer multiples of step
        long double d1 = norm2pi(A1 - A0);
        long double d2 = norm2pi(A2 - A0);
        long double q1 = d1 / step;
        long double q2 = d2 / step;
        long double k1 = roundl(q1);
        long double k2 = roundl(q2);
        if (fabsl(q1 - k1) < 1e-6L && fabsl(q2 - k2) < 1e-6L) {
            // valid n
            long double area = 0.5L * n * R * R * sinl(step);
            if (area < best) best = area;
        }
    }

    if (best > 1e200L) best = 0.0L;
    cout.setf(std::ios::fixed);
    cout<<setprecision(8)<< (double)best <<"\n";
    return 0;
}