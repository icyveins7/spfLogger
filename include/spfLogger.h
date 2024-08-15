#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <stdexcept>

namespace spf
{

class Logger
{
public:
  Logger() {
    // Empty
  }

  Logger(std::string logfilepath)
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

  template <typename... Args>
  void log_debug(const char* fmt, Args... args) {
    log("DEBUG: ", fmt, args...);
  }

protected:
  std::string m_logfilepath;
  FILE* m_fp = nullptr;
  FILE* m_printstream = stdout;

  void open_file() {
    // Close existing file if opened
    if (m_fp != nullptr)
    {
      fclose(m_fp);
      m_fp = nullptr;
    }

    // Open a new one
    m_fp = fopen(m_logfilepath.c_str(), "w");
    if (!m_fp)
      throw std::runtime_error("Failed to open logfile.");
  }

  template <typename... Args>
  void log(const char* prefix, const char* fmt, Args... args) {
    fprintf(m_printstream, prefix);
    fprintf(m_printstream, fmt, args...);
    fprintf(m_printstream, "\n");

    if (m_fp != nullptr)
    {
      fprintf(m_fp, prefix);
      fprintf(m_fp, fmt, args...);
      fprintf(m_fp, "\n");
    }
  }
};



}
