#pragma once
#include <winwrap/detail/flag.hpp>
#include <winwrap/exception.hpp>

#include <string>

#include <Windows.h>


namespace WinWrap {
    enum class FileHandleAccess : DWORD {
        All = GENERIC_ALL,
        Read = GENERIC_READ,
        Write = GENERIC_WRITE,
        Execute = GENERIC_EXECUTE,
    };
    template<>struct detail::flag::ops_def<FileHandleAccess>:std::true_type{};

    enum class FileHandleShare : DWORD {
        Read = FILE_SHARE_READ,
        Write = FILE_SHARE_WRITE,
        Delete = FILE_SHARE_DELETE
    };
    template<>struct detail::flag::ops_def<FileHandleShare>:std::true_type{};

    enum class FileHandleFlagsAndAttribute : DWORD {
        None = 0,
        Readonly = FILE_ATTRIBUTE_READONLY,
        Hidden = FILE_ATTRIBUTE_HIDDEN,
        System = FILE_ATTRIBUTE_SYSTEM,
        Directry = FILE_ATTRIBUTE_DIRECTORY,
        Archive = FILE_ATTRIBUTE_ARCHIVE,
        Device = FILE_ATTRIBUTE_DEVICE,
        Normal = FILE_ATTRIBUTE_NORMAL,
        Temporary = FILE_ATTRIBUTE_TEMPORARY,
        SparseFile = FILE_ATTRIBUTE_SPARSE_FILE,
        ReparsePoint = FILE_ATTRIBUTE_REPARSE_POINT,
        Compressed = FILE_ATTRIBUTE_COMPRESSED,
        Offline = FILE_ATTRIBUTE_OFFLINE,
        Indexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
        Encrypted = FILE_ATTRIBUTE_ENCRYPTED,
        IntegritySystem = FILE_ATTRIBUTE_INTEGRITY_STREAM,
        Virtual = FILE_ATTRIBUTE_VIRTUAL,
        NoScrubData = FILE_ATTRIBUTE_NO_SCRUB_DATA,
        Ea = FILE_ATTRIBUTE_EA,
        Pinned = FILE_ATTRIBUTE_PINNED,
        Unpinned = FILE_ATTRIBUTE_UNPINNED,
        RecallOnOpen = FILE_ATTRIBUTE_RECALL_ON_OPEN,
        RecallOnDataAccess = FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS,
        
        FirstPipeInstance = FILE_FLAG_FIRST_PIPE_INSTANCE,
        OpenNoRecall = FILE_FLAG_OPEN_NO_RECALL,
        OpenReparsePoint = FILE_FLAG_OPEN_REPARSE_POINT,
        SessionAware = FILE_FLAG_SESSION_AWARE,
        PosixSemantics = FILE_FLAG_POSIX_SEMANTICS,
        BackupSemantics = FILE_FLAG_BACKUP_SEMANTICS,
        DeleteOnClose = FILE_FLAG_DELETE_ON_CLOSE,
        SequentialScan = FILE_FLAG_SEQUENTIAL_SCAN,
        RandomAccess = FILE_FLAG_RANDOM_ACCESS,
        NoBuffering = FILE_FLAG_NO_BUFFERING,
        Overlapped = FILE_FLAG_OVERLAPPED,
        WriteThrough = FILE_FLAG_WRITE_THROUGH
    };
    template<>struct detail::flag::ops_def<FileHandleFlagsAndAttribute>:std::true_type{};

    enum class FileHandleCreationDisposition : DWORD {
        CreateNew = CREATE_NEW,
        CreateAlways = CREATE_ALWAYS,
        OpenExisting = OPEN_EXISTING,
        OpenAlways = OPEN_ALWAYS,
        TruncateExisting = TRUNCATE_EXISTING
    };

    class FileHandle {
        HANDLE m_hFile;
    public:
        FileHandle(
            std::string_view path,
            FileHandleAccess access = FileHandleAccess::Read,
            FileHandleShare share = FileHandleShare::Read,
            SECURITY_ATTRIBUTES* security_attributes = nullptr,
            FileHandleCreationDisposition creation_disposition = FileHandleCreationDisposition::OpenExisting,
            FileHandleFlagsAndAttribute flags_and_attribute = FileHandleFlagsAndAttribute::None,
            HANDLE template_file = NULL
        ) {
            m_hFile = CreateFileA(
                path.data(),
                static_cast<DWORD>(access),
                static_cast<DWORD>(share),
                security_attributes,
                static_cast<DWORD>(creation_disposition),
                static_cast<DWORD>(flags_and_attribute),
                template_file
            );
            if(m_hFile == NULL) {
                throw WinException(GetLastError());
            }
        }

        operator HANDLE() const { return m_hFile; }

        size_t size() const {
            auto ret = GetFileSize(m_hFile, NULL);
            if (ret == INVALID_FILE_SIZE) {
                throw WinException(GetLastError());
            }
            return ret;
        }

        LARGE_INTEGER size64() const {
            LARGE_INTEGER ret;
            auto b = GetFileSizeEx(m_hFile, &ret);
            if (!b) {
                throw WinException(GetLastError());
            }
            return ret;
        }
    };
} // namespace WinWrap
