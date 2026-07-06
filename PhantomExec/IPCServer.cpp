#include "IPCServer.h"
#include <iostream>
#include <sstream>

namespace Phantom {

    // ============ IPCServer Implementation ============

    bool IPCServer::Start() {
        std::cout << "[Phantom] Starting IPC server..." << std::endl;

        if (running) {
            std::cerr << "[Phantom] IPC server already running" << std::endl;
            return false;
        }

        if (!CreatePipe()) {
            std::cerr << "[Phantom] Failed to create named pipe" << std::endl;
            return false;
        }

        running = true;
        server_thread = std::thread(&IPCServer::ServerLoop, this);

        std::cout << "[Phantom] IPC server started successfully" << std::endl;
        return true;
    }

    void IPCServer::Stop() {
        std::cout << "[Phantom] Stopping IPC server..." << std::endl;

        running = false;
        if (server_thread.joinable()) {
            server_thread.join();
        }

        if (pipe_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(pipe_handle);
            pipe_handle = INVALID_HANDLE_VALUE;
        }

        std::cout << "[Phantom] IPC server stopped" << std::endl;
    }

    bool IPCServer::SendMessage(const IPCMessage& msg) {
        if (pipe_handle == INVALID_HANDLE_VALUE) {
            std::cerr << "[Phantom] Pipe not connected" << std::endl;
            return false;
        }

        std::string json = MessageToJSON(msg);
        DWORD bytes_written = 0;

        if (!WriteFile(pipe_handle, json.c_str(), static_cast<DWORD>(json.length()), &bytes_written, nullptr)) {
            std::cerr << "[Phantom] Failed to write to pipe" << std::endl;
            return false;
        }

        std::cout << "[Phantom] Message sent via IPC (" << bytes_written << " bytes)" << std::endl;
        return true;
    }

    void IPCServer::SetMessageCallback(std::function<void(const IPCMessage&)> callback) {
        message_callback = callback;
    }

    void IPCServer::ServerLoop() {
        std::cout << "[Phantom] IPC server loop started" << std::endl;

        while (running) {
            if (!WaitForConnection()) {
                continue;
            }

            IPCMessage msg;
            while (running && ReadMessage(msg)) {
                std::cout << "[Phantom] Received IPC message: " << msg.type << std::endl;

                if (msg.type == "execute") {
                    HandleExecute(msg);
                } else if (msg.type == "status") {
                    HandleStatus(msg);
                } else if (msg.type == "validate") {
                    HandleValidate(msg);
                }

                if (message_callback) {
                    message_callback(msg);
                }
            }

            if (pipe_handle != INVALID_HANDLE_VALUE) {
                DisconnectNamedPipe(pipe_handle);
            }
        }

        std::cout << "[Phantom] IPC server loop ended" << std::endl;
    }

    bool IPCServer::CreatePipe() {
        std::cout << "[Phantom] Creating named pipe..." << std::endl;

        pipe_handle = CreateNamedPipeA(
            "\\\\.\\pipe\\PhantomExecutor",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
            1,
            4096,
            4096,
            0,
            nullptr
        );

        if (pipe_handle == INVALID_HANDLE_VALUE) {
            std::cerr << "[Phantom] Failed to create named pipe: " << GetLastError() << std::endl;
            return false;
        }

        std::cout << "[Phantom] Named pipe created successfully" << std::endl;
        return true;
    }

    bool IPCServer::WaitForConnection() {
        if (pipe_handle == INVALID_HANDLE_VALUE) {
            return false;
        }

        std::cout << "[Phantom] Waiting for IPC client connection..." << std::endl;

        if (!ConnectNamedPipe(pipe_handle, nullptr)) {
            if (GetLastError() != ERROR_PIPE_CONNECTED) {
                std::cerr << "[Phantom] Failed to connect named pipe: " << GetLastError() << std::endl;
                return false;
            }
        }

        std::cout << "[Phantom] IPC client connected" << std::endl;
        return true;
    }

    bool IPCServer::ReadMessage(IPCMessage& msg) {
        char buffer[4096] = {0};
        DWORD bytes_read = 0;

        if (!ReadFile(pipe_handle, buffer, sizeof(buffer) - 1, &bytes_read, nullptr)) {
            if (GetLastError() != ERROR_BROKEN_PIPE) {
                std::cerr << "[Phantom] Failed to read from pipe: " << GetLastError() << std::endl;
            }
            return false;
        }

        if (bytes_read == 0) {
            return false;
        }

        buffer[bytes_read] = '\0';
        return ParseJSON(std::string(buffer), msg);
    }

