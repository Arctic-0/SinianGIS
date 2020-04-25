#pragma once

#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;

//std::string��ǿ��������
class StdStringEx
{
public:
	//�����ַ��з�string��������ǰ�������ַ�
	static void CutString(string line, vector<string> &subline, char a)
	{
		//����ĸΪa���޳�����ĸ
		if (line.size() < 1)
		{
			return;
		}
		if (line[0] == a)
		{
			line.erase(0, 1);
		}

		size_t pos = 0;
		while (pos < line.length())
		{
			size_t curpos = pos;
			pos = line.find(a, curpos);
			if (pos == string::npos)
			{
				pos = line.length();
			}
			subline.push_back(line.substr(curpos, pos - curpos));
			pos++;
		}

		return;
	}

	//���ݿսض��ַ���
	static void ChopStringLineEx(string line, vector<string> &substring)
	{
		std::stringstream linestream(line);
		string sub;

		while (linestream >> sub)
		{
			substring.push_back(sub);
		}
	}


};

