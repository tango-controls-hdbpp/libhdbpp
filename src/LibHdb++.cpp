//=============================================================================
//
// file :        LibHdbL.cpp
//
// description : Source for the LibHdb library.
//
// Author: Graziano Scalamera
//
//
//=============================================================================

#include "LibHdb++.h"
#include <dlfcn.h>

#ifndef LIB_BUILDTIME
#define LIB_BUILDTIME   RELEASE " " __DATE__ " "  __TIME__
#endif

const char version_string[] = "$Build: " LIB_BUILDTIME " $";
static const char __FILE__rev[] = __FILE__;


HdbClient::HdbClient(vector<string> configuration)
{
	map<string,string> db_conf;
	string_vector2map(configuration,"=",&db_conf);
	string libname;
	try
	{
		libname = db_conf.at("libname");
	}
	catch(const std::out_of_range& e)
	{
		stringstream tmp;
		tmp << "Configuration parsing error looking for key 'libname='";
		cout << __func__<<": VERSION: " << version_string << " file:" << __FILE__rev << endl;
		cout << __func__<< ": " << tmp.str() << endl;
		exit(1);
	}

	if ((hLib = dlopen(libname.c_str(), RTLD_NOW/*|RTLD_GLOBAL*/)))
	{
		if (getDBFactory_t* create_factory = (getDBFactory_t*)dlsym(hLib, "getDBFactory"))
		{
			db_factory = create_factory();
			db = db_factory->create_db(configuration);
			if(db == NULL)
			{
				cout << __func__<<": VERSION: " << version_string << " file:" << __FILE__rev << endl;
				cout << __func__<<": Error creating db" << endl;
				exit(1);
			}
		}
		else
		{
			cout << __func__<<": VERSION: " << version_string << " file:" << __FILE__rev << endl;
			cout << __func__<<": Error loading symbol getDBFactory from library " << libname << endl;
			exit(1);
		}
	}
	else
	{
		db = NULL;
		cout << __func__<<": VERSION: " << version_string << " file:" << __FILE__rev << endl;
		cout << __func__<<": Error loading library " << libname << endl;
		exit(1);
	}
}

int HdbClient::insert_Attr(Tango::EventData *data, HdbEventDataType ev_data_type)
{
	return db->insert_Attr(data, ev_data_type);
}

int HdbClient::insert_param_Attr(Tango::AttrConfEventData *data, HdbEventDataType ev_data_type)
{
	return db->insert_param_Attr(data, ev_data_type);
}

int HdbClient::configure_Attr(string name, int type/*DEV_DOUBLE, DEV_STRING, ..*/, int format/*SCALAR, SPECTRUM, ..*/, int write_type/*READ, READ_WRITE, ..*/, unsigned int ttl/*hours, 0=infinity*/)
{
	return db->configure_Attr(name, type, format, write_type, ttl);
}

int HdbClient::updateTTL_Attr(string name, unsigned int ttl/*hours, 0=infinity*/)
{
	return db->updateTTL_Attr(name, ttl);
}

int HdbClient::event_Attr(string name, unsigned char event)
{
	return db->event_Attr(name, event);
}

HdbClient::~HdbClient()
{
	delete db;
	delete db_factory;
	dlclose(hLib);
}

void HdbClient::string_explode(string str, string separator, vector<string>* results)
{
	string::size_type found;

	found = str.find_first_of(separator);
	while(found != string::npos)
	{
		if(found > 0)
		{
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}
	if(str.length() > 0)
	{
		results->push_back(str);
	}
}

void HdbClient::string_vector2map(vector<string> str, string separator, map<string,string>* results)
{
	for(vector<string>::iterator it=str.begin(); it != str.end(); it++)
	{
		string::size_type found_eq;
		found_eq = it->find_first_of(separator);
		if(found_eq != string::npos && found_eq > 0)
			results->insert(make_pair(it->substr(0,found_eq),it->substr(found_eq+1)));
	}
}
