#ifndef REGEDIT_H
#define REGEDIT_H

#include <QString>

/*******************************************************************
* ע���ļ�����
*
*  className    // �Զ�������
*  appPath      // �����ĳ���Ŀ¼
*  ext          // �������ļ�����
*  extDes       // ���ļ���������
*
*******************************************************************/
void registerFileRelation(const QString& className,const QString& appPath,const QString& ext,const QString& extDes);

#endif // REGEDIT_H
