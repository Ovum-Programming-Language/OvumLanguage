# System Library & Interop

The `sys` namespace provides essential system operations including I/O, time, process control, and foreign function interface capabilities.

## Basic I/O

* `sys::Print(msg: String): Void` - Prints a string to standard output
* `sys::PrintLine(msg: String): Void` - Prints a string followed by a newline
* `sys::ReadLine(): String?` - Reads a line from standard input, returns `null` on EOF
* `sys::ReadChar(): Char?` - Reads a single character from standard input, returns `null` on EOF

## Time and Date Operations

### Unix Time Functions

* `sys::UnixTime(): Int` - Returns current Unix timestamp (seconds since epoch)
* `sys::UnixTimeMs(): Int` - Returns current Unix timestamp in milliseconds
* `sys::UnixTimeNs(): Int` - Returns current Unix timestamp in nanoseconds
* `sys::NanoTime(): Int` - Returns high-resolution monotonic time in nanoseconds

### Date/Time Formatting

* `sys::FormatDateTime(timestamp: Int, format: String): String?` - Formats Unix timestamp using format string
* `sys::FormatDateTimeMs(timestampMs: Int, format: String): String?` - Formats millisecond timestamp
* `sys::ParseDateTime(dateString: String, format: String): Int?` - Parses date string to Unix timestamp

### Common Format Specifiers

* `%Y` - 4-digit year (e.g., 2024)
* `%m` - Month (01-12)
* `%d` - Day of month (01-31)
* `%H` - Hour (00-23)
* `%M` - Minute (00-59)
* `%S` - Second (00-59)
* `%s` - Unix timestamp
* `%f` - Microseconds (000000-999999)
* `%n` - Nanoseconds (000000000-999999999)

## File Operations

### File Opening

* `sys::OpenFile(path: String, mode: FileMode): File?` - Opens a file with specified mode
* `sys::OpenFile(path: String, mode: FileMode, permissions: Int): File?` - Opens file with custom permissions

### FileMode Enumeration

* `sys::FileMode::Read` - Open for reading only
* `sys::FileMode::Write` - Open for writing only (truncates existing file)
* `sys::FileMode::Append` - Open for writing, append to end
* `sys::FileMode::ReadWrite` - Open for both reading and writing
* `sys::FileMode::Create` - Create new file, fail if exists
* `sys::FileMode::CreateNew` - Create new file, fail if exists
* `sys::FileMode::Truncate` - Open and truncate to zero length

### File System Operations

* `sys::FileExists(path: String): Bool` - Checks if file exists
* `sys::DirectoryExists(path: String): Bool` - Checks if directory exists
* `sys::CreateDirectory(path: String): Bool` - Creates directory, returns `false` on error
* `sys::DeleteFile(path: String): Bool` - Deletes file, returns `false` on error
* `sys::DeleteDirectory(path: String): Bool` - Deletes empty directory, returns `false` on error
* `sys::MoveFile(source: String, destination: String): Bool` - Moves/renames file
* `sys::CopyFile(source: String, destination: String): Bool` - Copies file
* `sys::GetFileSize(path: String): Int?` - Returns file size in bytes, or `null` on error
* `sys::GetFileModifiedTime(path: String): Int?` - Returns file modification time as Unix timestamp

### Directory Operations

* `sys::ListDirectory(path: String): StringArray?` - Lists directory contents, returns `null` on error
* `sys::GetCurrentDirectory(): String?` - Returns current working directory
* `sys::ChangeDirectory(path: String): Bool` - Changes current directory, returns `false` on error
* `sys::GetAbsolutePath(path: String): String?` - Returns absolute path, or `null` on error

## Process Control

* `sys::Sleep(ms: Int): Void` - Sleeps for specified milliseconds
* `sys::SleepNs(ns: Int): Void` - Sleeps for specified nanoseconds
* `sys::Exit(code: Int): Never` - Terminates the process with exit code
* `sys::GetProcessId(): Int` - Returns current process ID
* `sys::GetEnvironmentVariable(name: String): String?` - Gets environment variable value
* `sys::SetEnvironmentVariable(name: String, value: String): Bool` - Sets environment variable

## Random Number Generation

