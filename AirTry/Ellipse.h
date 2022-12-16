#pragma once
#include <stdio.h>
#include <math.h>
#include "Lin.h"
#include "model.h"

/* Ellipse-circle collision detection
 *
 * by Olli Niemitalo in 2012-08-06.
 * This work is placed in the public domain.
 * source: https://math.stackexchange.com/questions/1114879/detect-if-two-ellipses-intersect
 */

class EllipseCollisionTest {
private:
    double* innerPolygonCoef;
    double* outerPolygonCoef;
    int maxIterations;

    bool iterate(double x, double y, double c0x, double c0y, double c2x, double c2y, double rr) const {
        for (int t = 1; t <= maxIterations; t++) {
            double c1x = (c0x + c2x) * innerPolygonCoef[t];
            double c1y = (c0y + c2y) * innerPolygonCoef[t];
            double tx = x - c1x;
            double ty = y - c1y;
            if (tx * tx + ty * ty <= rr) {
                return true;
            }
            double t2x = c2x - c1x;
            double t2y = c2y - c1y;
            if (tx * t2x + ty * t2y >= 0 && tx * t2x + ty * t2y <= t2x * t2x + t2y * t2y &&
                (ty * t2x - tx * t2y >= 0 || rr * (t2x * t2x + t2y * t2y) >= (ty * t2x - tx * t2y) * (ty * t2x - tx * t2y))) {
                return true;
            }
            double t0x = c0x - c1x;
            double t0y = c0y - c1y;
            if (tx * t0x + ty * t0y >= 0 && tx * t0x + ty * t0y <= t0x * t0x + t0y * t0y &&
                (ty * t0x - tx * t0y <= 0 || rr * (t0x * t0x + t0y * t0y) >= (ty * t0x - tx * t0y) * (ty * t0x - tx * t0y))) {
                return true;
            }
            double c3x = (c0x + c1x) * outerPolygonCoef[t];
            double c3y = (c0y + c1y) * outerPolygonCoef[t];
            if ((c3x - x) * (c3x - x) + (c3y - y) * (c3y - y) < rr) {
                c2x = c1x;
                c2y = c1y;
                continue;
            }
            double c4x = c1x - c3x + c1x;
            double c4y = c1y - c3y + c1y;
            if ((c4x - x) * (c4x - x) + (c4y - y) * (c4y - y) < rr) {
                c0x = c1x;
                c0y = c1y;
                continue;
            }
            double t3x = c3x - c1x;
            double t3y = c3y - c1y;
            if (ty * t3x - tx * t3y <= 0 || rr * (t3x * t3x + t3y * t3y) > (ty * t3x - tx * t3y) * (ty * t3x - tx * t3y)) {
                if (tx * t3x + ty * t3y > 0) {
                    if (fabs(tx * t3x + ty * t3y) <= t3x * t3x + t3y * t3y || (x - c3x) * (c0x - c3x) + (y - c3y) * (c0y - c3y) >= 0) {
                        c2x = c1x;
                        c2y = c1y;
                        continue;
                    }
                }
                else if (-(tx * t3x + ty * t3y) <= t3x * t3x + t3y * t3y || (x - c4x) * (c2x - c4x) + (y - c4y) * (c2y - c4y) >= 0) {
                    c0x = c1x;
                    c0y = c1y;
                    continue;
                }
            }
            return false;
        }
        return false; // Out of iterations so it is unsure if there was a collision. But have to return something.
    }
public:


