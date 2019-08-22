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

#ifndef _HDBPP_HDB_CLIENT_H
#define _HDBPP_HDB_CLIENT_H

#include "hdb++/AbstractDB.h"

#include <vector>
#include <string>
#include <tango.h>

namespace hdbpp
{
// Provides a client interface onto the backend storage system. Loads the 
// requested library and then passes events to it
class HdbClient
{
private:
	AbstractDB *db;
	DBFactory *db_factory;
	void *hLib;

	DBFactory *getDBFactory();

public:

	HdbClient(std::vector<std::string> configuration);
	~HdbClient();

	void insert_Attr(Tango::EventData *data, HdbEventDataType ev_data_type);
	void insert_param_Attr(Tango::AttrConfEventData *data, HdbEventDataType ev_data_type);
	void configure_Attr(std::string name, int type, int format, int write_type, unsigned int ttl);
	void updateTTL_Attr(std::string name, unsigned int ttl);
	void event_Attr(std::string name, unsigned char event);
};

} // namespace hdbpp
#endif // _HDBPP_HDB_CLIENT_H