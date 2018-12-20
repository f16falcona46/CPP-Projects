#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <stdexcept>
#include <fcgio.h>
#include <sqlite3.h>
#include <ctemplate/template.h>

std::map<std::string, std::string> Parse(const std::string& query)
{
	std::map<std::string, std::string> data;
	std::regex pattern("([\\w+%]+)=([^&]*)");
	auto words_begin = std::sregex_iterator(query.begin(), query.end(), pattern);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; i++) {
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		data[key] = value;
	}

	return data;
}


int main(void)
{
	//open page DB
	sqlite3* db = nullptr;
	char* errorMessage = nullptr;
	int returncode = 0;
	
	returncode = sqlite3_open("/var/www/html/cgi-bin/sites.db", &db);
	if (returncode) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return 1;
	}
	
	//prepare page statement
	sqlite3_stmt* page_statement = nullptr;
	returncode = sqlite3_prepare_v2(db, "SELECT Name, Content, Template, ContentType, Parent FROM Pages WHERE ID = ?;", -1, &page_statement, nullptr);
	if (returncode != SQLITE_OK) {
		std::cerr << "Statement prepare error: " << sqlite3_errmsg(db) << std::endl;
		return 1;
	}
	
	//prepare template statement
	sqlite3_stmt* templateStatement = nullptr;
	sqlite3* templateDB = nullptr;
	returncode = sqlite3_open("/var/www/html/cgi-bin/sites.db", &templateDB);
	if (returncode) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(templateDB) << std::endl;
		sqlite3_close(templateDB);
		return 1;
	}
	returncode = sqlite3_prepare_v2(templateDB, "SELECT TemplateText FROM Templates WHERE ID = ?;", -1, &templateStatement, nullptr);
	if (returncode != SQLITE_OK) {
		std::cerr << "Statement prepare error: " << sqlite3_errmsg(templateDB) << std::endl;
		return 1;
	}
	
	// Backup the stdio streambufs
	std::streambuf* cin_streambuf  = std::cin.rdbuf();
	std::streambuf* cout_streambuf = std::cout.rdbuf();
	std::streambuf* cerr_streambuf = std::cerr.rdbuf();

	FCGX_Request request;

	FCGX_Init();
	FCGX_InitRequest(&request, 0, 0);

	while (FCGX_Accept_r(&request) == 0) {
		//set up fcgi streambufs and redirect
		fcgi_streambuf cin_fcgi_streambuf(request.in);
		fcgi_streambuf cout_fcgi_streambuf(request.out);
		fcgi_streambuf cerr_fcgi_streambuf(request.err);

		std::cin.rdbuf(&cin_fcgi_streambuf);
		std::cout.rdbuf(&cout_fcgi_streambuf);
		std::cerr.rdbuf(&cerr_fcgi_streambuf);
		
		//get page number; if not given then assume it's the index page
		int pageIndex = -1;
		const char* uriptr = FCGX_GetParam("REQUEST_URI", request.envp);
		if (std::string("/") == uriptr) uriptr = "/cgi-bin/getpage.cgi?p=1";
		
		std::map<std::string, std::string> parsedRequest = Parse(uriptr);
		std::string requestPageString = parsedRequest["p"];
		
		try {
			pageIndex = std::stoi(requestPageString);
		}
		catch (const std::invalid_argument& e) {}
		
		
		if (std::string(uriptr).find("/cgi-bin/getpage.cgi?") != 0) pageIndex = -1;
		
		sqlite3_reset(page_statement);
		sqlite3_bind_int(page_statement, 1, pageIndex);

		int step = sqlite3_step(page_statement);

		if (step == SQLITE_ROW) {
			int templateID = sqlite3_column_int(page_statement, 2);
			if (templateID) { //template?
				sqlite3_reset(templateStatement);
				sqlite3_bind_int(templateStatement, 1, templateID);
				int templateStep = sqlite3_step(templateStatement);
				if (templateStep == SQLITE_ROW) {
					const char* templateText = reinterpret_cast<const char*>(sqlite3_column_text(templateStatement, 0));
					ctemplate::StringToTemplateCache("Template_" + std::to_string(templateID), templateText, ctemplate::DO_NOT_STRIP);
					
					std::string expandedText;
					ctemplate::TemplateDictionary dict("page");
					dict.SetValue("NAME", reinterpret_cast<const char*>(sqlite3_column_text(page_statement, 0)));
					dict.SetValue("CONTENT", reinterpret_cast<const char*>(sqlite3_column_text(page_statement, 1)));
					dict.SetValue("PARENT", reinterpret_cast<const char*>(sqlite3_column_text(page_statement, 4)));
					
					ctemplate::ExpandTemplate("Template_" + std::to_string(templateID), ctemplate::DO_NOT_STRIP, &dict, &expandedText);
					
					std::cout << "Content-type: " << sqlite3_column_text(page_statement, 3) << "\r\n\r\n" << expandedText;
				}
				else {
					std::cout << "Content-type: text/html\r\n\r\n"
						<< "<html>\n"
						<< "\t<head>\n"
						<< "\t\t<title>Internal Server Error</title>\n"
						<< "\t</head>\n"
						<< "\t<body>\n"
						<< "Template not found. Request URI: " << uriptr
						<< "\t</body>\n"
						<< "</html>\n";
				}
			}
			else {
				std::cout << "Content-type: " << sqlite3_column_text(page_statement, 3) << "\r\n\r\n";
				std::cout.write(static_cast<const char*>(sqlite3_column_blob(page_statement, 1)), sqlite3_column_bytes(page_statement, 1));
			}
		}
		else {
			std::cout << "Content-type: text/html\r\n\r\n"
				<< "<html>\n"
				<< "\t<head>\n"
				<< "\t\t<title>Error</title>\n"
				<< "\t</head>\n"
				<< "\t<body>\n"
				<< "Page not found. Request URI: " << uriptr
				<< "\t</body>\n"
				<< "</html>\n";
		}
	}

	// restore stdio streambufs
	std::cin.rdbuf(cin_streambuf);
	std::cout.rdbuf(cout_streambuf);
	std::cerr.rdbuf(cerr_streambuf);
	
	sqlite3_finalize(templateStatement);
	sqlite3_close(templateDB);
	
	sqlite3_finalize(page_statement);
	sqlite3_close(db);

	return 0;
}
