/**
  * D-LAN - A decentralized LAN file sharing software.
  * Copyright (C) 2010-2011 Greg Burri <greg.burri@gmail.com>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
  
#ifndef REMOTECONTROLMANAGER_IREMOTECONTROLMANAGER_H
#define REMOTECONTROLMANAGER_IREMOTECONTROLMANAGER_H

namespace RCM
{
   /**
     * When a new remote control manager is created it will automatically listen the port set in
     * 'Protos.Core.Settings.remote_control_port' (see "application/Protos/core_settings.proto").
     * This is the only way to interact with this component.
     */
   class IRemoteControlManager
   {
   public:
      virtual ~IRemoteControlManager() {}
   };
}
#endif