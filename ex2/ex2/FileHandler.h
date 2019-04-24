#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Manager.h"
#include <regex>

class FileHandler {
private:
	ifstream				m_fin;
	ofstream				m_fout;
	Errors					m_errors;
	Manager *				m_manager = nullptr;
	inline void				allowParsing(bool allow) { m_errors.allowParsing = allow; }
	void					checkErrors(void* (titleFunc));
	inline void				pushError(ErrorType type, const string & str) { m_errors.list.push_back(Pair(type, str)); }
	string					getName(string & line);
	int						getIntValue(const string & input, const ErrorType error, string & line);
	MazeBoard				getBoard(const int rows, const int cols, Coordinate & playerLocation, Coordinate & endLocation, string & line);
	void					handleSpecialChar(const char c, Coordinate & location, const int i, const int j, bool & seenChar, string & line, const ErrorType e);
	void					handleInvalidChar(const char c, const int i, const int j);

public:
	FileHandler(int argc, char* argv[]);
	~FileHandler();
	inline Manager *		getManager() { return m_manager; }
	void					parseInput();
	inline bool				parsingIsAllowed() { return m_errors.allowParsing; }
	void					pushActionsToOutputFile(vector<char> actions);
};

#endif