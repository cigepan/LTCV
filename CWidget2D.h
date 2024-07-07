#ifndef CWIDGET2D_H
#define CWIDGET2D_H
#include <CMyDef.h>

class CWidget2D : public QLabel
{
    Q_OBJECT
    SPIMG m_spIMG{NULL};

    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
//     virtual void mouseDoubleClickEvent(QMouseEvent *ev);
     virtual void mouseMoveEvent(QMouseEvent *ev);
// #if QT_CONFIG(wheelevent)
     virtual void wheelEvent(QWheelEvent *ev);
// #endif
//     virtual void keyPressEvent(QKeyEvent *ev);
//     virtual void keyReleaseEvent(QKeyEvent *ev);
//     virtual void focusInEvent(QFocusEvent *ev);
//     virtual void focusOutEvent(QFocusEvent *ev);
//     virtual void enterEvent(QEnterEvent *ev);
//     virtual void leaveEvent(QEvent *ev);
     virtual void paintEvent(QPaintEvent *ev);
//     virtual void moveEvent(QMoveEvent *ev);
    virtual void resizeEvent(QResizeEvent *ev);
//     virtual void closeEvent(QCloseEvent *ev);
    struct {
        QVector2D move{0,0};
        float scale{1.0};
        float SetScale(QSizeF o, QSizeF t)
        {
            return scale = qMax(o.width()/t.width(), o.height()/t.height());
        }
        QPointF Proj(const QPointF& pos){ //ui->img
            return pos*scale + move.toPointF();
        }
        QPointF UnProj(const QPointF& pos){ //img->ui
            return (pos-move.toPointF())/scale;
        }
        QRectF View(const QRectF& rc) const {
            auto sz = rc.size()/scale;
            return QRectF(move.x()-sz.width()/2,move.y()-sz.height()/2, sz.width(), sz.height());
        }
    } m_stView;
    struct StEditTool{
        bool enable{false};
        int size{5};
        SPPOS pen{NULL}; //钢笔
        SPPOS rub{NULL}; //橡皮
        void Reset() {
            pen = rub = NULL;
        }
        QRect Rect(const QPoint& pos) const
        {
            return QRect(pos-QPoint{size/2,size/2}, QSize{size, size}).normalized();
        }
        QRect Rect() const {
            if (pen){ return Rect(*pen); }
            if (rub){ return Rect(*rub); }
            return QRect();
        }
        QColor Color() const {
            if (pen){ return Qt::white; }
            if (rub){ return Qt::black; }
            return Qt::transparent;
        }
    };
    static  StEditTool m_stTool;
    void ApplyBrush(const StEditTool& st);
public:
    explicit CWidget2D(SPIMG sp=NULL, QWidget *parent = nullptr);
    SPMSG SetImg(SPIMG sp);
    SPIMG GetImg() const { return m_spIMG; }
    static StEditTool& Tool(){ return m_stTool; }
signals:
};

#endif // CWIDGET2D_H
