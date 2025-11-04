#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

struct P { double x,y; };
double sq(double x){ return x*x; }

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    double x1,y1,r1, x2,y2,r2, x3,y3,r3;
    if(!(cin>>x1>>y1>>r1)) return 0;
    if(!(cin>>x2>>y2>>r2)) return 0;
    if(!(cin>>x3>>y3>>r3)) return 0;

    // Build Apollonius equations for pairs (1,2) and (1,3)
    auto build = [&](double Ax,double Ay,double Ar, double Bx,double By,double Br){
        double s = (Ar/Br)*(Ar/Br);
        double A = 1.0 - s;
        double Dx = -2.0*Ax + 2.0*s*Bx;
        double Dy = -2.0*Ay + 2.0*s*By;
        double F  = Ax*Ax + Ay*Ay - s*(Bx*Bx + By*By);
        return tuple<double,double,double,double,double>(A,Dx,Dy,F,s);
    };

    double A1,D1x,D1y,F1,s12;
    tie(A1,D1x,D1y,F1,s12) = build(x1,y1,r1, x2,y2,r2);
    double A2,D2x,D2y,F2,s13;
    tie(A2,D2x,D2y,F2,s13) = build(x1,y1,r1, x3,y3,r3);

    vector<P> cand;

    auto add_candidate = [&](double x,double y){
        // dedupe
        for(auto &p:cand){
            if (fabs(p.x-x) < 1e-8 && fabs(p.y-y) < 1e-8) return;
        }
        cand.push_back({x,y});
    };

    // Helper: intersection of two lines: a1 x + b1 y + c1 = 0 and a2 x + b2 y + c2 = 0
    auto inter_line_line = [&](double a1,double b1,double c1, double a2,double b2,double c2){
        double det = a1*b2 - a2*b1;
        if (fabs(det) < EPS) return; // parallel or coincident (ignore)
        double x = (b1*c2 - b2*c1)/det;
        double y = (c1*a2 - c2*a1)/det;
        add_candidate(x,y);
    };

    // Circle-line: circle center (cx,cy), R, line a x + b y + c = 0
    auto inter_circle_line = [&](double cx,double cy,double R, double a,double b,double c){
        double norm = sqrt(a*a + b*b);
        if (norm < EPS) return;
        double dist = (a*cx + b*cy + c)/norm;
        double absd = fabs(dist);
        if (absd > R + 1e-9) return;
        // foot point
        double mult = (a*cx + b*cy + c)/(a*a + b*b);
        double fx = cx - a*mult;
        double fy = cy - b*mult;
        if (fabs(absd - R) < 1e-10){
            add_candidate(fx,fy);
            return;
        }
        double h = sqrt(max(0.0, R*R - dist*dist));
        // direction vector along line: (b,-a) normalized
        double vx = b / norm;
        double vy = -a / norm;
        add_candidate(fx + vx*h, fy + vy*h);
        add_candidate(fx - vx*h, fy - vy*h);
    };

    // Circle-circle intersection
    auto inter_circle_circle = [&](double x0,double y0,double r0, double x1,double y1,double r1){
        double dx = x1 - x0, dy = y1 - y0;
        double d2 = dx*dx + dy*dy;
        double d = sqrt(d2);
        if (d < EPS) return; // concentric (unlikely)
        if (d > r0 + r1 + 1e-9) return;
        if (d < fabs(r0 - r1) - 1e-9) return;
        // a = (r0^2 - r1^2 + d^2) / (2d)
        double a = (r0*r0 - r1*r1 + d2) / (2*d);
        double xm = x0 + dx * (a/d);
        double ym = y0 + dy * (a/d);
        double h2 = max(0.0, r0*r0 - a*a);
        if (h2 < 1e-12){
            add_candidate(xm, ym);
            return;
        }
        double h = sqrt(h2);
        double rx = -dy * (h/d);
        double ry = dx * (h/d);
        add_candidate(xm + rx, ym + ry);
        add_candidate(xm - rx, ym - ry);
    };

    // Convert each Apollonius to either circle (center, R) or line (a,b,c)
    bool isCircle1 = fabs(A1) > EPS;
    double c1_cx=0,c1_cy=0,c1_R=0;
    double l1_a=0,l1_b=0,l1_c=0;
    if (isCircle1){
        double g = D1x / (2.0*A1);
        double f = D1y / (2.0*A1);
        double c = F1 / A1;
        c1_cx = -g;
        c1_cy = -f;
        double rr = g*g + f*f - c;
        if (rr < -1e-12) {
            // no real circle -> no intersection
        } else {
            c1_R = sqrt(max(0.0, rr));
        }
    } else {
        l1_a = D1x;
        l1_b = D1y;
        l1_c = F1;
    }

    bool isCircle2 = fabs(A2) > EPS;
    double c2_cx=0,c2_cy=0,c2_R=0;
    double l2_a=0,l2_b=0,l2_c=0;
    if (isCircle2){
        double g = D2x / (2.0*A2);
        double f = D2y / (2.0*A2);
        double c = F2 / A2;
        c2_cx = -g;
        c2_cy = -f;
        double rr = g*g + f*f - c;
        if (rr < -1e-12) {
        } else {
            c2_R = sqrt(max(0.0, rr));
        }
    } else {
        l2_a = D2x;
        l2_b = D2y;
        l2_c = F2;
    }

    // intersect according to types
    if (!isCircle1 && !isCircle2){
        inter_line_line(l1_a,l1_b,l1_c, l2_a,l2_b,l2_c);
    } else if (!isCircle1 && isCircle2){
        inter_circle_line(c2_cx,c2_cy,c2_R, l1_a,l1_b,l1_c);
    } else if (isCircle1 && !isCircle2){
        inter_circle_line(c1_cx,c1_cy,c1_R, l2_a,l2_b,l2_c);
    } else {
        inter_circle_circle(c1_cx,c1_cy,c1_R, c2_cx,c2_cy,c2_R);
    }

    // Validate candidates: check ratio equality and that point not inside any circle (d >= r - tiny)
    vector<P> valid;
    for(auto &p:cand){
        double d1 = hypot(p.x - x1, p.y - y1);
        double d2 = hypot(p.x - x2, p.y - y2);
        double d3 = hypot(p.x - x3, p.y - y3);
        if (d1 < r1 - 1e-7 || d2 < r2 - 1e-7 || d3 < r3 - 1e-7) continue;
        double v1 = r1 / d1;
        double v2 = r2 / d2;
        double v3 = r3 / d3;
        if (fabs(v1 - v2) > 1e-6) continue;
        if (fabs(v1 - v3) > 1e-6) continue;
        valid.push_back(p);
    }

    if (valid.empty()) return 0;

    // choose one with maximal k = r1/d1
    double bestk = -1;
    P bestp = valid[0];
    for(auto &p:valid){
        double d1 = hypot(p.x - x1, p.y - y1);
        double k = r1 / d1;
        if (k > bestk + 1e-12){
            bestk = k;
            bestp = p;
        }
    }

    cout.setf(std::ios::fixed); cout<<setprecision(5);
    cout<<bestp.x<<" "<<bestp.y<<"\n";
    return 0;
}