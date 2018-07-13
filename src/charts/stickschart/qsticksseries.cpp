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

#include <QtCharts/QSticksSeries>
#include <private/qsticksseries_p.h>
#include <private/stickschartitem_p.h>
#include <private/chartdataset_p.h>
#include <private/charttheme_p.h>
#include <private/qchart_p.h>
QT_CHARTS_BEGIN_NAMESPACE

/*!
    \class QSticksSeries
    \inmodule QtCharts
    \brief The QSticksSeries class presents data in line charts.

    A line chart is used to show information as a series of data points
    connected by straight lines.

    \image examples_linechart.png

    Creating a basic line chart is simple:
    \code
    QSticksSeries* series = new QSticksSeries();
    series->append(0, 6);
    series->append(2, 4);
    ...
    chart->addSeries(series);
    \endcode
*/
/*!
    \qmltype LineSeries
    \instantiates QSticksSeries
    \inqmlmodule QtCharts

    \inherits XYSeries

    \brief Presents data in line charts.

    A line chart is used to show information as a series of data points
    connected by straight lines.

    The following QML example shows how to create a simple line chart:
    \snippet qmlchart/qml/qmlchart/View2.qml 1
    \beginfloatleft
    \image examples_qmlchart2.png
    \endfloat
    \clearfloat
*/

/*!
    \qmlproperty int LineSeries::count
    The number of data points in the series.
*/

/*!
    \qmlproperty real LineSeries::width
    The width of the line. By default, the width is 2.0.
*/

/*!
    \qmlproperty Qt::PenStyle LineSeries::style
    Controls the style of the line. Set to one of \l{Qt::NoPen}{Qt.NoPen},
    \l{Qt::SolidLine}{Qt.SolidLine}, \l{Qt::DashLine}{Qt.DashLine}, \l{Qt::DotLine}{Qt.DotLine},
    \l{Qt::DashDotLine}{Qt.DashDotLine}, or \l{Qt::DashDotDotLine}{Qt.DashDotDotLine}.
    Using \l{Qt::CustomDashLine}{Qt.CustomDashLine} is not supported in the QML API.
    By default, the style is Qt.SolidLine.

    \sa Qt::PenStyle
*/

/*!
    \qmlproperty Qt::PenCapStyle LineSeries::capStyle
    Controls the cap style of the line. Set to one of \l{Qt::FlatCap}{Qt.FlatCap},
    \l{Qt::SquareCap}{Qt.SquareCap} or \l{Qt::RoundCap}{Qt.RoundCap}. By
    default the cap style is Qt.SquareCap.

    \sa Qt::PenCapStyle
*/

/*!
    Constructs an empty series object that is a child of \a parent.
    When the series object is added to a QChartView or QChart instance, the ownership
    is transferred.
*/
QSticksSeries::QSticksSeries(QObject *parent)
    : QXYSeries(*new QSticksSeriesPrivate(this), parent)
{
	m_markerSize = 5.0;
}

/*!
    \internal
*/
QSticksSeries::QSticksSeries(QSticksSeriesPrivate &d, QObject *parent)
    : QXYSeries(d, parent)
{
	m_markerSize = 5.0;
}
/*!
    Destroys the object. Series added to QChartView or QChart instances are owned by the
    instances and deleted when the instances are destroyed.
*/
QSticksSeries::~QSticksSeries()
{
    Q_D(QSticksSeries);
    if (d->m_chart)
        d->m_chart->removeSeries(this);
}

/*!
    \reimp
*/
QAbstractSeries::SeriesType QSticksSeries::type() const
{
    return QAbstractSeries::SeriesTypeLine;
}

/*
QDebug operator<< (QDebug debug, const QSticksSeries series)
{
    Q_ASSERT(series.d_func()->m_x.size() == series.d_func()->m_y.size());
    int size = series.d_func()->m_x.size();
    for (int i=0; i<size; i++) {
        debug.nospace() << "(" << series.d_func()->m_x.at(i) << ','<< series.d_func()->m_y.at(i) << ") ";
    }
    return debug.space();
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QSticksSeriesPrivate::QSticksSeriesPrivate(QSticksSeries *q)
    : QXYSeriesPrivate(q)
{

}

void QSticksSeriesPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(QSticksSeries);
    SticksChartItem *line = new SticksChartItem(q,parent);
    m_item.reset(line);
    QAbstractSeriesPrivate::initializeGraphics(parent);
}

void QSticksSeriesPrivate::initializeTheme(int index, ChartTheme* theme, bool forced)
{
    Q_Q(QSticksSeries);
    const QList<QColor> colors = theme->seriesColors();

    if (forced || QChartPrivate::defaultPen() == m_pen) {
        QPen pen;
        pen.setColor(colors.at(index % colors.size()));
        pen.setWidthF(2);
        q->setPen(pen);
    }

    if (forced || QChartPrivate::defaultPen().color() == m_pointLabelsColor) {
        QColor color = theme->labelBrush().color();
        q->setPointLabelsColor(color);
    }
}

#include "moc_qsticksseries.cpp"

QT_CHARTS_END_NAMESPACE
