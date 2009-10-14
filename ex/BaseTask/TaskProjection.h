/* Generated by Together */

#ifndef TASKPROJECTION_H
#define TASKPROJECTION_H

#include "Navigation/Waypoint.hpp"

class TaskProjection {
public:
  TaskProjection(const GEOPOINT &ref)
  {
    reset(ref);
  }
  TaskProjection()
  {
    GEOPOINT zero;
    reset(zero);
  }
  
  void reset(const GEOPOINT &ref) {
    location_min = ref;
    location_max = ref;
    location_mid = ref;
  }
  void scan_location(const GEOPOINT &ref);
  FLAT_GEOPOINT project(const GEOPOINT& tp) const;
  void report();
private:
  GEOPOINT location_min;
  GEOPOINT location_max;
  GEOPOINT location_mid;
};

#endif //TASKPROJECTION_H
