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
#include <tuple>

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

enum class HdbppFeatures
{
    // Time to live feature. Attributes can be timed out by the database based
    // on the configured ttl value
    TTL,

    // Backend supports passing of multiple events and batching them into
    // the database. This is a performance improvement.
    BATCH_INSERTS,
};

// Abstract base class that backends are required to implement when offering
// a storage backend to the hdb++ system
class AbstractDB
{
public:

	virtual ~AbstractDB() {}

    // Inserts an attribute archive event for the EventData into the database. If the attribute
    // does not exist in the database, then an exception will be raised. If the attr_value
    // field of the data parameter if empty, then the attribute is in an error state
    // and the error message will be archived.
	virtual void insert_event(Tango::EventData *event, const HdbEventDataType &data_type) = 0;

    // Insert multiple attribute archive events. Any attributes that do not exist will
    // cause an exception. On failure the fall back is to insert events individually
    virtual void insert_events(std::vector<std::tuple<Tango::EventData*, HdbEventDataType>> events) = 0;

    // Inserts the attribute configuration data (Tango Attribute Configuration event data)
    // into the database. The attribute must be configured to be stored in HDB++,
    // otherwise an exception will be thrown.
	virtual void insert_param_event(Tango::AttrConfEventData *param_event, const HdbEventDataType &data_type) = 0;

    // Add an attribute to the database. Trying to add an attribute that already exists will
    // cause an exception
	virtual void add_attribute(const std::string &name, int type, int format, int write_type) = 0;

    // Update the attribute ttl. The attribute must have been configured to be stored in
    // HDB++, otherwise an exception is raised
	virtual void update_ttl(const std::string &name, unsigned int ttl) = 0;

    // Inserts a history event for the attribute name passed to the function. The attribute
    // must have been configured to be stored in HDB++, otherwise an exception is raised.
	virtual void insert_history_event(const std::string &name, unsigned char event) = 0;

    // Check what hdbpp features this library supports.
    virtual bool supported(HdbppFeatures feature) = 0;
};

// Abstract factory class that backend must implement to help create an instance
// of the storage class deriving from AbstractDB
class DBFactory
{
public:

    // Create a backend database object, and return it as a pointer
	virtual AbstractDB* create_db(const string &id, const std::vector<std::string> &configuration) = 0;
	virtual ~DBFactory(){};
};

} // namespace hdbpp

extern "C"
{
	typedef hdbpp::DBFactory* getDBFactory_t();
    hdbpp::DBFactory *getDBFactory();
}

#endif // _HDBPP_ABSTRACTDB_H