#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sceneproject3d.h"
#include "pathref.hpp"
#include "project3dform.h"
#include "loadphotogrammetrydialog.h"

#include <QDockWidget>
#include <QMenu>
#include <QFileDialog>
#include <QDebug>

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //����ribbon
    ui->ribbonTabWidget->setCurrentIndex(0);
    ui->toolBar->addWidget(ui->ribbonTabWidget);
    ui->toolBar->layout()->setMargin(0);
    ui->toolBar->layout()->setSpacing(0);

    //
    QMenu* ArcGISMenu=new QMenu(this);
    ArcGISMenu->addAction(ui->actionArcGISImage);
    ArcGISMenu->addAction(ui->actionArcGISTerrain);
    ui->tBArcGIS->setMenu(ArcGISMenu);
    ui->tBArcGIS->setPopupMode(QToolButton::InstantPopup);

    QMenu* BingMenu=new QMenu(this);
    BingMenu->addAction(ui->actionBingImage);
    BingMenu->addAction(ui->actionBingTerrain);
    ui->tBBing->setMenu(BingMenu);
    ui->tBBing->setPopupMode(QToolButton::InstantPopup);

    //ͣ��
    QDockWidget *projectDock = new QDockWidget(QString::fromLocal8Bit("��������"), this);
    projectDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    Project3DForm *project3DForm = new Project3DForm(projectDock);
    projectDock->setWidget(project3DForm);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);
    leftDockList.push_back(projectDock);

    sceneProject3D = make_shared<SceneProject3D>();
    ui->showWidget->load3DProject(sceneProject3D);
    project3DForm->LoadProject3d(sceneProject3D);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show()
{
    QWidget::show();
    ui->showWidget->onStartTimer();
}

void MainWindow::on_tBNewProject_clicked()
{
}

void MainWindow::on_tBNewImageLayer_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/hunan-laiyan");
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("�򿪱���Ӱ������"),
                                                     dir,QString::fromLocal8Bit("����Ӱ������(*.tif *.tiff *.img *.jpg *.png);;����Ӱ������(*.*)"));
    if(filePath.isNull())
    {
        return;
    }

    QByteArray path = filePath.toLocal8Bit();
    sceneProject3D->AddLocalImage(path.data());
    ui->showWidget->CalTerrainLayerViewPoint(path.data());
}

void MainWindow::on_actionArcGISImage_triggered()
{
    sceneProject3D->AddArcGISImagery();
}

void MainWindow::on_actionArcGISTerrain_triggered()
{
    sceneProject3D->AddArcGISTerrainImagery();
}

void MainWindow::on_actionBingImage_triggered()
{
    sceneProject3D->AddBingImagery();
}

void MainWindow::on_actionBingTerrain_triggered()
{
    sceneProject3D->AddBingTerrain();
}

void MainWindow::on_tBNewTerrainLayer_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/hunan-laiyan");
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("�򿪱��ص�������"),
                                                     dir,QString::fromLocal8Bit("���ص�������(*.tif *.img);;����Ӱ������(*.*)"));
    if(filePath.isNull())
    {
        return;
    }

    QByteArray path = filePath.toLocal8Bit();
    sceneProject3D->AddLocalTerrain(path.data());
    ui->showWidget->CalTerrainLayerViewPoint(path.data());
}

void MainWindow::on_tBSaveProject_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/Test");
    QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("���泡�������ļ�"),dir,
                                                    QString::fromLocal8Bit("���������ļ�(*.sj)"));
    if (fileName.isNull())
    {
        return;
    }
    QByteArray path = fileName.toLocal8Bit();
    sceneProject3D->write(path.data());
}

void MainWindow::on_tBOpenProject_clicked()
{
    QString dir = QString::fromLocal8Bit("D:/Data/Test");
    QString filePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("�򿪳��������ļ�"),dir,
                                                    QString::fromLocal8Bit("���������ļ�(*.sj);;����Ӱ������(*.*)"));
    if(filePath.isNull())
    {
        return;
    }
    QByteArray path = filePath.toLocal8Bit();
    string name = PathRef::GetNameWithFormat(path.data());

    OSGShowWidget *tabWidget = new OSGShowWidget(ui->centralTabWidget);
    ui->centralTabWidget->addTab(tabWidget, QString::fromLocal8Bit(name.c_str()));
    ui->centralTabWidget->setCurrentIndex(ui->centralTabWidget->indexOf(tabWidget));

    std::shared_ptr<SceneProject3D> _3dProject = make_shared<SceneProject3D>();
    _3dProject->read(path.data());
    tabWidget->load3DProject(_3dProject);

    QDockWidget *projectDock = new QDockWidget(QString::fromLocal8Bit("��������11"), this);
    projectDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    Project3DForm *project3DForm = new Project3DForm(projectDock);
    projectDock->setWidget(project3DForm);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);

    tabifyDockWidget(leftDockList.back(), projectDock);
    leftDockList.push_back(projectDock);

    project3DForm->LoadProject3d(_3dProject);
    projectDock->setVisible(true); //Ҫ�����ÿɼ�������ʾ
    projectDock->raise();
}

void MainWindow::on_centralTabWidget_currentChanged(int index)
{
    for (int i = 0; i < ui->centralTabWidget->count(); i++)
    {
        if( i == index)
        {
            OSGShowWidget *widget = static_cast<OSGShowWidget *>(ui->centralTabWidget->widget(i));
            if(widget && !widget->isWork())
            {
                widget->onStartTimer();
            }
        }
        else
        {
            OSGShowWidget *widget = static_cast<OSGShowWidget *>(ui->centralTabWidget->widget(i));
            if(widget && widget->isWork())
            {
                widget->onStopTimer();
            }
        }
    }
}

void MainWindow::on_centralTabWidget_tabCloseRequested(int index)
{
    QWidget *pItemWidget = ui->centralTabWidget->widget(index);
    if (pItemWidget)
    {
        delete pItemWidget;
        pItemWidget = nullptr;
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    for (int i = ui->centralTabWidget->count()-1; i >= 0; i--)
    {
        on_centralTabWidget_tabCloseRequested(i);
    }
}

void MainWindow::on_tBNewPhotogrammetry_clicked()
{
    LoadPhotogrammetryDialog loadDialog;
    loadDialog.exec();
}
