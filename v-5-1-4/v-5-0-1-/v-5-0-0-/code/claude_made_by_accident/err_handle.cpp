#pragma once

#include <stdexcept>
#include <string>
#include <source_location>
#include <format>
#include <iostream>

namespace caca::error {
    // Base exception for CACA project
    class CACAException : public std::runtime_error {
    private:
        std::source_location m_location;

    public:
        CACAException(
            const std::string& message, 
            const std::source_location& location = std::source_location::current()
        ) : std::runtime_error(message), m_location(location) {}

        // Detailed error information
        std::string detailed_what() const {
            return std::format(
                "Error: {}\nFile: {}\nLine: {}\nFunction: {}", 
                what(), 
                m_location.file_name(), 
                m_location.line(), 
                m_location.function_name()
            );
        }

        // Source location accessor
        const std::source_location& location() const noexcept {
            return m_location;
        }
    };

    // Specific exception types
    class ComputationError : public CACAException {
    public:
        using CACAException::CACAException;
    };

    class ConfigurationError : public CACAException {
    public:
        using CACAException::CACAException;
    };

    class IOError : public CACAException {
    public:
        using CACAException::CACAException;
    };

    // Error logging utility
    class ErrorLogger {
    public:
        // Log an exception with optional additional context
        static void log(const std::exception& e, 
                        const std::string& context = "") {
            std::cerr << "CACA Error" 
                      << (context.empty() ? "" : " in " + context) 
                      << ":\n" 
                      << e.what() << std::endl;
        }

        // Log a custom error message
        static void log(const std::string& message) {
            std::cerr << "CACA Message: " << message << std::endl;
        }
    };

    // Macro for quick error throwing with source location
    #define CACA_THROW(ExceptionType, message) \
        throw ExceptionType(message, std::source_location::current())
}