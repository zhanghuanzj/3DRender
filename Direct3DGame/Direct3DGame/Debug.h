#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <fstream>
using namespace std;


class Debug
{

public:
	static Debug& instance()
	{
		static Debug *p_debug = new Debug();
		return *p_debug;
	}
	template <typename T>
	void debug_output(const T &info)
	{
		output<<info;
	}
	template <typename T>
	ofstream& operator<<(const T &info)
	{
		output<<info;
		return output;
	}
	~Debug(){output.close();}
private:
	Debug(string fileName="Debug.txt"){ output.open(fileName);};
	ofstream output;
	
};
#endif