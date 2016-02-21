#include "leveldb\db.h"
#include "leveldb\env.h"


#include <iostream>
#include <memory>
#include <vector>

std::shared_ptr<leveldb::Logger> NewLogger(const std::string& fname)
{
	struct LoggerDeleter
	{
		void operator()(leveldb::Logger* logger)
		{
			//std::cout << "deleting logger\n";
			if (logger != NULL)
			{
				delete logger;
			}
		}
	};

	leveldb::Logger* logger_raw;
	leveldb::Status status;

	status = leveldb::Env::Default()->NewLogger(fname, &logger_raw);
	if (!status.ok())
	{
		std::cout << "Error in creating logger\n";
		logger_raw = NULL;
	}
	return std::shared_ptr<leveldb::Logger>(logger_raw, LoggerDeleter());
}

void PrintDBProperties(leveldb::DB* db)
{
	if (db == NULL)
		return;

	std::string result;

	std::vector<std::string> properties =
	{
		"leveldb.stats",
		"leveldb.sstables",
		"leveldb.approximate-memory-usage",

	};

	for (int i = 0; i < properties.size(); ++i)
	{
		std::cout << properties[i] <<":"<< std::endl;
		if (db->GetProperty(properties[i], &result))
		{
			std::cout << result << std::endl;
		}
	}

	//std::string num_of_files = "num-files-at-level";
	//for (int i = 0; i < leveldb::config::kNumLevels; ++i)
	//{
	//	char num_str[16];
	//	itoa(i, num_str, 10);
	//	if (db->GetProperty(num_of_files + num_str, &result))
	//	{
	//		std::cout << "Level " << i << ": " << result << std::endl;
	//	}

	//}
}
void TestDB()
{
	leveldb::Options options;
	options.create_if_missing = true;
	options.write_buffer_size = 20;

	leveldb::Status status;

	auto logger = NewLogger("dblogger.log");
	options.info_log = logger.get();

	leveldb::DB* db_ptr;

	status = leveldb::DB::Open(options, "test", &db_ptr);
	if (!status.ok())
	{
		std::cout << "Error in opening DB \"test\"" << std::endl;
		return;
	}

	std::shared_ptr<leveldb::DB> db(db_ptr);
	leveldb::WriteOptions write_options;

	char key[100];
	char value[100];

	for (int i = 0; i < 12000; ++i)
	{
		sprintf_s(key, "key%05d", i);
		sprintf_s(value, "veryveryveryveryverylongvalue%05d", i);
		status = db->Put(write_options, key, value);
		if (!status.ok())
		{
			std::cout << "Error in putting key( " << key << "), value( " << value << ") into DB" << std::endl;
			continue;
		}
	}


	PrintDBProperties(db.get());

}

int main()
{

	TestDB();

	system("pause");
	return 0;
}

