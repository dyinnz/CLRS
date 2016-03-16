/*******************************************************************************
 * Author: Dyinnz.HUST.UniqueStudio
 * Email:  ml_143@sina.com
 * Github: https://github.com/dyinnz
 * Date:   2015-12-23
 ******************************************************************************/

#include <vector>
#include <iostream>
#include <algorithm>
#include <set> 
#include <functional>

using namespace std;

struct Vector {
  Vector() = default;
  Vector(float x, float y) : x(x), y(y) {}
  float x {0}, y {0};

  Vector& operator+=(const Vector &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vector& operator-=(const Vector &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
};

Vector operator+(const Vector &lhs, const Vector &rhs) {
  Vector temp(lhs);
  temp += rhs;
  return temp;
}

Vector operator-(const Vector &lhs, const Vector &rhs) {
  Vector temp(lhs);
  temp -= rhs;
  return temp;
}

ostream& operator<<(ostream &out, const Vector &p) {
  cout << '(' << p.x << ", " << p.y << ')';
  return out;
}

struct Segment {
  Segment() = default;
  Segment(Vector l, Vector r) : l(l), r(r) {}
  Vector l, r;
};

float cross(const Vector &lhs, const Vector &rhs) {
  return lhs.x * rhs.y - rhs.x * lhs.y;
}

float direction(const Vector &pi, const Vector &pj, const Vector &pk) {
  return cross(pk - pi, pj - pi);
}

bool OnSegments(const Vector &pi, const Vector &pj, const Vector &pk) {
  if (min(pi.x, pj.x) <= pk.x && pk.x <= max(pi.x, pj.x) &&
      min(pi.y, pj.y) <= pk.x && pk.y <= max(pi.y, pj.y)) {
    return true;
  } else {
    return false;
  }
}

bool SegmentsIntersect(const Vector &p1, const Vector &p2, 
                       const Vector &p3, const Vector &p4) {
  int d1 = direction(p3, p4, p1);
  int d2 = direction(p3, p4, p2);
  int d3 = direction(p1, p2, p3);
  int d4 = direction(p1, p2, p4);
  if ( ((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
       ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
    return true;
  } else if (0 == d1 && OnSegments(p3, p4, p1)) {
    return true;
  } else if (0 == d2 && OnSegments(p3, p4, p2)) {
    return true;
  } else if (0 == d3 && OnSegments(p1, p2, p3)) {
    return true;
  } else if (0 == d4 && OnSegments(p1, p2, p4)) {
    return true;
  } else {
    return false;
  }
}

bool SegmentsIntersect(const Segment &s1, const Segment &s2) {
  return SegmentsIntersect(s1.l, s1.r, s2.l, s2.r);
}

bool AnySegmentsIntersect(vector<Segment> &S) {
  struct TempPoint {
    float x; 
    float y;
    int lr;
    Segment* ps;
  };
  vector<TempPoint> points;
  for (Segment &s : S) {
    points.push_back( {s.l.x, s.l.y, 0, &s} );
    points.push_back( {s.r.x, s.r.y, 1, &s} );
  }
  sort(points.begin(), points.end(), [](TempPoint &lhs, TempPoint &rhs) {
    if (lhs.x < rhs.y) {
      return true;
    } else if (lhs.x > rhs.y) {
      return false;
    }
    if (lhs.lr < rhs.lr) {
      return true;
    } else {
      return false;
    }
    return lhs.y < rhs.y;
  });
  
  auto cmp = [](Segment *lhs, Segment *rhs) {
    return lhs->l.y < rhs->l.y;
  };
  set<Segment*, decltype(cmp)> T(cmp);

  for (auto &p : points) {
    if (0 == p.lr) {
      T.insert(p.ps);
      auto iter = T.find(p.ps);
      if ((T.end() != next(iter) && SegmentsIntersect(**next(iter), *p.ps)) ||
          (T.end() != prev(iter) && SegmentsIntersect(**prev(iter), *p.ps))) {
        return true;
      }
    } else {
      auto iter = T.find(p.ps);
      if ((T.end() != next(iter) && T.end() != prev(iter)) &&
          SegmentsIntersect(**next(iter), **prev(iter))) {
        return true;
      }
      T.erase(iter);
    }
  }
  return false;
}

struct Point {
  float x, y;
};

float distance(Point &lhs, Point &rhs) {
  return sqrt( (lhs.x-rhs.x)*(lhs.x-rhs.x) + (lhs.y-rhs.y)*(lhs.y-rhs.y) );
}

float ClosestPairPoints(vector<Point> &X, vector<Point> &Y) {
  cout << "X: ";
  for (auto p : X) {
    cout << "(" << p.x << "," << p.y << ") ";
  }
  cout << endl;
  cout << "Y: ";
  for (auto p : Y) {
    cout << "(" << p.x << "," << p.y << ") ";
  }
  cout << endl;
  cout << endl;

  if (X.size() <= 1) {
    return 10e10;

  } else if (X.size() == 2) {
    return distance(X[0], X[1]);

  } else if (X.size() <= 3) {
    return min(distance(X[0], X[1]), min(distance(X[0], X[2]), distance(X[0], X[2])));

  } else {
    int half = X.size() / 2;
    float vertical = X[half].x;

    vector<Point> XL(X.begin(), X.begin()+half);
    vector<Point> XR(X.begin()+half, X.end());
    vector<Point> YL, YR;
    for (auto &p : Y) {
      if (p.x < vertical) {
        YL.push_back(p);
      } else { 
        YR.push_back(p);
      }
    }

    float left_d = ClosestPairPoints(XL, YL);
    float right_d = ClosestPairPoints(XR, YR);
    float current = min(left_d, right_d);

    auto judge = [&](float x) {
      return (vertical-current <= x && x <= vertical+current);
    };

    for (int i = 0; i < Y.size(); ++i) if (judge(Y[i].x)) {
      int j = i + 1;
      int cnt = 0;
      while (cnt < 7 && j < Y.size()) if (judge(Y[i].x)) {
        if (distance(Y[i], Y[j]) < current) {
          current = distance(Y[i], Y[j]);
        }
        ++cnt; ++j;
      }
    }

    cout << "left_d: " << left_d << endl; 
    cout << "right_d: " << right_d << endl; 
    cout << "vertical: " << vertical << endl;
    cout << "current: " << current << endl;
    return current;
  }
}

int main() {
  /*
  Vector p1(1, 1);
  Vector p2(0, 0);
  Vector p3(1, 0);
  Vector p4(0, 1);
  cout << cross(p1, p2) << endl;
  cout << direction(p1, p2, p3) << endl;
  cout << SegmentsIntersect(p1, p2, p3, p4) << endl;
  cout << SegmentsIntersect(p1, p3, p2, p4) << endl;
  */
  vector<Point> X { {1,0}, {2,2}, {3,0}, {4,1}, {6,0}, {7,2}, {8,0} };
  vector<Point> Y { {1,0}, {3,0}, {6,0}, {8,0}, {4,1}, {2,2}, {7,2} };
  cout << ClosestPairPoints(X, Y) << endl;
  return 0;
}
