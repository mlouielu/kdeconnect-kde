/**
 * Copyright 2019 Aleix Pol Gonzalez <aleixpol@kde.org>
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "presenterplugin.h"

#include <KLocalizedString>
#include <KPluginFactory>

#include <QDebug>
#include <QDBusConnection>
#include <QLoggingCategory>
#include <QQuickView>
#include <QQmlError>
#include <QQuickItem>
#include <QtGlobal>

#include <core/device.h>
#include <core/daemon.h>
#include <QScreen>
#include <QRect>
#include <QGuiApplication>
#include <QProcess>
#include <QCursor>

K_PLUGIN_CLASS_WITH_JSON(PresenterPlugin, "kdeconnect_presenter.json")

Q_LOGGING_CATEGORY(KDECONNECT_PLUGIN_PRESENT, "kdeconnect.plugin.presenter")

PresenterPlugin::PresenterPlugin(QObject* parent, const QVariantList& args)
    : KdeConnectPlugin(parent, args)
    , m_timer(new QTimer(this))
{
    m_timer->setInterval(100);
    m_timer->setSingleShot(true);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    this->height = screenGeometry.height();
    this->width = screenGeometry.width();
    this->isProjecteurEnable = 0;
}

PresenterPlugin::~PresenterPlugin()
{
}

void PresenterPlugin::enableProjecteur(void)
{
    this->isProjecteurEnable = 1;
    QProcess::execute(QString::fromLocal8Bit("projecteur -c spot=on"));
}

void PresenterPlugin::disableProjecteur(void)
{
    this->isProjecteurEnable = 0;
    QProcess::execute(QString::fromLocal8Bit("projecteur -c spot=off"));
}

bool PresenterPlugin::receivePacket(const NetworkPacket& np)
{
    if (np.get<bool>(QStringLiteral("stop"), false)) {
        return true;
    }

    if (!this->isProjecteurEnable) {
        m_xPos = 0.5f;
        m_yPos = 0.5f;
        connect(m_timer, &QTimer::timeout,
                this, QOverload<>::of(&PresenterPlugin::disableProjecteur));
        PresenterPlugin::enableProjecteur();
    }

    float ratio = this->width / this->height;
    m_xPos += np.get<float>(QStringLiteral("dx"));
    m_yPos += np.get<float>(QStringLiteral("dy")) * ratio;
    m_xPos = qBound(0.f, m_xPos, 1.f);
    m_yPos = qBound(0.f, m_yPos, 1.f);

    m_timer->start();
    m_cursor.setPos(m_xPos * this->width, m_yPos * this->height);

    return true;
}

#include "presenterplugin.moc"
