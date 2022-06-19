#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>


using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::shared_ptr;
using std::ostream;
using std::ifstream;
using std::istringstream;
using std::make_shared;
using std::set;
using std::map;
using std::vector;
using std::set_intersection;


class QueryResult;
class TextQuery
{
public:
    using line_num = vector<string>::size_type;
    TextQuery(ifstream&);
    QueryResult query(const string&) const;
private:
    shared_ptr<vector<string>> file;
    map<string, shared_ptr<set<line_num>>> wm;
    string cleanup_str(const string&) const;
};

string TextQuery::cleanup_str(const string &word) const
{
    string ret;
    for (string::const_iterator it = word.begin(); 
				it != word.end(); ++it) {
        if (!ispunct(*it))
            ret += tolower(*it);
    }
    return ret;
}

TextQuery::TextQuery(ifstream &ifs)
:file(new vector<string>)
{
    string text;
    while (getline(ifs, text)) {
        file->push_back(text);
        int n = file->size() - 1;
        istringstream line(text);
        string word;
        while (line >> word) {
            word = cleanup_str(word);
            auto &lines = wm[word];
            if (!lines) {
                lines.reset(new set<line_num>);
            }
            lines->insert(n);
        }
    }
}

class QueryResult
{
    friend ostream& print(ostream&, const QueryResult&);
public:
    using line_num = vector<string>::size_type;
    using line_it = set<line_num>::const_iterator;
    QueryResult(string s,
                shared_ptr<set<line_num>> p,
                shared_ptr<vector<string>> f)
    :sought(s)
    ,lines(p)
    ,file(f)
    {}

    set<line_num>::size_type size() const
    {
        return lines->size();
    }

    line_it begin() const
    {
        return lines->begin();
    }

    line_it end() const
    {
        return lines->end();
    }

    shared_ptr<vector<string>> get_file()
    {
        return file;
    }
private:
    string sought;
    shared_ptr<set<line_num>> lines;
    shared_ptr<vector<string>> file;
};


QueryResult TextQuery::query(const string &sought) const
{
    static shared_ptr<set<line_num>> nodata(new set<line_num>);
    auto loc = wm.find(sought);
    if (loc == wm.end()) {
        return QueryResult(sought, nodata, file);
    }else {
        return QueryResult(sought, loc->second, file);
    }
}

class Query_base
{
    friend class Query;

protected:
    using line_num = TextQuery::line_num;
    virtual ~Query_base() = default;
private:
    //返回与当前Query匹配的QueryResult
    virtual QueryResult eval(const TextQuery&) const = 0;
    //表示查询的一个string
    virtual string rep() const = 0;
};

class Query
{
    friend Query operator~(const Query &);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);
public:
    Query(const string&);//构建新的WordQuery
    //接口函数
    QueryResult eval(const TextQuery &t) const
    {
        return q->eval(t);
    }
    string rep() const
    {
        return q->rep();
    }
private:
    Query(shared_ptr<Query_base> query)
    :q(query)
    {}
    shared_ptr<Query_base> q;
};



ostream & operator<<(ostream &os, const Query &query)
{
    return os << query.rep();
}
  
class WordQuery
:public Query_base
{
    friend class Query;
    WordQuery(const string &s)
    :query_word(s)
    {}

    QueryResult eval(const TextQuery &t) const
    {
        return t.query(query_word);
    }
    string rep() const
    {
        return query_word;
    }
    string query_word;
};

inline Query::Query(const string &s):q(new WordQuery(s)){}

class NotQuery
:public Query_base
{
    friend Query operator~(const Query &);
    NotQuery(const Query &q)
    :query(q)
    {}

    string rep() const
    {
        return "~(" + query.rep() + ")";
    }

    QueryResult eval(const TextQuery&) const;
    Query query;
};

inline Query operator~(const Query &operand)
{
    return shared_ptr<Query_base>(new NotQuery(operand));
}

QueryResult NotQuery::eval(const TextQuery &text) const
{
    auto result = query.eval(text);
    auto ret_lines = make_shared<set<line_num>>();
    auto beg = result.begin(), end = result.end();
    auto sz = result.get_file()->size();
    for (size_t n = 0; n != sz; ++n) 
    {
        if (beg == end || *beg != n) 
            ret_lines->insert(n);
        else if(beg != end) 
            ++beg;
    }
    return QueryResult(rep(), ret_lines, result.get_file()); 
}

class BinaryQuery
:public Query_base
{
protected:
    BinaryQuery(const Query &l, const Query &r, string s)
    :lhs(l)
    ,rhs(r)
    ,opSym(s)
    {}

    string rep() const
    {
        return "(" + lhs.rep() + " " + opSym + " " +rhs.rep() + ")";
    }

    Query lhs, rhs;
    string opSym;
};

class AndQuery
:public BinaryQuery
{
    friend Query operator&(const Query&, const Query&);
    AndQuery(const Query &left, const Query &right)
    :BinaryQuery(left, right, "&")
    {}
    QueryResult eval(const TextQuery&) const;
};

inline Query operator&(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

QueryResult AndQuery::eval(const TextQuery &text) const
{
    auto right = rhs.eval(text), left = lhs.eval(text);
    auto ret_lines = std::make_shared<set<line_num>>();
    set_intersection(left.begin(), left.end(),
                    right.begin(), right.end(),
                    inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}

class OrQuery
:public BinaryQuery
{
    friend Query operator|(const Query&, const Query&);
    OrQuery(const Query &left, const Query &right)
    :BinaryQuery(left, right, "|")
    {}
    QueryResult eval(const TextQuery&) const;
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
    auto right = rhs.eval(text), left = lhs.eval(text);
    auto ret_lines = std::make_shared<set<line_num>>(left.begin(), left.end());
    ret_lines->insert(right.begin(), right.end());
    return QueryResult(rep(), ret_lines, left.get_file());
}

string make_plural (size_t ctr, const string& word, const string& ending)
{
return (ctr>1)?word+ending:word;
}

ostream &print(ostream & os, const QueryResult &qr)
{
    os << qr.sought << " occurs " << qr.lines->size() << " "
       << make_plural(qr.lines->size(), "time", "s") << endl;//多次加就s
    for (auto num : *qr.lines) {
        os << "\t(line " << num + 1 << ")"
           << *(qr.file->begin() + num) << endl;
    }
    return os;
}

TextQuery get_file(int argc, char **argv)
{
    ifstream infile;
    infile.open(argv[1]);
    return TextQuery(infile);
}

bool get_word(string &s1)
{
    cout << "enter a word to search for: ";
    cin >> s1;
    return true;
}


int main(int argc, char **argv)
{
    TextQuery file = get_file(argc, argv);
    Query q1(string("for"));
    Query q2(string("the"));
    Query q3(string("of"));
    Query name = q1 & q2 & q3;    
    const QueryResult results = name.eval(file);
    cout << "\nExecuting Query for: " << name << endl;
    print(cout, results) << endl;
    return 0;
}



