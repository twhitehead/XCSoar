/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2011 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Gauge/GlueGaugeVario.hpp"
#include "DeviceBlackboard.hpp"
#include "Interface.hpp"

void
GlueGaugeVario::invalidate_blackboard()
{
  /* protecting the flag with a Mutex would be correct, but not
     worth the overhead */

  if (!blackboard_valid)
    return;

  blackboard_valid = false;
  invalidate();
}

void
GlueGaugeVario::on_paint_buffer(Canvas &canvas)
{
  if (!blackboard_valid) {
    const ScopeLock protect(device_blackboard.mutex);
    ReadBlackboardBasic(device_blackboard.Basic());
    ReadBlackboardCalculated(device_blackboard.Calculated());
    ReadSettingsComputer(CommonInterface::SettingsComputer());
  }

  GaugeVario::on_paint_buffer(canvas);

  // wait until finish drawing until we declare we've drawn this frame.
  // This allows frames for the vario gauge to be dropped if the update
  // rate is too high for the load.
  blackboard_valid = true;
}
