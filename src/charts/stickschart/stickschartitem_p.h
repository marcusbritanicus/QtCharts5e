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

#ifndef STICKSCHARTITEM_H
#define STICKSCHARTITEM_H

#include <QtCharts/QChartGlobal>
#include <private/xychart_p.h>
#include <QtCharts/QChart>
#include <QtGui/QPen>
#include <QtCharts/private/qchartglobal_p.h>

QT_CHARTS_BEGIN_NAMESPACE

class QSticksSeries;
class ChartPresenter;

class QT_CHARTS_PRIVATE_EXPORT SticksChartItem :  public XYChart
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit SticksChartItem(QSticksSeries *series, QGraphicsItem *item = 0);
    ~SticksChartItem() {};

    //from QGraphicsItem
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    QPainterPath path() const { return m_fullPath; }

public Q_SLOTS:
    void handleUpdated();

protected:
    void updateGeometry();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void suppressPoints() { m_pointsVisible = false; }
    void forceChartType(QChart::ChartType chartType) { m_chartType = chartType; }

private:
    QSticksSeries *m_series;
    QPainterPath m_sticksPath;
    QPainterPath m_sticksPathPolarRight;
    QPainterPath m_sticksPathPolarLeft;
    QPainterPath m_fullPath;
    QPainterPath m_shapePath;

    QVector<QPointF> m_sticksPoints;
    QRectF m_rect;
    QPen m_sticksPen;
    bool m_pointsVisible;
    QChart::ChartType m_chartType;
    qreal m_markerSize;

    bool m_pointLabelsVisible;
    QString m_pointLabelsFormat;
    QFont m_pointLabelsFont;
    QColor m_pointLabelsColor;
    bool m_pointLabelsClipping;

    QPointF m_lastMousePos;
    bool m_mousePressed;
};

QT_CHARTS_END_NAMESPACE

#endif
