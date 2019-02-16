#include "Ruota.h"

std::string RuotaWrapper::path = "\\";
std::string RuotaWrapper::curr_file = "";
std::string RuotaWrapper::current_dir = "";

std::unordered_map<std::string, int> RuotaWrapper::operators = {
		{"load", 999},
		{"PASS_REG", 999},
		{".index", 14},
		{".exec", 14},
		{"from", 13},
		{".generic", 14},
		{"struct", 13},
		{"template", 16},
		{"new", 13},
		{"return", 2},
		{"::", 13},
		{"!", -14},
		{".!", -14},
		{".", 14},
		{"->", 3},
		{"=>", 12},
		{"len", -11},
		{"&", -3},
		{".negate", -11},
		{".positive", -11},
		{"**", -10},
		{".**", -10},
		{"*", 9},
		{".*", 9},
		{"/", 9},
		{"./", 9},
		{"%", 9},
		{".%", 9},
		{"+", 8},
		{".+", 8},
		{"..", 8},
		{"|", 7},
		{"-", 8},
		{".-", 8},
		{":", 13},
		{"<:", 7},
		{":>", 7},
		{"<:>", 7},
		{"==", 6},
		{".=", 6},
		{".==", 6},
		{"!=", 6},
		{".!=", 6},
		{"<", 6},
		{".<", 6},
		{">", 6},
		{".>", 6},
		{"<=", 6},
		{".<=", 6},
		{">=", 6},
		{".>=", 6},
		{"&&", 5},
		{"||", 5},
		{"=", -4},
		{"+=", -4},
		{".+=", -4},
		{"-=", -4},
		{".-=", -4},
		{"*=", -4},
		{".*=", -4},
		{"/=", -4},
		{"./=", -4},
		{"%=", -4},
		{".%=", -4},
		{"**=", -4},
		{".**=", -4},
		{"..=", -4},
		{"++=", -4},
		{":=", -4},
		{"&=", -4},
		{":&", -4},
		{":&=", -4},
		{"in", 3},
		{"switch", 3},
		{"do", 3},
		{"->>", -3},
		{"then", 3},
		{"else", -3},
		{"detach", -2},
		{"try", -16},
		{"catch", 15},
		{",", 2},
		{";", -1}};

RuotaWrapper::RuotaWrapper(std::string current_dir)
{
    RuotaWrapper::current_dir = current_dir;
    this->T = new Tokenizer(operators);
    this->MAIN = new Scope(NULL);
}

