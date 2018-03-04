#include <msclr/marshal.h>
#include <windows.h>
#include "APKInfoExtractor.h"
#include"ApkInfo.h"
#include"XmlParser.h"

namespace APKInfoExtraction {


	APKInfoExtractor::APKInfoExtractor()
	{
		vulnerabilities = gcnew List<Vulnerability>();
	}
	void APKInfoExtractor::getInfoFromManifest(String^ apkPath, Boolean% backupFlag, Boolean% externalStorageFlag)
	{
		string command = "/C sh C:\\apkanalyzer\\script\\apkanalyzer manifest print ";
		msclr::interop::marshal_context context;
		command += context.marshal_as<const char*>(apkPath);
		command += " > C:\\GPTempDir\\manifest.xml";
		//system(command.c_str());
		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = "cmd";
		ShExecInfo.lpParameters = command.c_str();
		ShExecInfo.lpDirectory = "C:\\";
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);


	//	HINSTANCE retVal = ShellExecute(NULL, "open", "cmd", command.c_str(), "C:\\", SW_HIDE);
		XmlParser* xmlParser = new XmlParser("C:\\GPTempDir\\manifest.xml");
		xmlParser->grebBackupModeEnabledFlag();
		xmlParser->grebExternalStorageFlag();
		xmlParser->grebExportedActivities();
		xmlParser->grebExportedServices();
		xmlParser->grebExportedContentProviders();
		xmlParser->grebExportedBroadcastReceivers();
		backupFlag = xmlParser->getBackupFlag();
		externalStorageFlag = xmlParser->getExternalStorageFlag();
		if (backupFlag)
		{
			//create new vulnerability
			Vulnerability vul;
			vul.extraInfo = "Backup flag is enabled" ;
			vul.type = "static";
			vul.category = "Backup";
			vul.severity = 0.4;
			vulnerabilities->Add(vul);
		}
		if (externalStorageFlag)
		{
			//create new vulnerability
			Vulnerability vul;
			vul.extraInfo = "External Storage flag is enabled";
			vul.type = "static";
			vul.category = "External Storage";
			vul.severity = 0.4;
			vulnerabilities->Add(vul);
		}
		vector<string> tempExportedActivities = xmlParser->getExportedActivities();
		vector<string> tempExportedBroadcasts = xmlParser->getExportedBroadcastReceivers();
		vector<string> tempExportedContentProviders = xmlParser->getExportedContentProviders();
		vector<string> tempExportedServices = xmlParser->getExportedServices();
		delete xmlParser;
		cli::array<String^>^exportedActivities = gcnew cli::array<String^>(tempExportedActivities.size());
		cli::array<String^>^	exportedBroadCastReceivers = gcnew cli::array<String^>(tempExportedBroadcasts.size());
		cli::array<String^>^exportedContentProviders = gcnew cli::array<String^>(tempExportedContentProviders.size());
		cli::array<String^>^	exportedServices = gcnew cli::array<String^>(tempExportedServices.size());
		for (int i = 0; i < tempExportedActivities.size(); ++i)
		{
			String^ t = gcnew String(&tempExportedActivities[i][0]);
			exportedActivities[i] = t;
			//create new vulnerability
			Vulnerability vul;
			vul.extraInfo = "Exported Activity : "+t;
			vul.type = "static";
			vul.category = "Exported Component";
			vul.severity = 0.25;
			vulnerabilities->Add(vul);
		}
		for (int i = 0; i < tempExportedBroadcasts.size(); ++i)
		{
			String^ t = gcnew String(&tempExportedBroadcasts[i][0]);
			exportedBroadCastReceivers[i] = t;
			//create new vulnerability
			Vulnerability vul;
			vul.extraInfo = "Exported Broad Cast Receiver : " + t;
			vul.type = "static";
			vul.category = "Exported Component";
			vul.severity = 0.25;
			vulnerabilities->Add(vul);
		}
		for (int i = 0; i < tempExportedContentProviders.size(); ++i)
		{
			String^ t = gcnew String(&tempExportedContentProviders[i][0]);
			exportedContentProviders[i] = t;
			//create new vulnerability
			Vulnerability vul;
			vul.extraInfo = "Exported Content Provider : " + t;
			vul.type = "static";
			vul.category = "Exported Component";
			vul.severity = 0.25;
			vulnerabilities->Add(vul);
		}
		for (int i = 0; i < tempExportedServices.size(); ++i)
		{
			String^ t = gcnew String(&tempExportedServices[i][0]);
			exportedServices[i] = t;
			//create new vulnerability
			Vulnerability vul;
			vul.extraInfo = "Exported Service : " + t;
			vul.type = "static";
			vul.category = "Exported Component";
			vul.severity = 0.25;
			vulnerabilities->Add(vul);
		}
	}
	void APKInfoExtractor::getInfoFromApk(String^ apkPath, Boolean % debuggableFlag, Boolean % testFlag, cli::array<String^>^% launchableActivities,
		cli::array<String^>^% permissions, String^% versionName, String^% versionCode, String^% packageName, String^% minSDKVersion,
		String^% targetSDKVersion, SupportedArchitectures % supportedArchitectures)
	{
		
		
		string command = "/C C:\\apkanalyzer\\build-tools\\27.0.3\\aapt.exe dump badging ";
		msclr::interop::marshal_context context;
		command += context.marshal_as<const char*>(apkPath);
		command += " > C:\\GPTempDir\\apkInfoLines.txt";
		//system(command.c_str());
		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = "cmd";
		ShExecInfo.lpParameters = command.c_str();
		ShExecInfo.lpDirectory = "C:\\";
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);


		//HINSTANCE retVal = ShellExecute(NULL, "open","cmd", command.c_str(), "C:\\", SW_HIDE);
		ApkInfo* apkInfo = new ApkInfo("C:\\GPTempDir\\apkInfoLines.txt", true);
		debuggableFlag = apkInfo->getAppDebuggableFlag();
		if (debuggableFlag)
		{
			//create new vulnerability
			Vulnerability vul;
			vul.extraInfo = "Debug mode flag is enabled";
			vul.type = "static";
			vul.category = "Debug mode";
			vul.severity = 0.8;
			vulnerabilities->Add(vul);
		}
		testFlag = apkInfo->getTestOnlyFlag();
		vector<string> tempLaunchableActivities = apkInfo->getLaunchableActivities();
		vector<string> tempPermissions = apkInfo->getPermissions();
		permissions = gcnew cli::array<String^>(tempPermissions.size());
		launchableActivities = gcnew cli::array<String^>(tempLaunchableActivities.size());
		for (int i = 0; i < tempLaunchableActivities.size(); ++i)
		{
			String^ t = gcnew String(&tempLaunchableActivities[i][0]);
			launchableActivities[i] = t;
		}
		for (int i = 0; i < tempPermissions.size(); ++i)
		{
			String^ t = gcnew String(&tempPermissions[i][0]);
			permissions[i] = t;
		}

		versionName = gcnew String(&apkInfo->getVersionName()[0]);
		versionCode = gcnew String(&apkInfo->getVersionCode()[0]);
		packageName = gcnew String(&apkInfo->getPackageName()[0]);
		minSDKVersion = gcnew String(&apkInfo->getMinSDKVersion()[0]);
		targetSDKVersion = gcnew String(&apkInfo->getTargetSDKVersion()[0]);
		ApkInfo::SupportedArchi temp = apkInfo->getSupportedArchi();
		supportedArchitectures.all = temp.all;
		supportedArchitectures.arm64_v8a = temp.arm64_v8a;
		supportedArchitectures.armeabi = temp.armeabi;
		supportedArchitectures.armeabi_v7a = temp.armeabi_v7a;
		supportedArchitectures.mips = temp.mips;
		supportedArchitectures.mips64 = temp.mips64;
		supportedArchitectures.x86 = temp.x86;
		supportedArchitectures.x86_64 = temp.x86_64;
		delete apkInfo;
		
	
	}
}