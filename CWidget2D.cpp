#include "CWidget2D.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>

CWidget2D::StEditTool CWidget2D::m_stTool;

CWidget2D::CWidget2D(SPIMG sp, QWidget *parent)
    : QLabel{parent}
{
    setAlignment(Qt::AlignTop|Qt::AlignLeft);
    SetImg(sp);
}

SPMSG CWidget2D::SetImg(SPIMG sp)
{
    m_spIMG=sp;
    resizeEvent(NULL);
    return _ok;
}
void CWidget2D::resizeEvent(QResizeEvent *ev)
{
    if (m_spIMG){
        m_stView.SetScale(m_spIMG->size(), size());
    }
    if (ev) { ev->accept(); }
    update();
}
void CWidget2D::paintEvent(QPaintEvent *e)
{
    if (!m_spIMG){ QLabel::paintEvent(e); return;}
    auto vw = m_spIMG->copy();
    QPainter pt(&vw);
    if (Tool().enable){
        pt.setPen(Qt::gray);
        auto pos = mapFromGlobal(cursor().pos());
        pos = m_stView.Proj(pos).toPoint();
        pt.drawRect(Tool().Rect(pos));
    }
    pt.end();
    //vw = vw.copy(m_stView.View(rect()).toRect());
    vw = vw.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(QPixmap::fromImage(vw));
    QLabel::paintEvent(e);
}

void CWidget2D::ApplyBrush(const StEditTool& st)
{
    if (!m_spIMG){ return; }
    const QRect rc = st.Rect();
    if (!rc.isValid()){ return; }
    const QColor clr = st.Color();
    if (clr == Qt::transparent){ return; }
    for (auto x=rc.left(); x<rc.right(); x++){
        for (auto y=rc.top(); y<rc.bottom(); y++){
            m_spIMG->setPixel(QPoint{x,y}, clr.rgb());
        }
    }
    return;
}

void CWidget2D::mousePressEvent(QMouseEvent *ev)
{
    if (!Tool().enable){ return; }
    if(ev->button()==Qt::LeftButton){
        Tool().pen = SPXY(m_stView.Proj(ev->pos()).toPoint());
    } else if(ev->button()==Qt::RightButton){
        Tool().rub = SPXY(m_stView.Proj(ev->pos()).toPoint());
    }
    ApplyBrush(Tool());
    ev->accept();
    update();
}
void CWidget2D::mouseMoveEvent(QMouseEvent *ev)
{
    if (!Tool().enable){ return; }
    if(Tool().pen){
        Tool().pen = SPXY(m_stView.Proj(ev->pos()).toPoint());
    } else if(Tool().rub){
        Tool().rub = SPXY(m_stView.Proj(ev->pos()).toPoint());
    }
    ApplyBrush(Tool());
    ev->accept();
    update();
}
void CWidget2D::mouseReleaseEvent(QMouseEvent *ev)
{
    Tool().Reset();
}

void CWidget2D::wheelEvent(QWheelEvent *ev)
{
    // if ((ev->angleDelta().y()<0)){
    //     m_stView.scale = qMin(m_stView.scale*1.25, 8.0f);
    // }else if ((ev->angleDelta().y()>0)){
    //     m_stView.scale = qMax(m_stView.scale*0.8, 0.1f);
    // }
    ev->accept();
    update();
}
