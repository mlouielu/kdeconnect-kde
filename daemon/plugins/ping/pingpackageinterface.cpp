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

#include "pingpackageinterface.h"

#include <KDebug>
#include <kicon.h>

K_PLUGIN_FACTORY( KdeConnectPluginFactory, registerPlugin< PingPackageInterface >(); )
K_EXPORT_PLUGIN( KdeConnectPluginFactory("kdeconnect_ping", "kdeconnect_ping") )

PingPackageInterface::PingPackageInterface(QObject* parent, const QVariantList& args)
    : PackageInterface(parent)
{
    Q_UNUSED(args);
}

bool PingPackageInterface::receivePackage(const Device& device, const NetworkPackage& np)
{

    if (np.type() != PACKAGE_TYPE_PING) return false;

    KNotification* notification = new KNotification("pingReceived"); //KNotification::Persistent
    notification->setPixmap(KIcon("dialog-ok").pixmap(48, 48));
    notification->setComponentData(KComponentData("kdeconnect", "kdeconnect"));
    notification->setTitle("Ping!");
    notification->setText(device.name());
    notification->sendEvent();

    return true;

}