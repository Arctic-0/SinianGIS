#ifndef PATHREF_H
#define PATHREF_H

#include <string>
#include <vector>
#include <QApplication>
#include <QDir>

using std::string;
using std::vector;

class PathRef
{
public:
    PathRef()
    {
    }

    //�õ��ļ���   C:\\b\\a(.txt) -> a.txt
    static string GetNameWithFormat(string filePath)
    {
        size_t p = filePath.find_last_of(slash);
        if (p != string::npos)
        {
            filePath = filePath.substr(p+1, filePath.length() - p - 1);
        }

        return filePath;
    }

    //�õ��ļ�·���ĸ�ʽ��׺ C:\\b\\a.txt -> .txt
    static string GetFormat(string filePath)
    {
        size_t p = filePath.find_last_of('.');
        if (p == string::npos)
        {
            return string();
        }
        return filePath.erase(0, p);
    }

    //�õ��ļ�·���ϲ�Ŀ¼   C:\\b\\a(.txt) -> C:\\b
    static string GetPathLastDir(string filePath)
    {
        size_t p = filePath.find_last_of(slash);
        if (p != string::npos)
        {
            filePath.erase(p);
        }
        return filePath;
    }

	//�õ��ļ�·�����ļ���   C:\\b\\a(.txt) -> a
	static std::string DirOrPathGetName(std::string filePath)
	{
		size_t m = filePath.find_last_of(slash);
        if (m == string::npos)
		{
			return filePath;
		}

		size_t p = filePath.find_last_of('.');
        if (p != string::npos && p > m)				//û�е�Ż���
		{
			filePath.erase(p);
		}

		std::string dirPath = filePath;
		dirPath.erase(0, m + 1);
		return dirPath;
	}

    //�ı�·�����ƣ����ڱ��� C:\\b\\a.txt->c--b-c.txt
    static string GetSaveString(string filePath)
    {
        for(size_t i = 0; i<filePath.length(); i++)
        {
            if(filePath[i] == '\\' || filePath[i] == '/' || filePath[i] == ':')
            {
                filePath[i] = '-';
            }
        }
        return filePath;
    }

    //��ȡAPPĿ¼
    static std::string GetAppDir()
    {
        QByteArray currentDir = QCoreApplication::applicationDirPath().toLocal8Bit();
        return currentDir.data();
    }

    //����Ŀ¼�����е��ļ���
    static void findDir(string dir, vector<string>& subDirs)
    {
        //
        subDirs.clear();
        QDir fromDir(QString::fromLocal8Bit(dir.c_str()));
        QStringList filters;

        //
        QFileInfoList fileInfoList = fromDir.entryInfoList(filters, QDir::AllDirs|QDir::Files);
        foreach(QFileInfo fileInfo, fileInfoList)
        {
            if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            {
                continue;
            }

            if (fileInfo.isDir())
            {
                QByteArray dir = fileInfo.filePath().toLocal8Bit();
                subDirs.push_back(dir.data());
            }
        }
    }

    static bool isDirExist(std::string filePath)//����
    {
        QFile mFile(QString::fromLocal8Bit(filePath.c_str()));
        if(mFile.exists())
        {
            return true;
        }
        return false;
    }

    const static char slash = '/';
};

#endif // PATHREF_H
