#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>

class MainWindow;
class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    static QSize getNormalSize();
public:
    CustomGraphicsView(QWidget *parent = Q_NULLPTR);
    CustomGraphicsView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
    //
    void setMainWidget(QWidget *pWidget);

protected:
    bool event(QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:
    void slotGeometryChanged();

private:
    QWidget *m_pMainWindow = nullptr;
};

#endif // CUSTOMGRAPHICSVIEW_H
