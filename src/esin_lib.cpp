// Implementation of ESIN library functions

#include <esin/error>
#include <esin/util>
#include <esin/gen_driver>
#include <fstream>
#include <sstream>
#include <cstring>

// ============ error class implementation ============

std::map<int, string> error::tabla_mensajes;
std::map<int, string> error::tabla_modulos;

error::error(int codigo, const string& nom_mod, const string& msg)
    : _codigo(codigo), _modulo(nom_mod), _mensaje(msg)
{
    if (_mensaje.empty() && tabla_mensajes.find(codigo) != tabla_mensajes.end()) {
        _mensaje = tabla_mensajes[codigo];
    }
    if (_modulo.empty() && tabla_modulos.find(codigo) != tabla_modulos.end()) {
        _modulo = tabla_modulos[codigo];
    }
}

void error::load_messages(istream& is) {
    int code;
    string mod, msg;
    while (is >> code) {
        is >> mod;
        std::getline(is, msg);
        // Trim leading whitespace
        size_t start = msg.find_first_not_of(" \t");
        if (start != string::npos) {
            msg = msg.substr(start);
        }
        tabla_mensajes[code] = msg;
        tabla_modulos[code] = mod;
    }
}

int error::codigo() const noexcept { return _codigo; }
const string& error::modulo() const noexcept { return _modulo; }
const string& error::mensaje() const noexcept { return _mensaje; }

void error::print(ostream& os) const noexcept {
    os << "Error " << _codigo;
    if (!_modulo.empty()) os << " [" << _modulo << "]";
    if (!_mensaje.empty()) os << ": " << _mensaje;
}

const char* error::what() const noexcept {
    static thread_local string msg_buffer;
    std::ostringstream oss;
    print(oss);
    msg_buffer = oss.str();
    return msg_buffer.c_str();
}

ostream& operator<<(ostream& os, const error& e) {
    e.print(os);
    return os;
}

// ============ util namespace implementation ============

