/* header file common to all platforms and languages */

#ifndef __COMMON_H__
#define __COMMON_H__

#define VERSION "1.3"

#define nfree(a) { if (a != NULL) { free(a); a = NULL; } }

#define numZipFiles 10
char *zipFiles[numZipFiles];

#define numPrograms 2
char *programs[numPrograms];

#define numNodelists 2
char *nodelists[numNodelists];

#define groupNameIdx                        1
#define fidoNameIdx                         2
#define usersIdx                            3
#define libDirIdx                           4
#define binDirIdx                           5
#define manDirIdx                           6
#define cfgDirIdx                           7
#define logDirIdx                           8
#define incDirIdx                           9
#define debugIdx                           10
#define libcversionIdx                     11
#define locationIdx                        12
#define sysOpNameIdx                       13
#define workDirIdx                         14
#define inboundIdx                         15
#define protInboundIdx                     16
#define localInboundIdx                    17
#define tempInboundIdx                     18
#define tempOutboundIdx                    19
#define outboundIdx                        20
#define msgbaseDirIdx                      21
#define nodelistDirIdx                     22
#define netmailDirIdx                      23
#define scriptDirIdx                       24
#define isdnDevIdx                         25
#define modemDevIdx                        26
#define modemBaudIdx                       27
#define internatPrefixIdx                  28
#define localPrefixIdx                     29
#define voiceNumIdx                        30
#define dataNumIdx                         31
#define amtNumIdx                          32
#define localNumIdx                        33
#define internatNumIdx                     34
#define packerIdx                          35
#define pointNrIdx                         36
#define uplinkAddrIdx                      37
#define uplinkNameIdx                      38
#define uplinkPwdIdx                       39
#define homeDirIdx                         40
#define infoDirIdx                         41
#define htmlDirIdx                         42
#define numIdx                             42

typedef struct _tCfg
{
  char *groupName, *fidoName, *users, *userName;
  char *libDir, *binDir, *manDir, *cfgDir, *logDir, *incDir, *infoDir;
  char *htmlDir;
  char *debug;
  char *libcversion;
  char *location, *sysOpName;
  char *workDir, *inbound, *protInbound, *localInbound, *tempInbound;
  char *tempOutbound, *outbound, *msgbaseDir, *nodelistDir, *netmailDir;
  char *scriptDir;
  char *isdnDev, *modemDev;
  char *modemBaud;
  char *internatPrefix, *localPrefix, *voiceNum, *dataNum, *amtNum, *localNum;
  char *internatNum;
  char *packer;

  char *pointNr, *uplinkAddr, *uplinkName, *uplinkPwd;

  int groupId;
  int userId;
  char *homeDir;
} tCfg;


extern tCfg Cfg;

char *ask(char *prompt, char *defaultValue);

// 0 if everything's allright
int askAllright();

char *findVar(char *varName);

// returns 0 if successfull
int processTemplate(char *templName, char *outName);

#endif

