/*
 * TspCity.h
 *
 *  Created on: Jul 4, 2017
 *      Author: root
 */

//#ifndef TSPCITY_H_
//#define TSPCITY_H_

#pragma once

#include <string>
//#include <hash_map>
#include <ext/hash_map>


//#if defined(GNU_STL)
//	#include <ext/hash_map>
//	namespace std { using __gnu_cxx::hash_map; }
//#endif
using namespace std;
using namespace __gnu_cxx;

#define CITY_SIZE 14

class TspCity
{

private:

	static int _nextId;
	int _id;
	int _originId;
	wstring _name;	//
	double _x;
	double _y;
	wstring _guestId;		// guestId 추가
	wstring _isShop;		// ERP주문, 쇼핑몰주문여부( 0:ERP주문, 1:쇼핑몰주문)
	wstring _jusoSubId;		// jusoSubId 추가
	wstring _accno;			// accno 추가
	wstring _accnoDuple;	// accnoDuple추가
	wstring _guestName;		// guestName 추가
	wstring _guestTel;		// guestTel
	wstring _guestJuso;		// guestJuso
	wstring _pay;			// pay

	wstring _guestLat;		// lat
	wstring _guestLon;		// lon

public:

	//TspCity(const wstring& name, int x, int y) : _name(name), _x(x), _y(y) { _id = ++_nextId; }
	TspCity(const wstring& name
				,double x
				,double y
				,const wstring& guestId
				,const wstring& isShop
				,const wstring& jusoSubId
				,int originId
				,const wstring& accno
				,const wstring& accnoDuple
				,const wstring& guestName
				,const wstring& guestTel
				,const wstring& guestJuso
				,const wstring& pay
				,const wstring& guestLat
				,const wstring& guestLon)
				:
				_name(name)
				,_x(x)
				,_y(y)
				,_guestId(guestId)
				,_isShop(isShop)
				,_jusoSubId(jusoSubId)
				,_originId(originId)
				,_accno(accno)
				,_accnoDuple(accnoDuple)
				,_guestName(guestName)
				,_guestTel(guestTel)
				,_guestJuso(guestJuso)
				,_pay(pay)
				,_guestLat(guestLat)
				,_guestLon(guestLon)
				{ _id = ++_nextId; }

	inline int GetID() const { return _id; }
	inline void ResetNextId() const { _nextId = 0; }

	inline double GetX() const { return _x; }
	inline double GetY() const { return _y; }

	inline wstring GetGuestId() const { return _guestId; }
	inline wstring GetIsShop() const { return _isShop; }

	inline wstring GetJusoSubId() const { return _jusoSubId; }

	inline wstring GetAccno() const { return _accno; }
	inline wstring GetAccnoDuple() const { return _accnoDuple; }

	inline wstring GetGuestName() const { return _guestName; }
	inline wstring GetGuestTel() const { return _guestTel; }
	inline wstring GetGuestJuso() const { return _guestJuso; }
	inline wstring GetPay() const { return _pay; }

	inline wstring GetGuestLat() const { return _guestLat; }
	inline wstring GetGuestLon() const { return _guestLon; }

	float GetDistance(const TspCity& city) const;

	inline const wstring& GetName() const { return _name; }

	inline bool PointWithinCity(int x, int y) { return	x >= _x - CITY_SIZE / 2 && x <= _x + CITY_SIZE / 2 && y >= _y - CITY_SIZE / 2 && y <= _y + CITY_SIZE / 2; }

};

class TspCities
{

private:

	static TspCities _instance;
	hash_map<int, TspCity*> _cities;

public:
	static int nGenerationLimit;	// GenerationLimit
	float fTmp[6][6];
	void locationInitialize();

	inline static TspCities& GetInstance() { return _instance; }

	~TspCities() { Clear(); }

	//void AddCity(const wstring& name, int x, int y);
	void AddCity(const wstring& name
				,double x
				,double y
				,const wstring& guestId
				,const wstring& isShop
				,const wstring& jusoSubId
				,const wstring& accno
				,const wstring& accnoDuple
				,const wstring& guestName
				,const wstring& guestTel
				,const wstring& guestJuso
				,const wstring& pay
				,const wstring& guestLat
				,const wstring& guestLon
				);