Node RuotaWrapper::parseLine(const std::string &line, const std::string &local_file)
{
    auto tokens = T->tokenize(line);
    tokens = T->infixToPostfix(tokens);
    std::vector<Node> stack;
    /*  for (auto &t : tokens)
        std::cout << t << "\t";*/


    for (auto &t : tokens)
    {
        if (t == ";" || t == ",")
            continue;
        if (operators.find(t) != operators.end())
        {
            if (t == ":")
            {
                Node key = stack.back();
                stack.pop_back();
                std::string value = stack.back().values[0];
                stack.pop_back();
                stack.push_back(Node(B_DEC, {value}));
                stack.back().params.push_back(key);
            }
            else if (t == "load")
            {
                Scope temp(NULL);
                auto dp = stack.back().evaluate(&temp).getVector();
                std::string var_string = "";
                for (auto &d : dp)
                    var_string += std::string(1, d.getChar());
                if (var_string.back() == '*')
                {
                    std::vector<std::string> files;
                    std::string path = this->path.substr(1) + var_string;
                    path.pop_back();
                    for (auto &p : boost::filesystem::directory_iterator(path))
                    {
                        std::string file = p.path().string();
                        if (file.length() > 4 && file.compare(file.length() - 4, 4, ".ruo") == 0)
                            files.push_back(file);
                    }

                    for (auto &f : files)
                    {
                        std::string filename_raw = f;
                        std::string path = local_file;
                        stack.pop_back();
                        std::string filename = "";

                        //if (local_file == "") {
                        path += filename_raw;
                        while (path.back() != '\\' && path.back() != '/')
                        {
                            filename = std::string(1, path.back()) + filename;
                            path.pop_back();
                            if (path.empty())
                                break;
                        }
                        //}

                        if (std::find(LOADED.begin(), LOADED.end(), filename) == LOADED.end())
                        {
                            std::string content = "";
                            std::string line;
                            std::ifstream myfile(path.substr(1) + filename);
                            if (myfile.is_open())
                            {
                                while (getline(myfile, line))
                                    content += line + "\n";
                                myfile.close();
                            }
                            else
                            {
                                std::ifstream myfilelocal(this->current_dir + filename_raw);
                                if (myfilelocal.is_open())
                                {
                                    while (getline(myfilelocal, line))
                                        content += line + "\n";
                                    myfilelocal.close();
                                }
                                else
                                {
                                    throw std::runtime_error(("Error: Cannot Load File: " + this->current_dir + filename + "!\nload \"" + filename_raw.substr(0, filename_raw.length() - 4) + "\";").c_str());
                                }
                            }
                            LOADED.push_back(filename);
                            this->path = path;
                            std::string old_file = this->curr_file;
                            this->curr_file = filename;
                            auto gen = parseLine(content, path).params;
                            this->path = local_file;
                            this->curr_file = old_file;
                            for (auto &p : gen)
                                stack.push_back(p);
                        }
                        else
                        {
                            stack.push_back(Node(DataPoint((int)0)));
                        }
                    }
                }
                else
                {
                    std::string filename_raw = var_string + ".ruo";
                    std::string path = local_file;
                    stack.pop_back();
                    std::string filename = "";

                    //if (local_file == "") {
                    path += filename_raw;
                    while (path.back() != '\\' && path.back() != '/')
                    {
                        filename = std::string(1, path.back()) + filename;
                        path.pop_back();
                        if (path.empty())
                            break;
                    }
                    //}

                    if (std::find(LOADED.begin(), LOADED.end(), filename) == LOADED.end())
                    {
                        std::string content = "";
                        std::string line;
                        std::ifstream myfile(path.substr(1) + filename);
                        if (myfile.is_open())
                        {
                            while (getline(myfile, line))
                                content += line + "\n";
                            myfile.close();
                        }
                        else
                        {
                            std::ifstream myfilelocal(this->current_dir + filename_raw);
                            if (myfilelocal.is_open())
                            {
                                while (getline(myfilelocal, line))
                                    content += line + "\n";
                                myfilelocal.close();
                            }
                            else
                            {
                                throw std::runtime_error(("Error: Cannot Load File: " + path.substr(1) + filename + "!\nload \"" + filename_raw.substr(0, filename_raw.length() - 4) + "\";").c_str());
                            }
                        }
                        LOADED.push_back(filename);
                        this->path = path;
                        std::string old_file = this->curr_file;
                        this->curr_file = filename;
                        auto gen = parseLine(content, path).params;
                        this->path = local_file;
                        this->curr_file = old_file;
                            for (auto &p : gen)
                                stack.push_back(p);
                    }
                    else
                    {
                        stack.push_back(Node(DataPoint((int)0)));
                    }
                }
            }
            else if (t == "PASS_REG")
            {
                Node key = stack.back();
                stack.pop_back();
                std::string value = stack.back().values[0];
                stack.pop_back();
                stack.push_back(Node(B_REG, {value}));
                stack.back().params.push_back(key);
            }
            else if (t == "return")
            {
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(RETURN, {a_}));
            }
            else if (t == ".negate")
            {
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_MUL, {a_, Node(DataPoint((int)-1))}));
            }
            else if (t == "&")
            {
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(FLAG_POINTER, {a_}));
            }
            else if (t == "=")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_SET, {a_, b_}));
            }
            else if (t == "+")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_ADD, {a_, b_}));
            }
            else if (t == ".generic")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_GENERIC, {a_, b_}));
            }
            else if (t == "-")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_SUB, {a_, b_}));
            }
            else if (t == "*")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_MUL, {a_, b_}));
            }
            else if (t == "/")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_DIV, {a_, b_}));
            }
            else if (t == "%")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_MOD, {a_, b_}));
            }
            else if (t == "**")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_POW, {a_, b_}));
            }
            else if (t == "<")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_LESS, {a_, b_}));
            }
            else if (t == ">")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_MORE, {a_, b_}));
            }
            else if (t == "<=")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_ELESS, {a_, b_}));
            }
            else if (t == ">=")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_EMORE, {a_, b_}));
            }
            else if (t == "==")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_EQUALS, {a_, b_}));
            }
            else if (t == "!=")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_NEQUALS, {a_, b_}));
            }
            else if (t == "||")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_OR, {a_, b_}));
            }
            else if (t == "&&")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_AND, {a_, b_}));
            }
            else if (t == "then")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_IF_ELSE, {a_, b_}));
            }
            else if (t == "|" || t == "else")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                a_.params.push_back(b_);
                stack.push_back(a_);
            }
            else if (t == ".index")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_INDEX, {a_, b_}));
            }
            else if (t == ".exec")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                b_.nt = SEQUENCE;
                stack.push_back(Node(B_EXEC, {a_, b_}));
            }
            else if (t == ".")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_OBJ_INDEX, {a_, b_}));
            }
            else if (t == "->")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_RANGE, {a_, b_}));
            }
            else if (t == "..")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_RANGE_INDIC, {a_, b_}));
            }
            else if (t == "do")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_DO_LOOP, {a_, b_}));
            }
            else if (t == "in")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(B_ITER, {a_, b_}));
            }
            else if (t == "=>")
            {
                Node b_ = stack.back();
                stack.pop_back();
                Node a_ = stack.back();
                stack.pop_back();
                b_.nt == SEQUENCE;
                stack.push_back(Node(LAM_DEC, {a_, b_}));
            }
            else if (t == "new")
            {
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(U_NEW, {a_}));
            }
            else if (t == "len")
            {
                Node a_ = stack.back();
                stack.pop_back();
                stack.push_back(Node(U_LEN, {a_}));
            }
            else if (t == "struct")
            {
                Node key = stack.back();
                stack.pop_back();
                std::string value = stack.back().values[0];
                stack.pop_back();
                stack.push_back(Node(B_OBJ, {value}));
                stack.back().params.push_back(key);
            }
            else if (t == "template")
            {
                std::string value = stack.back().values[0];
                stack.pop_back();
                stack.push_back(Node(B_TEMP, {value}));
            }
            else if (t == "::")
            {
                Node key = stack.back();
                stack.pop_back();
                std::string value = stack.back().values[0];
                stack.pop_back();
                stack.push_back(Node(B_STATIC, {value}));
                stack.back().params.push_back(key);
            }
        }
        else if (t[0] == '\'')
        {
            stack.push_back(Node(DataPoint((char)t[1])));
        }
        else if (t[0] == '\"')
        {
            std::vector<DataPoint> chars;
            for (auto &c : t.substr(1))
            {
                chars.push_back(DataPoint((char)c));
            }
            DataPoint dp(DataType("char", {}, true, {chars.size()}));
            dp.value_vector = chars;

            stack.push_back(Node(dp));
        }
        else if (t == "}")
        {
            std::vector<Node> seq;
            while (stack.back().values.empty() || stack.back().values[0] != "{")
            {
                seq.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
            std::reverse(seq.begin(), seq.end());
            stack.push_back(Node(SCOPE, {Node(SEQUENCE_LAX, seq)}));
        }
        else if (t == "]")
        {
            std::vector<Node> seq;
            while (stack.back().values.empty() || stack.back().values[0] != "[")
            {
                seq.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
            std::reverse(seq.begin(), seq.end());
            stack.push_back(Node(SEQUENCE, seq));
        }
        else if (t == ")")
        {
            std::vector<Node> seq;
            while (stack.back().values.empty() || stack.back().values[0] != "(")
            {
                seq.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
            std::reverse(seq.begin(), seq.end());
            stack.push_back(Node(SEQUENCE_LAX, seq));
        }
        else if (t == "~")
        {
            stack.push_back(Node(DataPoint((int)0)));
        }
        else if (t == "true")
        {
            stack.push_back(Node(DataPoint(true)));
        }
        else if (t == "false")
        {
            stack.push_back(Node(DataPoint(false)));
        }
        else if (isdigit(t[0]))
        {
            if (t.back() == 'D')
            {
                stack.push_back(Node(std::stod(t.substr(0, t.size() - 1))));
            }
            else
                stack.push_back(Node(DataPoint(std::stol(t))));
        }
        else if (t == "break")
        {
            stack.push_back(Node(DataPoint(DataType("break"))));
            stack.back().dp.flag_return = true;
        }
        else
        {
            stack.push_back(Node(VARIABLE, {t}));
        }
    }

    Node n(SEQUENCE, stack);
    return n;
}

const DataPoint RuotaWrapper::evaluate(const std::string &line)
{
    Node n = parseLine(line, "\\");
    return n.evaluate(MAIN);
}

RuotaWrapper::~RuotaWrapper()
{
    delete T;
    delete MAIN;
}