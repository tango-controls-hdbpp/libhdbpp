//=============================================================================
//
 // file :        LibHdb++.h
//
// description : Include for the LibHdb library.
//
// Author: Graziano Scalamera
//
//
//=============================================================================

#ifndef _LIB_HDB_H
#define _LIB_HDB_H

#include <tango.h>
#include <vector>
#include <stdint.h>

#define DB_INSERT		0
#define DB_START		1
#define DB_STOP			2
#define DB_REMOVE		3
#define DB_INSERT_PARAM	4
#define DB_PAUSE		5
#define DB_UPDATETTL	6

typedef struct HdbEventDataType_
{
	string attr_name;
	int max_dim_x;
	int max_dim_y;
	int data_type;
	Tango::AttrDataFormat data_format;
	int write_type;

} HdbEventDataType;

class  HdbCmdData
{
public:
	HdbCmdData(Tango::EventData *ev_data_, HdbEventDataType ev_data_type_){ev_data=ev_data_; ev_data_param=NULL; ev_data_type=ev_data_type_; op_code=DB_INSERT;};
	HdbCmdData(Tango::AttrConfEventData *ev_data_param_, HdbEventDataType ev_data_type_){ev_data=NULL; ev_data_param=ev_data_param_; ev_data_type=ev_data_type_; op_code=DB_INSERT_PARAM;};
	HdbCmdData(uint8_t op_code_, string attr_name_){op_code=op_code_; attr_name=attr_name_; ev_data=NULL; ev_data_param=NULL;};
	HdbCmdData(uint8_t op_code_, unsigned int ttl_, string attr_name_){op_code=op_code_; attr_name=attr_name_; ttl=ttl_; ev_data=NULL; ev_data_param=NULL;};
    ~HdbCmdData(){if(ev_data) delete ev_data; if(ev_data_param) delete ev_data_param;};
	Tango::EventData *ev_data;
	Tango::AttrConfEventData *ev_data_param;
	HdbEventDataType ev_data_type;
	uint8_t op_code;	//operation code
	unsigned int  ttl;
	string attr_name;

} ;

class AbstractDB
{

public:


	virtual void insert_Attr(Tango::EventData *data, HdbEventDataType ev_data_type) = 0;

	virtual void insert_param_Attr(Tango::AttrConfEventData *data, HdbEventDataType ev_data_type) = 0;

	virtual void configure_Attr(string name, int type/*DEV_DOUBLE, DEV_STRING, ..*/, int format/*SCALAR, SPECTRUM, ..*/, int write_type/*READ, READ_WRITE, ..*/, unsigned int ttl/*hours, 0=infinity*/) = 0;

	virtual void updateTTL_Attr(string name, unsigned int ttl/*hours, 0=infinity*/) = 0;

	virtual void event_Attr(string name, unsigned char event) = 0;

	virtual ~AbstractDB() {}

};

class DBFactory
{

public:

	virtual AbstractDB* create_db(vector<string> configuration) = 0;
	virtual ~DBFactory(){};

};

class HdbClient
{

private:
	AbstractDB *db;
	DBFactory *db_factory;
	void* hLib;

	DBFactory *getDBFactory();
	void string_explode(string str, string separator, vector<string>* results);
	void string_vector2map(vector<string> str, string separator, map<string,string>* results);

public:
	HdbClient(vector<string> configuration);

	void insert_Attr(Tango::EventData *data, HdbEventDataType ev_data_type);

	void insert_param_Attr(Tango::AttrConfEventData *data, HdbEventDataType ev_data_type);

	void configure_Attr(string name, int type/*DEV_DOUBLE, DEV_STRING, ..*/, int format/*SCALAR, SPECTRUM, ..*/, int write_type/*READ, READ_WRITE, ..*/, unsigned int ttl/*hours, 0=infinity*/);

	void updateTTL_Attr(string name, unsigned int ttl/*hours, 0=infinity*/);

	void event_Attr(string name, unsigned char event);

	~HdbClient();

};

extern "C"
{
	typedef DBFactory * getDBFactory_t();
    DBFactory *getDBFactory();
}


#endif