* `sys::Random(): Int` - Returns random 64-bit integer
* `sys::RandomRange(min: Int, max: Int): Int` - Returns random integer in range [min, max)
* `sys::RandomFloat(): Float` - Returns random float in range [0.0, 1.0)
* `sys::RandomFloatRange(min: Float, max: Float): Float` - Returns random float in range [min, max)
* `sys::SeedRandom(seed: Int): Void` - Seeds the random number generator

## Memory and Performance

* `sys::GetMemoryUsage(): Int` - Returns current memory usage in bytes
* `sys::GetPeakMemoryUsage(): Int` - Returns peak memory usage in bytes
* `sys::ForceGarbageCollection(): Void` - Forces garbage collection
* `sys::GetProcessorCount(): Int` - Returns number of available CPU cores

## Network Operations

* `sys::ResolveHostname(hostname: String): String?` - Resolves hostname to IP address
* `sys::IsPortOpen(host: String, port: Int): Bool` - Checks if TCP port is open
* `sys::GetLocalIpAddress(): String?` - Returns local machine's IP address

## System Information

* `sys::GetOsName(): String` - Returns operating system name
* `sys::GetOsVersion(): String` - Returns operating system version
* `sys::GetArchitecture(): String` - Returns CPU architecture (e.g., "x64", "arm64")
* `sys::GetUserName(): String?` - Returns current username
* `sys::GetHomeDirectory(): String?` - Returns user's home directory

## Error Handling

* `sys::GetLastError(): String?` - Returns description of last system error
* `sys::ClearError(): Void` - Clears the last error state

## Foreign Function Interface (FFI)

* `sys::Interope(dllName: String, functionName: String, input: ByteArray, output: ByteArray): Int`
  * **All interop calls are `unsafe`.**
  * Returns 0 on success, non-zero error code on failure
  * `input` contains parameters to pass to the function
  * `output` buffer receives the function's return value

## Usage Examples

### Date/Time Operations

```ovum
// Get current time in different formats
val unixTime: Int = sys::UnixTime()
val unixTimeMs: Int = sys::UnixTimeMs()
val unixTimeNs: Int = sys::UnixTimeNs()

// Format current time
val formatted: String? = sys::FormatDateTime(unixTime, "%Y-%m-%d %H:%M:%S")
if (formatted != null) {
    sys::PrintLine("Current time: " + formatted)
}

// High-precision timing
val start: Int = sys::NanoTime()
// ... do some work ...
val end: Int = sys::NanoTime()
val duration: Int = end - start
sys::PrintLine("Operation took " + duration.ToString() + " nanoseconds")
```

### File Operations

```ovum
// Read and write files
val file: File? = sys::OpenFile("data.txt", sys::FileMode::Read)
if (file != null) {
    val content: String? = file.ReadAllText()
    if (content != null) {
        sys::PrintLine("File content: " + content)
    }
    file.Close()
}

// Write to file
val outputFile: File? = sys::OpenFile("output.txt", sys::FileMode::Write)
if (outputFile != null) {
    val success: Bool = outputFile.WriteAllText("Hello, World!")
    if (success) {
        sys::PrintLine("File written successfully")
    }
    outputFile.Close()
}

// Directory operations
if (sys::CreateDirectory("new_folder")) {
    sys::PrintLine("Directory created")
}

val files: StringArray? = sys::ListDirectory(".")
if (files != null) {
    for (filename in files) {
        sys::PrintLine("File: " + filename)
    }
}
```

### System Information

```ovum
// Get system information
sys::PrintLine("OS: " + sys::GetOsName() + " " + sys::GetOsVersion())
sys::PrintLine("Architecture: " + sys::GetArchitecture())
sys::PrintLine("CPU cores: " + sys::GetProcessorCount().ToString())
sys::PrintLine("Memory usage: " + sys::GetMemoryUsage().ToString() + " bytes")

// Environment variables
val path: String? = sys::GetEnvironmentVariable("PATH")
if (path != null) {
    sys::PrintLine("PATH: " + path)
}
```

> **Note**: All function names use **PascalCase** (e.g., `Print`, `UnixTime`, `OpenFile`). The namespace remains `sys`.
