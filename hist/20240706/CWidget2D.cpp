#include "CWidget2D.h"
#include <QPaintEvent>
#include <QPainter>

CWidget2D::CWidget2D(QWidget *parent)
    : QLabel{parent}
{}


void CWidget2D::paintEvent(QPaintEvent *e)
{
    if (!m_spIMG){ QLabel::paintEvent(e); return;}
    QPainter pt(this);
    pt.setPen(Qt::red);
    pt.drawRect(rect().adjusted(0,0,-1,-1));
    pt.drawImage(0,0, m_spIMG->scaled(size(), Qt::KeepAspectRatio));
    pt.end();
    QLabel::paintEvent(e);
}
