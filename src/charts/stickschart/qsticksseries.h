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

#ifndef QSTICKSSERIES_H
#define QSTICKSSERIES_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QXYSeries>
#include <QtGui/QPen>

QT_CHARTS_BEGIN_NAMESPACE

class QSticksSeriesPrivate;

class QT_CHARTS_EXPORT QSticksSeries : public QXYSeries
{
    Q_OBJECT

public:
    explicit QSticksSeries(QObject *parent = nullptr);
    ~QSticksSeries();
    QAbstractSeries::SeriesType type() const;

    qreal markerSize() const { return m_markerSize; };
    void setMarkerSize( qreal size ) {
		m_markerSize = size;
	};

protected:
    QSticksSeries(QSticksSeriesPrivate &d, QObject *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(QSticksSeries)
    Q_DISABLE_COPY(QSticksSeries)
    friend class SticksChartItem;

    qreal m_markerSize;
};

QT_CHARTS_END_NAMESPACE

#endif // QSTICKSSERIES_H
