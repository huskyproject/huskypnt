/* header file common to all platforms and languages */

#ifndef __COMMON_H__
#define __COMMON_H__

#define VERSION "0.2alpha1"

#define nfree(a) { if (a != NULL) { free(a); a = NULL; } }

#define numNodelists 2
extern char *nodelists[numNodelists];

#define rcNotRoot                           1
#define rcCheckNeededPrograms               2
#define rcCheckFiles                        3
#define rcCreateUsers                       4
#define rcCreateDirs                        5
#define rcCreateSystemConfigPrecompile      6
#define rcInstallSmapi                      7
#define rcInstallFconf                      8
#define rcInstallProgs                      9
#define rcCopyScripts                      10
#define rcUnpackSources                    11
#define rcCreateMakeConfig                 12
#define rcCompileSmapi                     13
#define rcCompileFconf                     14
#define rcCompileProgs                     15
#define rcCreateConfig                     16
#define rcCreateUserConfig                 17
#define rcCopyNodelists                    18
#define rcCompileNodelists                 19

#define groupNameIdx                        0
#define fidoNameIdx                         1
#define usersIdx                            2
#define libDirIdx                           3
#define binDirIdx                           4
#define manDirIdx                           5
#define cfgDirIdx                           6
#define logDirIdx                           7
#define incDirIdx                           8
#define debugIdx                            9
#define libcVersionIdx                     10
#define locationIdx                        11
#define sysOpNameIdx                       12
#define workDirIdx                         13
#define inboundIdx                         14
#define protInboundIdx                     15
#define localInboundIdx                    16
#define tempInboundIdx                     17
#define tempOutboundIdx                    18
#define outboundIdx                        19
#define msgbaseDirIdx                      20
#define nodelistDirIdx                     21
#define netmailDirIdx                      22
#define scriptDirIdx                       23
#define isdnDevIdx                         24
#define modemDevIdx                        25
#define modemBaudIdx                       26
#define internatPrefixIdx                  27
#define localPrefixIdx                     28
#define voiceNumIdx                        29
#define dataNumIdx                         30
#define amtNumIdx                          31
#define localNumIdx                        32
#define internatNumIdx                     33
#define packerIdx                          34
#define pointNrIdx                         35
#define uplinkAddrIdx                      36
#define uplinkNameIdx                      37
#define uplinkPwdIdx                       38
#define homeDirIdx                         39
#define infoDirIdx                         40
#define htmlDirIdx                         41
#define userNameIdx                        42
#define numIdx                             43

typedef struct _tCfgFileMap
{
  char *sourceFile;
  char *destFile;
  int destMode;
} tCfgFileMap;

typedef struct _tMenuEntry
{
  char key;
  char *text;
  int idx;
} tMenuEntry;


extern char *cfg[numIdx];

char *ask(char *prompt, char *defaultValue);

// 0 if everything's allright
int askAllright();

// returns 0 if successfull
int processTemplate(char *templName, char *outName);

#endif

