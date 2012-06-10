#pragma once

#include <string>

class Token {
public:
	Token(const char *data);

	const char * GetString() const;
	float GetFloat() const;
	int GetInteger() const;

private:
	friend class Lexer;

	const char *string;
	int line;
};

class Lexer {
public:
	Lexer();
	~Lexer();

	bool LoadFile(const char *filename);
	//bool LoadData(const char *dataName, const byte *data, int size);

	const Token* ReadToken();
	void UnreadToken();

	//void ExpectToken(const char *string);
	bool CheckToken(const char *string);

	int ReadInt();
	float ReadFloat();
	const char *ReadString();

	bool FindToken(const char *str); //void

	int GetLine() { return token.line; }
	bool GotoNextLine();

	//void SetEscapeCharacters(bool enable) { useEscapeChars = enable; }

	void SetSingleTokenChars(const char *chars);

	void SetCommentStrings(const char *line/*, const char *blockStart, const char *blockEnd*/);

private:
	//void ParseQuoted();
		
	//void SkipToCommentEnd();

	void SkipRestOfLine();
	void SkipWhiteSpaces();

	void FinishLine();

	void FinishToken(bool allowEmpty = false);

	void AddToToken(char c);

	std::string name; //!< The file or data name
	bool hasByteOrderMark; //!< True if the file has an utf-8 Byte Order Mark
	char tokenBuffer[4096];	//!< Buffer for the token
	Token token; //!< The current token
	unsigned char* buffer; //!< The file buffer
	bool bufferIsFile; //!< Set if this data was loaded from a file
	bool tokIsUnread; //!< Set if a token was unread
	int bufSize; //!< Size of the buffer
	int bufPos; //!< The buffer position
	int tokPos; //!< The token position
	int line; //!< The line the parser is on (not the current token line)

	//bool useEscapeChars; //!< true to use escape characters
	std::string singleTokenChars; //!< The single token characters
	std::string lineComment; //!< The line comment string
	//std::string blockComment[2]; //!< The block comment strings

	//temp
	void* filemapping;
	void* file;
};