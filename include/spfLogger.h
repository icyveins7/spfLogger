#pragma once

#include <cstdio>
#include <ctime>
#include <iostream>
#include <source_location>
#include <string>
#include <stdexcept>

namespace spf
{

// Forward declaration
class Logger;

/**
 * @class PrivateLogWriter
 * @brief This has no public constructor, as intended.
 * Instantiate the `Logger` class for actual usage;
 * this class is automatically initialized by `Logger`,
 * and is only used for its message-type methods:
 * debug(), info(), and error().
 */
class PrivateLogWriter
{
  friend class Logger;
public:
  /**
   * @brief Logs something for debugging
   *
   * @param fmt A character array string, like in printf.
   * @param args Variable substitutions for the format specifiers in the string.
   */
  template <typename... Args>
  void debug(
    const char* fmt,
    Args&&... args)
  {
    log("DEBUG", fmt, args...);
  }

  /**
   * @brief Logs something informational
   *
   * @param fmt A character array string, like in printf.
   * @param args Variable substitutions for the format specifiers in the string.
   */
  template <typename... Args>
  void info(
    const char* fmt,
    Args&&... args)
  {
    log("INFO", fmt, args...);
  }

  /**
   * @brief Logs an error message
   *
   * @param fmt A character array string, like in printf.
   * @param args Variable substitutions for the format specifiers in the string.
   */
  template <typename... Args>
  void error(
    const char* fmt,
    Args&&... args)
  {
    log("ERROR", fmt, args...);
  }

private:
  std::source_location m_loc;
  FILE* m_fp;
  FILE* m_printstream;
  // TODO: this is not particularly safe, since you can construct this
  // and hold the pointer after it's destroyed?

  // Private constructor
  PrivateLogWriter(
    std::source_location loc,
    FILE* fp,
    FILE* printstream
  ) : m_loc(std::move(loc)),
  m_fp(fp),
  m_printstream(printstream)
  {
  }

  /**
   * @brief Primary handler for all log method types.
   * Is called by those methods.
   *
   * @param prefix Additional prefix like 'DEBUG' or 'ERROR'.
   * @param fmt Main format string.
   * @param args Variable arguments for format specifier substitutions.
   */
  template <typename... Args>
  void log(
    const char* prefix,
    const char* fmt,
    Args... args)
  {
    datetime_formatter();
    fprintf(m_printstream, "%s ", prefix);
    fprintf(m_printstream, "(%s: %d) ",
            m_loc.file_name(), m_loc.line());
    fprintf(m_printstream, fmt, args...);
    fprintf(m_printstream, "\n");

    if (m_fp != nullptr)
    {
      fprintf(m_fp, "%s ", prefix);
      fprintf(m_fp, "(%s: %d) ",
              m_loc.file_name(), m_loc.line());
      fprintf(m_fp, fmt, args...);
      fprintf(m_fp, "\n");
    }
  }

  /**
   * @brief This is used to format the current date and time,
   * and write them to both streams. Generally meant to only
   * be called by log().
   */
  void datetime_formatter() {
    std::time_t t = std::time({});
    char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
    std::strftime(std::data(timeString), std::size(timeString),
                  "%FT%TZ", std::gmtime(&t));

    fprintf(m_printstream, "%s ", timeString);
    if (m_fp != nullptr)
      fprintf(m_fp, "%s ", timeString);
  }


};

/**
 * @class Logger
 * @brief This is the class you should instantiate.
 * It handles the safe opening/closing of the log file.
 */
class Logger
{
public:
  /**
   * @brief Constructs a logger with an optional log file.
   *
   * @param logfilepath Path to log file. Leave empty to disable.
   */
  Logger(std::string logfilepath="")
    : m_logfilepath(logfilepath)
  {
    open_file();
  }


  ~Logger() {
    // Close file if open
    if (m_fp != nullptr)
      fclose(m_fp);
  }


  void set_file(std::string logfilepath) {
    m_logfilepath = logfilepath;
    if (m_logfilepath.size() == 0)
    {
      fclose(m_fp);
      m_fp = nullptr;
    }
    else
      open_file();
  }

  std::string get_file() {
    return m_logfilepath;
  }

  void set_print(FILE* stream) {
    m_printstream = stream;
  }

  PrivateLogWriter operator()(
    std::source_location loc=std::source_location::current()
  ) const
  {
    return PrivateLogWriter(loc, m_fp, m_printstream);
  }


protected:
  std::string m_logfilepath = "";
  FILE* m_fp = nullptr;
  FILE* m_printstream = stdout;

  void open_file() {
    // Close existing file if opened
    if (m_fp != nullptr)
    {
      fclose(m_fp);
      m_fp = nullptr;
    }

    // Open a new one if specified
    if (m_logfilepath.size() > 0) {
      m_fp = fopen(m_logfilepath.c_str(), "a");
      if (!m_fp)
        throw std::runtime_error("Failed to open logfile.");
    }
  }

};



}
