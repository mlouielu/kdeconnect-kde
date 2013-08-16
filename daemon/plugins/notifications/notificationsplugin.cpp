/**
 * Copyright 2013 Albert Vaca <albertvaka@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "notificationsplugin.h"

#include <QDebug>
#include <kicon.h>

K_PLUGIN_FACTORY( KdeConnectPluginFactory, registerPlugin< NotificationsPlugin >(); )
K_EXPORT_PLUGIN( KdeConnectPluginFactory("kdeconnect_notifications", "kdeconnect_notifications") )

NotificationsPlugin::NotificationsPlugin(QObject* parent, const QVariantList& args)
    : KdeConnectPlugin(parent, args)
{
    trayIcon = new KStatusNotifierItem(parent);
    trayIcon->setIconByName("smartphone");
    trayIcon->setTitle(device()->name());
}

NotificationsPlugin::~NotificationsPlugin()
{
    delete trayIcon;
}

bool NotificationsPlugin::receivePackage(const NetworkPackage& np)
{
    if (np.type() != PACKAGE_TYPE_NOTIFICATION) return false;

    return true;
}
