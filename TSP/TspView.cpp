//============================================================================
// Name        : TSP.cpp
// Author      :
// Version     :
// Copyright   :
// Description : TSPS in C++, Ansi-style
//============================================================================


#include "stdafx.h"

#include "TspView.h"

//#include <iostream>
//#include <stdio.h>

int TSPView::_nSearchIndexStart = 1;

TSPView *TSPView::pInstance= NULL;

TSPView::TSPView(){

	//pTSPView = this;
	pInstance = this;

	_generation = 0;
	_fitness = 0;

	nVehicleFlg = new int[512]();				// 일단 32개 할당하자 넉넉하게 ㅅㅂ
	nVehiclePay = new int[512]();				// 일단 32개 할당하자 넉넉하게 ㅅㅂ
	nVehicleCnt = new int[512]();				// 일단 32개 할당하자 넉넉하게 ㅅㅂ


	//TSP::GetInstance().GetAlgorithm()->SubscribeObserver( this );
	TSP::GetInstance().GetAlgorithm()->SubscribeObserver( pInstance );

}

TSPView::~TSPView()
{

}


//TSPView::TSPView(){}
//TSPView::~TSPView(){}



/*
void TSPView::init()
{
	_generation = 0;
	_fitness = 0;
	TSP::GetInstance().GetAlgorithm()->SubscribeObserver( this );
	return;
}
*/

void TSPView::init()
{
	_generation = 0;
	_fitness = 0;
	TSP::GetInstance().GetAlgorithm()->SubscribeObserver( this );
}

void TSPView::StatisticUpdate(const GaStatistics& statistics, const GaAlgorithm& algorithm)
{
	_generation = statistics.GetCurrentGeneration();

	/*
	if( _generation % 1000 == 0 ){
		//
	}
	*/
	//Invalidate();
}

void TSPView::NewBestChromosome(const GaChromosome& newChromosome, const GaAlgorithm& algorithm)
{
	LOCK_THIS_OBJECT( lock );

	_bestChromosome.clear();
	_bestChromosome = dynamic_cast<const TspChromosome*>( &newChromosome )->GetCode();

	_fitness = newChromosome.GetFitness();

	UNLOCK( lock );

}

// Step00 - OnInit
int TSPView::OnInit()
{

	printf("Step00 OnInit Start\n");
	fflush(stdout);

	_generation = 0;
	_fitness = 0;

	nVehicleFlg = new int[512]();				// 일단 32개 할당하자 넉넉하게 ㅅㅂ
	nVehiclePay = new int[512]();				// 일단 32개 할당하자 넉넉하게 ㅅㅂ

	TSP::GetInstance().GetAlgorithm()->SubscribeObserver( this );


	/*
	printf("%s\n", pChrDeliveryDate);
	fflush(stdout);
	printf("%s\n", pChrMeridiemType);
	fflush(stdout);
	printf("%s\n", pChrLocationId);
	fflush(stdout);
	printf("%d\n", nVehicleNumber);
	fflush(stdout);
	*/

	printf("Step00 OnInit End\n");
	fflush(stdout);

	return 0;
}