    bool IPCServer::ParseJSON(const std::string& json, IPCMessage& msg) {
        // Simple JSON parsing (in real implementation, use a proper JSON library)
        std::cout << "[Phantom] Parsing IPC message JSON..." << std::endl;

        // Extract type
        size_t type_pos = json.find("\"type\":");
        if (type_pos != std::string::npos) {
            size_t start = json.find("\"", type_pos + 8) + 1;
            size_t end = json.find("\"", start);
            msg.type = json.substr(start, end - start);
        }

        // Extract script
        size_t script_pos = json.find("\"script\":");
        if (script_pos != std::string::npos) {
            size_t start = json.find("\"", script_pos + 9) + 1;
            size_t end = json.find("\"", start);
            msg.script = json.substr(start, end - start);
        }

        // Extract id
        size_t id_pos = json.find("\"id\":");
        if (id_pos != std::string::npos) {
            msg.id = std::stoi(json.substr(id_pos + 5));
        }

        return true;
    }

    std::string IPCServer::MessageToJSON(const IPCMessage& msg) {
        std::stringstream ss;
        ss << "{"
           << "\"type\":\"" << msg.type << "\","
           << "\"id\":" << msg.id << ","
           << "\"status\":\"" << msg.status << "\","
           << "\"output\":\"" << msg.output << "\","
           << "\"unc_compliance\":" << msg.unc_compliance << ","
           << "\"sunc_compliance\":" << msg.sunc_compliance << ","
           << "\"myriad_valid\":" << (msg.myriad_valid ? "true" : "false") << ","
           << "\"memory_usage\":" << msg.memory_usage
           << "}";
        return ss.str();
    }

    void IPCServer::HandleExecute(const IPCMessage& msg) {
        std::cout << "[Phantom] Handling execute command..." << std::endl;
        // In real implementation: execute the script and return results
    }

    void IPCServer::HandleStatus(const IPCMessage& msg) {
        std::cout << "[Phantom] Handling status query..." << std::endl;
        // In real implementation: return current executor status
    }

    void IPCServer::HandleValidate(const IPCMessage& msg) {
        std::cout << "[Phantom] Handling validation request..." << std::endl;
        // In real implementation: run UNC/sUNC/Myriad validation
    }

    // ============ IPCClient Implementation ============

    bool IPCClient::Connect() {
        std::cout << "[Phantom] Connecting to IPC server..." << std::endl;

        pipe_handle = CreateFileA(
            "\\\\.\\pipe\\PhantomExecutor",
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
        );

        if (pipe_handle == INVALID_HANDLE_VALUE) {
            std::cerr << "[Phantom] Failed to connect to IPC server: " << GetLastError() << std::endl;
            return false;
        }

        std::cout << "[Phantom] Connected to IPC server" << std::endl;
        return true;
    }

    bool IPCClient::SendMessage(const IPCMessage& msg) {
        if (pipe_handle == INVALID_HANDLE_VALUE) {
            std::cerr << "[Phantom] Not connected to IPC server" << std::endl;
            return false;
        }

        std::string json = MessageToJSON(msg);
        DWORD bytes_written = 0;

        if (!WriteFile(pipe_handle, json.c_str(), static_cast<DWORD>(json.length()), &bytes_written, nullptr)) {
            std::cerr << "[Phantom] Failed to write to pipe: " << GetLastError() << std::endl;
            return false;
        }

        std::cout << "[Phantom] Message sent (" << bytes_written << " bytes)" << std::endl;
        return true;
    }

    bool IPCClient::ReceiveMessage(IPCMessage& msg) {
        if (pipe_handle == INVALID_HANDLE_VALUE) {
            std::cerr << "[Phantom] Not connected to IPC server" << std::endl;
            return false;
        }

        char buffer[4096] = {0};
        DWORD bytes_read = 0;

        if (!ReadFile(pipe_handle, buffer, sizeof(buffer) - 1, &bytes_read, nullptr)) {
            std::cerr << "[Phantom] Failed to read from pipe: " << GetLastError() << std::endl;
            return false;
        }

        if (bytes_read == 0) {
            return false;
        }

        buffer[bytes_read] = '\0';
        return ParseJSON(std::string(buffer), msg);
    }

    void IPCClient::Disconnect() {
        if (pipe_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(pipe_handle);
            pipe_handle = INVALID_HANDLE_VALUE;
            std::cout << "[Phantom] Disconnected from IPC server" << std::endl;
        }
    }

    bool IPCClient::ParseJSON(const std::string& json, IPCMessage& msg) {
        // Simple JSON parsing
        std::cout << "[Phantom] Parsing response JSON..." << std::endl;

        size_t status_pos = json.find("\"status\":");
        if (status_pos != std::string::npos) {
            size_t start = json.find("\"", status_pos + 9) + 1;
            size_t end = json.find("\"", start);
            msg.status = json.substr(start, end - start);
        }

        return true;
    }

    std::string IPCClient::MessageToJSON(const IPCMessage& msg) {
        std::stringstream ss;
        ss << "{"
           << "\"type\":\"" << msg.type << "\","
           << "\"id\":" << msg.id << ","
           << "\"script\":\"" << msg.script << "\""
           << "}";
        return ss.str();
    }

}
