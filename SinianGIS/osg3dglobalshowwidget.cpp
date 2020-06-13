#include "osg3dglobalshowwidget.h"

#include <osgEarthUtil/ViewFitter>
#include <osgEarthUtil/AutoClipPlaneHandler>

#include <iostream>
#include <QMessageBox>

using namespace std;

OSG3DGlobalShowWidget::OSG3DGlobalShowWidget(QWidget *parent):OSGShowWidget(parent)
{
    sceneProject3D = nullptr;

    mainManipulator = new osgEarth::Util::EarthManipulator;
    view->setCameraManipulator(mainManipulator);
}

OSG3DGlobalShowWidget::~OSG3DGlobalShowWidget()
{
}

bool OSG3DGlobalShowWidget::load3DProject(std::shared_ptr<SceneProject3D> project)
{
    sceneProject3D = project;
    name = sceneProject3D?sceneProject3D->getFileName():"";

    view->setSceneData(sceneProject3D->GetRootNode());
    mainManipulator->setViewpoint(*(sceneProject3D->GetHomeViewPoint()));
    mainManipulator->setHomeViewpoint(*(sceneProject3D->GetHomeViewPoint()));

    view->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(sceneProject3D->GetMapNode()));           //�����Զ��ü���������ʾ������ڵ�

    return true;
}

void OSG3DGlobalShowWidget::SetTerrainLayerViewPoint(std::string name)
{
    auto layer = sceneProject3D->GetMap()->getLayerByName<osgEarth::TerrainLayer>(name);
    if (!layer)
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�Ҳ������ʵ��ӵ�"), QMessageBox::Ok);
        return;
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

void OSG3DGlobalShowWidget::SetNodeViewPoint(std::string name)
{
    auto layer = sceneProject3D->GetMap()->getLayerByName<osgEarth::VisibleLayer>(name);
    if (!layer)
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�Ҳ������ʵ��ӵ�"), QMessageBox::Ok);
        return;
    }

    //�����ӵ�
    std::shared_ptr<osgEarth::Viewpoint> vp =  std::make_shared<osgEarth::Viewpoint>();
    if(!layer->getNode() || !CalViewPointNode(layer->getNode(), vp))
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�Ҳ������ʵ��ӵ�"), QMessageBox::Ok);
        //        out->focalPoint() = point;
        //        out->heading() = 0;
        //        out->pitch() = -90;
        //        out->range() = 16000;
    }

    sceneProject3D->insertViewPoint(name, vp);
    mainManipulator->setViewpoint(*vp);
}

//
bool OSG3DGlobalShowWidget::CalViewPointGeoExtend(const osgEarth::GeoExtent& extent, std::shared_ptr<osgEarth::Viewpoint> out)
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

//�����ӵ�
bool OSG3DGlobalShowWidget::CalViewPointNode(osg::ref_ptr<osg::Node> node, std::shared_ptr<osgEarth::Viewpoint> out)
{
    //���㷶Χ ������:�����Сzֵ����
    //    osg::ComputeBoundsVisitor boundvisitor;
    //    xform->accept(boundvisitor);
    //    osg::BoundingBox bb = boundvisitor.getBoundingBox();

    //�����Χ��
    osg::BoundingSphere bs;
    node->computeBound();
    bs = node->getBound();

    //
    double xMin = bs.center().x() - bs.radius();
    double xMax = bs.center().x() + bs.radius();
    double yMin = bs.center().y() - bs.radius();
    double yMax = bs.center().y() + bs.radius();
    double zMin = bs.center().z() - bs.radius();
    double zMax = bs.center().z() + bs.radius();

    //��ȡ��Χ���ĸ���
    vector<osg::Vec3d> boundPoint= {osg::Vec3d(xMin, yMin, zMin), osg::Vec3d(xMax, yMin, zMin),
                                    osg::Vec3d(xMax, yMax, zMin), osg::Vec3d(xMin, yMax, zMin),
                                    osg::Vec3d(xMin, yMin, zMax), osg::Vec3d(xMax, yMin, zMax),
                                    osg::Vec3d(xMax, yMax, zMax), osg::Vec3d(xMin, yMax, zMax)};
    //    vector<osg::Vec3d> boundPoint= {osg::Vec3d(xMin, yMin, bs.center().z()), osg::Vec3d(xMax, yMin, bs.center().z()),
    //                                    osg::Vec3d(xMax, yMax, bs.center().z()), osg::Vec3d(xMin, yMax, bs.center().z())};
    std::vector<osgEarth::GeoPoint> points;
    for(auto &it: boundPoint)
    {
        osgEarth::GeoPoint convertPoint;
        convertPoint.fromWorld(sceneProject3D->GetMap()->getSRS(), it);
        points.push_back(convertPoint);
    }

    //�����ӵ�
    osgEarth::Util::ViewFitter vf(sceneProject3D->GetMap()->getSRS(), view->getCamera());
    double value_meters = bs.radius()*0.2;
    vf.setBuffer(value_meters);
    if(!vf.createViewpoint(points, *out))
    {
        return false;
    }

    //    qDebug()<<out->focalPoint()->x()<<'\t'<<out->focalPoint()->y()<<'\t'<<out->focalPoint()->z();
    //    qDebug()<<out->range()->getValue();
    out->heading() = 0;             //�������ӵ���û������heading�ģ������õ���������ʱ�򣬾ͻ��õ�ǰ��heading

    return true;
}

void OSG3DGlobalShowWidget::slotViewPoint(std::string name)
{
    auto vp = sceneProject3D->getViewPoint(name);
    if (!vp)
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�Ҳ������ʵ��ӵ�"), QMessageBox::Ok);
        return;
    }

    mainManipulator->setViewpoint(*vp);
}

