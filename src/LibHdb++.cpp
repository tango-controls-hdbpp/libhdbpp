//=============================================================================
//
// file :        LibHdbL.cpp
//
// description : Source for the LibHdb library.
//
// Author: Graziano Scalamera
//
// $Revision: 1.6 $
//
// $Log: LibHdb++.cpp,v $
// Revision 1.6  2014-03-07 13:14:38  graziano
// added remove_Attr, start_Attr, stop_Attr
// removed _HDBEVENTDATA
//
// Revision 1.5  2014-02-20 14:15:01  graziano
// name and path fixing
//
// Revision 1.4  2014-02-06 14:35:12  graziano
// added api for CM
//
// Revision 1.3  2013-09-24 08:48:37  graziano
// support for HdbEventData
//
// Revision 1.2  2013-09-02 12:37:25  graziano
// fixed release string
//
// Revision 1.1  2013-09-02 12:25:36  graziano
// libhdb refurbishing
//
//
//
//=============================================================================

#include "LibHdb++.h"

#ifndef LIB_BUILDTIME
#define LIB_BUILDTIME   RELEASE " " __DATE__ " "  __TIME__
#endif

const char version_string[] = "$Build: " LIB_BUILDTIME " $";
static const char __FILE__rev[] = __FILE__ " $Id: 1.6 $";



#if _DB_AT_RUNTIME
HdbClient::HdbClient(string dbtype, string host, string user, string password, string dbname, int port)
{
	if(dbtype == "MySQL")
	{
		HdbMySQLFactory db_mysql_factory;
		db_factory = &db_mysql_factory;
		db = db_factory->create_db(host, user, password, dbname, port);
	}
	else
	{
		cout << __func__<<": VERSION: " << version_string << " file:" << __FILE__rev << endl;
		cout << __func__<<": Type '" << dbtype << "' not supported" << endl;
		throw(string("Type '") + dbtype + string("' not supported"));
	}
}
#else
HdbClient::HdbClient(string host, string user, string password, string dbname, int port)
{
		db_factory = getDBFactory();
		db = db_factory->create_db(host, user, password, dbname, port);
		if(db == NULL)
		{
			cout << __func__<<": VERSION: " << version_string << " file:" << __FILE__rev << endl;
			cout << __func__<<": Error creating db" << endl;
		}
}
#endif

int HdbClient::insert_Attr(Tango::EventData *data, HdbEventDataType ev_data_type)
{
	return db->insert_Attr(data, ev_data_type);
}

int HdbClient::insert_param_Attr(Tango::AttrConfEventData *data, HdbEventDataType ev_data_type)
{
	return db->insert_param_Attr(data, ev_data_type);
}

int HdbClient::configure_Attr(string name, int type/*DEV_DOUBLE, DEV_STRING, ..*/, int format/*SCALAR, SPECTRUM, ..*/, int write_type/*READ, READ_WRITE, ..*/)
{
	return db->configure_Attr(name, type, format, write_type);
}

int HdbClient::event_Attr(string name, unsigned char event)
{
	return db->event_Attr(name, event);
}

HdbClient::~HdbClient()
{
	delete db;
	delete db_factory;
}
