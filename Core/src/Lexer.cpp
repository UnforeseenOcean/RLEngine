#include "Core.hpp"
#include "Lexer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool IsDigit(char c) {
	return c >= '0' && c <= '9';
}

bool IsNumeric(const char *text) {
	if( !text || text[0] == '\0')
		return false;

	if(text[0] == '-' || text[0] == '+') {
		text++;
	}

	bool hasDot = false;
	for(int i=0; text[i] != '\0'; i++) {
		if(text[i] == '.' && !hasDot) {
			hasDot = true;
		}
		else if(!IsDigit(text[i])) {
			return false;
		}
	}
	return true;
}

Token::Token(const char *data) {
	string = data;
}

const char* Token::GetString() const {
	return string;
}

float Token::GetFloat() const {
	return static_cast<float>(atof(string));
}

int Token::GetInteger() const {
	return atoi(string);
}

Lexer::Lexer() : token(tokenBuffer) {
	tokenBuffer[0] = '\0';
	buffer = nullptr;
	bufferIsFile = false;
	hasByteOrderMark = false;
	tokIsUnread = true;
	tokPos = -1;

	//useEscapeChars = true;
	SetSingleTokenChars("{}(),;$=/");
	SetCommentStrings( "//"/*, "/*", "*./" */);
}

Lexer::~Lexer() {
	if (buffer) {
		if (!bufferIsFile) {
			delete[] buffer;
		}
		else {
			//TODO:
			UnmapViewOfFile(buffer);
			CloseHandle(filemapping);
			CloseHandle(file);
			//file = INVALID_HANDLE_VALUE;
		}
	}
}

const Token* Lexer::ReadToken() {
	if (tokPos == -1) {
		tokPos = 0;
	}
	else if (tokIsUnread) {
		tokIsUnread = false;
		return &token;
	}
	while (bufPos < bufSize) {
		// Single line comments
		if (!lineComment.empty() && strncmp((const char *)(buffer + bufPos), lineComment.c_str(), lineComment.length()) == 0) {
			if(tokPos > 0) {
				FinishToken();
				return &token;
			}
			/*bufPos += 2;
			if ( tokPos > 0 ) {
				SkipRestOfLine();
				return &token;
			}*/
			SkipRestOfLine();

			continue;
		}

		switch(buffer[bufPos]) {
			// Ignore whitespaces
			case ' ':
			case '\t':
				SkipWhiteSpaces();
				if(tokPos > 0) {
					FinishToken();
					return &token;
				}
				continue;

			// Newlines
			case '\n':
			case '\r':
				if (!tokPos) {
					FinishLine();
					continue;
				}
				FinishLine();
				return &token;

			/*// Quoted tokens
			case '\"':
			case '\'':
				if (tokPos > 0) {
					FinishToken();
					return &token;
				}
				ParseQuoted();
				return &token;*/

			default:
				// Check for single char tokens
				if (!singleTokenChars.empty()) {
					char c = buffer[bufPos];
					const char *chars = singleTokenChars.c_str();
					int num = singleTokenChars.length();
					for(int i=0; i<num; i++) {
						if(c != chars[i]) {
							continue;
						}
						if(tokPos == 0) {
							AddToToken( buffer[bufPos] );
							bufPos++;
						}
						FinishToken();
						return &token;
					}
				}
				// Add this char to the token
				AddToToken(buffer[bufPos]);
				bufPos++;
				continue;
		}
	}
	if(tokPos > 0) {
		FinishToken();
		return &token;
	}
	line = 0;

	return nullptr;
}

void Lexer::UnreadToken() {
	RL_ASSERT(!tokIsUnread, "You can not Unread a Token Twice!");
	tokIsUnread = true;
}

bool Lexer::CheckToken(const char *string) {
	// End of File
	if(!ReadToken()) {
		return false;
	}
	// Missing Token
	if(_stricmp(token.GetString(), string) != 0) {
		return false;
	}
	return true;
}

int Lexer::ReadInt() {
	// End of File
	if(!ReadToken()) {
		return INT_MIN; //TODO: wtf?
	}
	if(!IsNumeric(token.GetString())) {
		return INT_MIN;
	}
	return atoi(token.GetString());
}

float Lexer::ReadFloat() {
	// End of File
	if(!ReadToken()) {
		return FLT_MIN; //TODO: wtf?
	}
	if(!IsNumeric(token.GetString())) {
		return FLT_MIN;
	}
	return static_cast<float>(atof(token.GetString()));
}

const char *Lexer::ReadString( void ) {
	// End of File
	if(!ReadToken()) {
		return nullptr; //TODO: wtf?
	}

	return token.GetString();
}

bool Lexer::GotoNextLine() {
	int oldLine = token.line;
	while(ReadToken()) {
		if (token.line > oldLine) {
			UnreadToken();
			return true;
		}
	}
	return false;
}

bool Lexer::FindToken(const char *str) {
	while(ReadToken()) {
		if(_stricmp( token.GetString(), str ) == 0) {
			return true;
		}
	}
	return false;
}

bool Lexer::LoadFile(const char *filename) {
	file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
	if (file == INVALID_HANDLE_VALUE) {
		return false;
	}

	filemapping = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, 0, NULL);
	if (filemapping == INVALID_HANDLE_VALUE) {
		return false;
	}
	buffer = (unsigned char* const)MapViewOfFile(filemapping, FILE_MAP_READ, 0, 0, 0);

	name = filename;
	bufSize = GetFileSize(file, nullptr);
	if (bufSize == -1 || bufSize == 0) {
		return false;
	}

	unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
	hasByteOrderMark = memcmp(buffer, bom, 3) == 0;
	if(hasByteOrderMark) {
		Console::Print("Warning: File has Byte Order Mark");
		bufPos = 3;
	}
	else {
		bufPos = 0;
	}

	bufferIsFile = true;

	tokPos = -1;
	line = 1;
	return true;
}

void Lexer::SkipRestOfLine() {
	while(bufPos < bufSize) {
		if(buffer[bufPos] == '\n' || buffer[bufPos] == '\r') {
			FinishLine();
			return;
		}
		bufPos++;
	}
	// End of file
}

void Lexer::SkipWhiteSpaces( void ) {
	bufPos++;
	while(bufPos < bufSize && (buffer[bufPos] == ' ' || buffer[bufPos] == '\t') )
		bufPos++;
}

void Lexer::FinishLine() {
	if (buffer[bufPos] == '\r')
		bufPos += 2; // skip \n too
	else
		bufPos++;

	FinishToken();
	line++;
}

void Lexer::FinishToken(bool allowEmpty) {
	if ( tokPos || allowEmpty ) {
		tokenBuffer[tokPos] = '\0';
		token.line = line;
		tokPos = 0;
		tokIsUnread = false;
	}
}

void Lexer::AddToToken(char c) {
	if ( !hasByteOrderMark && c < 0 )
		tokenBuffer[tokPos] = '?';
	else
		tokenBuffer[tokPos] = c; 
	tokPos++;
}

void Lexer::SetSingleTokenChars(const char *chars) {
	singleTokenChars = chars;
}

void Lexer::SetCommentStrings(const char *line/*, const char *blockStart, const char *blockEnd*/) {
	lineComment = line;
	//blockComment[0] = blockStart;
	//blockComment[1] = blockEnd;
}