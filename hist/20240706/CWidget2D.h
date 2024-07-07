#ifndef CWIDGET2D_H
#define CWIDGET2D_H
#include <CMyDef.h>

class CWidget2D : public QLabel
{
    Q_OBJECT
    SPIMG m_spIMG{NULL};
    //     bool event(QEvent *e) override;
    //     void keyPressEvent(QKeyEvent *ev) override;
    void paintEvent(QPaintEvent *) override;
    //     void changeEvent(QEvent *) override;
    //     void mousePressEvent(QMouseEvent *ev) override;
    //     void mouseMoveEvent(QMouseEvent *ev) override;
    //     void mouseReleaseEvent(QMouseEvent *ev) override;
    //     void focusInEvent(QFocusEvent *ev) override;
    //     void focusOutEvent(QFocusEvent *ev) override;
    //     bool focusNextPrevChild(bool next) override;
public:
    explicit CWidget2D(QWidget *parent = nullptr);
    SPMSG SetImg(SPIMG sp){ m_spIMG=sp; update(); return _ok; }
signals:
};

#endif // CWIDGET2D_H
