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

#include <private/stickschartitem_p.h>
#include <QtCharts/QSticksSeries>
#include <private/qsticksseries_p.h>
#include <private/chartpresenter_p.h>
#include <private/polardomain_p.h>
#include <private/chartthememanager_p.h>
#include <private/charttheme_p.h>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneMouseEvent>

QT_CHARTS_BEGIN_NAMESPACE

SticksChartItem::SticksChartItem(QSticksSeries *series, QGraphicsItem *item)
    : XYChart(series,item),
      m_series(series),
      m_pointsVisible(false),
      m_chartType(QChart::ChartTypeUndefined),
      m_pointLabelsVisible(false),
      m_pointLabelsFormat(series->pointLabelsFormat()),
      m_pointLabelsFont(series->pointLabelsFont()),
      m_pointLabelsColor(series->pointLabelsColor()),
      m_pointLabelsClipping(true),
      m_mousePressed(false)
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(ChartPresenter::LineChartZValue);
    QObject::connect(series->d_func(), SIGNAL(updated()), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(visibleChanged()), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(opacityChanged()), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsFormatChanged(QString)), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsVisibilityChanged(bool)), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsFontChanged(QFont)), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsColorChanged(QColor)), this, SLOT(handleUpdated()));
    QObject::connect(series, SIGNAL(pointLabelsClippingChanged(bool)), this, SLOT(handleUpdated()));

    handleUpdated();
}

QRectF SticksChartItem::boundingRect() const
{
    return m_rect;
}

QPainterPath SticksChartItem::shape() const
{
    return m_shapePath;
}

void SticksChartItem::updateGeometry()
{
    if (m_series->useOpenGL()) {
        if (!m_rect.isEmpty()) {
            prepareGeometryChange();
            // Changed signal seems to trigger even with empty region
            m_rect = QRectF();
        }
        update();
        return;
    }

    // Store the points to a local variable so that the old sticks gets properly cleared
    // when animation starts.
    m_sticksPoints = geometryPoints();
    const QVector<QPointF> &points = m_sticksPoints;

    if (points.size() == 0) {
        prepareGeometryChange();
        m_fullPath = QPainterPath();
        m_sticksPath = QPainterPath();
        m_rect = QRect();
        return;
    }

    QPainterPath sticksPath;
    QPainterPath fullPath;
    // Use worst case scenario to determine required margin.
    qreal margin = m_sticksPen.width() * 1.42;
    m_markerSize = m_series->markerSize();

    // Area series use component sticks series that aren't necessarily added to the chart themselves,
    // so check if chart type is forced before trying to obtain it from the chart.
    QChart::ChartType chartType = m_chartType;
    if (chartType == QChart::ChartTypeUndefined)
        chartType = m_series->chart()->chartType();

    // For polar charts, we need special handling for angular (horizontal)
    // points that are off-grid.
    if (chartType == QChart::ChartTypePolar) {

        return;
    }

	else { // not polar

		bool okay;
		qreal geomy = domain()->calculateGeometryPoint( QPoint( 0, 0 ), okay ).y();

		for (int i = 0; i < points.size(); i++) {
			sticksPath.moveTo(points.at(i));
			if ( m_pointsVisible )
				sticksPath.addEllipse( points.at(i), m_markerSize, m_markerSize );

			QPointF pt = points.at( i );
			pt.setY( geomy );
			sticksPath.lineTo( pt );
		}

        fullPath = sticksPath;
    }

    QPainterPathStroker stroker;
    // QPainter::drawLine does not respect join styles, for example BevelJoin becomes MiterJoin.
    // This is why we are prepared for the "worst case" scenario, i.e. use always MiterJoin and
    // multiply line width with square root of two when defining shape and bounding rectangle.
    stroker.setWidth(margin);
    stroker.setJoinStyle( Qt::RoundJoin );
    stroker.setCapStyle( Qt::RoundCap );
    stroker.setMiterLimit( m_sticksPen.miterLimit() );

    QPainterPath checkShapePath = stroker.createStroke(fullPath);

    // Only zoom in if the bounding rects of the paths fit inside int limits. QWidget::update() uses
    // a region that has to be compatible with QRect.
    if (checkShapePath.boundingRect().height() <= INT_MAX
            && checkShapePath.boundingRect().width() <= INT_MAX
            && sticksPath.boundingRect().height() <= INT_MAX
            && sticksPath.boundingRect().width() <= INT_MAX
            && fullPath.boundingRect().height() <= INT_MAX
            && fullPath.boundingRect().width() <= INT_MAX) {
        prepareGeometryChange();

        m_sticksPath = sticksPath;
        m_fullPath = fullPath;
        m_shapePath = checkShapePath;

        m_rect = m_shapePath.boundingRect();
    }

    else {

        update();
    }
};

