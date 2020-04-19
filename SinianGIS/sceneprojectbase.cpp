#include "sceneprojectbase.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <QJsonDocument>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "pathref.hpp"

using namespace std;

SceneProjectBase::SceneProjectBase()
{
    appDir = PathRef::GetAppDir();
}

SceneProjectBase::~SceneProjectBase()
{

}

void SceneProjectBase::read(std::string path)
{
    QFile file(QString::fromLocal8Bit(path.c_str()));
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ܶ�ȡ�����ļ�"));
        return;
    }

    QByteArray ba = file.readAll();
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
    if(e.error == QJsonParseError::NoError && !jsonDoc.isNull())
    {
        projectJSON = jsonDoc.object();
    }
}

void SceneProjectBase::write(std::string path)
{
    QJsonDocument document;
    document.setObject(projectJSON);
    QByteArray simpbyte_array = document.toJson(QJsonDocument::Indented);

    QFile file(QString::fromLocal8Bit(path.c_str()));
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ܱ��湤���ļ�"));
        return;
    }
    file.write(simpbyte_array);
    file.close();
}
