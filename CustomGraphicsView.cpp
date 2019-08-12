#include "CustomGraphicsView.h"
#include "View/MainWindow/MainWindow.h"
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

QSize CustomGraphicsView::getNormalSize()
{
    return QSize(1920,1080);
}

CustomGraphicsView::CustomGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setFrameShape(QFrame::NoFrame);
    setLineWidth(0);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
}

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, QWidget *parent): QGraphicsView(scene, parent)
{
    setFrameShape(QFrame::NoFrame);
    setLineWidth(0);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
}

void CustomGraphicsView::setMainWidget(QWidget *pWidget)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if(scene())
    {
        pWidget->installEventFilter(this);
        m_pMainWindow = pWidget;
        connect(pWidget,SIGNAL(destroyed(QObject*)),this,SLOT(deleteLater()));
        //
        this->setWindowFlags(pWidget->windowFlags());
        QGraphicsProxyWidget *pProxyWidget = scene()->addWidget(pWidget,pWidget->windowFlags());
        connect(pProxyWidget,&QGraphicsProxyWidget::geometryChanged,this,&CustomGraphicsView::slotGeometryChanged);
        pProxyWidget->setWindowFlags(pWidget->windowFlags());

        connect(pProxyWidget,&QGraphicsProxyWidget::visibleChanged,this,[this,pWidget]{
            if(pWidget->windowState().testFlag(Qt::WindowMinimized))
            {
                this->showMinimized();
                return;
            }
            setVisible(pWidget->isVisible());
        });

        connect(pProxyWidget, &QGraphicsProxyWidget::childrenChanged,this, [pProxyWidget]{
            qDebug()<<__FUNCTION__<<pProxyWidget->childItems().count();
            for(auto objItem : pProxyWidget->childItems())
            {
                QGraphicsWidget *pGraphicsWidget = qobject_cast<QGraphicsWidget *>(objItem->toGraphicsObject());
                if(pGraphicsWidget)
                {
                    if(!pGraphicsWidget->windowFlags().testFlag(Qt::FramelessWindowHint))
                    {
                        qDebug()<<__FUNCTION__<<"pGraphicsWidget->setWindowFlags";
                        pGraphicsWidget->setWindowFlags(pGraphicsWidget->windowFlags()|Qt::FramelessWindowHint);
                    }
                }
            }
        });
    }
}

bool CustomGraphicsView::event(QEvent *event)
{
    bool isState = QGraphicsView::event(event);
    if(event->type()==QEvent::WindowStateChange)
    {
        if(this->windowState()!=(Qt::WindowMinimized|Qt::WindowFullScreen)&&m_pMainWindow)
        {
            m_pMainWindow->setWindowState(this->windowState());
        }
    }
    return isState;
}

bool CustomGraphicsView::eventFilter(QObject *obj, QEvent *event)
{
    bool result = QGraphicsView::eventFilter(obj,event);
    if(m_pMainWindow==obj)
    {
        if(event->type()==QEvent::Show)
        {
            this->show();
        }
        else if(event->type()==QEvent::Close)
        {
            exit(0);
        }
    }
    return result;
}

void CustomGraphicsView::slotGeometryChanged()
{
    QSize sizeNormal = CustomGraphicsView::getNormalSize();
    if(m_pMainWindow->size()!=sizeNormal)
    {
        m_pMainWindow->setGeometry(0,0,sizeNormal.width(),sizeNormal.height());
        //
        this->scale(double(this->width())/sizeNormal.width(),double(this->height())/sizeNormal.height());
    }
}
