/*
 * TspCity.cpp
 *
 *  Created on: Jul 4, 2017
 *      Author: root
 */


#include "stdafx.h"
#include "TspCity.h"

#include <math.h>

#include "TspView.h"

int TspCity::_nextId = 0;

int TspCities::nGenerationLimit = 100000;	// GenerationLimit 초기화

float TspCity::GetDistance(const TspCity& city) const
{

	int idStart;
	int idEnd;

	if (_originId > 0) {
		idStart = (_originId - 1);
		idEnd = (city._originId - 1);
	} else {
		idStart = (_id - 1);
		idEnd = (city._id - 1);
	}

	float fDistanceValue = 0;
	//fDistanceValue = atof(pTSPView->_vectorMutualData[idStart][idEnd].vd_distanceValue);
	fDistanceValue = atof(TSPView::pInstance->_vectorMutualData[idStart][idEnd].vd_distanceValue);

	return fDistanceValue;
}

void TspCities::AddCity(const wstring& name
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
						,const wstring& guestLon)
{
	TspCity* _newCity = new TspCity(name, x, y, guestId, isShop, jusoSubId, 0, accno, accnoDuple, guestName, guestTel, guestJuso, pay, guestLat, guestLon);

	_cities.insert( pair<int, TspCity*>( _newCity->GetID(), _newCity ) );

}

void TspCities::AddCityExt(const wstring& name
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
{
	TspCity* _newCity = new TspCity(name, x, y, guestId, isShop, jusoSubId, originId, accno, accnoDuple, guestName, guestTel, guestJuso, pay, guestLat, guestLon);

	_cities.insert(pair<int, TspCity*>(_newCity->GetID(), _newCity));

}

void TspCities::AddLocation()
{
	return;
}

TspCities TspCities::_instance;

bool TspCities::RemoveCity(int id)
{
	TspCity* city = _cities[ id ];
	if( city )
	{
		_cities.erase( id );
		delete city;

		return true;
	}

	return false;
}

void TspCities::Clear()
{
	for( hash_map<int, TspCity*>::iterator it = _cities.begin(); it != _cities.end(); ++it )
		delete it->second;

	_cities.clear();
}

float TspCities::GetDistance(int cityA, int cityB) const
{
	const TspCity* a = GetCityById( cityA );
	if( !a )
		return -1;

	const TspCity* b = GetCityById( cityB );
	if( !b )
		return -1;

	return a->GetDistance( *b );
}

const TspCity* TspCities::GetCityById(int id) const
{
	hash_map<int, TspCity*>::const_iterator it = _cities.find( id );
	if( it != _cities.end() )
		return it->second;

	return NULL;
}

const TspCity* TspCities::GetCityByPoint(int x, int y) const
{
	for( hash_map<int, TspCity*>::const_iterator it = _cities.begin(); it != _cities.end(); ++it )
	{
		if( it->second->PointWithinCity( x, y ) )
			return it->second;
	}

	return NULL;
}

void TspCities::GetCities(vector<const TspCity*>& output) const
{
	output.resize( _cities.size() );

	int i = 0;
	for( hash_map<int, TspCity*>::const_iterator it = _cities.begin(); it != _cities.end(); ++it, ++i )
		output[ i ] = it->second;
}

void TspCities::locationInitialize()
{
	int nRandom = 0;

	float nValue[36] = {0,200,200,220,420,450,200,0,220,200,400,420,200,220,0,200,220,420,220,200,200,0,200,220,420,400,220,200,0,200,450,420,420,220,200,0};
	//int nValue[36] = { 0, 201, 202, 223, 421, 450, 203, 0, 227, 204, 409, 422, 205, 224, 0, 206, 225, 423, 226, 207, 208, 0, 209, 227, 424, 401, 228, 210, 0, 211, 451, 425, 426, 229, 212, 0 };

	int nIndex = 0;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {

			//nRandom = rand() % 6 + 1;

			//fTmp[i][j] = 1111;
			fTmp[i][j] = nValue[nIndex];
			nIndex++;
		}
	}
}


