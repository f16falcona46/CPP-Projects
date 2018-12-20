#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[])
{
	if (argc != 4) {
		std::cout << "Wrong number of arguments, arguments should be: \n"
			<< "DB MIME-TYPE FILE\n";
		return 1;
	}
	sqlite3* db = nullptr;
	char* errorMessage = nullptr;
	int rc = 0;
	
	rc = sqlite3_open(argv[1], &db);
	if (rc) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return 1;
	}
	
	std::ifstream ifs(argv[3], std::ios::binary|std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  file(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&file[0], pos);
	
	sqlite3_stmt* statement;
	rc = sqlite3_prepare_v2(db, "INSERT INTO Pages (ContentType, Template, Name, Content) VALUES (?, ?, ?, ?)", -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "prepare failed\n";
		return 1;
	}
	rc = sqlite3_bind_text(statement, 1, argv[2], -1, SQLITE_STATIC);
	if (rc != SQLITE_OK) {
		std::cerr << "bind 1 failed\n";
		return 1;
	}
	rc = sqlite3_bind_int(statement, 2, 0);
	if (rc != SQLITE_OK) {
		std::cerr << "bind 2 failed\n";
		return 1;
	}
	rc = sqlite3_bind_text(statement, 3, argv[3], -1, SQLITE_STATIC);
	if (rc != SQLITE_OK) {
		std::cerr << "bind 3 failed\n";
		return 1;
	}
	rc = sqlite3_bind_blob(statement, 4, file.data(), file.size(), SQLITE_TRANSIENT);
	if (rc != SQLITE_OK) {
		std::cerr << "bind 4 failed\n";
		return 1;
	}
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		std::cerr << "execution failed\n";
		return 1;
	}
	sqlite3_finalize(statement);
	sqlite3_close(db);
}