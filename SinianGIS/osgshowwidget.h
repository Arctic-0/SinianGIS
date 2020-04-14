#ifndef OSGSHOWWIDGET_H
#define OSGSHOWWIDGET_H

#include "sceneproject3d.h"

#include <QWidget>
#include <QTimer>

#include <osgViewer/CompositeViewer>
#include <osgEarthUtil/EarthManipulator>

class OSGShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OSGShowWidget(QWidget *parent = nullptr);

    bool load3DProject(std::shared_ptr<SceneProject3D> project);

    //������ʱ������
    void onStartTimer();
    //�رն�ʱ������
    void onStopTimer();

protected:
    //virtual void paintEvent(QPaintEvent* e);
    virtual void timerEvent(QTimerEvent* );

    void addView();

    QTimer                     _timer;
    int _timerID;               //��ʱ��ID
    osgViewer::CompositeViewer _viewer;

    osg::ref_ptr<osgViewer::View> view;
    std::shared_ptr<SceneProject3D> sceneProject3D;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mainManipulator;

signals:

public slots:
};

#endif // OSGSHOWWIDGET_H
