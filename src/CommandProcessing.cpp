#include "CommandProcessing.h"
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>

// ---------------------- Helpers ----------------------
static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    return s.substr(a, b - a + 1);
}

static std::string tolower_copy(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return (char)std::tolower(c); });
    return s;
}

static std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> tokens;
    std::stringstream ss(s); // Initialize stringstream with the input string
    std::string token;

    // Read words (tokens) separated by whitespace
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}


// ---------------------- Command ----------------------
Command::Command(const std::string& r, const std::string& a)
    : raw(new std::string(r)), arg(new std::string(a)), effect(new std::string("")) {}

Command::Command(const Command& other)
    : raw(new std::string(*other.raw)),
      arg(new std::string(*other.arg)),
      effect(new std::string(*other.effect)) {}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        *raw = *other.raw;
        *arg = *other.arg;
        *effect = *other.effect;
    }
    return *this;
}

Command::~Command() { delete raw; delete arg; delete effect; }

const std::string& Command::getRaw() const    { return *raw; }
const std::string& Command::getArg() const    { return *arg; }
const std::string& Command::getEffect() const { return *effect; }
void Command::saveEffect(const std::string& eff) { *effect = eff; }

std::ostream& operator<<(std::ostream& os, const Command& c) {
    os << "Command: " << c.getRaw();
    if (!c.getArg().empty()) os << " " << c.getArg();
    if (!c.getEffect().empty()) os << " | effect: " << c.getEffect();
    return os;
}

// ---------------------- FileLineReader ----------------------
FileLineReader::FileLineReader(const std::string& filename)
    : path(new std::string(filename)), in(nullptr) {
    std::ifstream* fin = new std::ifstream(filename);
    in = fin;
}

FileLineReader::FileLineReader(const FileLineReader& other)
    : path(new std::string(*other.path)), in(nullptr) {
    std::ifstream* fin = new std::ifstream(*path);
    in = fin;
}

FileLineReader& FileLineReader::operator=(const FileLineReader& other) {
    if (this != &other) {
        *path = *other.path;
        delete in;
        std::ifstream* fin = new std::ifstream(*path);
        in = fin;
    }
    return *this;
}

FileLineReader::~FileLineReader() { delete path; delete in; }

bool FileLineReader::readLine(std::string& out) {
    std::istream& is = *in;
    if (!is.good()) return false;
    std::string line;
    if (!std::getline(is, line)) return false;
    out = trim(line);
    return true;
}

// ---------------------- CommandProcessor ----------------------
CommandProcessor::CommandProcessor()
    : history(new std::vector<Command*>()), engine(nullptr) {}

CommandProcessor::CommandProcessor(const CommandProcessor& other)
    : history(new std::vector<Command*>()), engine(other.engine) {
    history->reserve(other.history->size());
    for (Command* c : *other.history) history->push_back(new Command(*c));
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        for (Command* c : *history) delete c;
        history->clear();
        engine = other.engine;
        history->reserve(other.history->size());
        for (Command* c : *other.history) history->push_back(new Command(*c));
    }
    return *this;
}

CommandProcessor::~CommandProcessor() {
    for (Command* c : *history) delete c;
    delete history;
}

void CommandProcessor::attachEngine(GameEngine* ge) { engine = ge; }

void CommandProcessor::parse(const std::string& line, std::string& outRaw, std::string& outArg) {
    std::istringstream iss(line);
    std::string cmd;
    if (!(iss >> cmd)) { outRaw.clear(); outArg.clear(); return; }
    std::string rest;
    std::getline(iss, rest); // keep spaces in arg
    outRaw = tolower_copy(trim(cmd));
    outArg = trim(rest);
}

bool CommandProcessor::validate(const Command& c) const {
    if (!engine) return true; // if no engine attached, allow reading/recording

    GameState s = engine->getCurrentState();
    const std::string& r = c.getRaw();

    // Validity table per assignment spec / diagram
    if (r == "loadmap")           return (s == GameState::Start || s == GameState::MapLoaded) && !c.getArg().empty();
    if (r == "validatemap")       return (s == GameState::MapLoaded);
    if (r == "addplayer")         return (s == GameState::MapValidated || s == GameState::PlayersAdded) && !c.getArg().empty();
    if (r == "gamestart")         return (s == GameState::PlayersAdded);
    if (r == "replay")            return (s == GameState::Win);
    if (r == "quit")              return (s == GameState::Win);
	if (r == "tournament")        return (s == GameState::Start) && !c.getArg().empty();

    return false;
}

