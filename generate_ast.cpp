#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

using namespace std;

void defineType(ofstream& ofs, const string& baseName, const string& className, const string& fieldList) {
	ofs << endl;
	ofs << "template<class T>" << endl;
	ofs << "class " + className + " : public Expr<T>, public std::enable_shared_from_this<" + 
		className + "<T>> {" << endl;
	ofs << "public:" << endl;

	vector<string> fields;
	boost::split(fields, fieldList, boost::is_any_of(","));
	for_each(begin(fields), end(fields), [] (auto& s) { boost::trim(s); });

	vector<pair<string, string>> parameters;

	for (auto& field : fields) {
		vector<string> res;
		boost::split(res, field, boost::is_any_of(" "));
		string& type = res[0];
		type += type == "Token" || type == "any" ? "" : "<T>";
		string& name = res[1];

		parameters.emplace_back(type, name);
	}
	

	// Constructor
	ofs << "\t" + className + "(";
	for (auto& p : parameters) {
		auto& [type, name] = p;
		ofs << "std::shared_ptr<" + type + "> " + name + (&parameters.back() - &p != 0 ? ", " : "");
	}
	ofs << ") {" << endl;

	// Store parameters in fields
	for (auto& p : parameters) {
		auto& [type, name] = p;
		ofs << "\t\tthis->" + name + " = " + name + ";" << endl;
	}

	ofs << "\t}" << endl << endl;

	ofs << "\tT accept(std::shared_ptr<Visitor<T>> visitor) override {" << endl;
	ofs << "\t\treturn visitor->visit" + className + baseName + "( this->shared_from_this() );" << endl;
	ofs << "\t}" << endl << endl;

	// Fields
	for (auto& p : parameters) {
		auto& [type, name] = p;
		ofs << "\tstd::shared_ptr<" + type + "> " + name + ";" << endl;
	}

	ofs << "};" << endl;
}

void defineVisitor(ofstream& ofs, const string& baseName, const vector<string>& types) {
	vector<string> dataTypes;
	for (auto& type : types) {
		vector<string> res;
		boost::split(res, type, boost::is_any_of(":"));
		for_each(begin(res), end(res), [] (auto& s) { boost::trim(s); });

		const string& className = res[0];
		const string& fields = res[1];

		dataTypes.push_back(className);
	}

	// Forward class declarations
	ofs << endl;
	for (auto& t : dataTypes) {
		ofs << "template<class T> class " + t + ";" << endl;
	}

	ofs << endl << "template<class T>" << endl;
	ofs << "class Visitor {" << endl;
	ofs << "public:" << endl;

	ofs << "\tvirtual ~Visitor() = default;" << endl;

	for (auto& t : dataTypes) {
		ofs << "\tvirtual T visit" + t + baseName + "(std::shared_ptr<" + t + "<T>> " + boost::to_lower_copy<string>(baseName) + ") = 0;" << endl;
	}

	ofs << "};" << endl;
	
}

void defineAst(const string& output_dir, const string& baseName, const vector<string>& types) {
	const string path = output_dir + "/" + baseName + ".h";

	ofstream ofs(path, ofstream::out);

	ofs << "#ifndef CRAFTING_INTERPRETER_EXPR_H" << endl;
	ofs << "#define CRAFTING_INTERPRETER_EXPR_H" << endl << endl;

	ofs << "#include <vector>" << endl;
	ofs << "#include <memory>" << endl;
	ofs << "#include <any>" << endl << endl;
	ofs << "#include \"Token.h\"" << endl << endl;
	ofs << "using std::any;" << endl << endl;

	ofs << "enum class Expression {" << endl;
	ofs << "\tExpr," << endl;

	for (auto& type : types) {
		vector<string> res;
		boost::split(res, type, boost::is_any_of(":"));
		for_each(begin(res), end(res), [] (auto& s) { boost::trim(s); });

		const string& className = res[0];
		const string& fields = res[1];

		ofs << "\t" + className + (&types.back() != &type ? "," : "") << endl;
	}

	ofs << "};" << endl << endl;

	defineVisitor(ofs, baseName, types);
	ofs << endl;
	// ofs << "class Visitor;" << endl << endl;
	ofs << "template<class T>" << endl;
	ofs << "class Expr {" << endl;
	ofs << "public:" << endl;

	ofs << "\tvirtual ~Expr() = default;" << endl;
	ofs << "\tvirtual T accept(std::shared_ptr<Visitor<T>> visitor) = 0;" << endl;
	
	ofs << "};" << endl;

	for (auto& type : types) {
		vector<string> res;
		boost::split(res, type, boost::is_any_of(":"));
		for_each(begin(res), end(res), [] (auto& s) { boost::trim(s); });

		const string& className = res[0];
		const string& fields = res[1];

		defineType(ofs, baseName, className, fields);
	}

	// defineVisitor(ofs, baseName, types);

	ofs << endl << "#endif" << endl;
	ofs.close();

}


int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: generate_ast <output_dir>" << endl;
		exit(1);
	}

	string output_dir = argv[1];
	defineAst(output_dir, "Expr", vector<string>{
      "Binary   : Expr left, Token op, Expr right",
      "Grouping : Expr expression",
      "Literal  : any value",
      "Unary    : Token op, Expr right"
	});
}
