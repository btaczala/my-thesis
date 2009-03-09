project(muparser)
cmake_minimum_required(VERSION 2.6)

message ( STATUS "Building muParser library")
set (muparser_SOURCES 
muParser.cpp
muParserBase.cpp
muParserBytecode.cpp
muParserCallback.cpp
muParserError.cpp
muParserTokenReader.cpp
)
set ( muparser_HDRS 
muParser.h
muParserBase.h
muParserBytecode.h
muParserCallback.h
muParserDef.h
muParserError.h
muParserFixes.h
muParserStack.h
muParserToken.h
muParserTokenReader.h
)

add_library (muparser STATIC ${muparser_SOURCES} ${muparser_HDRS})
