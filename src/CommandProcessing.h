#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "GameEngine.h"   // uses GameState and your GameEngine transitions

// ======================= Command =======================
class Command {
public:
    Command(const std::string& raw = "", const std::string& arg = "");
    Command(const Command& other);
    Command& operator=(const Command& other);
    ~Command();

    const std::string& getRaw() const;        // e.g., "loadmap", "addplayer"
    const std::string& getArg() const;        // e.g., "Earth.map", "Alice"
    const std::string& getEffect() const;

    void saveEffect(const std::string& eff);

    friend std::ostream& operator<<(std::ostream& os, const Command& c);

private:
    std::string* raw;      // command keyword
    std::string* arg;      // optional argument
    std::string* effect;   // textual effect/result
};

// Forward decl for adapter
class FileLineReader;

// ==================== CommandProcessor ====================
class CommandProcessor {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);
    CommandProcessor& operator=(const CommandProcessor& other);
    virtual ~CommandProcessor();

    // The GameEngine whose state we validate against and mutate
    void attachEngine(GameEngine* ge);

    // Public API for clients (GameEngine/driver)
    // Returns the next parsed+stored command (nullptr if none)
    Command* getCommand();

    // Print recorded commands
    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp);

protected:
    // Read one command from the source (console or file) and store it
    virtual void readCommand();

    // Split first token as raw command, rest as arg (trimmed)
    static void parse(const std::string& line, std::string& outRaw, std::string& outArg);

    // Check if the command is valid **in current state** (table)
    // If invalid, writes the error into Command::effect
    bool validate(const Command& c) const;

    // Execute (mutate GameEngine state) and set Command::effect
    // If no GameEngine attached, only writes the effect
    void effect(Command& c);

protected:
    std::vector<Command*>* history;   // owned
    GameEngine* engine;                // not owned
};

// =========== Adapter: FileCommandProcessorAdapter ===========
class FileCommandProcessorAdapter : public CommandProcessor {
public:
    explicit FileCommandProcessorAdapter(const std::string& filename);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other);
    ~FileCommandProcessorAdapter() override;

protected:
    void readCommand() override;

private:
    FileLineReader* flr;   // owned
};

// ====================== FileLineReader ======================
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
    std::istream* in;   // dynamically allocated to satisfy “user-defined -> pointer” rule
};
