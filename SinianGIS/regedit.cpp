#include "regedit.h"

#include <QSettings>

void registerFileRelation(const QString& className,const QString& appPath,const QString& ext,const QString& extDes)
{
    QString baseUrl("HKEY_CURRENT_USER\\Software\\Classes");    // Ҫ��ӵĶ���Ŀ¼
    QSettings settingClasses(baseUrl,QSettings::NativeFormat);

    // ��...Classes\����´���һ���µ���𣬲����ø������ļ�ʱ�ĵ��ò���
    settingClasses.setValue("/" + className + "/Shell/Open/Command/.","\"" + appPath + "\" \"%1\"");

    // �ļ���������
    settingClasses.setValue("/" + className + "/.",extDes);

    // ���ø�����Ĭ��ͼ��Ĭ��ͼ��
    settingClasses.setValue("/" + className + "/DefaultIcon/.",appPath + ",0");

    // ����ext �� ���
    settingClasses.setValue("/" + ext + "/OpenWithProgIds/" + className,"");

    // ����������޸�
    settingClasses.sync();
}