// Step01 - OnLoadDeliveryData
int TSPView::OnLoadDeliveryData()
{
	printf("Step01 OnLoadDeliveryData Start\n");
	fflush(stdout);

	/*
	printf("%s\n", pChrDeliveryDate);
	fflush(stdout);
	printf("%s\n", pChrMeridiemType);
	fflush(stdout);
	printf("%s\n", pChrLocationId);
	fflush(stdout);
	printf("%d\n", nVehicleNumber);
	fflush(stdout);

	printf("OnLoadDeliveryData End\n");
	fflush(stdout);
	*/

	bComleteLoadDeliveryData = false;

	MYSQL_RES *resOrderData;	// 주문데이터 OrderData Result
	MYSQL_ROW rowOrderData;		// 주문데이터 OrderData Rows
	MYSQL_ROW rowOrderPay;		// 주문금액정보 Rows

	MYSQL_ROW rowOrderDataSe;	// 주문데이터 OrderData Rows


	DbConn = mysql_init(NULL);

	if (!mysql_real_connect(DbConn, MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DB, 0, NULL, 0)) {
		return 100;
	}

	char *pSqlMutualData = new char[2048]();	// 배열쿼리문 메모리할당
	char *pSqlOrderData = new char[2048]();		// 배열쿼리문 메모리할당
	char *pSqlOrderDataCnt = new char[2048]();	// 배열쿼리문 메모리할당

	char *pSqlOrderDataSe = new char[2048]();	// 배열쿼리문 메모리할당

	char *pSqlSetNames = new char[512]();		// 배열쿼리문 메모리할당
	char *pSqlOrderPay = new char[2048]();

	sprintf(pSqlSetNames, "set names euckr");
	if (mysql_query(DbConn, pSqlSetNames)) {
		return 100;
	}

	sprintf(pSqlOrderDataCnt
			//,"select count(*) from (SELECT count(A.ve_seq) AS dupleCount, A.ve_guestJuso, ve_guestLat, ve_guestLon, ve_accnoDupleJuso FROM (SELECT * FROM vehicleGuestOrderData WHERE ve_deliveryDate = '%s' AND ve_meridiemType = '%s' AND ve_meridiemFlag = '%s' AND ve_locationId = '%s' AND ve_guestJuso != '' AND ve_guestLat != '0' AND ve_guestLon != '0' GROUP by ve_accno ) AS A GROUP BY A.ve_guestLat, A.ve_guestLon ORDER BY A.ve_accno ) as B"
			//,"SELECT COUNT(*) FROM ( SELECT * FROM ( SELECT * FROM vehicleAllocateResult WHERE 1=1 AND vr_deliveryDate='%s' AND vr_meridiemType='%s' AND vr_meridiemFlag='%s' AND vr_vehicleNo='%s' AND vr_locationId='%s' GROUP BY vr_deguestAccno ) AS A GROUP BY A.vr_deguestLat, A.vr_deguestLon ORDER BY A.vr_deguestAccno ) AS B"
			,"SELECT COUNT(*) FROM ( SELECT * FROM ( SELECT * FROM vehicleAllocateResult WHERE 1=1 AND vr_deliveryDate='%s' AND vr_meridiemType='%s' AND vr_meridiemFlag='%s' AND vr_vehicleNo='%s' AND vr_locationId='%s' AND vr_errorJusoFlag != 'Y' GROUP BY vr_deguestAccno ) AS A GROUP BY A.vr_deguestLat, A.vr_deguestLon ORDER BY A.vr_deguestAccno ) AS B"
			,pChrDeliveryDate
			,pChrMeridiemType
			,pChrMeridiemFlag
			,pChrVehicleNo
			,pChrLocationId);

	sprintf(pSqlOrderData
			//,"SELECT * FROM (SELECT * FROM (SELECT * FROM vehicleGuestOrderData WHERE ve_deliveryDate = '%s' AND ve_meridiemType = '%s' AND ve_meridiemFlag = '%s' AND ve_locationId = '%s' AND ve_guestJuso != '' AND ve_guestLat != '0' AND ve_guestLon != '0' GROUP by ve_accno ) AS A GROUP BY A.ve_guestLat, A.ve_guestLon ORDER BY A.ve_accno ) AS B"
			//,"SELECT * FROM ( SELECT * FROM ( SELECT * FROM vehicleAllocateResult WHERE 1=1 AND vr_deliveryDate='%s' AND vr_meridiemType='%s' AND vr_meridiemFlag='%s' AND vr_vehicleNo='%s' AND vr_locationId='%s' GROUP BY vr_deguestAccno ) AS A GROUP BY A.vr_deguestLat, A.vr_deguestLon ORDER BY A.vr_deguestAccno ) AS B"
			,"SELECT *        FROM ( SELECT * FROM ( SELECT * FROM vehicleAllocateResult WHERE 1=1 AND vr_deliveryDate='%s' AND vr_meridiemType='%s' AND vr_meridiemFlag='%s' AND vr_vehicleNo='%s' AND vr_locationId='%s' AND vr_errorJusoFlag != 'Y' GROUP BY vr_deguestAccno ) AS A GROUP BY A.vr_deguestLat, A.vr_deguestLon ORDER BY A.vr_deguestAccno ) AS B"
			,pChrDeliveryDate
			,pChrMeridiemType
			,pChrMeridiemFlag
			,pChrVehicleNo
			,pChrLocationId); //아이코스

	if (mysql_query(DbConn, pSqlOrderDataCnt)) {
		return 100;
	}
	resOrderData = mysql_use_result(DbConn);

	rowOrderData = mysql_fetch_row(resOrderData);
	nCountOrderData = atoi(rowOrderData[0]);

	nVehiclePayIndex = new int[nCountOrderData]();	// (초기화) 각차량에 할당된 금액 - 경로갯수만큼 할당됨 - 데이터비교용

	mysql_free_result(resOrderData); // 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ

	_vectorOrderData.resize(nCountOrderData);	// 배송지데이터 리사이즈

	if (mysql_query(DbConn, pSqlOrderData)) {
		return 100;
	}
	resOrderData = mysql_use_result(DbConn);

	DbConn2nd = mysql_init(NULL);
	if (!mysql_real_connect(DbConn2nd, MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DB, 0, NULL, 0)) {
		return 100;
	}
	if (mysql_query(DbConn2nd, pSqlSetNames)) {
		return 100;
	}

	int nIndexOrder = 0;

	std::string ve_accno		= "";
	//std::string ve_locationId	= "";
	//std::string ve_delilveryData= "";
	//std::string ve_meridiemType = "";
	//std::string ve_meridiemFlag = "";
	std::string ve_guestId		= "";
	std::string ve_isShop		= "";

	while (rowOrderData = mysql_fetch_row(resOrderData)) {	// count 뽑긔 왜케 어렵냐..

		ve_accno	= rowOrderData[1];	// vr_deguestAccno
		ve_guestId	= rowOrderData[6];	// vr_deguestId
		ve_isShop	= rowOrderData[28];	// vr_deguestIsShop

		MYSQL_RES *resOrderDataSe;
		sprintf(pSqlOrderDataSe
				//,"select * from vehicleGuestOrderData where 1=1 and ve_accno='%s' and ve_locationId='%s' and ve_deliveryDate='%s' and ve_meridiemType='%s' and ve_meridiemFlag='%s' and ve_guestId='%s' and ve_isShop='%s'"
				,"select * from vehicleGuestOrderData where 1=1 and ve_accno='%s' and ve_locationId='%s' and ve_deliveryDate='%s' and ve_meridiemType='%s' and ve_meridiemFlag='%s' and ve_guestId='%s' and ve_isShop='%s'"
				,ve_accno.c_str()	// vr_deguestAccno
				,pChrLocationId		// vr_locationId
				,pChrDeliveryDate	// vr_deliveryDate
				,pChrMeridiemType	// vr_meridiemType
				,pChrMeridiemFlag	// vr_meridiemFlag
				,ve_guestId.c_str()	// vr_deguestId
				,ve_isShop.c_str()	// vr_deguestIsShop
			);

		if (mysql_query(DbConn2nd, pSqlOrderDataSe)) {
			return 0;
		}
		resOrderDataSe = mysql_use_result(DbConn2nd);
		rowOrderDataSe = mysql_fetch_row(resOrderDataSe);

		strcpy(_vectorOrderData[nIndexOrder].ve_seq, rowOrderDataSe[0]);				// ve_seq
		strcpy(_vectorOrderData[nIndexOrder].ve_accno, rowOrderDataSe[1]);			// ve_accno
		strcpy(_vectorOrderData[nIndexOrder].ve_locationId, rowOrderDataSe[2]);		// ve_locationId
		strcpy(_vectorOrderData[nIndexOrder].ve_deliveryDate, rowOrderDataSe[3]);		// ve_deliveryDate
		strcpy(_vectorOrderData[nIndexOrder].ve_meridiemType, rowOrderDataSe[4]);		// ve_meridiemType
		strcpy(_vectorOrderData[nIndexOrder].ve_guestId, rowOrderDataSe[5]);			// ve_guestId
		strcpy(_vectorOrderData[nIndexOrder].ve_guestName, rowOrderDataSe[6]);		// ve_guestName
		strcpy(_vectorOrderData[nIndexOrder].ve_guestTel, rowOrderDataSe[7]);			// ve_guestTel
		strcpy(_vectorOrderData[nIndexOrder].ve_guestJusoSubId, rowOrderDataSe[8]);	// ve_guestJusoSubId
		strcpy(_vectorOrderData[nIndexOrder].ve_guestJuso, rowOrderDataSe[9]);		// ve_guestJuso
		strcpy(_vectorOrderData[nIndexOrder].ve_pay, rowOrderDataSe[10]);				// ve_pay 밑에서 재정의합니다.
		strcpy(_vectorOrderData[nIndexOrder].ve_guestLat, rowOrderDataSe[11]);		// ve_guestLat
		strcpy(_vectorOrderData[nIndexOrder].ve_guestLon, rowOrderDataSe[12]);		// ve_guestLon
		strcpy(_vectorOrderData[nIndexOrder].ve_isNew, rowOrderDataSe[13]);			// ve_isNew
		strcpy(_vectorOrderData[nIndexOrder].ve_isShop, rowOrderDataSe[14]);			// ve_isShop
		strcpy(_vectorOrderData[nIndexOrder].ve_isRoad, rowOrderDataSe[15]);			// ve_isRoad
		strcpy(_vectorOrderData[nIndexOrder].ve_createDate, rowOrderDataSe[16]);		// ve_createDate
		strcpy(_vectorOrderData[nIndexOrder].ve_accnoDupleJuso, rowOrderDataSe[17]);	// ve_accnoDupleJuso
		nIndexOrder++;

		mysql_free_result(resOrderDataSe);	// free


	}

	mysql_free_result(resOrderData);

	delete[] pSqlOrderData;		// 주문데이터쿼리문 배열 메모리 해제
	pSqlOrderData = NULL;

	delete[] pSqlOrderDataCnt;	// 주문데이터카운터쿼리문 배열 메모리 해제
	pSqlOrderDataCnt = NULL;

	delete[] pSqlSetNames;
	pSqlSetNames = NULL;

	for (int i = 0; i < _vectorOrderData.size(); i++) {

		if (strcmp(_vectorOrderData[i].ve_accnoDupleJuso, "") != 0) {

			MYSQL_RES *resOrderPay;	// 주문금액정보 Result

			sprintf(pSqlOrderPay
				//, "SELECT SUM(ve_pay) FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate = '%s' AND ve_meridiemType = '%s' AND ve_meridiemFlag = '%s' AND ve_locationId = '%s' AND ve_accnoDupleJuso='%s'"
				//, "SELECT SUM(ve_pay) FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate = '%s' AND ve_meridiemType = '%s' AND ve_meridiemFlag = '%s' AND ve_locationId = '%s' AND ve_accnoDupleJuso='%s'"
				, "SELECT SUM(ve_pay) FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate = '%s' AND ve_meridiemType = '%s' AND ve_meridiemFlag = '%s' AND ve_locationId = '%s' AND ve_accnoDupleJuso='%s' AND ve_errorJusoFlag !='Y'"
				, pChrDeliveryDate
				, pChrMeridiemType
				, pChrMeridiemFlag
				, pChrLocationId
				, _vectorOrderData[i].ve_accnoDupleJuso);

			if (mysql_query(DbConn, pSqlOrderPay)) {
				return 100;
			}
			resOrderPay = mysql_use_result(DbConn);
			rowOrderPay = mysql_fetch_row(resOrderPay);
			strcpy(_vectorOrderData[i].ve_pay, rowOrderPay[0]);				// ve_pay
			mysql_free_result(resOrderPay); // 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ
		}
	}

	delete[] pSqlOrderPay;
	pSqlOrderPay = NULL;

	int nIndexMutual = 0;

	MYSQL_ROW rowMutualData;		// 상대거리데이터 MutualData Rows
									// 1. 자기자신의 거리를 제외하고 검색
									// 2. 크기는 nCountOrderData -1 값

	for (int i = 0; i < nCountOrderData; i++) {

		vector<VectorMutualData> tmpVectorMutualData;
		_vectorMutualData.push_back(tmpVectorMutualData);
		_vectorMutualData[i].resize(nCountOrderData);			// [i] 배열 리사이즈

		nIndexMutual = 0;// 초기화

		for (int j = 0; j < nCountOrderData; j++) {

			MYSQL_RES *resMutualData;		// 상대거리데이터 MutualData Result
			sprintf(pSqlMutualData
						, "SELECT	 vd_seq \
									,vd_guestId \
									,vd_guestJusoSubId \
									,vd_deguestId \
									,vd_deguestJusoSubId \
									,vd_guestLat \
									,vd_guestLon \
									,vd_deguestLat \
									,vd_deguestLon \
									,vd_distanceValue \
									,vd_guestDate \
									,vd_deguestDate \
									,vd_guestIsShop \
									,vd_deguestIsShop \
									,vd_createDate \
									,vd_updateDate \
										FROM vehicleGuestMutualDistance WHERE 1 = 1 \
												AND vd_guestId = '%s' \
												AND vd_guestJusoSubId = '%s' \
												AND vd_guestIsShop='%s' \
												AND vd_deguestId = '%s' \
												AND vd_deguestJusoSubId = '%s' \
												AND vd_deguestIsShop='%s'"
						, _vectorOrderData[i].ve_guestId
						, _vectorOrderData[i].ve_guestJusoSubId
						, _vectorOrderData[i].ve_isShop
						, _vectorOrderData[j].ve_guestId
						, _vectorOrderData[j].ve_guestJusoSubId
						, _vectorOrderData[j].ve_isShop );

			if (mysql_query(DbConn, pSqlMutualData)) {
				return 100;
			}

			resMutualData = mysql_use_result(DbConn);
			if ((rowMutualData = mysql_fetch_row(resMutualData)) != NULL) {

				strcpy(_vectorMutualData[i][nIndexMutual].vd_seq, rowMutualData[0]);				// vd_seq
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestId, rowMutualData[1]);			// vd_guestId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestJusoSubId, rowMutualData[2]);		// vd_guestJusoSubId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestId, rowMutualData[3]);			// vd_deguestId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestJusoSubId, rowMutualData[4]);	// vd_deguestJusoSubId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestLat, rowMutualData[5]);			// vd_guestLat
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestLon, rowMutualData[6]);			// vd_guestLon
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestLat, rowMutualData[7]);			// vd_deguestLat
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestLon, rowMutualData[8]);			// vd_deguestLon
				strcpy(_vectorMutualData[i][nIndexMutual].vd_distanceValue, rowMutualData[9]);		// vd_distanceValue
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestDate, rowMutualData[10]);			// vd_guestDate
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestDate, rowMutualData[11]);		// vd_deguestDate
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestIsShop, rowMutualData[12]);		// vd_guestIsShop
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestIsShop, rowMutualData[13]);		// vd_deguestIsShop
				strcpy(_vectorMutualData[i][nIndexMutual].vd_createDate, rowMutualData[14]);		// vd_createDate
				strcpy(_vectorMutualData[i][nIndexMutual].vd_updateDate, rowMutualData[15]);		// vd_updateDate

			} else {

				strcpy(_vectorMutualData[i][nIndexMutual].vd_seq, "");								// vd_seq
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestId, "");							// vd_guestId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestJusoSubId, "");					// vd_guestJusoSubId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestId, "");						// vd_deguestId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestJusoSubId, "");					// vd_deguestJusoSubId
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestLat, "");							// vd_guestLat
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestLon, "");							// vd_guestLon
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestLat, "");						// vd_deguestLat
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestLon, "");						// vd_deguestLon
				strcpy(_vectorMutualData[i][nIndexMutual].vd_distanceValue, "");					// vd_distanceValue
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestDate, "");						// vd_guestDate
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestDate, "");						// vd_deguestDate
				strcpy(_vectorMutualData[i][nIndexMutual].vd_guestIsShop, "");						// vd_guestIsShop
				strcpy(_vectorMutualData[i][nIndexMutual].vd_deguestIsShop, "");					// vd_deguestIsShop
				strcpy(_vectorMutualData[i][nIndexMutual].vd_createDate, "");						// vd_createDate
				strcpy(_vectorMutualData[i][nIndexMutual].vd_updateDate, "");						// vd_updateDate

			}
			nIndexMutual++;
			mysql_free_result(resMutualData);	// 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ

		} // End For

	} // End For

	delete[] pSqlMutualData;	// 상대거리정보쿼리문 배열 메모리 해제
	pSqlMutualData = NULL;

	mysql_close(DbConn);

	printf("Step01 OnLoadDeliveryData End\n");
	fflush(stdout);

	return 0;

}

