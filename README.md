# PcSystemAutoFullscreen
用于给QT开发的带ui程序，进行快速的全屏保持比例自适应不同分辨率的屏幕，无需进行特殊布局

具体使用：
m_pMainWindow = new MainWindow();
//
QRect rect = QApplication::desktop()->screenGeometry();
if(rect.size()!=CustomGraphicsView::getNormalSize())
{
    CustomGraphicsView pCustomGraphicsView = new CustomGraphicsView(new QGraphicsScene());
    pCustomGraphicsView->setMainWidget(m_pMainWindow);
    pCustomGraphicsView->setGeometry(0,0,rect.width(),rect.height());
}
//
m_pMainWindow->setGeometry(0,0,rect.width(),rect.height());
m_pMainWindow->show();