    // Test for collision between two ellipses, "0" and "1". Ellipse is at (x, y) with major or minor radius 
    // vector (wx, wy) and the other major or minor radius perpendicular to that vector and hw times as long.
    bool collide(double x0, double y0, double wx0, double wy0, double hw0,
        double x1, double y1, double wx1, double wy1, double hw1) const {
        float rr = hw1 * hw1 * (wx1 * wx1 + wy1 * wy1) * (wx1 * wx1 + wy1 * wy1) * (wx1 * wx1 + wy1 * wy1);
        float x = hw1 * wx1 * (wy1 * (y1 - y0) + wx1 * (x1 - x0)) - wy1 * (wx1 * (y1 - y0) - wy1 * (x1 - x0));
        float y = hw1 * wy1 * (wy1 * (y1 - y0) + wx1 * (x1 - x0)) + wx1 * (wx1 * (y1 - y0) - wy1 * (x1 - x0));
        float temp = wx0;
        wx0 = hw1 * wx1 * (wy1 * wy0 + wx1 * wx0) - wy1 * (wx1 * wy0 - wy1 * wx0);
        float temp2 = wy0;
        wy0 = hw1 * wy1 * (wy1 * wy0 + wx1 * temp) + wx1 * (wx1 * wy0 - wy1 * temp);
        float hx0 = hw1 * wx1 * (wy1 * (temp * hw0) - wx1 * temp2 * hw0) - wy1 * (wx1 * (temp * hw0) + wy1 * temp2 * hw0);
        float hy0 = hw1 * wy1 * (wy1 * (temp * hw0) - wx1 * temp2 * hw0) + wx1 * (wx1 * (temp * hw0) + wy1 * temp2 * hw0);

        if (wx0 * y - wy0 * x < 0) {
            x = -x;
            y = -y;
        }

        if ((wx0 - x) * (wx0 - x) + (wy0 - y) * (wy0 - y) <= rr) {
            return true;
        }
        else if ((wx0 + x) * (wx0 + x) + (wy0 + y) * (wy0 + y) <= rr) {
            return true;
        }
        else if ((hx0 - x) * (hx0 - x) + (hy0 - y) * (hy0 - y) <= rr) {
            return true;
        }
        else if ((hx0 + x) * (hx0 + x) + (hy0 + y) * (hy0 + y) <= rr) {
            return true;
        }
        else if (x * (hy0 - wy0) + y * (wx0 - hx0) <= hy0 * wx0 - hx0 * wy0 &&
            y * (wx0 + hx0) - x * (wy0 + hy0) <= hy0 * wx0 - hx0 * wy0) {
            return true;
        }
        else if (x * (wx0 - hx0) - y * (hy0 - wy0) > hx0 * (wx0 - hx0) - hy0 * (hy0 - wy0)
            && x * (wx0 - hx0) - y * (hy0 - wy0) < wx0 * (wx0 - hx0) - wy0 * (hy0 - wy0)
            && (x * (hy0 - wy0) + y * (wx0 - hx0) - hy0 * wx0 + hx0 * wy0) * (x * (hy0 - wy0) + y * (wx0 - hx0) - hy0 * wx0 + hx0 * wy0)
            <= rr * ((wx0 - hx0) * (wx0 - hx0) + (wy0 - hy0) * (wy0 - hy0))) {
            return true;
        }
        else if (x * (wx0 + hx0) + y * (wy0 + hy0) > -wx0 * (wx0 + hx0) - wy0 * (wy0 + hy0)
            && x * (wx0 + hx0) + y * (wy0 + hy0) < hx0 * (wx0 + hx0) + hy0 * (wy0 + hy0)
            && (y * (wx0 + hx0) - x * (wy0 + hy0) - hy0 * wx0 + hx0 * wy0) * (y * (wx0 + hx0) - x * (wy0 + hy0) - hy0 * wx0 + hx0 * wy0)
            <= rr * ((wx0 + hx0) * (wx0 + hx0) + (wy0 + hy0) * (wy0 + hy0))) {
            return true;
        }
        else {
            if ((hx0 - wx0 - x) * (hx0 - wx0 - x) + (hy0 - wy0 - y) * (hy0 - wy0 - y) <= rr) {
                return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
            }
            else if ((hx0 + wx0 - x) * (hx0 + wx0 - x) + (hy0 + wy0 - y) * (hy0 + wy0 - y) <= rr) {
                return iterate(x, y, wx0, wy0, hx0, hy0, rr);
            }
            else if ((wx0 - hx0 - x) * (wx0 - hx0 - x) + (wy0 - hy0 - y) * (wy0 - hy0 - y) <= rr) {
                return iterate(x, y, -hx0, -hy0, wx0, wy0, rr);
            }
            else if ((-wx0 - hx0 - x) * (-wx0 - hx0 - x) + (-wy0 - hy0 - y) * (-wy0 - hy0 - y) <= rr) {
                return iterate(x, y, -wx0, -wy0, -hx0, -hy0, rr);
            }
            else if (wx0 * y - wy0 * x < wx0 * hy0 - wy0 * hx0 && fabs(hx0 * y - hy0 * x) < hy0 * wx0 - hx0 * wy0) {
                if (hx0 * y - hy0 * x > 0) {
                    return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
                }
                return iterate(x, y, wx0, wy0, hx0, hy0, rr);
            }
            else if (wx0 * x + wy0 * y > wx0 * (hx0 - wx0) + wy0 * (hy0 - wy0) && wx0 * x + wy0 * y < wx0 * (hx0 + wx0) + wy0 * (hy0 + wy0)
                && (wx0 * y - wy0 * x - hy0 * wx0 + hx0 * wy0) * (wx0 * y - wy0 * x - hy0 * wx0 + hx0 * wy0) < rr * (wx0 * wx0 + wy0 * wy0)) {
                if (wx0 * x + wy0 * y > wx0 * hx0 + wy0 * hy0) {
                    return iterate(x, y, wx0, wy0, hx0, hy0, rr);
                }
                return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
            }
            else {
                if (hx0 * y - hy0 * x < 0) {
                    x = -x;
                    y = -y;
                }
                if (hx0 * x + hy0 * y > -hx0 * (wx0 + hx0) - hy0 * (wy0 + hy0) && hx0 * x + hy0 * y < hx0 * (hx0 - wx0) + hy0 * (hy0 - wy0)
                    && (hx0 * y - hy0 * x - hy0 * wx0 + hx0 * wy0) * (hx0 * y - hy0 * x - hy0 * wx0 + hx0 * wy0) < rr * (hx0 * hx0 + hy0 * hy0)) {
                    if (hx0 * x + hy0 * y > -hx0 * wx0 - hy0 * wy0) {
                        return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
                    }
                    return iterate(x, y, -wx0, -wy0, -hx0, -hy0, rr);
                }
                return false;
            }
        }
    }
    
