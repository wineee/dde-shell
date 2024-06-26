/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DBUSDOCKADAPTORS_H
#define DBUSDOCKADAPTORS_H

#include <QtDBus/QtDBus>
#include <QDBusArgument>
#include <DGuiApplicationHelper>

/*
 * Adaptor class for interface org.deepin.dde.Dock1
 */
class QGSettings;
class WindowManager;
class PluginsItemInterface;

DGUI_USE_NAMESPACE

struct DockItemInfo
{
    QString name;
    QString displayName;
    QString itemKey;
    QString settingKey;
    QByteArray iconLight;
    QByteArray iconDark;
    bool visible;
};

QDebug operator<<(QDebug argument, const DockItemInfo &info);
QDBusArgument &operator<<(QDBusArgument &arg, const DockItemInfo &info);
const QDBusArgument &operator>>(const QDBusArgument &arg, DockItemInfo &info);

Q_DECLARE_METATYPE(DockItemInfo)

typedef QList<DockItemInfo> DockItemInfos;

Q_DECLARE_METATYPE(DockItemInfos)

void registerPluginInfoMetaType();

class OldDBusDock: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry NOTIFY geometryChanged)
    Q_PROPERTY(bool showInPrimary READ showInPrimary WRITE setShowInPrimary NOTIFY showInPrimaryChanged)

public:
    explicit OldDBusDock(/*WindowManager *parent*/);
    virtual ~OldDBusDock();

public Q_SLOTS:
    void callShow();
    void ReloadPlugins();

    QStringList GetLoadedPlugins();
    DockItemInfos plugins();

    void resizeDock(int offset, bool dragging);

    QString getPluginKey(const QString &pluginName);

    bool getPluginVisible(const QString &pluginName);
    void setPluginVisible(const QString &pluginName, bool visible);
    void setItemOnDock(const QString settingKey, const QString &itemKey, bool visible);

public:
    QRect geometry() const;

    bool showInPrimary() const;
    void setShowInPrimary(bool showInPrimary);

signals:
    void geometryChanged(QRect geometry);
    void showInPrimaryChanged(bool);
    void pluginVisibleChanged(const QString &pluginName, bool visible);

private:
    bool isPluginValid(const QString &name);
    QList<PluginsItemInterface *> localPlugins() const;
    QIcon getSettingIcon(PluginsItemInterface *plugin, QSize &pixmapSize, DGuiApplicationHelper::ColorType colorType) const;

private:
    QGSettings *m_gsettings;
    // WindowManager *m_windowManager;
};

#endif //DBUSDOCKADAPTORS
