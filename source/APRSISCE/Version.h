#pragma once
//	Hopefully leading zeros don't change to octal!
#define VERSION_MAJOR 2026
#define VERSION_MINOR 11
#define VERSION_REVISION 22
#define VERSION_HOUR 08
#define VERSION_MINUTE 13
#define VERSION_BUILD 0813
#define VERSION_BUILD_NUMBER 813

// Strange thing is the *100 is completely ignored!
//#define VERSION_BUILD (VERSION_HOUR*100)+VERSION_MINUTE

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)
#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD_NUMBER
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
                                    "." STRINGIZE(VERSION_MINOR)    \
                                    "." STRINGIZE(VERSION_REVISION) \
                                    "." STRINGIZE(VERSION_BUILD)
#define VER_PRODUCT_VERSION_STR     STRINGIZE(VERSION_MAJOR)        \
                                    "/" STRINGIZE(VERSION_MINOR)    \
                                    "/" STRINGIZE(VERSION_REVISION) \
                                    " " STRINGIZE(VERSION_HOUR)		\
									":" STRINGIZE(VERSION_MINUTE)
#define VER_COPYRIGHT				"© 2009," STRINGIZE(VERSION_MAJOR) " KJ4ERJ"