namespace util {

static timeval _start_time;

void start_time() noexcept {
    gettimeofday(&_start_time, nullptr);
}

double stop_time() noexcept {
    timeval end_time;
    gettimeofday(&end_time, nullptr);
    double elapsed = (end_time.tv_sec - _start_time.tv_sec) +
                     (end_time.tv_usec - _start_time.tv_usec) / 1000000.0;
    return elapsed;
}

string tostring(long int n) noexcept {
    return std::to_string(n);
}

string tostring(int n) noexcept {
    return std::to_string(n);
}

string tostring(double x) noexcept {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

int toint(const string& s) {
    size_t pos;
    int result = std::stoi(s, &pos);
    if (pos != s.length()) {
        throw error(0, nom_mod, "Invalid integer: " + s);
    }
    return result;
}

double todouble(const string& s) {
    size_t pos;
    double result = std::stod(s, &pos);
    if (pos != s.length()) {
        throw error(0, nom_mod, "Invalid double: " + s);
    }
    return result;
}

bool is_nat(const string& s) noexcept {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool is_int(const string& s) noexcept {
    if (s.empty()) return false;
    size_t start = 0;
    if (s[0] == '+' || s[0] == '-') start = 1;
    if (start == s.length()) return false;
    for (size_t i = start; i < s.length(); ++i) {
        if (!std::isdigit(s[i])) return false;
    }
    return true;
}

bool is_double(const string& s) noexcept {
    if (s.empty()) return false;
    try {
        size_t pos;
        std::stod(s, &pos);
        return pos == s.length();
    } catch (...) {
        return false;
    }
}

byte pack(const string& s) {
    if (s.length() != 8) {
        throw error(0, nom_mod, "pack requires 8-character string");
    }
    byte result = 0;
    for (int i = 0; i < 8; ++i) {
        if (s[i] == '1') result |= (1 << (7 - i));
        else if (s[i] != '0') throw error(0, nom_mod, "Invalid character in pack");
    }
    return result;
}

string unpack(byte c) noexcept {
    string result(8, '0');
    for (int i = 0; i < 8; ++i) {
        if (c & (1 << (7 - i))) result[i] = '1';
    }
    return result;
}

void split(const string& s, vector<string>& x) noexcept {
    x.clear();
    std::istringstream iss(s);
    string word;
    while (iss >> word) {
        x.push_back(word);
    }
}

void split(char* line, vector<string>& x) noexcept {
    split(string(line), x);
}

// Random class implementation
Random::Random(long seed) {
    init_rand(seed);
}

void Random::init_rand(long seed) {
    A[0] = seed;
    for (int i = 1; i < 56; ++i) {
        A[i] = (A[i-1] * 1103515245 + 12345) & 0x7FFFFFFF;
    }
    fptr = A + 54;
}

long Random::cycle() {
    long *p = A;
    long *q = A + 31;
    while (q < A + 55) {
        *p = (*p - *q) & 0x7FFFFFFF;
        ++p; ++q;
    }
    q = A;
    while (p < A + 55) {
        *p = (*p - *q) & 0x7FFFFFFF;
        ++p; ++q;
    }
    fptr = A + 54;
    return *fptr--;
}

long Random::operator()(long a, long b) {
    return a + rand() % (b - a + 1);
}

long Random::operator()(long n) {
    return rand() % n;
}

double Random::operator()() noexcept {
    return rand() / double(0x7FFFFFFF);
}

} // namespace util

// ============ gen_driver implementation ============

const string gen_driver::nom_mod = "gen_driver";
const string gen_driver::NoOpMsg = "Operation not found";
const string gen_driver::WrongNumArgsMsg = "Wrong number of arguments";
const string gen_driver::WrongTypeArgsMsg = "Wrong type of arguments";
const string gen_driver::NoObjMsg = "Object not found";
const string gen_driver::WrongTypeObjMsg = "Wrong object type";
const string gen_driver::NoFileMsg = "File not found";

const string gen_driver::input_line::nom_mod = "input_line";
const string gen_driver::input_line::ArgNoExisteMsg = "Argument does not exist";
bool gen_driver::input_line::_echoing = true;
std::ofstream* gen_driver::_null_stream = nullptr;

void* user_init(gen_driver& dr);

gen_driver::gen_driver(const string& error_filename, bool active_memdin,
                       bool echoing, istream& is, ostream& os)
    : _echoing(echoing), _memdin(active_memdin), _isdriv(is), _osdriv(os)
{
    _il = new input_line(_osdriv);
    _curr_obj = "";

    if (!error_filename.empty()) {
        std::ifstream errfile(error_filename);
        if (errfile) {
            error::load_messages(errfile);
        }
    }

    // Built-in commands
    add_call("init", [](gen_driver& d) { d.init(); }, "*", "", "Create a new object");
    add_call("copy", [](gen_driver& d) { d.copy(); }, "*", "any", "Copy an object");
    add_call("initcopy", [](gen_driver& d) { d.initcopy(); }, "*", "any", "Create copy of object");
    add_call("destroy", [](gen_driver& d) { d.destroy(); }, "any", "", "Destroy an object");
    add_call("test_memory", [](gen_driver& d) { d.test_memory(); }, "*", "", "Test memory");
    add_call("set_memory", [](gen_driver& d) { d.set_memory(); }, "*", "", "Set memory");
    add_call("print_memory", [](gen_driver& d) { d.print_memory(); }, "*", "", "Print memory");
    add_call("memory_on", [](gen_driver& d) { d.memory(true); }, "*", "", "Enable memory");
    add_call("memory_off", [](gen_driver& d) { d.memory(false); }, "*", "", "Disable memory");
    add_call("echo", [](gen_driver& d) { d.echo(); }, "*", "", "Toggle echo");
    add_call("load", [](gen_driver& d) { d.load(); }, "*", "string", "Load from file");
    add_call("help", [](gen_driver& d) { d.help(); }, "*", "", "Show help");
    add_call("select", [](gen_driver& d) { d.select_curr_obj(); }, "*", "any", "Select object");
    add_call("list", [](gen_driver& d) { d.list_objects(); }, "*", "", "List objects");
    add_call("curr", [](gen_driver& d) { d.curr_obj_name(); }, "*", "", "Current object");
    add_call("types", [](gen_driver& d) { d.known_types(); }, "*", "", "Known types");
    add_call("echo_output", [](gen_driver& d) { d.echo_output(); }, "*", "", "Echo output");
    add_call("echo_input", [](gen_driver& d) { d.echo_input(); }, "*", "", "Echo input");
    add_call("timer_on", [](gen_driver& d) { d.timer_on(); }, "*", "", "Start timer");
    add_call("timer_off", [](gen_driver& d) { d.timer_off(); }, "*", "", "Stop timer");
    add_call("iter", [](gen_driver& d) { d.iter(); }, "*", "int", "Iterate");
}

gen_driver::~gen_driver() {
    clear_all();
    delete _il;
    if (_null_stream) {
        delete _null_stream;
        _null_stream = nullptr;
    }
}

void gen_driver::add_call(const string& fname, DriverFunction f,
                          const string& applies_to, const string& type_args,
                          const string& helpmsg) {
    _funct[fname] = f;
    _applies_to[fname] = applies_to;
    _help[fname] = helpmsg;

    vector<string> args;
    util::split(type_args, args);
    _type_args[fname] = args;
}

void gen_driver::add_check(const string& tid, CheckFunction f) {
    _check[tid] = f;
}

const string& gen_driver::object_type(const string& obj_name) {
    static string empty = "";
    if (_type.find(obj_name) == _type.end()) return empty;
    return _type[obj_name];
}

const string& gen_driver::object_type() {
    return object_type(_il->object());
}

const string& gen_driver::object_type(int i) {
    return object_type(_il->args(i));
}

bool gen_driver::has_type(int i) {
    return _type.find(_il->args(i)) != _type.end();
}

const string& gen_driver::args(int i) {
    return _il->args(i);
}

nat gen_driver::nargs() {
    return _il->nargs();
}

void gen_driver::go() {
    while (_isdriv.good()) {
        try {
            _il->read(_isdriv);
            if (!_isdriv.good()) break;
            process_operation(_il);
        } catch (const error& e) {
            _osdriv << e << std::endl;
        } catch (const std::exception& e) {
            _osdriv << "Exception: " << e.what() << std::endl;
        }
    }
}

void gen_driver::process_operation(input_line* L) {
    string op = L->op();
    if (op.empty()) return;

    if (_funct.find(op) == _funct.end()) {
        throw error(NoOp, nom_mod, NoOpMsg + ": " + op);
    }

    string applies = _applies_to[op];
    if (applies != "*" && applies != "any") {
        string obj_type = object_type();
        if (obj_type != applies && applies != obj_type) {
            // Check if applies is a comma-separated list
            bool found = false;
            std::istringstream iss(applies);
            string token;
            while (std::getline(iss, token, ' ')) {
                if (token == obj_type || token == "any") {
                    found = true;
                    break;
                }
            }
            if (!found && applies != "any") {
                throw error(WrongTypeObj, nom_mod, WrongTypeObjMsg);
            }
        }
    }

    _funct[op](*this);
}

void gen_driver::init() {
    void* p = user_init(*this);
    string obj_name = _il->args(1);
    string type_name = _il->args(2);

    if (_obj.find(obj_name) != _obj.end()) {
        if (_dtor.find(_type[obj_name]) != _dtor.end()) {
            _dtor[_type[obj_name]](_obj[obj_name]);
        }
    }

    _obj[obj_name] = p;
    _type[obj_name] = type_name;
    _curr_obj = obj_name;
}

void gen_driver::copy() {
    string dest = _il->args(1);
    string src = _il->object();

    if (_obj.find(src) == _obj.end()) {
        throw error(NoObj, nom_mod, NoObjMsg);
    }

    string type = _type[src];
    if (_assignment.find(type) == _assignment.end()) {
        throw error(WrongTypeObj, nom_mod, "Copy not supported for type");
    }

    if (_obj.find(dest) == _obj.end()) {
        throw error(NoObj, nom_mod, "Destination object does not exist");
    }

    _assignment[type](_obj[dest], _obj[src]);
}

void gen_driver::initcopy() {
    string src_name = _il->args(1);

    if (_obj.find(src_name) == _obj.end()) {
        throw error(NoObj, nom_mod, NoObjMsg);
    }

    string type = _type[src_name];
    if (_copyctor.find(type) == _copyctor.end()) {
        throw error(WrongTypeObj, nom_mod, "Copy constructor not available");
    }

    string dest_name = _il->object();
    void* copy = _copyctor[type](_obj[src_name]);

    if (_obj.find(dest_name) != _obj.end()) {
        if (_dtor.find(_type[dest_name]) != _dtor.end()) {
            _dtor[_type[dest_name]](_obj[dest_name]);
        }
    }

    _obj[dest_name] = copy;
    _type[dest_name] = type;
    _curr_obj = dest_name;
}

void gen_driver::destroy() {
    string obj_name = _il->object();

    if (_obj.find(obj_name) == _obj.end()) {
        throw error(NoObj, nom_mod, NoObjMsg);
    }

    string type = _type[obj_name];
    if (_dtor.find(type) != _dtor.end()) {
        _dtor[type](_obj[obj_name]);
    }

    _obj.erase(obj_name);
    _type.erase(obj_name);

    if (_curr_obj == obj_name) {
        _curr_obj = _obj.empty() ? "" : _obj.begin()->first;
    }
}

void gen_driver::clear_all() {
    for (auto& p : _obj) {
        string type = _type[p.first];
        if (_dtor.find(type) != _dtor.end()) {
            _dtor[type](p.second);
        }
    }
    _obj.clear();
    _type.clear();
    _curr_obj = "";
}

void gen_driver::test_memory() {
    _osdriv << "memory test not implemented" << std::endl;
}

void gen_driver::set_memory() {
    // Not implemented
}

void gen_driver::print_memory() {
    _osdriv << "memory: 0" << std::endl;
}

void gen_driver::memory(bool on) {
    _memdin = on;
}

void gen_driver::echo() {
    _echoing = !_echoing;
}

void gen_driver::load() {
    string filename = _il->args(1);
    std::ifstream file(filename);
    if (!file) {
        throw error(NoFile, nom_mod, NoFileMsg + ": " + filename);
    }

    input_line saved = *_il;
    istream& saved_is = _isdriv;

    while (file.good()) {
        try {
            _il->read(file);
            if (!file.good()) break;
            process_operation(_il);
        } catch (const error& e) {
            _osdriv << e << std::endl;
        }
    }

    *_il = saved;
}

void gen_driver::help() {
    for (auto& p : _funct) {
        _osdriv << p.first;
        if (!_help[p.first].empty()) {
            _osdriv << " - " << _help[p.first];
        }
        _osdriv << std::endl;
    }
}

void gen_driver::select_curr_obj() {
    string obj_name = _il->args(1);
    if (_obj.find(obj_name) == _obj.end()) {
        throw error(NoObj, nom_mod, NoObjMsg);
    }
    _curr_obj = obj_name;
}

void gen_driver::list_objects() {
    for (auto& p : _obj) {
        _osdriv << p.first << " : " << _type[p.first] << std::endl;
    }
}

void gen_driver::curr_obj_name() {
    _osdriv << _curr_obj << std::endl;
}

void gen_driver::applies_to() {
    // Not implemented
}

void gen_driver::known_types() {
    for (auto& p : _dtor) {
        _osdriv << p.first << std::endl;
    }
}

void gen_driver::echo_output() {
    _echoing = true;
}

void gen_driver::echo_input() {
    input_line::echo_on();
}

void gen_driver::timer_on() {
    util::start_time();
}

void gen_driver::timer_off() {
    _osdriv << "time: " << util::stop_time() << std::endl;
}

void gen_driver::iter() {
    int n = util::toint(_il->args(1));
    input_line saved = *_il;
    _il->shift_args(1);
    for (int i = 0; i < n; ++i) {
        process_operation(_il);
    }
    *_il = saved;
}

string gen_driver::build_help_message(const string& op) {
    return _help[op];
}

bool gen_driver::generic_memtest(BuildFunct b, const string& cl, const string& msg) {
    return true;
}

bool gen_driver::copyctor_memtest(BuildFunct b, const string& cl) {
    return true;
}

bool gen_driver::assgn_memtest(BuildFunct b1, BuildFunct b2, const string& cl) {
    return true;
}

void gen_driver::test_memory(const string& cl) {
    // Not implemented
}

// input_line implementation

const string& gen_driver::input_line::args(int i) const {
    if (i < 1 || i > (int)_args.size()) {
        throw error(ArgNoExiste, nom_mod, ArgNoExisteMsg);
    }
    return _args[i - 1];
}

void gen_driver::input_line::push_back(const input_line& L) {
    for (size_t i = 0; i < L._args.size(); ++i) {
        _args.push_back(L._args[i]);
    }
}

void gen_driver::input_line::push_back(const string& s) {
    _args.push_back(s);
}

void gen_driver::input_line::shift_args(int n) {
    if (n > 0 && n <= (int)_args.size()) {
        _args.erase(_args.begin(), _args.begin() + n);
    }
}

void gen_driver::input_line::read(istream& is) {
    _args.clear();
    _orig.clear();
    _object = "";
    _op = "";

    string line;
    if (!std::getline(is, line)) return;

    if (_echoing) {
        _echo_stream << line << std::endl;
    }

    // Skip comments (lines starting with #)
    size_t start = line.find_first_not_of(" \t");
    if (start != string::npos && line[start] == '#') {
        return;
    }

    vector<string> tokens;
    util::split(line, tokens);

    if (tokens.empty()) return;

    // Parse: object.operation args... or object operation args...
    string first = tokens[0];
    size_t pos = first.find('.');
    if (pos != string::npos) {
        // Format: object.operation
        _object = first.substr(0, pos);
        _op = first.substr(pos + 1);
        for (size_t i = 1; i < tokens.size(); ++i) {
            _args.push_back(tokens[i]);
            _orig.push_back(tokens[i]);
        }
    } else if (tokens.size() >= 2) {
        // Could be: "init obj type args" or "obj operation args"
        // Check if first token is a known operation
        // For simplicity, assume if we have "word1 word2" and word1 is not a built-in,
        // then it's "object operation"

        // List of built-in commands that don't require an object
        if (first == "init" || first == "initcopy" || first == "load" ||
            first == "set_memory" || first == "memory_on" || first == "memory_off" ||
            first == "print_memory" || first == "test_memory" || first == "help" ||
            first == "list" || first == "types" || first == "echo" ||
            first == "echo_input" || first == "echo_output" ||
            first == "timer_on" || first == "timer_off" || first == "exit" ||
            first == "select" || first == "curr" || first == "iter") {
            _op = first;
            for (size_t i = 1; i < tokens.size(); ++i) {
                _args.push_back(tokens[i]);
                _orig.push_back(tokens[i]);
            }
        } else {
            // Format: object operation args...
            _object = first;
            _op = tokens[1];
            for (size_t i = 2; i < tokens.size(); ++i) {
                _args.push_back(tokens[i]);
                _orig.push_back(tokens[i]);
            }
        }
    } else {
        // Single token - it's an operation
        _op = first;
    }
}

void gen_driver::input_line::write(ostream& os) const {
    if (!_object.empty()) {
        os << _object << ".";
    }
    os << _op;
    for (size_t i = 0; i < _args.size(); ++i) {
        os << " " << _args[i];
    }
    os << std::endl;
}
