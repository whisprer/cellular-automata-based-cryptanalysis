#pragma once

#include <stdexcept>
#include <string>
#include <source_location>
#include <format>

namespace caca::core::error {
    // Custom exception base class
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

    class MemoryError : public CACAException {
    public:
        using CACAException::CACAException;
    };

    class ConfigurationError : public CACAException {
    public:
        using CACAException::CACAException;
    };

    // Error handling utilities
    class ErrorHandler {
    public:
        // Centralized error logging
        static void log_error(const CACAException& e) {
            // TODO: Implement proper logging mechanism
            // For now, just print to stderr
            std::cerr << e.detailed_what() << std::endl;
        }

        // Generic error handling with optional recovery
        template<typename Func, typename... Args>
        static auto handle_safely(Func&& func, Args&&... args) 
            -> std::invoke_result_t<Func, Args...>
        {
            try {
                return std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
            }
            catch (const CACAException& e) {
                log_error(e);
                // Potential recovery or rethrow logic
                throw;
            }
            catch (const std::exception& e) {
                // Wrap standard exceptions
                throw CACAException(e.what());
            }
        }
    };

    // Macro for quick error throwing with source location
    #define CACA_THROW(ExceptionType, message) \
        throw ExceptionType(message, std::source_location::current())
}


/* Usage Example:

ProgressMonitor monitor;

for (size_t i = 0; i < total_tests; ++i) {
    monitor.update_progress("NIST Statistical Tests", i, total_tests);
    run_single_nist_test(tests[i]);
} */
