#include "osgshowwidget.h"
#include "ViewWidget"

#include <QLayout>
#include <QMessageBox>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgEarthUtil/ViewFitter>

using namespace std;

OSGShowWidget::OSGShowWidget(QWidget *parent) : QWidget(parent), viewWidget(nullptr),
    bWork(false)
{
    _viewer.setThreadingModel(_viewer.SingleThreaded);

    // we have to add a starting view, otherwise the CV will automatically
    // mark itself as done :/
    addView();

    // timer fires a paint event.
    //    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    //    _timer.start(10);

    sceneProject3D = nullptr;
}

OSGShowWidget::~OSGShowWidget()
{
    if(viewWidget)
    {
        delete viewWidget;
        viewWidget = nullptr;
    }
}

bool OSGShowWidget::load3DProject(std::shared_ptr<SceneProject3D> project)
{
    this->sceneProject3D = project;

    view->setSceneData(sceneProject3D->GetRootNode());
    mainManipulator->setViewpoint(*(sceneProject3D->GetHomeViewPoint()));
    mainManipulator->setHomeViewpoint(*(sceneProject3D->GetHomeViewPoint()));

    return true;
}

//void OSGShowWidget::paintEvent(QPaintEvent* e)
//{
// refresh all the views.
//    if (_viewer.getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS || _viewer.checkNeedToDoFrame() )
//    {
//        _viewer.frame();
//    }
//}

//������ʱ������
void OSGShowWidget::onStartTimer()
{
    _timerID = startTimer(10);
    bWork = true;
}

//�رն�ʱ������
void OSGShowWidget::onStopTimer()
{
    killTimer(_timerID);
    bWork = false;
}

//��ʱ���¼�
void OSGShowWidget::timerEvent(QTimerEvent* )
{
    // refresh all the views.
    if (_viewer.getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS || _viewer.checkNeedToDoFrame() )
    {
        _viewer.frame();
    }
}

void OSGShowWidget::addView()
{
    // the new View we want to add:
    view = new osgViewer::View();

    // a widget to hold our view:
    viewWidget = new osgEarth::QtGui::ViewWidget(view);

    setLayout(new QHBoxLayout());
    layout()->setMargin(1);
    layout()->addWidget(viewWidget);

    // set up the view
    mainManipulator = new osgEarth::Util::EarthManipulator;
    view->setCameraManipulator(mainManipulator);
    //view->setSceneData( root );
    view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,false);

    //osgEarth::Util::MapNodeHelper().configureView(view);         // add some stock OSG handlers:
    view->addEventHandler(new osgViewer::StatsHandler());
    view->addEventHandler(new osgViewer::HelpHandler);

    // add it to the composite viewer.
    _viewer.addView( view );
}

void OSGShowWidget::CalTerrainLayerViewPoint(std::string name)
{
    auto layer = sceneProject3D->GetMap()->getLayerByName<osgEarth::TerrainLayer>(name);
    if (!layer)
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�Ҳ������ʵ��ӵ�"), QMessageBox::Ok);
    }

    //��ȡ�ӵ�λ��
    std::shared_ptr<osgEarth::Viewpoint> vp =  std::make_shared<osgEarth::Viewpoint>();
    if (!CalViewPointGeoExtend(layer->getDataExtentsUnion(), vp))
    {
      QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�Ҳ������ʵ��ӵ�"), QMessageBox::Ok);
      auto cp = layer->getDataExtentsUnion().getCentroid();
      osgEarth::GeoPoint point(layer->getProfile()->getSRS(), cp.x(), cp.y(), cp.z());
      osgEarth::GeoPoint newPoint = point.transform(sceneProject3D->GetMap()->getSRS());
      vp->focalPoint() = newPoint;
      vp->heading() = 0;
      vp->pitch() = -90;
      vp->range() = 16000;
    }

    sceneProject3D->insertViewPoint(name, vp);
    mainManipulator->setViewpoint(*vp);
}

//
bool OSGShowWidget::CalViewPointGeoExtend(const osgEarth::GeoExtent& extent, std::shared_ptr<osgEarth::Viewpoint> out)
{
    //
    double xMin = extent.xMin();
    double xMax = extent.xMax();
    double yMin = extent.yMin();
    double yMax = extent.yMax();
    double z = extent.getCentroid().z();

    //��ȡ��Χ���ĸ���
    std::vector<osgEarth::GeoPoint> points =
    {
        osgEarth::GeoPoint(extent.getSRS(), xMin, yMin, z),
        osgEarth::GeoPoint(extent.getSRS(), xMax, yMin, z),
        osgEarth::GeoPoint(extent.getSRS(), xMax, yMax, z),
        osgEarth::GeoPoint(extent.getSRS(), xMin, yMax, z)
    };

    //�����ӵ�
    osgEarth::Util::ViewFitter vf(sceneProject3D->GetMap()->getSRS(), view->getCamera());
    if(!vf.createViewpoint(points, *out))
    {
        return false;
    }
    out->heading() = 0;             //�������ӵ���û������heading�ģ������õ���������ʱ�򣬾ͻ��õ�ǰ��heading

    return true;
}