void SticksChartItem::handleUpdated()
{
    // If points visibility has changed, a geometry update is needed.
    // Also, if pen changes when points are visible, geometry update is needed.
    bool doGeometryUpdate = ( m_pointsVisible != m_series->pointsVisible() );
    doGeometryUpdate = doGeometryUpdate || ( m_series->pointsVisible() && ( m_sticksPen != m_series->pen() ) );

    bool visibleChanged = m_series->isVisible() != isVisible();
    setVisible(m_series->isVisible());
    setOpacity(m_series->opacity());
    m_pointsVisible = m_series->pointsVisible();
    m_sticksPen = m_series->pen();
    m_markerSize = m_series->markerSize();
    m_pointLabelsFormat = m_series->pointLabelsFormat();
    m_pointLabelsVisible = m_series->pointLabelsVisible();
    m_pointLabelsFont = m_series->pointLabelsFont();
    m_pointLabelsColor = m_series->pointLabelsColor();
    m_pointLabelsClipping = m_series->pointLabelsClipping();

    if (doGeometryUpdate)
        updateGeometry();

    else if (m_series->useOpenGL() && visibleChanged)
        refreshGlChart();

    update();
}

void SticksChartItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)

    if (m_series->useOpenGL())
        return;

    QRectF clipRect = QRectF(QPointF(0, 0), domain()->size());
    // Adjust clip rect half a pixel in required dimensions to make it include sticks along the
    // plot area edges, but never increase clip so much that any portion of the line is draw beyond
    // the plot area.
    const qreal x1 = pos().x() - int(pos().x());
    const qreal y1 = pos().y() - int(pos().y());
    const qreal x2 = (clipRect.width() + 0.5) - int(clipRect.width() + 0.5);
    const qreal y2 = (clipRect.height() + 0.5) - int(clipRect.height() + 0.5);
    clipRect.adjust(-x1, -y1, qMax(x1, x2), qMax(y1, y2));

    painter->save();
    painter->setPen(m_sticksPen);

    if (m_series->chart()->chartType() == QChart::ChartTypePolar) {
        qreal halfWidth = domain()->size().width() / 2.0;
        QRectF clipRectLeft = QRectF(0, 0, halfWidth, domain()->size().height());
        QRectF clipRectRight = QRectF(halfWidth, 0, halfWidth, domain()->size().height());
        QRegion fullPolarClipRegion(clipRect.toRect(), QRegion::Ellipse);
        QRegion clipRegionLeft(fullPolarClipRegion.intersected(clipRectLeft.toRect()));
        QRegion clipRegionRight(fullPolarClipRegion.intersected(clipRectRight.toRect()));
        painter->setClipRegion(clipRegionLeft);
        painter->drawPath(m_sticksPathPolarLeft);
        painter->setClipRegion(clipRegionRight);
        painter->drawPath(m_sticksPathPolarRight);
        painter->setClipRegion(fullPolarClipRegion);
    } else {
        painter->setClipRect(clipRect);
    }

    if (m_pointsVisible) {
        painter->setBrush(m_sticksPen.color());
        painter->drawPath(m_sticksPath);
    } else {
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->drawPath(m_sticksPath);
    }

    if (m_pointLabelsVisible) {
        if (m_pointLabelsClipping)
            painter->setClipping(true);
        else
            painter->setClipping(false);
        m_series->d_func()->drawSeriesPointLabels(painter, m_sticksPoints, m_sticksPen.width() / 2);
    }

    painter->restore();

}

void SticksChartItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit XYChart::pressed(domain()->calculateDomainPoint(event->pos()));
    m_lastMousePos = event->pos();
    m_mousePressed = true;
    QGraphicsItem::mousePressEvent(event);
}

void SticksChartItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit XYChart::hovered(domain()->calculateDomainPoint(event->pos()), true);
//    event->accept();
    QGraphicsItem::hoverEnterEvent(event);
}

void SticksChartItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit XYChart::hovered(domain()->calculateDomainPoint(event->pos()), false);
//    event->accept();
    QGraphicsItem::hoverEnterEvent(event);
}

void SticksChartItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit XYChart::released(domain()->calculateDomainPoint(m_lastMousePos));
    if (m_mousePressed)
        emit XYChart::clicked(domain()->calculateDomainPoint(m_lastMousePos));
    m_mousePressed = false;
    QGraphicsItem::mouseReleaseEvent(event);
}

void SticksChartItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit XYChart::doubleClicked(domain()->calculateDomainPoint(m_lastMousePos));
    QGraphicsItem::mouseDoubleClickEvent(event);
}

#include "moc_stickschartitem_p.cpp"

QT_CHARTS_END_NAMESPACE