// Step02 - OnSetDeliveryData
int TSPView::OnSetDeliveryData()
{
	printf("Step02 OnSetDeliveryData Start\n");
	fflush(stdout);

	std::wstring stdwstr;

	for (int i = 0; i < nCountOrderData; i++) {

		stdwstr = L"point";
		LOCK_THIS_OBJECT(lock);

		std::wstring stdwstrGuestId(_vectorOrderData[i].ve_guestId, _vectorOrderData[i].ve_guestId + strlen(_vectorOrderData[i].ve_guestId));							// guestId
		std::wstring stdwstrJusoSubId(_vectorOrderData[i].ve_guestJusoSubId, _vectorOrderData[i].ve_guestJusoSubId + strlen(_vectorOrderData[i].ve_guestJusoSubId));	// guestJusoSubId
		std::wstring stdwstrIsShop(_vectorOrderData[i].ve_isShop, _vectorOrderData[i].ve_isShop + strlen(_vectorOrderData[i].ve_isShop));								// isShop
		std::wstring stdwstrAccno(_vectorOrderData[i].ve_accno, _vectorOrderData[i].ve_accno + strlen(_vectorOrderData[i].ve_accno));									// accno
		std::wstring stdwstrAccnoDuple(_vectorOrderData[i].ve_accnoDupleJuso, _vectorOrderData[i].ve_accnoDupleJuso + strlen(_vectorOrderData[i].ve_accnoDupleJuso));	// accnoDuple

		//std::wstring stdwstrGuestName(_vectorOrderData[i].ve_guestName, _vectorOrderData[i].ve_guestName + strlen(_vectorOrderData[i].ve_guestName));
		//ConvertChar(_vectorOrderData[i].ve_guestName);

		//std::string strName(_vectorOrderData[i].ve_guestName);
		//std::wstring stdwstrGuestName = Utf8StringToWString(strName);

		//stdwstrGuestName = Func_MultiToWide((const char*)_vectorOrderData[i].ve_guestName, strlen((char*)_vectorOrderData[i].ve_guestName), CP_ACP);					// guestName
		//stdwstrGuestName = _vectorOrderData[i].ve_guestName;

		std::wstring stdwstrGuestTel(_vectorOrderData[i].ve_guestTel, _vectorOrderData[i].ve_guestTel + strlen(_vectorOrderData[i].ve_guestTel));						// guestTel

		//std::wstring stdwstrGuestJuso;
		//stdwstrGuestJuso = Func_MultiToWide((const char*)_vectorOrderData[i].ve_guestJuso, strlen((char*)_vectorOrderData[i].ve_guestJuso), CP_ACP);					// guestJuso
		//ConvertChar(_vectorOrderData[i].ve_guestJuso); // <~~~~~~~~~~~ !!
		//stdwstrGuestJuso = Utf8StringToWString(_vectorOrderData[i].ve_guestJuso);

		//std::string strJuso(_vectorOrderData[i].ve_guestJuso);
		//std::wstring stdwstrJuso = Utf8StringToWString(strJuso);

		std::wstring stdwstrPay(_vectorOrderData[i].ve_pay, _vectorOrderData[i].ve_pay + strlen(_vectorOrderData[i].ve_pay));											// pay

		std::wstring stdwstrGuestLat(_vectorOrderData[i].ve_guestLat, _vectorOrderData[i].ve_guestLat + strlen(_vectorOrderData[i].ve_guestLat));						// guestLat
		std::wstring stdwstrGuestLon(_vectorOrderData[i].ve_guestLon, _vectorOrderData[i].ve_guestLon + strlen(_vectorOrderData[i].ve_guestLon));						// guestLon

		TspCities::GetInstance().AddCity(stdwstr
										,atof(_vectorOrderData[i].ve_guestLat)
										,atof(_vectorOrderData[i].ve_guestLon)
										,stdwstrGuestId
										,stdwstrIsShop
										,stdwstrJusoSubId
										,stdwstrAccno
										,stdwstrAccnoDuple
										,L"guestName"
										,stdwstrGuestTel
										,L"guestJuso"
										,stdwstrPay
										,stdwstrGuestLat
										,stdwstrGuestLon
										);

		_bestChromosome.clear();

		UNLOCK(lock);
	}

	printf("Step02 OnSetDeliveryData End\n");
	fflush(stdout);
	return 0;
}

// Step03 - OnSearchDeliveryData
int TSPView::OnSearchDeliveryData()
{
	printf("Step03 OnSearchDeliveryData Start\n");
	fflush(stdout);

	//TspCities::nGenerationLimit = 200000;	// GenerationLimit 재설정
	TspCities::nGenerationLimit = 70000;	// GenerationLimit 재설정

	TSP::GetInstance().smartInit();

	LOCK(_algControlSect);

	bool resume = TSP::GetInstance().GetAlgorithm()->GetState() == GAS_PAUSED;

	TSP::GetInstance().GetAlgorithm()->StartSolving(resume);

	UNLOCK(_algControlSect);

	while (1) {

		if (TSP::GetInstance().GetAlgorithm()->GetState() == GAS_CRITERIA_STOPPED) {
			break;
		}
		sleep(2);
	}

	printf("Step03 OnSearchDeliveryData End\n");
	fflush(stdout);

	return 0;
}

// Step04 - OnRelocationDeliveryData
int TSPView::OnRelocationDeliveryData()
{
	printf("Step04 OnRelocationDeliveryData Start\n");
	fflush(stdout);

	// 배열갯수 초기화
	int nBestChromosomeSize = 0;
	nBestChromosomeSize = _bestChromosome.size();

	// Relocation 에 값복사
	_bestRelocation = _bestChromosome;

	int nFirstIndex = 0;// 첫출발지(거점 위치)

						// 출발지 index 찾긔
	if (_bestChromosome.size()) {

		for (int i = 0; i<_bestChromosome.size(); i++) {
			const TspCity* c = _bestChromosome[i];
			if (c->GetID() == 1) {
				nFirstIndex = i;
				break;
			}
		}
	}

	if (_bestRelocation.size()) {

		int nIndex = 0;

		// 1번 출발지(거점)위치부터 ~ 마지막까지 저장
		for (int i = nFirstIndex; i < _bestChromosome.size(); i++) {
			_bestRelocation[nIndex] = _bestChromosome[i];
			nIndex++;
		}

		// 배열0번부터 ~ 출발지(거점)위치까지 저장
		for (int i = 0; i < nFirstIndex; i++) {
			_bestRelocation[nIndex] = _bestChromosome[i];
			nIndex++;
		}

		// 순서대로 재배열된 경로값 백업( 저장 )
		ChromosomeCopy tmpChromosomeCopy;
		for (int i = 0; i < (int)_bestChromosome.size(); i++) {

			tmpChromosomeCopy._id			= _bestRelocation[i]->GetID();			//
			tmpChromosomeCopy._name			= _bestRelocation[i]->GetName();		//
			tmpChromosomeCopy._x			= _bestRelocation[i]->GetX();			//
			tmpChromosomeCopy._y			= _bestRelocation[i]->GetY();			//
			tmpChromosomeCopy._guestId		= _bestRelocation[i]->GetGuestId();		//
			tmpChromosomeCopy._isShop		= _bestRelocation[i]->GetIsShop();		//
			tmpChromosomeCopy._jusoSubId	= _bestRelocation[i]->GetJusoSubId();	//
			tmpChromosomeCopy._accno		= _bestRelocation[i]->GetAccno();		// accno 추가
			tmpChromosomeCopy._accnoDuple	= _bestRelocation[i]->GetAccnoDuple();	// accnoDuple 추가

			tmpChromosomeCopy._guestName	= _bestRelocation[i]->GetGuestName();	// guestName 추가
			tmpChromosomeCopy._guestTel		= _bestRelocation[i]->GetGuestTel();	// guestTel 추가
			tmpChromosomeCopy._guestJuso	= _bestRelocation[i]->GetGuestJuso();	// guestJuso 추가
			tmpChromosomeCopy._pay			= _bestRelocation[i]->GetPay();			// pay 추가

			tmpChromosomeCopy._guestLat		= _bestRelocation[i]->GetGuestLat();	// guestLat 추가
			tmpChromosomeCopy._guestLon		= _bestRelocation[i]->GetGuestLon();	// guestLon 추가

			_bestChromosomeCopy.push_back(tmpChromosomeCopy);
		}
	}

	printf("Step04 OnRelocationDeliveryData End\n");
	fflush(stdout);

	return 0;
}

