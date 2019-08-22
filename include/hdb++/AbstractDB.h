/* Copyright (C) 2014-2017
   Elettra - Sincrotrone Trieste S.C.p.A.
   Strada Statale 14 - km 163,5 in AREA Science Park
   34149 Basovizza, Trieste, Italy.

   This file is part of libhdb++.

   libhdb++ is free software: you can redistribute it and/or modify
   it under the terms of the Lesser GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   libhdb++ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser
   GNU General Public License for more details.

   You should have received a copy of the Lesser GNU General Public License
   along with libhdb++.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _HDBPP_ABSTRACTDB_H
#define _HDBPP_ABSTRACTDB_H

#include <tango.h>
#include <vector>

namespace hdbpp
{
#define DB_INSERT		0
#define DB_START		1
#define DB_STOP			2
#define DB_REMOVE		3
#define DB_INSERT_PARAM	4
#define DB_PAUSE		5
#define DB_UPDATETTL	6

// Data struct used to pass information to the backend
typedef struct HdbEventDataType_
{
	std::string attr_name;
	int max_dim_x;
	int max_dim_y;
	int data_type;
	Tango::AttrDataFormat data_format;
	int write_type;

} HdbEventDataType;

// Abstract base class that backends are required to implement when offering
// a storage backend to the hdb++ system
class AbstractDB
{
public:

	virtual ~AbstractDB() {}

	virtual void insert_Attr(Tango::EventData *data, HdbEventDataType ev_data_type) = 0;
	virtual void insert_param_Attr(Tango::AttrConfEventData *data, HdbEventDataType ev_data_type) = 0;
	virtual void configure_Attr(std::string name, int type, int format, int write_type, unsigned int ttl) = 0;
	virtual void updateTTL_Attr(std::string name, unsigned int ttl) = 0;
	virtual void event_Attr(std::string name, unsigned char event) = 0;
};

// Abstract factory class that backend must implement to help create an instance
// of the storage class deriving from AbstractDB
class DBFactory
{
public:

	virtual AbstractDB* create_db(std::vector<std::string> configuration) = 0;
	virtual ~DBFactory(){};
};

extern "C"
{
	typedef DBFactory * getDBFactory_t();
    DBFactory *getDBFactory();
}

} // namespace hdbpp
#endif // _HDBPP_ABSTRACTDB_H