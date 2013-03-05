#include "plcgraphicsedgeitem.h"
#include <QPainter>
#include <QDebug>
#include "../EdgeConfiguration/edgeconfiguration.h"

PLCGraphicsEdgeItem::PLCGraphicsEdgeItem(PLCGraphicsNodeItem *fromNode, QPointF toPoint, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    Q_ASSERT(fromNode != 0);


    this->edge = new EdgeModel(fromNode->getNodeModel()->getName(), "Disconnected");
    setupLabels();
    this->fromNode = fromNode;
    this->toNode = 0;

    this->setZValue(10);

    setToPoint(toPoint);
    setFlags(QGraphicsItem::ItemIsSelectable);


}

PLCGraphicsEdgeItem::PLCGraphicsEdgeItem(EdgeModel* edge, PLCGraphicsNodeItem* fromNode, PLCGraphicsNodeItem* toNode){
    this->edge = edge;
    setupLabels();
    this->fromNode = fromNode;
    setToNode(toNode);

    setZValue(-1);
    setFlags(QGraphicsItem::ItemIsSelectable);

}

PLCGraphicsEdgeItem::~PLCGraphicsEdgeItem(){
    delete(this->edge);
}

void PLCGraphicsEdgeItem::setupLabels(){
    this->lengthLabel = new QGraphicsTextItem(QString::number(edge->getLength()) + "m", this);

    this->lengthLabel->setZValue(-1);
    this->lengthLabel->setFlag(QGraphicsItem::ItemStacksBehindParent);

    QFont labelFont("Ariel", 12);
    labelFont.setHintingPreference(QFont::PreferNoHinting);
    labelFont.setStyleStrategy(QFont::PreferAntialias);
    lengthLabel->setVisible(true);
    lengthLabel->setFont(labelFont);

    this->typeLabel = new QGraphicsTextItem(edge->getCableType(), this);
    this->typeLabel->setZValue(-1);
    this->lengthLabel->setFlag(QGraphicsItem::ItemStacksBehindParent);

    typeLabel->setVisible(true);
    typeLabel->setFont(labelFont);

    lengthLabel->setZValue(-5);
    typeLabel->setZValue(-5);


}

QRectF PLCGraphicsEdgeItem::boundingRect() const {
    if(toNode != 0){
        QRectF br(fromNode->pos(), toNode->pos());
        br = br.normalized();

        if(br.width() < 20){
            br.adjust(-10, 0, 10, 0);
        }

        if(br.height() < 20){
            br.adjust(0, -10, 0, 10);
        }

        return br;
    }

    QRectF br(fromNode->pos(), toPoint);

    br = br.normalized();

    if(br.width() < 20){
        br.adjust(-10, 0, 10, 0);
    }

    if(br.height() < 20){
        br.adjust(0, -10, 0, 10);
    }

    return br;
}

void PLCGraphicsEdgeItem::setToPoint(QPointF point){
    this->toPoint = point;

    updateLabelAngle();

    prepareGeometryChange();
    update();

}


void PLCGraphicsEdgeItem::updateLabelAngle(){
    QPointF endPoint;

    if(toNode == 0){
        endPoint = toPoint;
    }
    else{
        endPoint = toNode->pos();
    }

    lengthLabel->setPlainText(QString::number(edge->getLength()) + "m");
    typeLabel->setPlainText(edge->getName());

    lengthLabel->setVisible(getEdgeModel()->isCableModel());

//    if(edge->isCableModel()){
//        typeLabel->setPlainText(edge->getCableType());
//    }
//    else{
//        typeLabel->setPlainText("Two-Port Network");
//    }


    QPointF lineVector = endPoint - fromNode->pos();
    double lineLength = sqrt((lineVector.x() * lineVector.x()) +
                      (lineVector.y() * lineVector.y()));
    QPointF lineUnit = lineVector/lineLength;

    QPointF perpDir = QPointF(-lineUnit.y(), lineUnit.x());

    if(lineUnit.y() >= 0 && perpDir.y() <= 0){
        //perpDir *= -1.0;
    }

    double labelAngle = atan2(lineVector.y(), lineVector.x());
    labelAngle *= abs((90.0/asin(-1)));

    lengthLabel->setRotation(labelAngle);
    typeLabel->setRotation(labelAngle);

    QPointF centerPos = fromNode->pos() + (lineVector/2.0);

    QPointF lengthLabelCenter = lengthLabel->mapToScene(lengthLabel->boundingRect().center());
    QPointF typeLabelCenter = typeLabel->mapToScene(typeLabel->boundingRect().center());

    lengthLabel->setPos(lengthLabel->pos() + (centerPos - lengthLabelCenter));
    lengthLabel->translate(perpDir.x() * -12.0, perpDir.y() * -12.0);

    typeLabel->setPos(typeLabel->pos() + (centerPos - typeLabelCenter));
    typeLabel->translate(perpDir.x() * 12.0, perpDir.y() * 12.0);


    lengthLabel->update();
    typeLabel->update();

}

void PLCGraphicsEdgeItem::setToNode(PLCGraphicsNodeItem *node){
    this->toNode = node;
    this->edge->setToNode(node->getNodeModel()->getName());

    prepareGeometryChange();
    updateLabelAngle();
    update();
}

void PLCGraphicsEdgeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event);
    EdgeConfiguration * edgeConfig = new EdgeConfiguration(getEdgeModel());
    edgeConfig->exec();
    delete edgeConfig;
}

void PLCGraphicsEdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //TODO: Draw an indicator of the directionality of this edge (especially for twoports)
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPointF endPoint;

    if(toNode == 0){
        endPoint = toPoint;
    }
    else {
        endPoint = toNode->pos();
    }

    updateLabelAngle();
    prepareGeometryChange();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing, true);

    QPen linePen;

    if(this->isSelected()){
        linePen.setColor(Qt::magenta);
    }
    else {
        linePen.setColor(Qt::black);
    }

    if(!this->edge->isCableModel()){
        linePen.setStyle(Qt::DashDotLine);
    }

    painter->setPen(linePen);
    painter->drawLine(fromNode->pos(), endPoint);
}
