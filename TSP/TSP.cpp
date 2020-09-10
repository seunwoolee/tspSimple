//============================================================================
// Name        : TSP.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
#include <iostream>
using namespace std;

#include "TSP"

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
*/

#include "stdafx.h"

//#include <pthread.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <stdlib.h>

//#include "atlstr.h"

#include <iostream>
#include "TspView.h"

using namespace std;
using namespace __gnu_cxx;


// Main Thread Function
void *t_function(void *pParam)
{
	int nResult = 0;
	TSPView *pTSPView = (TSPView *)pParam;
	//pTSPView->ConvertChar();

	// Step00 OnInit
	/*
	nResult = pTSPView->OnInit();
	if(nResult != 0){
		// OnLoadDeliveryData + nResult // OnLoadDeliveryData DB Error(100)
		printf("ResultCode : OnInit ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step00 OnInit Complete\n");
		fflush(stdout);
	}
	*/

	// Step01 OnLoadDeliveryData
	nResult = pTSPView->OnLoadDeliveryData();
	if(nResult != 0){
		printf("ResultCode : OnLoadDeliveryData ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step01 OnLoadDeliveryData Complete\n");
		fflush(stdout);
	}

	// Step02 OnSetDeliveryData
	nResult = pTSPView->OnSetDeliveryData();
	if(nResult != 0){
		printf("ResultCode : OnSetDeliveryData ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step02 OnSetDeliveryData Complete\n");
		fflush(stdout);
	}

	// Step03 OnSearchDeliveryData
	nResult = pTSPView->OnSearchDeliveryData();
	if(nResult != 0){
		printf("ResultCode : OnSearchDeliveryData ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step03 OnSearchDeliveryData Complete\n");
		fflush(stdout);
	}

	// Step04 OnRelocationDeliveryData
	nResult = pTSPView->OnRelocationDeliveryData();
	if(nResult != 0){
		printf("ResultCode : OnRelocationDeliveryData ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step04 OnRelocationDeliveryData Complete\n");
		fflush(stdout);
	}

	// Step08 OnVehicleDataInsert
	nResult = pTSPView->OnVehicleDataInsert();
	if(nResult != 0){
		printf("ResultCode : OnVehicleDataInsert ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step08 OnVehicleDataInsert Complete\n");
		fflush(stdout);
	}

	/*

	// Step05 OnCalcDeliveryData
	nResult = pTSPView->OnCalcDeliveryData();
	if(nResult != 0){
		printf("ResultCode : OnCalcDeliveryData ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step05 OnCalcDeliveryData Complete\n");
		fflush(stdout);
	}

	// Step06 OnResearchDeliveryData
	nResult = pTSPView->OnResearchDeliveryData();
	if(nResult != 0){
		printf("ResultCode : OnResearchDeliveryData ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step06 OnResearchDeliveryData Complete\n");
		fflush(stdout);
	}

	// Step07 OnRelocationVehicleData
	nResult = pTSPView->OnRelocationVehicleData();
	if(nResult != 0){
		printf("ResultCode : OnRelocationVehicleData ErrorCode[%d]\n", nResult);
		fflush(stdout);
	} else {
		printf("Step07 OnRelocationVehicleData Complete\n");
		fflush(stdout);
	}
	*/


}

wstring Utf8StringToWString( string in )
{
	wchar_t pOut[2048] = {0,};
	char pIn[2048]={0,};
	char *pIn2 = pIn;
	char *pOut2 = (char *)pOut;
	memcpy(pIn,in.c_str(),in.length());
	unsigned long int szIn = in.length();
	unsigned long int szOut = sizeof(pOut) /  sizeof(wchar_t);


	//iconv_t it = iconv_open("UTF-16LE", "UTF-8"); // EUC-KR을 UTF-8로
	iconv_t it = iconv_open("UTF-16", "UTF-8"); // EUC-KR을 UTF-8로
	int ret = iconv(it, &pIn2, &szIn, &pOut2, &szOut); //
	if (ret < 0) {
		char buf[1024] = {0,};
		int err = errno;
		strerror_r(errno, buf, sizeof(buf) );
		printf("ret : %d, errno : %d(%s)\n", ret, errno,buf);
		return wstring(L"");
	} else {

	}
	iconv_close(it);
	wstring result = wstring(pOut);
	return result;
}

//TSPView *ppTSPView;

int main(int argc, char *argv[])
{

	TSPView *pTSPView = new TSPView();

	pTSPView->pChrDeliveryDate = new char[64]();
	pTSPView->pChrMeridiemType = new char[64]();
	pTSPView->pChrMeridiemFlag = new char[64]();
	pTSPView->pChrLocationId   = new char[64]();
	pTSPView->pChrVehicleNo	   = new char[64]();
	pTSPView->nVehicleNumber = 0;

	strcpy(pTSPView->pChrDeliveryDate,	argv[1]);
	strcpy(pTSPView->pChrMeridiemType,	argv[2]);
	strcpy(pTSPView->pChrMeridiemFlag,	argv[3]);
	strcpy(pTSPView->pChrLocationId,	argv[4]);
	strcpy(pTSPView->pChrVehicleNo,		argv[5]);
	pTSPView->nVehicleNumber = atoi(argv[6]);


	pthread_t pThreadTsp;
	int thr_id;
	int a = 30;
	int status;

	//thr_id = pthread_create(&pThreadTsp, NULL, t_function, pTSPView);
	thr_id = pthread_create(&pThreadTsp, NULL, t_function, pTSPView);


	// wating for thread terminate
	pthread_join(pThreadTsp, (void **)&status);
	printf("Thread Normal End %d\n", status);
	fflush(stdout);

	delete[] pTSPView->pChrDeliveryDate;
	delete[] pTSPView->pChrMeridiemType;
	delete[] pTSPView->pChrMeridiemFlag;
	delete[] pTSPView->pChrVehicleNo;
	delete[] pTSPView->pChrLocationId;


	return 0;
}


bool dbConnectTest()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char *server = "14.63.197.114";
	char *user = "root";
	char *password = "ff323232";
	char *database = "jangbogo_db";

    conn = mysql_init(NULL);

    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
            exit(1);
    }
    if(mysql_query(conn,"show tables"))
    {
            exit(1);
    }

    res = mysql_use_result(conn);
    printf("MYSQL Tables in mysql database : \n");

    while((row = mysql_fetch_row(res)) != NULL)
            printf("%s \n",row[0]);

    if(mysql_query(conn,"SELECT * FROM R_TEST"))
    {
            return 1;
    }

    res = mysql_use_result(conn);

    printf("Returning List of Names : \n");
    while((row = mysql_fetch_row(res)) != NULL)
            printf("%s %s %s \n",row[0],row[1],row[2]);

    mysql_free_result(res);
    mysql_close(conn);

    return true;
}

bool dbClose()
{


	return true;
}

void onSave()
{

}


void fileSave()
{

	//_best

}