// Step05 - OnCalcDeliveryData
int TSPView::OnCalcDeliveryData()
{
	printf("Step05 OnCalcDeliveryData Start\n");
	fflush(stdout);

	MYSQL_RES *resOrderData;	// 주문데이터 OrderData Result
	MYSQL_ROW rowOrderData;		// 주문데이터 OrderData Rows
	MYSQL_ROW rowSumData;		// 주문데이터 OrderData Rows

	DbConn = mysql_init(NULL);

	if (!mysql_real_connect(DbConn, MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DB, 0, NULL, 0)) {
		return 500;
	}

	char *pSqlOrderData = new char[1024]();		// 쿼리 char 생성 및 초기화!

	sprintf(pSqlOrderData
		, "SELECT SUM(ve_pay) FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate='%s' AND ve_meridiemType='%s' AND ve_meridiemFlag='%s' AND ve_locationId='%s' AND ve_isJuso = 'Y'"
		, pChrDeliveryDate
		, pChrMeridiemType
		, pChrMeridiemFlag
		, pChrLocationId);

	if (mysql_query(DbConn, pSqlOrderData)) {
		return 500;
	}
	resOrderData = mysql_use_result(DbConn);
	rowOrderData = mysql_fetch_row(resOrderData);

	mysql_free_result(resOrderData); // 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ

	int nTmpSum = 0; // 루프 누적
	int nTmpSumIndex = 0; // 증가 누적

	for (int i = 0; i < _bestChromosomeCopy.size(); i++) {



		MYSQL_RES *resSumData;	// 주문데이터 OrderData Result

		std::string strIsShop = "";
		strIsShop.assign(_bestChromosomeCopy[i]._isShop.begin(), _bestChromosomeCopy[i]._isShop.end());

		if (_bestChromosomeCopy[i]._accnoDuple == L"") {

			std::string strAccno = "";
			strAccno.assign(_bestChromosomeCopy[i]._accno.begin(), _bestChromosomeCopy[i]._accno.end());
			sprintf(pSqlOrderData
					,"SELECT SUM(ve_pay) AS paySum FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate='%s' AND ve_meridiemType='%s' AND ve_meridiemFlag='%s' AND ve_locationId='%s' AND ve_accno='%s' AND ve_isShop='%s' AND ve_isJuso = 'Y'"
					,pChrDeliveryDate
					,pChrMeridiemType
					,pChrMeridiemFlag
					,pChrLocationId
					,strAccno.c_str()	//_bestChromosomeCopy[i]._accno.c_str()	// required Charset Check
					,strIsShop.c_str()	//_bestChromosomeCopy[i]._isShop.c_str()	// required Charset Check
					);
		} else {
			std::string strAccnoDuple = "";
			strAccnoDuple.assign(_bestChromosomeCopy[i]._accnoDuple.begin(), _bestChromosomeCopy[i]._accnoDuple.end());
			sprintf(pSqlOrderData
					,"SELECT SUM(ve_pay) AS paySum FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate='%s' AND ve_meridiemType='%s' AND ve_meridiemFlag='%s' AND ve_locationId='%s' AND ve_accnoDupleJuso='%s' AND ve_isJuso = 'Y'"
					,pChrDeliveryDate
					,pChrMeridiemType
					,pChrMeridiemFlag
					,pChrLocationId
					,strAccnoDuple.c_str()	//_bestChromosomeCopy[i]._accnoDuple.c_str()		// required Charset Check
					,strIsShop.c_str()		//_bestChromosomeCopy[i]._isShop.c_str()			// required Charset Check
					);
		} // End If

		if (mysql_query(DbConn, pSqlOrderData)) {
			return 500;
		}
		resSumData = mysql_use_result(DbConn);
		rowSumData = mysql_fetch_row(resSumData);

		nTmpSum += atoi(rowSumData[0]);		// 금얙합산 중간값(누적값) 루프누적
		nTmpSumIndex += atoi(rowSumData[0]);	// 금얙합산 중간값(누적값) 증가누적

		nVehiclePayIndex[i] = atoi(rowOrderData[0]);	// 경로가 130개이면, 0번배열부터 ~ 129배열까지, 한코스당 할당된 누적금액을 배열에 저장함

		mysql_free_result(resSumData); // 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ

	} // End For

	mysql_close(DbConn);


	// 금액합산이 평균값보다 크면 배차 구분 플래그 지정
	//if (nTmpSum > pCChildView->nPayMod) {
	/*************************************************************/

	int nTmp = 0;
	int nCount = _bestChromosomeCopy.size();

	int nInnerIndex = 0;

	int nBasePay = 1000;	// 증가값
	int nBaseCheckPay = 10000;
	int nCheckPay = 10000;	//

	//int nAccuPay = 10000.0; // 가중치  1만원 단위로 증가 및 감소
	//int nAccuPay = 5000.0; // 가중치  1만원 단위로 증가 및 감소
	//int nAccuPay = 4500.0; // 가중치  1만원 단위로 증가 및 감소
	//int nAccuPay = 2000.0; // 가중치  1만원 단위로 증가 및 감소
	int nAccuPay = 2000.0; // 가중치  1만원 단위로 증가 및 감소

	int nBaseHighLimit = 2000000.0;

	int nAvg = (nTmpSum / nVehicleNumber);
	nAvg = (nAvg * 0.9);
	int nIndex = 0;
	bool bFind = false;

	bool bCheckComplete = true;		//  최종완료상태

	char *chrMessage = new char[1024]();

	int nLoopCnt = 0;

	while (bCheckComplete) {

		nTmp = 0;
		nIndex = 0;
		nInnerIndex = 0;

		for (int i = 0; i <nCount; i++) {

			nTmp += nVehiclePayIndex[i];

			//if (nTmp > nAvg) {
			//if ( (nTmp > nAvg) || (nInnerIndex >= 10)) {
			if ( (nInnerIndex >= 10) || (nTmp > nAvg)) {
				nVehicleFlg[nIndex] = i;
				nVehiclePay[nIndex] = nTmp;
				nTmp = 0;
				nIndex++;
				nInnerIndex=0;
			} else {
				nVehicleFlg[nIndex] = i;
				nVehiclePay[nIndex] = nTmp;
				nInnerIndex++;
			}
			//usleep(100);

		}

		if (nIndex > (nVehicleNumber - 1)) {
			// 초과
			//nAvg = (nAvg + nAccuPay);
			nAvg += nAccuPay;
			nVehicleFlg = new int[512]();				// more nVehicleNumber then Reset
			nVehiclePay = new int[512]();				// more nVehicleNumber then Reset
			sprintf(chrMessage, "Step05 OnCalcDeliveryData more nVehicleNumber then Reset nAvg[%d]\n",  nAvg);
			printf(chrMessage);
			fflush(stdout);
		} else if (nIndex < (nVehicleNumber - 1)) {
			// 미달
			//nAvg = (nAvg - nAccuPay);
			nAvg -= nAccuPay;
			nVehicleFlg = new int[512]();				// less nVehicleNumber then Reset
			nVehiclePay = new int[512]();				// less nVehicleNumber then Rest
			sprintf(chrMessage, "Step05 OnCalcDeliveryData less nVehicleNumber then Reset nAvg[%d]\n",  nAvg);
			printf(chrMessage);
			fflush(stdout);
		} else {

			if (nVehiclePay[nIndex] == 0) {
				//nAvg = (nAvg - nAccuPay);
				nAvg -= nAccuPay;
				sprintf(chrMessage, "Step05 OnCalcDeliveryData equal Reset nAvg[%d]\n",  nAvg);
				printf(chrMessage);
				fflush(stdout);
			} else {
				//bCheckComplete = FALSE;
				break;
			}
		}

		//sleep(1);
		usleep(150);

		if(nLoopCnt >= 130){
			break;
		}
		nLoopCnt++;

	} // End While

	delete[] pSqlOrderData;	// 메모리 해제
	delete[] chrMessage;

	if(nLoopCnt >= 130){
		printf("Step05 OnCalcDeliveryData ReCall\n");
		fflush(stdout);
		OnCalcDeliveryData();
	}

	printf("Step05 OnCalcDeliveryData End\n");
	fflush(stdout);

	return 0;
}

int TSPView::OnResearchDeliveryReady()
{




	return 0;
}


// Step06 - OnResearchDeliveryData
int TSPView::OnResearchDeliveryData()
{
	printf("Step06 OnResearchDeliveryData Start\n");
	fflush(stdout);

	TspCities::nGenerationLimit = 70000;	// GenerationLimit 재설정
	TSP::GetInstance().smartInit();

	if (TSP::GetInstance().GetAlgorithm()->GetState() == GAS_CRITERIA_STOPPED) {

		bool bSearchEndStatus = true;		// 최종 검색 완료 여부
		bool bSearchEndStatusStep = true;	// 순차 검색 완료 여부

		int nIndex = 0;

		_nSearchIndexStart = 1;

		_bestRouteEachResult.erase(_bestRouteEachResult.begin(), _bestRouteEachResult.end()); // 초기화

		for (int j = 0; j<nVehicleNumber; j++) {

			bSearchEndStatusStep = true;
			bSearchEndStatus = true;

			while (bSearchEndStatus) {

				if (bSearchEndStatusStep == true) {

					bSearchEndStatusStep = false;
					routeBestSearch(_nSearchIndexStart, nVehicleFlg[j]);
				}

				if (TSP::GetInstance().GetAlgorithm()->GetState() == GAS_CRITERIA_STOPPED) {

					bSearchEndStatus = false;

					vector<RouteEachResult> tmpRouteEachResult;
					_bestRouteEachResult.push_back(tmpRouteEachResult);
					_bestRouteEachResult[j].resize(_bestChromosome.size());

					for (int i = 0; i < _bestChromosome.size(); i++) {						// 검색완료되면 데이터 저장

						const TspCity * c = _bestChromosome[i];
						_bestRouteEachResult[j][i]._id			= c->GetID();
						_bestRouteEachResult[j][i]._name		= c->GetName();
						_bestRouteEachResult[j][i]._x			= c->GetX();
						_bestRouteEachResult[j][i]._y			= c->GetY();
						_bestRouteEachResult[j][i]._guestId		= c->GetGuestId();
						_bestRouteEachResult[j][i]._isShop		= c->GetIsShop();
						_bestRouteEachResult[j][i]._jusoSubId	= c->GetJusoSubId();
						_bestRouteEachResult[j][i]._accno		= c->GetAccno();		// accno 추가
						_bestRouteEachResult[j][i]._accnoDuple	= c->GetAccnoDuple();	// accnoDuple 추가

						_bestRouteEachResult[j][i]._guestName	= c->GetGuestName();	// guestName 추가
						_bestRouteEachResult[j][i]._guestTel	= c->GetGuestTel();		// guestTel 추가
						_bestRouteEachResult[j][i]._guestJuso	= c->GetGuestJuso();	// guestJuso 추가
						_bestRouteEachResult[j][i]._pay			= c->GetPay();			// pay 추가

						_bestRouteEachResult[j][i]._guestLat	= c->GetGuestLat();		// guestLat 추가
						_bestRouteEachResult[j][i]._guestLon	= c->GetGuestLon();		// guestLon 추가

						nIndex++;
					}
				}

				sleep(1);

			} // End While

			_nSearchIndexStart = (nVehicleFlg[j] + 1);	//

		} // End for

	} else {
		printf("Step06 Start Error");
		fflush(stdout);
	}

	printf("Step06 OnResearchDeliveryData End\n");
	fflush(stdout);

	return 0;
}