	void AddCityExt(const wstring& name
					,double x
					,double y
					,const wstring& guestId
					,const wstring& isShop
					,const wstring& jusoSubId
					,int originId
					,const wstring& accno
					,const wstring& accnoDuple
					,const wstring& guestName
					,const wstring& guestTel
					,const wstring& guestJuso
					,const wstring& pay
					,const wstring& guestLat
					,const wstring& guestLon
					);

	void AddLocation();

	bool RemoveCity(int id);

	void Clear();

	//void DrawCities(CDC& dc) const;

	float GetDistance(int cityA, int cityB) const;

	const TspCity* GetCityById(int id) const;

	const TspCity* GetCityByPoint(int x, int y) const;

	void GetCities(vector<const TspCity*>& output) const;

	inline int GetCount() const { return (int)_cities.size(); }

};

typedef struct ChromosomeCopy {

	int _id;
	int _originId;
	wstring _name;
	double _x;
	double _y;

	wstring _guestId;			// guestId 추가
	wstring _isShop;			// jusoSubId 추가
	wstring _jusoSubId;
	wstring _accno;
	wstring _accnoDuple;

	wstring _guestName;
	wstring _guestTel;
	wstring _guestJuso;
	wstring _pay;

	wstring _guestLat;			// lat
	wstring _guestLon;			// lon
};

typedef struct RouteEachResult {
	int _id;
	wstring _name;
	double _x;
	double _y;


	wstring _guestId;	// guestId 추가
	wstring _isShop;
	wstring _jusoSubId;	// jusoSubId 추가
	wstring _accno;
	wstring _accnoDuple;
	wstring _guestName;		// guestName 추가
	wstring _guestTel;		// guestTel
	wstring _guestJuso;		// guestJuso
	wstring _pay;			// pay

	wstring _guestLat;			// lat
	wstring _guestLon;			// lon
};

typedef struct RouteDBResult {

	double _start;			// 출발지점
	double _end;			// 도착지점
	double _distance;		// 거리값(Distance)

};

typedef struct VectorOrderData {

	/*
	double _start;			// 출발지점
	double _end;			// 도착지점
	double _distance;		// 거리값(Distance)
	*/
	char ve_seq[32];				// 0
	char ve_accno[32];				// 1
	char ve_locationId[4];			// 2
	char ve_deliveryDate[10];		// 3
	char ve_meridiemType[4];		// 4
	char ve_guestId[64];			// 5
	char ve_guestName[100];			// 6
	char ve_guestTel[100];			// 7
	char ve_guestJusoSubId[2];		// 8
	char ve_guestJuso[256];			// 9
	char ve_pay[32];				// 10
	char ve_guestLat[64];			// 11
	char ve_guestLon[64];			// 12
	char ve_isNew[2];				// 13
	char ve_isShop[2];				// 14
	char ve_isRoad[2];				// 15
	char ve_createDate[32];			// 16
	char ve_accnoDupleJuso[100];	// 17

};

typedef struct VectorMutualData {

	char vd_seq[32];				// vd_seq
	char vd_guestId[64];			// vd_guestId
	char vd_guestJusoSubId[2];		// vd_guestJusoSubId
	char vd_deguestId[64];			// vd_deguestId
	char vd_deguestJusoSubId[2];	// vd_deguestJusoSubId
	char vd_guestLat[64];			// vd_guestLat
	char vd_guestLon[64];			// vd_guestLon
	char vd_deguestLat[64];			// vd_deguestLat
	char vd_deguestLon[64];			// vd_deguestLon
	char vd_distanceValue[64];		// vd_distanceValue
	char vd_guestDate[64];			// vd_guestDate
	char vd_deguestDate[64];		// vd_deguestDate
//	char vd_jsonData;
	char vd_createDate[64];			// vd_createDate
	char vd_updateDate[64];			// vd_updateDate
	char vd_guestIsShop[2];			// vd_guestIsShop
	char vd_deguestIsShop[2];		// vd_deguestIsShop
};

//#endif /* TSPCITY_H_ */
