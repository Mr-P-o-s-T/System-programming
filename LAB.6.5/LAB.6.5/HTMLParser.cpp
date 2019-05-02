#include "HTMLParser.h"

#include <algorithm>
using namespace std;

HTMLParser::HTMLParser(const char *resIdPath, const char *logExprPath) {
	resId = new DataHolder(resIdPath);
	logExpr = new DataHolder(logExprPath);
}

HTMLParser::~HTMLParser() {
	delete resId;
	delete logExpr;
}

void HTMLParser::prepareOutputFile(std::ofstream & output) {
	output << "\
<!DOCTYPE html>\n\
<html>\n\
\n\
<head>\n\
<title>input.f</title>\n\
<link rel=\"stylesheet\" href=\"dtb\\css.css\">\n\
</head>\n\
\n\
<body>\n\
<pre>\n";
}

void HTMLParser::finishOutputFile(std::ofstream & output) {
	output << "\
\n</pre>\n</body>\n\
\n\
</html>";
}

string HTMLParser::parse(const string &str, const char *type) {
	if ((type[0] == '@') && (str.size() > 0)) {
		size_t i;
		for (i = 0; i < str.size() && ((str[i] == ' ') || 
			(str[i] == '\t') || (str[i] == '\n')); i++);
		string res(str.substr(0, i));
		res.append("<span class=\"");

		switch (type[1]) {
		case 'i':
			if (resId->compareStr(str.substr(i))) {
				res.append("reserved\">");
				res.append(str.substr(i));
				res.append("</span>");
			}
			else {
				res.append("id\">");
				res.append(str.substr(i));
				res.append("</span>");
			}
			break;
		case 'n':
			res.append("number\">");
			res.append(str.substr(i));
			res.append("</span>");
			break;
		case 'l':
			if (logExpr->compareStr(str.substr(i))) {
				string tmp(str);
				std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
				if ((tmp.rfind("TRUE") != tmp.npos) || (tmp.rfind("FALSE") != tmp.npos)) {
					res.append("logicalConst\">");
					res.append(str.substr(i));
					res.append("</span>");
				}
				else {
					res.append("logicalOperator\">");
					res.append(str.substr(i));
					res.append("</span>");
				}
			}
			else {
				res.append("error\">");
				res.append(str.substr(i));
				res.append("</span>");
			}
			break;
		case 'o':
			res.append("operator\">");
			res.append(str.substr(i));
			res.append("</span>");
			break;
		case 's':
			res.append("string\">");
			res.append(str.substr(i));
			res.append("</span>");
			break;
		case 'c':
			res.append("comment\">");
			res.append(str.substr(i));
			res.append("</span>");
			break;
		default:
			res.append("error\">");
			res.append(str.substr(i));
			res.append("</span>");
			break;
		}

		return res;
	}
	return "";
}
