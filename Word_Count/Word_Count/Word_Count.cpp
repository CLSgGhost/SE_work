#include"pch.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <regex>
using namespace std;

class Word_Count
{	
public:
	int Ch_count,     //字符数
		Wo_count,     //单词数
		Li_count,     //行数
		NuLi_count,   //空行数
		CmLi_count,   //注释行数
		CdLi_count;   //代码行数
	int Char_count(string File_name);      //统计字符数
	int Word_count(string File_name);      //统计单词数
	int Line_count(string File_name);      //统计行数
	int NullLine_count(string File_name);  //统计空行数
	int ComLine_count(string File_name);   //统计注释行数
	int CodeLine_count(string File_name, int L1, int L2);  //统计代码行数
};

int main()
{
	char c;
	Word_Count wc;
	ifstream file;
	string Order, File_name;
	cout << "输入格式：wc.exe(空格)[parameter](空格)[file_name]" << endl << endl;
	cout << "功能列表：-c 统计文件字符数" << endl;
	cout << "          -w 统计文件单词数" << endl;
	cout << "          -l 统计文行数数" << endl;
	cout << "          -a 统计文件空行数、注释行数、代码行数" << endl << endl;
	while (1)
	{
		cout << "wc.exe" << " ";
		cin >> Order >> File_name;
		file.open(File_name, ios::in);
		if (!file.is_open())    //查找文件失败
		{
			cout << "文件不存在，无法打开" << endl << endl;
			continue;
		}
		c = file.get();
		if (c == EOF)        //文件为空
		{
			cout << "文件为空" << endl;
		}
		if (Order[0] == '-')
		{
			switch (Order[1])
			{
			case 'c':wc.Ch_count = wc.Char_count(File_name);    //统计字符数
				cout << "文件字符数为：" << wc.Ch_count << endl << endl;
				break;
			case 'w':wc.Wo_count = wc.Word_count(File_name);    //统计单词数
				cout << "文件单词数为：" << wc.Wo_count << endl << endl;
				break;
			case 'l':wc.Li_count = wc.Line_count(File_name);    //统计行数
				cout << "文件总行数为：" << wc.Li_count << endl << endl;
				break;
			case 'a':
				wc.NuLi_count = wc.NullLine_count(File_name);   //统计空行数
				wc.CmLi_count = wc.ComLine_count(File_name);    //统计空行数
				wc.CdLi_count = wc.CodeLine_count(File_name, wc.NuLi_count, wc.CmLi_count); //统计代码行数
				cout << "文件空行数为：" << wc.NuLi_count << endl;
				cout << "文件注释行数为：" << wc.CmLi_count << endl;
				cout << "文件代码行数为：" << wc.CdLi_count << endl << endl;
				break;
			default:
				cout << "无效指令" << endl << endl;
				break;
			}
		}
		file.close();
	}
	system("pause");
	return 0;
}

int Word_Count::Char_count(string File_name)   //统计字符数
{
	int n1 = 0;
	char ch;
	ifstream f;
	f.open(File_name, ios::in);
	f >> ch;
	while (!f.eof())
	{
		f >> ch;        //逐个读入字符并统计数量
		n1++;
	}
	f.close();
	return n1;
}

int Word_Count::Word_count(string File_name)   //统计单词数
{
	int n2 = 0;
	char ch;
	ifstream f;
	f.open(File_name, ios::in);
	while (!f.eof())
	{
		ch = f.get();
		if ((ch >= 'A'&& ch <= 'Z') || (ch >= 'a'&& ch <= 'z'))  //发现单词
		{
			while ((ch >= 'A'&& ch <= 'Z') || (ch >= 'a'&& ch <= 'z'))
			{
				ch = f.get();           //读取字符直到退出单词
			}
			n2++;     //单词计数器+1
		}
	}
	f.close();
	return n2;
}

