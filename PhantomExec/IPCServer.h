#pragma once
#include <windows.h>
#include <string>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>

namespace Phantom {

    /**
     * JSON-based message structure for IPC communication
     */
    struct IPCMessage {
        std::string type;           // "execute", "status", "validate", etc.
        std::string script;         // Script content (for execute)
        int id;                     // Message ID for correlation
        std::string status;         // Response status
        std::string output;         // Response output
        float unc_compliance;       // Compliance metrics
        float sunc_compliance;
        bool myriad_valid;
        size_t memory_usage;
    };

    /**
     * Named Pipe IPC Server
     * Handles communication between web UI and injected DLL
     */
    class IPCServer {
    public:
        static IPCServer& GetInstance() {
            static IPCServer instance;
            return instance;
        }

        /**
         * Start IPC server
         * Creates named pipe and listens for connections
         */
        bool Start();

        /**
         * Stop IPC server
         */
        void Stop();

        /**
         * Send message to connected client
         */
        bool SendMessage(const IPCMessage& msg);

        /**
         * Register callback for received messages
         */
        void SetMessageCallback(std::function<void(const IPCMessage&)> callback);

    private:
        IPCServer() : running(false), pipe_handle(INVALID_HANDLE_VALUE) {}
        ~IPCServer() { Stop(); }

        bool running;
        HANDLE pipe_handle;
        std::thread server_thread;
        std::function<void(const IPCMessage&)> message_callback;
        std::queue<IPCMessage> message_queue;
        std::mutex queue_mutex;

        /**
         * Server thread main loop
         */
        void ServerLoop();

        /**
         * Create named pipe
         */
        bool CreatePipe();

        /**
         * Wait for client connection
         */
        bool WaitForConnection();

        /**
         * Read message from pipe
         */
        bool ReadMessage(IPCMessage& msg);

        /**
         * Parse JSON message
         */
        bool ParseJSON(const std::string& json, IPCMessage& msg);

        /**
         * Serialize message to JSON
         */
        std::string MessageToJSON(const IPCMessage& msg);

        /**
         * Handle execute command
         */
        void HandleExecute(const IPCMessage& msg);

        /**
         * Handle status query
         */
        void HandleStatus(const IPCMessage& msg);

        /**
         * Handle validation request
         */
        void HandleValidate(const IPCMessage& msg);
    };

    /**
     * IPC Client (for testing/debugging)
     */
    class IPCClient {
    public:
        static IPCClient& GetInstance() {
            static IPCClient instance;
            return instance;
        }

        /**
         * Connect to IPC server
         */
        bool Connect();

        /**
         * Send message to server
         */
        bool SendMessage(const IPCMessage& msg);

        /**
         * Receive message from server
         */
        bool ReceiveMessage(IPCMessage& msg);

        /**
         * Disconnect from server
         */
        void Disconnect();

    private:
        IPCClient() : pipe_handle(INVALID_HANDLE_VALUE) {}
        ~IPCClient() { Disconnect(); }

        HANDLE pipe_handle;

        /**
         * Parse JSON message
         */
        bool ParseJSON(const std::string& json, IPCMessage& msg);

        /**
         * Serialize message to JSON
         */
        std::string MessageToJSON(const IPCMessage& msg);
    };

}
