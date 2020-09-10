/*
 * TSP.h
 *
 *  Created on: Jul 5, 2017
 *      Author: root
 */

//#pragma once

#include "stdafx.h"

#include "TspAlgorithm.h"

using namespace std;
using namespace __gnu_cxx;

class TSPView : public GaObserver
{

	DEFINE_SYNC_CLASS

// Construction
public:
	TSPView();
	//~TSPView();

public:
	virtual ~TSPView();

public:
	static TSPView *pInstance;

// Attributes
public:

	// Notifies the observer that new statistical information is available
	virtual void GACALL StatisticUpdate(const GaStatistics& statistics, const GaAlgorithm& algorithm);

	// Notifies observer that new best chromosome has found
	virtual void GACALL NewBestChromosome(const GaChromosome& newChromosome, const GaAlgorithm& algorithm);

	// Notifies observer that state of evolution (problem sloving) has changed.
	virtual void GACALL EvolutionStateChanged(GaAlgorithmState newState, const GaAlgorithm& algorithm) { }

private:

	GaCriticalSection _algControlSect;
	vector<const TspCity*> _bestChromosome;
	vector<const TspCity*> _bestRelocation;		// 경로 재배열값
	vector<ChromosomeCopy> _bestChromosomeCopy;	//

	int _generation;
	float _fitness;

// Operations
public:
	vector<VectorOrderData> _vectorOrderData;						// 당일 주문데이터 저장용
	vector< vector<VectorMutualData > > _vectorMutualData;			// 당일 주문데이터 상대경로 저장용

	vector< vector<RouteEachResult > > _bestRouteEachResult;		// 각 차량별 최적경로 뽑은 결과저장용 ( nVehicle 갯수만큼 동적할당 저장용 )
	vector< vector<RouteEachResult > > _bestRouteEachRelocation;	// 각 차량별 최적경로 뽑은 결과 재배치저장용 ( nVehicle 갯수만큼 동적할당 저장용 )

// Overrides
	protected:
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	char *pChrDeliveryDate;	// 배송날짜
	char *pChrMeridiemType;	// 배송오전오후 구분
	char *pChrMeridiemFlag;	// 배송 오전,오후 플래그
	char *pChrLocationId;	// 거점ID locationId
	char *pChrVehicleNo;	// VehicleNo
	int nVehicleNumber;		//

	bool bComleteLoadDeliveryData;											// Step01		DeliveryData Load 완료상태값
	bool bStatusLoadDeliveryData;											// Step01		DeliveryData Load 처리상태값
	int loadDeliveryData(char* chrDeliveryDate, char* chrMeridiemType);		// Step01		DeliveryData Load - 배송데이터 로딩

	// db connection
	MYSQL *DbConn;
	MYSQL *DbConn2nd;
	MYSQL_RES *DbRes;
	MYSQL_ROW DbRow;

	int nCountOrderData;

	static int _nSearchIndexStart;

	int *nVehicleFlg;		// 경로구분 플래그 - 차량댓수만큼 할당됨
	int *nVehiclePay;		// 각차량에 할당된 금액 - 차량댓수만큼 할당됨
	int *nVehiclePayIndex;	// 각차량에 할당된 금액 - 경로갯수만큼 할당됨 - 데이터비교용
	int *nVehicleCnt;		// 각차량에 할당된 금액 - 경로갯수만큼 할당됨 - 데이터비교용

	// Generated message map functions
protected:
	//afx_msg void OnPaint();
	//DECLARE_MESSAGE_MAP()

public:
	void init();
	void OnLButtonDown();

	int OnInit();											// Step00
	int OnLoadDeliveryData();								// Step01
	int OnSetDeliveryData();								// Step02
	int OnSearchDeliveryData();								// Step03
	int OnRelocationDeliveryData();							// Step04
	int OnCalcDeliveryData();								// Step05

	int OnResearchDeliveryReady();							// Step06
	int OnResearchDeliveryData();							// Step06
	void routeBestSearch(int nRefIndexStart, int nRefIndexStop);

	int OnRelocationVehicleData();							// Step07
	int OnVehicleDataInsert();								// Step08


	int ConvertChar( char* pMessage );
	wstring Utf8StringToWString(string in);

	TSPView *pTSPView;

};
