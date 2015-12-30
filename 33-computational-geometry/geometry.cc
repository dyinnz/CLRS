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

int main() {
  Vector p1(1, 1);
  Vector p2(0, 0);
  Vector p3(1, 0);
  Vector p4(0, 1);
  cout << cross(p1, p2) << endl;
  cout << direction(p1, p2, p3) << endl;
  cout << SegmentsIntersect(p1, p2, p3, p4) << endl;
  cout << SegmentsIntersect(p1, p3, p2, p4) << endl;
  return 0;
}
