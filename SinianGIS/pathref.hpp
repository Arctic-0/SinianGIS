#ifndef PATHREF_H
#define PATHREF_H

#include <string>
#include <QApplication>

using std::string;

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

    const static char slash = '/';
};

#endif // PATHREF_H