int Word_Count::Line_count(string File_name)   //统计行数
{
	int n3 = 0;
	char ch;
	ifstream f;
	f.open(File_name, ios::in);
	ch = f.get();
	if (ch == EOF)      //文件为空则返回0
	{
		return 0;
	}
	while (!f.eof())
	{
		if (ch == '\n')   //读取到换行符时行数+1
		{
			n3++;
		}
		ch = f.get();
	}
	f.close();
	return n3 + 1;
}

int Word_Count::NullLine_count(string File_name)   //统计空行数
{
	int n4 = 0, i = 0;
	char ch;
	char str[2000];
	bool is_comline = 0;    //判断当前行是否在注释行"/*...*/"内
	string s;
	string pattern = "^[\\s]{1,}[\\S]?";  //正则表达式，匹配任何空字符一个以上和非空字符0或1个
	regex exception(pattern);
	fstream f;
	f.open(File_name, ios::in);   //打开文件
	ch = f.get();
	if (ch == EOF)      //文件为空则返回0
	{
		return 0;
	}
	while (!f.eof())   //判断文件是否读取完毕
	{
		getline(f, s);     //逐行读取文件
		i = 0;    //i初始化
		if (!s.empty())
		{
			strcpy_s(str, s.c_str());  //string非空就则转换位字符数组
		}
		while (str[i] == ' ' || str[i] == '\t')   //过滤空格和'\t'
		{
			i++;
		}
		if (str[i] == '/' && str[i + 1] == '*')  //读取到'/*'，开始注释行识别
		{
			is_comline = 1;
			i += 2;
		}
		while (is_comline == 1 && str[i] != '\0')
		{
			if (str[i] == '*' && str[i + 1] == '/')  //读取到'*/',结束注释行识别
			{
				is_comline = 0;
			}
			i++;
		}
		s.erase(0, 1);
		if (s.empty() && is_comline == 0)     //判断读取的行是否只有回车换行符
		{
			n4++;
			continue;
		}
		if (regex_match(s, exception) && is_comline == 0)
		{
			n4++;        //匹配任何空字1个以上，非空字符只有0或1个的空行 ,且不在注释行内则空行数增加
		}
	}
	f.close();
	return n4;
}

int Word_Count::ComLine_count(string File_name)   //统计注释行数
{                           
	int n5 = 0, i = 0;
	char str[2000];
	bool is_comline = 0;
	ifstream f;
	f.open(File_name, ios::in);   //打开文件
	while (!f.eof())   //判断文件是否读取完毕
	{
		f.getline(str, 2000);     //逐行读取文件
		i = 0;      //i初始化
		if (str[0] == '}')
		{
			i++;
		}
		while (str[i] == ' ' || str[i] == '\t')   //过滤空格和'\t'
		{
			i++;
		}
		if (str[i] == '/' && str[i + 1] == '/')  //读取'//'
		{
			if (is_comline == 0)   //当前行不在注释'/*...*/'内，注释行加一
			{
				n5++; 
				continue;
			}
		}
		if (str[i] == '/' && str[i + 1] == '*')  //读取到'/*'，开始注释行计算
		{
			is_comline = 1;
			i += 2;
		}
		if (is_comline == 1)
		{
			n5++;                //当前行在注释'/*...*/'内，注释行加一 
		}
		while (is_comline == 1 && str[i] != '\0')
		{
			if (str[i] == '*' && str[i + 1] == '/')  //读取到'*/',结束注释行计算
			{
				is_comline = 0;
			}
			i++;
		}
	}
	f.close();
	return n5;
} //注：不考虑注释符'/*...*/'不闭合的情况。若出现'/*'而没有'*/'，默认文本本身出错且'/*'以下全为注释行

int Word_Count::CodeLine_count(string File_name, int L1,int L2)   //统计代码行数
{
	int n6 = 0;
	int	L = 0;  //定义总行数
	Word_Count w;
	L = w.Line_count(File_name);
	n6 = L - L1 - L2;    //注释行数 = 总行数 - 空行数 - 注释行数
	return n6;
}