void TSPView::routeBestSearch(int nRefIndexStart, int nRefIndexStop)
{
	printf("Step06 routeBestSearch Start\n");
	fflush(stdout);

	_generation = 0;
	_fitness = 0;

	TSP::GetInstance().GetAlgorithm()->SubscribeObserver(this);
	TspCities::GetInstance().Clear();

	std::wstring wstrGuestId;		// guestId
	std::wstring wstrJusoSubId;		// jusoSubId
	std::wstring wstrIsShop;
	wstrGuestId = L"admin";
	wstrJusoSubId = L"1";
	wstrIsShop = L"0";

	const TspCity* c = _bestChromosome[0];
	c->ResetNextId();	// id 초기화


	LOCK_THIS_OBJECT(lock);
	TspCities::GetInstance().AddCityExt(_bestChromosomeCopy[0]._name
										,_bestChromosomeCopy[0]._x
										,_bestChromosomeCopy[0]._y
										,wstrGuestId							// guestId
										,wstrIsShop								// isShop
										,wstrJusoSubId							// JusoSubId
										,_bestChromosomeCopy[0]._id				// originId 추가들어감
										,_bestChromosomeCopy[0]._accno			// accno
										,_bestChromosomeCopy[0]._accnoDuple		// accnoDuple
										,_bestChromosomeCopy[0]._guestName		// guestName
										,_bestChromosomeCopy[0]._guestTel		// guestTel
										,_bestChromosomeCopy[0]._guestJuso		// guestJuso
										,_bestChromosomeCopy[0]._pay			// pay
										,_bestChromosomeCopy[0]._guestLat		// guestLat
										,_bestChromosomeCopy[0]._guestLon		// guestLon
										);
	_bestChromosome.clear();
	UNLOCK(lock);

	bool bWhileFlg = true;
	int nWhileCnt = 0;
	int nIndexAdd = nRefIndexStart;

	int nIndexEnd = 0;

	for (int i = nRefIndexStart; i <= nRefIndexStop; i++) {


		LOCK_THIS_OBJECT(lock);
		TspCities::GetInstance().AddCityExt(_bestChromosomeCopy[i]._name
											, _bestChromosomeCopy[i]._x
											, _bestChromosomeCopy[i]._y
											, _bestChromosomeCopy[i]._guestId		// guestId
											, _bestChromosomeCopy[i]._isShop		// isShop
											, _bestChromosomeCopy[i]._jusoSubId		// JusoSubId
											, _bestChromosomeCopy[i]._id			// originId 추가들어감
											, _bestChromosomeCopy[i]._accno			// accno
											, _bestChromosomeCopy[i]._accnoDuple	// accnoDuple
											, _bestChromosomeCopy[i]._guestName		// guestName
											, _bestChromosomeCopy[i]._guestTel		// guestTel
											, _bestChromosomeCopy[i]._guestJuso		// guestJuso
											, _bestChromosomeCopy[i]._pay			// pay
											, _bestChromosomeCopy[i]._guestLat		// lat
											, _bestChromosomeCopy[i]._guestLon		// lon
											);
		UNLOCK(lock);

	} // End For

	_bestChromosome.erase(_bestChromosome.begin(), _bestChromosome.end());
	_bestChromosome.clear();

	TspCities::nGenerationLimit		= 20000;	// GenerationLimit 재설정
	TSP::GetInstance().smartInit();

	LOCK(_algControlSect);

	bool resume = TSP::GetInstance().GetAlgorithm()->GetState() == GAS_PAUSED;
	TSP::GetInstance().GetAlgorithm()->StartSolving(resume);

	UNLOCK(_algControlSect);

	printf("Step06 routeBestSearch End\n");
	fflush(stdout);
}

// Step07 - OnRelocationVehicleData
int TSPView::OnRelocationVehicleData()
{
	printf("Step07 OnRelocationVehicleData Start\n");
	fflush(stdout);

	int nFirstIndex;
	int nIndex;

	for (int i = 0; i < nVehicleNumber; i++) {

		nFirstIndex = 0;
		for (int j = 0; j < _bestRouteEachResult[i].size(); j++) {
			if (_bestRouteEachResult[i][j]._id == 1) {
				nFirstIndex = j;
				break;
			}
		}

		vector<RouteEachResult> tmpRouteEachResult;							// 일케 해도됨
		_bestRouteEachRelocation.push_back(tmpRouteEachResult);				// push_back 으로 밀어 놓고
		_bestRouteEachRelocation[i].resize(_bestRouteEachResult[i].size());	// 2차배열 사이즈 제조정함

		nIndex = 0;
		for (int k = nFirstIndex; k < _bestRouteEachResult[i].size(); k++) {
			_bestRouteEachRelocation[i][nIndex] = _bestRouteEachResult[i][k];
			nIndex++;
		}
		for (int l = 0; l < nFirstIndex; l++) {
			_bestRouteEachRelocation[i][nIndex] = _bestRouteEachResult[i][l];
			nIndex++;
		}

	}

	printf("Step07 OnRelocationVehicleData End\n");
	fflush(stdout);

	return 0;
}