void CommandProcessor::effect(Command& c) {
    if (!engine) { c.saveEffect("Recorded (no GameEngine attached)."); return; }

    if (!validate(c)) {
        c.saveEffect("Rejected: invalid in state '" + std::string(stateToString(engine->getCurrentState())) + "'.");
        return;
    }

 
    const std::string& r = c.getRaw();
    if (r == "tournament") {
        std::vector<std::string> tokens = split(c.getArg());

        std::vector<std::string> maps;
        std::vector<std::string> strategies;
        int numGames = 0;
        int maxTurns = 0;

        std::string currentFlag = "";
        bool parsingError = false;

        for (const auto& token : tokens) {
            if (token == "-M" || token == "-P" || token == "-G" || token == "-D") {
                currentFlag = token;
                continue;
            }

            if (currentFlag == "-M") {
                maps.push_back(token);
            }
            else if (currentFlag == "-P") {
                strategies.push_back(token);
            }
            else if (currentFlag == "-G") {
                try {
                    numGames = std::stoi(token);
                }
                catch (...) { parsingError = true; }
            }
            else if (currentFlag == "-D") {
                try {
                    maxTurns = std::stoi(token);
                }
                catch (...) { parsingError = true; }
            }
        }

        std::string errorMsg = "";

        if (parsingError) {
            errorMsg = "Error: Non-numeric value for games or turns.";
        }
        else if (maps.size() < 1 || maps.size() > 5) {
            errorMsg = "Error: Invalid number of maps (1-5 required).";
        }
        else if (strategies.size() < 2 || strategies.size() > 4) {
            errorMsg = "Error: Invalid number of strategies (2-4 required).";
        }
        else if (numGames < 1 || numGames > 5) {
            errorMsg = "Error: Invalid number of games (1-5 required).";
        }
        else if (maxTurns < 10 || maxTurns > 50) {
            errorMsg = "Error: Invalid max turns (10-50 required).";
        }

        if (!errorMsg.empty()) {
            c.saveEffect(errorMsg);
        }
        else {
            engine->startTournament(maps, strategies, numGames, maxTurns);
            c.saveEffect("Tournament Started.");
        }
        return;
    }
   
    if (r == "loadmap") {
        c.saveEffect("Map loaded: " + c.getArg());
        engine->loadMap();           // uses your existing prompt
        return;
    }
    if (r == "validatemap") {
        engine->validateMap();
        c.saveEffect("Map validated.");
        return;
    }
    if (r == "addplayer") {
        engine->addPlayer();         // uses your existing prompt
        c.saveEffect("Player added: " + c.getArg());
        return;
    }
    if (r == "gamestart") {
        engine->assignCountries();
        c.saveEffect("Game started (Assign Reinforcement).");
        return;
    }
    if (r == "replay") {
        engine->play();
        c.saveEffect("Replay → Start.");
        return;
    }
    if (r == "quit") {
        engine->end();
        c.saveEffect("Quit: exiting program.");
        return;
    }

    c.saveEffect("No effect.");
}

void CommandProcessor::readCommand() {
    std::cout << "\n> ";
    std::string line;
    std::getline(std::cin, line);
    line = trim(line);

    std::string raw, arg;
    parse(line, raw, arg);
    if (raw.empty()) return;

    Command* c = new Command(raw, arg);
    if (!validate(*c)) {
        c->saveEffect("Rejected: invalid command for current state.");
    } else {
        effect(*c);
    }
    history->push_back(c);
}

Command* CommandProcessor::getCommand() {
    readCommand();
    if (history->empty()) return nullptr;
    return history->back();
}

std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp) {
    os << "Commands history:\n";
    for (size_t i = 0; i < cp.history->size(); ++i) {
        os << "  " << i << ") " << *cp.history->at(i) << "\n";
    }
    return os;
}

// ---------------------- FileCommandProcessorAdapter ----------------------
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& filename)
    : CommandProcessor(), flr(new FileLineReader(filename)) {}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other)
    : CommandProcessor(other), flr(new FileLineReader(*other.flr)) {}

FileCommandProcessorAdapter&
FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& other) {
    if (this != &other) {
        CommandProcessor::operator=(other);
        delete flr;
        flr = new FileLineReader(*other.flr);
    }
    return *this;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() { delete flr; }

void FileCommandProcessorAdapter::readCommand() {
    std::string line;
    std::string raw, arg;

    while (true) {
        if (!flr->readLine(line)) {
            std::cout << "[EOF] End of file reached. Injecting 'quit' command.\n";
            Command* c = new Command("quit", "");
            if (!validate(*c)) {
                c->saveEffect("Rejected: invalid command for current state.");
            }
            else {
                effect(*c);
            }
            history->push_back(c);
            return;
        }

        parse(line, raw, arg);

        if (raw.empty()) {
            continue;
        }

        std::cout << "Read from file: " << line << "\n";
        Command* c = new Command(raw, arg);

        if (!validate(*c)) {
            c->saveEffect("Rejected: invalid command for current state.");
        }
        else {
            effect(*c);
        }

        history->push_back(c);
        return;
    }
}
