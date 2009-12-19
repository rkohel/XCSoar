#ifndef COMMON_STATS_HPP
#define COMMON_STATS_HPP

#include "Math/fixed.hpp"
#include "Navigation/Geometry/GeoVector.hpp"
#include <set>

typedef std::set<unsigned> WaypointIdSet;

class Waypoint;

class CommonStats 
{
public:
/** 
 * Constructor, initialises all to zero
 * 
 */
  CommonStats();
  
  bool landable_reachable; /**< Whether the task found landable reachable waypoints (aliases abort) */
  bool task_finished; /**< Whether the task is finished (aliases ordered task) */
  fixed aat_time_remaining; /**< Time (s) until assigned minimum time is achieved */
  fixed aat_remaining_speed; /**< Speed to achieve remaining task in minimum assigned time (m/s), negative if already beyond minimum time */ 
  fixed task_time_remaining; /**< Time (s) remaining for ordered task */
  fixed task_time_elapsed; /**< Time (s) elapsed for ordered task */
  GeoVector vector_home; /**< Vector to home waypoint */
  bool mode_abort; /**< Whether task is abort mode */
  bool mode_ordered; /**< Whether task is ordered mode */
  bool mode_goto; /**< Whether task is goto mode */
  bool ordered_valid; /**< Whether ordered task is valid */

  WaypointIdSet waypoints_in_task;

/** 
 * Clears the set of waypoints listed as in the task
 * 
 */
  void clear_waypoints_in_task();

/** 
 * Mark a waypoint as being in the task
 * 
 * @param wp Waypoint to add
 */
  void append_waypoint_in_task(const Waypoint& wp);

/** 
 * Test whether a waypoint is in the task
 * 
 * @param wp Waypoint to test
 * @return True if in task
 */
  bool is_waypoint_in_task(const Waypoint& wp) const;

/** 
 * Reset the task as if never flown
 * 
 */
  void reset();

};

#endif
