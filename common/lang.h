/* lang.h - language headerfile */

#ifndef __LANG_H__
#define __LANG_H__

extern char *langDir;
extern char *configMenus;

#define numSystemConfigEntries 15
extern char *systemConfigTitle;
extern tMenuEntry systemConfigEntries[numSystemConfigEntries];

#define numPersonalConfigEntries 2
extern char *personalConfigTitle;
extern tMenuEntry personalConfigEntries[numPersonalConfigEntries];

#define numFidoDirConfigEntries 10
extern char *fidoDirConfigTitle;
extern tMenuEntry fidoDirConfigEntries[numFidoDirConfigEntries];

#define numCommConfigEntries 10
extern char *commConfigTitle;
extern tMenuEntry commConfigEntries[numCommConfigEntries];

#define numUplinkConfigEntries 5
extern char *uplinkConfigTitle;
extern tMenuEntry uplinkConfigEntries[numUplinkConfigEntries];

extern char *menuInvalidChoice;

extern char *descTexts[numIdx];

extern char *homeDirQuestion;
extern char *chooseBossText;
extern char *pressReturnText;
extern char *allrightText;
extern char *notFoundText;
extern char *pleaseInstallProgramsText;
extern char *pleaseCopyFilesText;
extern char *groupExistsText;
extern char *userExistsText;
extern char *createdGroupText;
extern char *createdUserText;
extern char *creatingUserAndGroupText;
extern char *creatingDirsText;
extern char *settingRightsText;
extern char *createdDirsText;
extern char *unzipSourcesText;
extern char *creatingMakefileCfgsText;
extern char *createdMakefileCfgsText;
extern char *compilingText;
extern char *compiledText;
extern char *installingText;
extern char *installedText;
extern char *pleaseExecuteAsRootText;
extern char *creatingCfgText;
extern char *createdCfgText;
extern char *configLdsoText;
extern char *LdsoEntryMadeText;
extern char *configLdsoDoneText;
extern char *creatingUserCfgText;
extern char *createdUserCfgText;
extern char *configProfileText;
extern char *configProfileDoneText;
extern char *copyingNodelistsText;
extern char *copiedNodelistsText;
extern char *copyingScriptsText;
extern char *copiedScriptsText;
extern char *compilingNodelistsText;
extern char *compiledNodelistsText;
extern char *installDoneText;
extern char *chdirErrorText;
extern char yesKey;
extern char noKey;

#endif