    // Проверка пересечения двух эллипсов, заданных нормальным способом через малую и большую полуось a, b и угол поворота theta
    bool collide_ab(double x0, double y0, double a0, double b0, double theta0, 
        double x1, double y1, double a1, double b1, double theta1) const 
    {    
        double wx0, wy0, hw0, wx1, wy1, hw1;
        
        double tmp0 = tan(theta0);
        wx0 = sqrt(a0 * a0 / 4 / (tmp0 * tmp0 + 1));
        wy0 = wx0 * tmp0;
        hw0 = b0 / a0;

        double tmp1 = tan(theta1);
        wx1 = sqrt(a1 * a1 / 4 / (tmp1 * tmp1 + 1));
        wy1 = wx1 * tmp1;
        hw1 = b1 / a1;

        return collide(x0, y0, wx0, wy0, hw0, x1, y1, wx1, wy1, hw1);
    }

    bool collide_la(LA* la0, LA* la1,double time_safe, double safezoneback, double safezoneside) const
    {
        double x0, y0, a0, b0, theta0, x1, y1, a1, b1, theta1;

        a0 = (la0->X[3] * time_safe + safezoneback);
        b0 = safezoneside * 2;
        theta0 = la0->X[4] + M_PI / 2;
        x0 = la0->X[2] + (a0 / 2 - safezoneside) * cos(theta0);
        y0 = la0->X[0] + (a0 / 2 - safezoneside) * sin(theta0);


        a1 = (la1->X[3] * time_safe + safezoneback);
        b1 = safezoneside * 2;
        theta1 = la1->X[4] + M_PI / 2;
        x1 = la1->X[2] + (a1 / 2 - safezoneside) * cos(theta1);
        y1 = la1->X[0] + (a1 / 2 - safezoneside) * sin(theta1);

        // Podaem a/2 & b/2 potomu chto zdes a - eto diagonal, a v algoritme a eto radius
        return collide_ab(x0, y0, a0, b0, theta0, x1, y1, a1, b1, theta1);
    }

    EllipseCollisionTest(int maxIterations) {
        this->maxIterations = maxIterations;
        innerPolygonCoef = new double[maxIterations + 1];
        outerPolygonCoef = new double[maxIterations + 1];
        for (int t = 0; t <= maxIterations; t++) {
            int numNodes = 4 << t;
            innerPolygonCoef[t] = 0.5 / cos(4 * acos(0.0) / numNodes);
            outerPolygonCoef[t] = 0.5 / (cos(2 * acos(0.0) / numNodes) * cos(2 * acos(0.0) / numNodes));
        }
    }

    ~EllipseCollisionTest() {
        delete[] innerPolygonCoef;
        delete[] outerPolygonCoef;
    }
};