// Step08 - OnVehicleDataInsert
int TSPView::OnVehicleDataInsert()
{
	printf("Step08 OnVehicleDataInsert Start\n");
	fflush(stdout);

	MYSQL_ROW rowMutualData;	//
	MYSQL_ROW rowVehicleDataInsert;	//

	int nMaxIndex = 0;

	DbConn = mysql_init(NULL);

	if (!mysql_real_connect(DbConn, MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DB, 0, NULL, 0)) {
		return 0;
	}

	char *pSqlMutualData = new char[2048]();			// 정보조회용 - select
	char *pSqlVehicleDataInsert = new char[404800]();	// 정보입력용 - insert
	char *pJsonData = new char[202400]();				// Json Data용
	char *pSqlSetNames = new char[512]();				// 배열쿼리문 메모리할당
	int nDistance = 0;

	// Set Names Euckr
	sprintf(pSqlSetNames, "set names euckr");
	if (mysql_query(DbConn, pSqlSetNames)) {
		return 0;
	}

	// delete
	char *pChrVehicleDataDelete = new char[1024]();
	sprintf(pChrVehicleDataDelete
			,"DELETE FROM vehicleAllocateResult WHERE 1=1 AND vr_deliveryDate='%s' AND vr_meridiemType='%s' AND vr_meridiemFlag='%s' AND vr_vehicleNo='%s' and vr_errorJusoFlag != 'Y'"
			,pChrDeliveryDate
			,pChrMeridiemType
			,pChrMeridiemFlag
			,pChrVehicleNo);

	if(mysql_query(DbConn, pChrVehicleDataDelete)){
		return 800;
	}

	for (int j = 0; j < _bestChromosomeCopy.size(); j++) {

		MYSQL_RES *resMutualData;			// 상대거리데이터 MutualData Result
		MYSQL_RES *resVehicleDataInsert;	// 상대거리데이터 MutualData Result
		nDistance = 0;

		std::string strGuestId = "";
		std::string strGuestJusoSubId = "";
		std::string strGuestIsShop = "";
		std::string strDeGuestId = "";
		std::string strDeGuestJusoSubId = "";
		std::string strDeGuestIsShop = "";

		// 1. 경로정보 먼저 뽑긔
		if (j == (_bestChromosomeCopy.size() - 1)) {		// 마지막경로는 거점위치로

			strGuestId.assign(_bestChromosomeCopy[j]._guestId.begin(), _bestChromosomeCopy[j]._guestId.end());
			strGuestJusoSubId.assign(_bestChromosomeCopy[j]._jusoSubId.begin(), _bestChromosomeCopy[j]._jusoSubId.end());
			strGuestIsShop.assign(_bestChromosomeCopy[j]._isShop.begin(), _bestChromosomeCopy[j]._isShop.end());
			strDeGuestId.assign(_bestChromosomeCopy[0]._guestId.begin(), _bestChromosomeCopy[0]._guestId.end());
			strDeGuestJusoSubId.assign(_bestChromosomeCopy[0]._jusoSubId.begin(), _bestChromosomeCopy[0]._jusoSubId.end());
			strDeGuestIsShop.assign(_bestChromosomeCopy[0]._isShop.begin(), _bestChromosomeCopy[0]._isShop.end());

			sprintf(pSqlMutualData
				, "SELECT vd_seq, vd_guestId, vd_guestJusoSubId, vd_deguestId, vd_deguestJusoSubId, vd_guestLat, vd_guestLon, vd_deguestLat, vd_deguestLon, vd_distanceValue, vd_guestDate, vd_deguestDate, vd_jsonData, vd_guestIsShop, vd_deguestIsShop, vd_createDate, vd_updateDate \
											FROM vehicleGuestMutualDistance WHERE 1 = 1 \
													AND vd_guestId = '%s' \
													AND vd_guestJusoSubId = '%s' \
													AND vd_guestIsShop='%s' \
													AND vd_deguestId = '%s' \
													AND vd_deguestJusoSubId = '%s' \
													AND vd_deguestIsShop='%s'"
				, strGuestId.c_str()			//_bestChromosomeCopy[j]._guestId.c_str()		// vd_guestId
				, strGuestJusoSubId.c_str()		//_bestChromosomeCopy[j]._jusoSubId.c_str()		// vd_guestJusoSubId
				, strGuestIsShop.c_str()		//_bestChromosomeCopy[j]._isShop.c_str()		// vd_guestIsShop
				, strDeGuestId.c_str()			//_bestChromosomeCopy[0]._guestId.c_str()		// vd_deguestId
				, strDeGuestJusoSubId.c_str()	//_bestChromosomeCopy[0]._jusoSubId.c_str()		// vd_deguestJusoSubId
				, strDeGuestIsShop.c_str());	//_bestChromosomeCopy[0]._isShop.c_str()		// vd_guestIsShop


		} else {

			strGuestId.assign(_bestChromosomeCopy[j]._guestId.begin(), _bestChromosomeCopy[j]._guestId.end());
			strGuestJusoSubId.assign(_bestChromosomeCopy[j]._jusoSubId.begin(), _bestChromosomeCopy[j]._jusoSubId.end());
			strGuestIsShop.assign(_bestChromosomeCopy[j]._isShop.begin(), _bestChromosomeCopy[j]._isShop.end());
			strDeGuestId.assign(_bestChromosomeCopy[j+1]._guestId.begin(), _bestChromosomeCopy[j+1]._guestId.end());
			strDeGuestJusoSubId.assign(_bestChromosomeCopy[j+1]._jusoSubId.begin(), _bestChromosomeCopy[j+1]._jusoSubId.end());
			strDeGuestIsShop.assign(_bestChromosomeCopy[j+1]._isShop.begin(), _bestChromosomeCopy[j+1]._isShop.end());

			sprintf(pSqlMutualData
				, "SELECT vd_seq, vd_guestId, vd_guestJusoSubId, vd_deguestId, vd_deguestJusoSubId, vd_guestLat, vd_guestLon, vd_deguestLat, vd_deguestLon, vd_distanceValue, vd_guestDate, vd_deguestDate, vd_jsonData, vd_guestIsShop, vd_deguestIsShop, vd_createDate, vd_updateDate \
											FROM vehicleGuestMutualDistance WHERE 1 = 1 \
													AND vd_guestId = '%s' \
													AND vd_guestJusoSubId = '%s' \
													AND vd_guestIsShop='%s' \
													AND vd_deguestId = '%s' \
													AND vd_deguestJusoSubId = '%s' \
													AND vd_deguestIsShop='%s'"
				, strGuestId.c_str()			//_bestChromosomeCopy[j]._guestId.c_str()		// vd_guestId
				, strGuestJusoSubId.c_str()		//_bestChromosomeCopy[j]._jusoSubId.c_str()		// vd_guestJusoSubId
				, strGuestIsShop.c_str()		//_bestChromosomeCopy[j]._isShop.c_str()		// vd_guestIsShop
				, strDeGuestId.c_str()			//_bestChromosomeCopy[j + 1]._guestId.c_str()	// vd_deguestId
				, strDeGuestJusoSubId.c_str()	//_bestChromosomeCopy[j + 1]._jusoSubId.c_str()	// vd_deguestJusoSubId
				, strDeGuestIsShop.c_str()		//_bestChromosomeCopy[j + 1]._isShop.c_str()	// vd_guestIsShop
				);
		}

		if (mysql_query(DbConn, pSqlMutualData)) {
			return 800;
		}
		resMutualData = mysql_use_result(DbConn);
		rowMutualData = mysql_fetch_row(resMutualData);

		// 2. 결과데이터 입력
		nDistance = atoi(rowMutualData[9]);
		strcpy(pJsonData, rowMutualData[12]);
		mysql_free_result(resMutualData);			// 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ


		std::string vr_deguestAccno = "";
		std::string vr_deguestName = "";
		std::string vr_deguestTel = "";
		std::string vr_deguestPay = "";
		std::string vr_guestId = "";
		std::string vr_deguestId = "";
		std::string vr_guestJusoSubId = "";
		std::string vr_deguestJusoSubId = "";

		std::string vr_guestLat = "";
		std::string vr_guestLon = "";
		std::string vr_deguestLat = "";
		std::string vr_deguestLon = "";
		std::string vr_Juso = "";

		std::string vr_guestIsShop = "";
		std::string vr_deguestIsShop = "";
		std::string vr_accnoDupleJuso = "";

		if (j == (_bestChromosomeCopy.size() - 1)) {		// 마지막경로는 거점위치로

			vr_deguestAccno.assign(_bestChromosomeCopy[0]._accno.begin(), _bestChromosomeCopy[0]._accno.end());
			vr_deguestName.assign(_bestChromosomeCopy[0]._guestName.begin(), _bestChromosomeCopy[0]._guestName.end());
			vr_deguestTel.assign(_bestChromosomeCopy[0]._guestTel.begin(), _bestChromosomeCopy[0]._guestTel.end());
			vr_deguestPay.assign(_bestChromosomeCopy[0]._pay.begin(), _bestChromosomeCopy[0]._pay.end());
			vr_guestId.assign(_bestChromosomeCopy[j]._guestId.begin(), _bestChromosomeCopy[j]._guestId.end());
			vr_deguestId.assign(_bestChromosomeCopy[0]._guestId.begin(), _bestChromosomeCopy[0]._guestId.end());
			vr_guestJusoSubId.assign(_bestChromosomeCopy[j]._jusoSubId.begin(), _bestChromosomeCopy[j]._jusoSubId.end());
			vr_deguestJusoSubId.assign(_bestChromosomeCopy[0]._jusoSubId.begin(), _bestChromosomeCopy[0]._jusoSubId.end());

			vr_guestLat.assign(_bestChromosomeCopy[j]._guestLat.begin(), _bestChromosomeCopy[j]._guestLat.end());
			vr_guestLon.assign(_bestChromosomeCopy[j]._guestLon.begin(), _bestChromosomeCopy[j]._guestLon.end());
			vr_deguestLat.assign(_bestChromosomeCopy[0]._guestLat.begin(), _bestChromosomeCopy[0]._guestLat.end());
			vr_deguestLon.assign(_bestChromosomeCopy[0]._guestLon.begin(), _bestChromosomeCopy[0]._guestLon.end());
			vr_Juso.assign(_bestChromosomeCopy[0]._guestJuso.begin(), _bestChromosomeCopy[0]._guestJuso.end());

			vr_guestIsShop.assign(_bestChromosomeCopy[j]._isShop.begin(), _bestChromosomeCopy[j]._isShop.end());
			vr_deguestIsShop.assign(_bestChromosomeCopy[0]._isShop.begin(), _bestChromosomeCopy[0]._isShop.end());
			vr_accnoDupleJuso.assign(_bestChromosomeCopy[0]._accnoDuple.begin(), _bestChromosomeCopy[0]._accnoDuple.end());

			sprintf(pSqlVehicleDataInsert
				, "insert into vehicleAllocateResult set vr_deguestAccno='%s', vr_deguestName='%s', vr_deguestTel='%s', vr_deguestPay='%s', vr_guestId='%s', vr_deguestId='%s', vr_guestJusoSubId='%s', vr_deguestJusoSubId='%s', vr_vehicleNo='%s', vr_vehicleNoIndex='%d', vr_guestLat='%s', vr_guestLon='%s', vr_deguestLat='%s', vr_deguestLon='%s', vr_Juso='%s', vr_distanceValue='%d', vr_locationId='%s', vr_deliveryDate='%s', vr_meridiemType='%s', vr_delivererId='%s',vr_deliveryEndId='%s', vr_deliveryStatus='%s', vr_jsonData='%s', vr_createDate=%s, vr_guestIsShop='%s', vr_deguestIsShop='%s', vr_accnoDupleJuso='%s', vr_meridiemFlag='%s' "
				, vr_deguestAccno.c_str() //_bestChromosomeCopy[0]._accno.c_str()			// vr_deguestAccno
				, vr_deguestName.c_str() //_bestChromosomeCopy[0]._guestName.c_str()		// vr_deguestName
				, vr_deguestTel.c_str() //_bestChromosomeCopy[0]._guestTel.c_str()			// vr_deguestTel
				, vr_deguestPay.c_str() //_bestChromosomeCopy[0]._pay.c_str()				// vr_deguestPay
				, vr_guestId.c_str() //_bestChromosomeCopy[j]._guestId.c_str()				// vr_guestId
				, vr_deguestId.c_str() //_bestChromosomeCopy[0]._guestId.c_str()			// vr_deguestId
				, vr_guestJusoSubId.c_str() //_bestChromosomeCopy[j]._jusoSubId.c_str()		// vr_guestJusoSubId
				, vr_deguestJusoSubId.c_str() //_bestChromosomeCopy[0]._jusoSubId.c_str()	// vr_deguestJusoSubId
				, pChrVehicleNo																// vr_vehicleNo
				, j																			// vr_vehicleNoIndex
				, vr_guestLat.c_str() //_bestChromosomeCopy[j]._guestLat.c_str()			// vr_guestLat
				, vr_guestLon.c_str() //_bestChromosomeCopy[j]._guestLon.c_str()			// vr_guestLon
				, vr_deguestLat.c_str() //_bestChromosomeCopy[0]._guestLat.c_str()			// vr_deguestLat
				, vr_deguestLon.c_str() //_bestChromosomeCopy[0]._guestLon.c_str()			// vr_deguestLon
				, vr_Juso.c_str() //_bestChromosomeCopy[0]._guestJuso.c_str()				// vr_Juso
				, nDistance																			// vr_distanceValue
				, pChrLocationId																	// vr_locationId
				, pChrDeliveryDate																	// vr_deliveryDate
				, pChrMeridiemType																	// vr_meridiemType
				, ""																				// vr_delivererId		// 배송기사ID (배정된)
				, ""																				// vr_deliveryEndId		// 배송기사ID (배송완료한)
				, ""																				// vr_deliveryStatus	// 배송완료상태값 배송완료:Y, 배송중:N
				, pJsonData //resMutualData[12]														// vr_jsonData			// 맵데이터
				, "now()"																			// vr_createDate		// 작성일자
				, vr_guestIsShop.c_str() //_bestChromosomeCopy[j]._isShop.c_str()			// vr_guestIsShop		// 고객 ERP, SHOP 구분
				, vr_deguestIsShop.c_str() //_bestChromosomeCopy[0]._isShop.c_str()			// vr_deguestIsShop		// 고객 ERP, SHOP 구분
				, vr_accnoDupleJuso.c_str() //_bestChromosomeCopy[0]._accnoDuple.c_str()	// vr_accnoDupleJuso	// deguest
				, pChrMeridiemFlag
				);
			// 결과데이터 입력
			if (mysql_query(DbConn, pSqlVehicleDataInsert)) {
				return 800;
			}
			resVehicleDataInsert = mysql_use_result(DbConn);
			mysql_free_result(resVehicleDataInsert);	// 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ

		} else {

			if(_bestChromosomeCopy[j+1]._accnoDuple == L""){

				vr_deguestAccno.assign(_bestChromosomeCopy[j+1]._accno.begin(), _bestChromosomeCopy[j+1]._accno.end());
				//vr_deguestName.assign(_bestChromosomeCopy[j+1]._guestName.begin(), _bestChromosomeCopy[j+1]._guestName.end());
				vr_deguestTel.assign(_bestChromosomeCopy[j+1]._guestTel.begin(), _bestChromosomeCopy[j+1]._guestTel.end());
				vr_deguestPay.assign(_bestChromosomeCopy[j+1]._pay.begin(), _bestChromosomeCopy[j+1]._pay.end());
				vr_guestId.assign(_bestChromosomeCopy[j]._guestId.begin(), _bestChromosomeCopy[j]._guestId.end());
				vr_deguestId.assign(_bestChromosomeCopy[j+1]._guestId.begin(), _bestChromosomeCopy[j+1]._guestId.end());
				vr_guestJusoSubId.assign(_bestChromosomeCopy[j]._jusoSubId.begin(), _bestChromosomeCopy[j]._jusoSubId.end());
				vr_deguestJusoSubId.assign(_bestChromosomeCopy[j+1]._jusoSubId.begin(), _bestChromosomeCopy[j+1]._jusoSubId.end());

				vr_guestLat.assign(_bestChromosomeCopy[j]._guestLat.begin(), _bestChromosomeCopy[j]._guestLat.end());
				vr_guestLon.assign(_bestChromosomeCopy[j]._guestLon.begin(), _bestChromosomeCopy[j]._guestLon.end());
				vr_deguestLat.assign(_bestChromosomeCopy[j+1]._guestLat.begin(), _bestChromosomeCopy[j+1]._guestLat.end());
				vr_deguestLon.assign(_bestChromosomeCopy[j+1]._guestLon.begin(), _bestChromosomeCopy[j+1]._guestLon.end());
				//vr_Juso.assign(_bestChromosomeCopy[j+1]._guestJuso.begin(), _bestChromosomeCopy[j+1]._guestJuso.end());

				vr_guestIsShop.assign(_bestChromosomeCopy[j]._isShop.begin(), _bestChromosomeCopy[j]._isShop.end());
				vr_deguestIsShop.assign(_bestChromosomeCopy[j+1]._isShop.begin(), _bestChromosomeCopy[j+1]._isShop.end());
				vr_accnoDupleJuso.assign(_bestChromosomeCopy[j+1]._accnoDuple.begin(), _bestChromosomeCopy[j+1]._accnoDuple.end());

				// 고객정보가져오긔
				char *pSqlDeGuestInfo = new char[2048]();		// 고객명,고객주소 가져오긔
				MYSQL_RES *resDeGuestInfo;
				MYSQL_ROW rowDeGuestInfo;

				sprintf(pSqlDeGuestInfo
						," SELECT ve_guestName, ve_guestJuso FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate='%s' AND ve_meridiemType='%s' AND ve_meridiemFlag='%s' AND ve_locationId='%s' AND ve_accno='%s' AND ve_guestId='%s' AND ve_guestJusoSubId='%s' AND ve_guestLat='%s' AND ve_guestLon='%s' AND ve_IsShop='%s'"
						,pChrDeliveryDate
						,pChrMeridiemType
						,pChrMeridiemFlag
						,pChrLocationId
						,vr_deguestAccno.c_str()
						,vr_deguestId.c_str()
						,vr_deguestJusoSubId.c_str()
						,vr_deguestLat.c_str()
						,vr_deguestLon.c_str()
						,vr_deguestIsShop.c_str()
				);

				if(mysql_query(DbConn, pSqlDeGuestInfo)){
					return 800;
				}
				resDeGuestInfo = mysql_use_result(DbConn);
				rowDeGuestInfo = mysql_fetch_row(resDeGuestInfo);
				vr_deguestName = rowDeGuestInfo[0];
				vr_Juso = rowDeGuestInfo[1];
				mysql_free_result(resDeGuestInfo);	// 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ
				delete[] pSqlDeGuestInfo;
				pSqlDeGuestInfo = NULL;

				sprintf(pSqlVehicleDataInsert
					, "insert into vehicleAllocateResult set vr_deguestAccno='%s', vr_deguestName='%s', vr_deguestTel='%s', vr_deguestPay='%s', vr_guestId='%s', vr_deguestId='%s', vr_guestJusoSubId='%s', vr_deguestJusoSubId='%s', vr_vehicleNo='%s', vr_vehicleNoIndex='%d', vr_guestLat='%s', vr_guestLon='%s', vr_deguestLat='%s', vr_deguestLon='%s', vr_Juso='%s', vr_distanceValue='%d', vr_locationId='%s', vr_deliveryDate='%s', vr_meridiemType='%s', vr_delivererId='%s',vr_deliveryEndId='%s', vr_deliveryStatus='%s', vr_jsonData='%s', vr_createDate=%s, vr_guestIsShop='%s', vr_deguestIsShop='%s', vr_accnoDupleJuso='%s', vr_meridiemFlag='%s' "
					, vr_deguestAccno.c_str()//_bestChromosomeCopy[j+1]._accno.c_str()			// vr_deguestAccno
					, vr_deguestName.c_str()//_bestChromosomeCopy[j+1]._guestName.c_str()		// vr_deguestName
					, vr_deguestTel.c_str()//_bestChromosomeCopy[j+1]._guestTel.c_str()			// vr_deguestTel
					, vr_deguestPay.c_str()//_bestChromosomeCopy[j+1]._pay.c_str()				// vr_deguestPay
					, vr_guestId.c_str()//_bestChromosomeCopy[j]._guestId.c_str()				// vr_guestId
					, vr_deguestId.c_str()//_bestChromosomeCopy[j+1]._guestId.c_str()			// vr_deguestId
					, vr_guestJusoSubId.c_str()//_bestChromosomeCopy[j]._jusoSubId.c_str()		// vr_guestJusoSubId
					, vr_deguestJusoSubId.c_str()//_bestChromosomeCopy[j+1]._jusoSubId.c_str()	// vr_deguestJusoSubId
					, pChrVehicleNo																// vr_vehicleNo
					, j																			// vr_vehicleNoIndex
					, vr_guestLat.c_str()//_bestChromosomeCopy[j]._guestLat.c_str()				// vr_guestLat
					, vr_guestLon.c_str()//_bestChromosomeCopy[j]._guestLon.c_str()				// vr_guestLon
					, vr_deguestLat.c_str()//_bestChromosomeCopy[j+1]._guestLat.c_str()			// vr_deguestLat
					, vr_deguestLon.c_str()//_bestChromosomeCopy[j+1]._guestLon.c_str()			// vr_deguestLon
					, vr_Juso.c_str()//_bestChromosomeCopy[j+1]._guestJuso.c_str()				// vr_Juso
					, nDistance//atoi(rowMutualData[9])													// vr_distanceValue
					, pChrLocationId																	// vr_locationId
					, pChrDeliveryDate																	// vr_deliveryDate
					, pChrMeridiemType																	// vr_meridiemType
					, ""																				// vr_delivererId		// 배송기사ID (배정된)
					, ""																				// vr_deliveryEndId		// 배송기사ID (배송완료한)
					, ""																				// vr_deliveryStatus	// 배송완료상태값 배송완료:Y, 배송중:N
					, pJsonData //rowMutualData[12]														// vr_jsonData			// 맵데이터
					, "now()"																			// vr_createDate		// 작성일자
					, vr_guestIsShop.c_str()//_bestChromosomeCopy[j]._isShop.c_str()			// vr_guestIsShop		// 고객 ERP, SHOP 구분
					, vr_deguestIsShop.c_str()//_bestChromosomeCopy[j+1]._isShop.c_str()		// vr_deguestIsShop		// 고객 ERP, SHOP 구분
					, vr_accnoDupleJuso.c_str()//_bestChromosomeCopy[j+1]._accnoDuple.c_str()	// vr_accnoDupleJuso
					, pChrMeridiemFlag
				);

				// 결과데이터 입력
				if (mysql_query(DbConn, pSqlVehicleDataInsert)) {
					return 800;
				}
				resVehicleDataInsert = mysql_use_result(DbConn);
				mysql_free_result(resVehicleDataInsert);	// 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ

			} else {


				// duple 조회
				char *pSqlAccnoDupleData = new char[2048]();			// AccDuple조회용 - select
				MYSQL_RES *resAccnoDupleData;
				MYSQL_ROW rowAccnoDupleData;

				vr_accnoDupleJuso.assign(_bestChromosomeCopy[j+1]._accnoDuple.begin(), _bestChromosomeCopy[j+1]._accnoDuple.end());

				sprintf(pSqlAccnoDupleData
						," SELECT * FROM vehicleGuestOrderData WHERE 1=1 AND ve_deliveryDate='%s' AND ve_meridiemType='%s' AND ve_meridiemFlag='%s' AND ve_locationId='%s' AND ve_accnoDupleJuso='%s' "
						,pChrDeliveryDate
						,pChrMeridiemType
						,pChrMeridiemFlag
						,pChrLocationId
						,vr_accnoDupleJuso.c_str());

				if(mysql_query(DbConn, pSqlAccnoDupleData)){
					return 800;
				}
				resAccnoDupleData = mysql_use_result(DbConn);

				DbConn2nd = mysql_init(NULL);
				if (!mysql_real_connect(DbConn2nd, MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DB, 0, NULL, 0)) {
					return 800;
				}
				if (mysql_query(DbConn2nd, pSqlSetNames)) {
					return 100;
				}

				while(rowAccnoDupleData = mysql_fetch_row(resAccnoDupleData)){


					MYSQL_RES *resAccnoDupleDataInsert;

					vr_deguestAccno = rowAccnoDupleData[1];		// ve_accno(deguestAccno)
					vr_deguestName = rowAccnoDupleData[6];		// ve_guestName(deguestName)
					vr_deguestTel = rowAccnoDupleData[7];		// ve_guestTel(deguestTel)
					vr_deguestPay = rowAccnoDupleData[10];		// ve_pay(deguestPay)
					vr_guestId.assign(_bestChromosomeCopy[j]._guestId.begin(), _bestChromosomeCopy[j]._guestId.end());
					vr_deguestId = rowAccnoDupleData[5];		// ve_guestId(deguestId)
					vr_guestJusoSubId.assign(_bestChromosomeCopy[j]._jusoSubId.begin(), _bestChromosomeCopy[j]._jusoSubId.end());
					vr_deguestJusoSubId = rowAccnoDupleData[8];	// ve_guestId(deguestJusoSubId)

					vr_guestLat.assign(_bestChromosomeCopy[j]._guestLat.begin(), _bestChromosomeCopy[j]._guestLat.end());
					vr_guestLon.assign(_bestChromosomeCopy[j]._guestLon.begin(), _bestChromosomeCopy[j]._guestLon.end());
					vr_deguestLat = rowAccnoDupleData[11];		// ve_guestLat(deguestLat)
					vr_deguestLon = rowAccnoDupleData[12];		// ve_guestLon(deguestLon)
					vr_Juso = rowAccnoDupleData[9];				// ve_Juso

					vr_guestIsShop.assign(_bestChromosomeCopy[j]._isShop.begin(), _bestChromosomeCopy[j]._isShop.end());
					vr_deguestIsShop = rowAccnoDupleData[14];	// ve_isShop(deguestIsShop)
					vr_accnoDupleJuso = rowAccnoDupleData[17];	// ve_accDupleJuso

					sprintf(pSqlVehicleDataInsert
						, "insert into vehicleAllocateResult set vr_deguestAccno='%s', vr_deguestName='%s', vr_deguestTel='%s', vr_deguestPay='%s', vr_guestId='%s', vr_deguestId='%s', vr_guestJusoSubId='%s', vr_deguestJusoSubId='%s', vr_vehicleNo='%s', vr_vehicleNoIndex='%d', vr_guestLat='%s', vr_guestLon='%s', vr_deguestLat='%s', vr_deguestLon='%s', vr_Juso='%s', vr_distanceValue='%d', vr_locationId='%s', vr_deliveryDate='%s', vr_meridiemType='%s', vr_delivererId='%s',vr_deliveryEndId='%s', vr_deliveryStatus='%s', vr_jsonData='%s', vr_createDate=%s, vr_guestIsShop='%s', vr_deguestIsShop='%s', vr_accnoDupleJuso='%s', vr_meridiemFlag='%s' "
						, vr_deguestAccno.c_str()//_bestChromosomeCopy[j+1]._accno.c_str()			// vr_deguestAccno
						, vr_deguestName.c_str()//_bestChromosomeCopy[j+1]._guestName.c_str()		// vr_deguestName
						, vr_deguestTel.c_str()//_bestChromosomeCopy[j+1]._guestTel.c_str()			// vr_deguestTel
						, vr_deguestPay.c_str()//_bestChromosomeCopy[j+1]._pay.c_str()				// vr_deguestPay
						, vr_guestId.c_str()//_bestChromosomeCopy[j]._guestId.c_str()				// vr_guestId
						, vr_deguestId.c_str()//_bestChromosomeCopy[j+1]._guestId.c_str()			// vr_deguestId
						, vr_guestJusoSubId.c_str()//_bestChromosomeCopy[j]._jusoSubId.c_str()		// vr_guestJusoSubId
						, vr_deguestJusoSubId.c_str()//_bestChromosomeCopy[j+1]._jusoSubId.c_str()	// vr_deguestJusoSubId
						, pChrVehicleNo																// vr_vehicleNo
						, j																			// vr_vehicleNoIndex
						, vr_guestLat.c_str()//_bestChromosomeCopy[j]._guestLat.c_str()				// vr_guestLat
						, vr_guestLon.c_str()//_bestChromosomeCopy[j]._guestLon.c_str()				// vr_guestLon
						, vr_deguestLat.c_str()//_bestChromosomeCopy[j+1]._guestLat.c_str()			// vr_deguestLat
						, vr_deguestLon.c_str()//_bestChromosomeCopy[j+1]._guestLon.c_str()			// vr_deguestLon
						, vr_Juso.c_str()//_bestChromosomeCopy[j+1]._guestJuso.c_str()				// vr_Juso
						, nDistance//atoi(rowMutualData[9])													// vr_distanceValue
						, pChrLocationId																	// vr_locationId
						, pChrDeliveryDate																	// vr_deliveryDate
						, pChrMeridiemType																	// vr_meridiemType
						, ""																				// vr_delivererId		// 배송기사ID (배정된)
						, ""																				// vr_deliveryEndId		// 배송기사ID (배송완료한)
						, ""																				// vr_deliveryStatus	// 배송완료상태값 배송완료:Y, 배송중:N
						, pJsonData //rowMutualData[12]														// vr_jsonData			// 맵데이터
						, "now()"																			// vr_createDate		// 작성일자
						, vr_guestIsShop.c_str()//_bestChromosomeCopy[j]._isShop.c_str()			// vr_guestIsShop		// 고객 ERP, SHOP 구분
						, vr_deguestIsShop.c_str()//_bestChromosomeCopy[j+1]._isShop.c_str()		// vr_deguestIsShop		// 고객 ERP, SHOP 구분
						, vr_accnoDupleJuso.c_str()//_bestChromosomeCopy[j+1]._accnoDuple.c_str()	// vr_accnoDupleJuso
						, pChrMeridiemFlag
						);

					// 결과데이터 입력
					if (mysql_query(DbConn2nd, pSqlVehicleDataInsert)) {
						return 800;
					}
					resAccnoDupleDataInsert = mysql_use_result(DbConn2nd);
					mysql_free_result(resAccnoDupleDataInsert);	// 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ

				} // End While

				mysql_close(DbConn2nd);

				mysql_free_result(resAccnoDupleData);	// 비워줘야됨 안비워주면 좃됨 ... 아 ㅅㅂ
				delete[] pSqlAccnoDupleData;
				pSqlAccnoDupleData = NULL;
			} // End If

		} // End If
		sleep(1);

	}

	nMaxIndex = _bestChromosomeCopy.size();

	MYSQL_ROW rowErrorJusoData;		// errorJusoData
	MYSQL_RES *resErrorJusoData;	// errorJusoData Result

	char *pSqlErrorJusoData = new char[2048]();
	char *pSqlErrorJusoDataUpdate = new char[2048]();

	sprintf(pSqlErrorJusoData
			,"select * from vehicleAllocateResult where 1=1 and vr_deliveryDate='%s' AND vr_meridiemType='%s' AND vr_meridiemFlag='%s' AND vr_vehicleNo='%s' and vr_errorJusoFlag = 'Y'"
			,pChrDeliveryDate
			,pChrMeridiemType
			,pChrMeridiemFlag
			,pChrVehicleNo);
	if(mysql_query(DbConn, pSqlErrorJusoData)){
		return 800;
	}
	resErrorJusoData = mysql_use_result(DbConn);

	DbConn2nd = mysql_init(NULL);
	if (!mysql_real_connect(DbConn2nd, MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DB, 0, NULL, 0)) {
		return 800;
	}
	if (mysql_query(DbConn2nd, pSqlSetNames)) {
		return 100;
	}

	std::string vr_seq = "";

	while(rowErrorJusoData = mysql_fetch_row(resErrorJusoData)){

		vr_seq = rowErrorJusoData[0];

		// nMaxIndex
		//MYSQL_RES *resErrorJusoDataSe;
		sprintf(pSqlErrorJusoDataUpdate
				,"update vehicleAllocateResult set vr_vehicleNoIndex='%d' where vr_seq='%s'"
				,nMaxIndex
				,vr_seq.c_str());

		if(mysql_query(DbConn2nd, pSqlErrorJusoDataUpdate)){
			return 800;
		}

		nMaxIndex++;
	}
	mysql_free_result(resErrorJusoData);					// please Check !!


	mysql_close(DbConn);

	delete[] pSqlErrorJusoData;
	pSqlErrorJusoData = NULL;
	delete[] pSqlErrorJusoDataUpdate;
	pSqlErrorJusoDataUpdate = NULL;

	delete[] pSqlMutualData;
	pSqlMutualData = NULL;
	delete[] pSqlVehicleDataInsert;
	pSqlVehicleDataInsert = NULL;
	delete[] pJsonData;
	pJsonData = NULL;
	delete[] pSqlSetNames;
	pSqlSetNames = NULL;

	printf("Step08 OnVehicleDataInsert End\n");
	fflush(stdout);

	return 0;

}





wstring TSPView::Utf8StringToWString( string in )
{
	/*
	wchar_t pOut[2048] = {0,};
	char pIn[2048]={0,};
	memcpy(pIn,in.c_str(),in.length());
	size_t szIn = in.length();
	size_t szOut = sizeof(pOut) /  sizeof(wchar_t);


	iconv_t it = iconv_open("UTF-16LE", "UTF-8"); // EUC-KR을 UTF-8로
	int ret = iconv(it, (char**)&pIn, &szIn, (char**)&pOut, &szOut); //
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
	*/

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
