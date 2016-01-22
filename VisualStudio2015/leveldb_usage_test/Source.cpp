#include "leveldb\db.h"

#include <iostream>
#include <memory>


void TestDB()
{
	leveldb::Options options;
	options.create_if_missing = true;

	leveldb::Status status;
	leveldb::DB* db_ptr;

	status = leveldb::DB::Open(options, "test", &db_ptr);
	if (!status.ok())
	{
		std::cout << "Error in opening DB \"test\"" << std::endl;
		return;
	}

	std::shared_ptr<leveldb::DB> db(db_ptr);
	leveldb::WriteOptions write_options;
	status = db_ptr->Put(write_options, "key", "value");
	if (!status.ok())
	{
		std::cout << "Error in putting \"key\" into DB" << std::endl;
		return;
	}
}

int main()
{

	TestDB();

	system("pause");
	return 0;
}

