/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2011  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#ifndef GUIPORT_H
#define GUIPORT_H

#include "dmcompilersettings.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QString>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QColor>
#include <QObject>
#include "dmcompilersettings.h"

namespace DM {
//class Port;
class Module;
}


#define PORT_DRAW_SIZE 14
#define PORT_DRAW_SELECTED_SIZE 16

class ModelNode;
class GUILink;
class GUISimulation;


enum PortType
{
	INPORT = 0,
	OUTPORT,
};

class DM_HELPER_DLL_EXPORT PortNode : public QGraphicsItem
{
public:

private:
    QVector<GUILink * > linkNodes;

    QColor color;
    ModelNode * modelNode;
    //GUILink * tmp_link;
	
	GUILink* unstableLink;


    QString PortName;
   // int PortType;
    bool isHover;
    bool LinkMode;
    PortNode * hoverElement;
    float l;
    float h;
    float x1;
    QGraphicsSimpleTextItem portname_graphics;
    GUISimulation * simulation;
	 
	PortType portType;
public:
    PortNode(QString portName, ModelNode * parentModelNode, PortType type/*, DM::Port * p*/);
    ~PortNode();
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //void setLink(GUILink * l);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QRectF boundingRect() const;
    QPointF getCenterPos();
    //int getPortType();
    QString getPortName(){return this->PortName;}
    bool isLinked();
    void setHover(bool b)
	{
		this->isHover = b;
		this->prepareGeometryChange();
	}
    //void refreshLinks();
    //DM::Port * getVIBePort();
    void removeLink(GUILink * l);
	void addLink(GUILink* l);
    //void updatePort(DM::Port * p);
    GUISimulation * getSimulation() {return this->simulation;}
    void  setSimulation(GUISimulation *s) {this->simulation = s;}
	DM::Module* getModule();
	PortType getType(){return portType;};
};

#endif // GUIPORT_H
