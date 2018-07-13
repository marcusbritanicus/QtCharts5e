/*
	*
	* Copyright 2018 Britanicus <marcusbritanicus@gmail.com>
	*

	*
	* This program is free software; you can redistribute it and/or modify
	* it under the terms of the GNU General Public License as published by
	* the Free Software Foundation; either version 2 of the License, or
	* (at your option) any later version.
	*

	*
	* This program is distributed in the hope that it will be useful,
	* but WITHOUT ANY WARRANTY; without even the implied warranty of
	* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	* GNU General Public License for more details.
	*

	*
	* You should have received a copy of the GNU General Public License
	* along with this program; if not, write to the Free Software
	* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
	* MA 02110-1301, USA.
	*
*/

//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QSTICKSSERIES_P_H
#define QSTICKSSERIES_P_H

#include <private/qxyseries_p.h>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE


class QT_CHARTS_PRIVATE_EXPORT QSticksSeriesPrivate: public QXYSeriesPrivate
{
public:
    QSticksSeriesPrivate(QSticksSeries *q);
    void initializeGraphics(QGraphicsItem* parent);
    void initializeTheme(int index, ChartTheme* theme, bool forced = false);

private:
    Q_DECLARE_PUBLIC(QSticksSeries);
};

QT_CHARTS_END_NAMESPACE

#endif
