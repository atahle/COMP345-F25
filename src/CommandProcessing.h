#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "GameEngine.h"

// ---------------------- Command ----------------------
class Command {
public:
    Command(const std::string& raw = "", const std::string& arg = "");
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    const std::string& getRaw() const;
    const std::string& getArg() const;
    const std::string& getEffect() const;

    void saveEffect(const std::string& eff);

    friend std::ostream& operator<<(std::ostream& os, const Command& c);

private:
    std::string* raw;     // command keyword (e.g., "loadmap")
    std::string* arg;     // optional argument (e.g., "Canada.map")
    std::string* effect;  // textual effect/result
};

// Forward-declare adapter helper
class FileLineReader;

// ---------------------- CommandProcessor ----------------------
class CommandProcessor {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);
    CommandProcessor& operator=(const CommandProcessor& other);
    virtual ~CommandProcessor();

    void attachEngine(GameEngine* ge);

    // Reads one command from the source, stores it, and returns it (nullptr on no input)
    Command* getCommand();

    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp);

protected:
    // Source hook (console by default; file adapter overrides this)
    virtual void readCommand();

    // Parsing + validation + effect helpers
    static void parse(const std::string& line, std::string& outRaw, std::string& outArg);
    bool validate(const Command& c) const;
    void effect(Command& c);

protected:
    std::vector<Command*>* history;  // owned
    GameEngine* engine;              // not owned
};

// ---------------------- FileLineReader ----------------------
class FileLineReader {
public:
    explicit FileLineReader(const std::string& filename);
    FileLineReader(const FileLineReader& other);
    FileLineReader& operator=(const FileLineReader& other);
    ~FileLineReader();

    // Returns true and fills 'out' if a line was read; false on EOF
    bool readLine(std::string& out);

private:
    std::string* path;
    std::istream* in; // dynamically allocated stream (to honor pointer rule)
};

// ---------------------- FileCommandProcessorAdapter ----------------------
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    explicit FileCommandProcessorAdapter(const std::string& filename);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
    ~FileCommandProcessorAdapter() override;

protected:
    void readCommand() override;

private:
    FileLineReader* flr; // owned
};

// ---------------------- Driver (free function) ----------------------
// Required by the rubric: demonstrates console and file modes, and validation.
void testCommandProcessor();
